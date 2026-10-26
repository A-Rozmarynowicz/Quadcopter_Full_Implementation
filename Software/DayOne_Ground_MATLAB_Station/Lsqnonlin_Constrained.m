
function [P, dFit, residuals, rmse] = ...
    Lsqnonlin_Constrained(d, N, knownCoords)
% Estimate N 3D points from measured pairwise distances.
%
% INPUT:
%   d           M-element vector of measured distances.
%   N           Number of points (optional if all pairwise
%               distances are supplied).
%   knownCoords N-by-3 matrix of known coordinates.
%               Use NaN for unknown coordinates.
%               Example: knownCoords(4,3) = 0.75;
%
% OUTPUT:
%   P           N-by-3 estimated coordinates
%   dFit        M-by-1 fitted distances
%   residuals   M-by-1 residuals (fitted - measured)
%   rmse        Root mean square distance residual
%
% Requires Optimization Toolbox.
%
% The reference frame is fixed by:
%   P(1,:) = [0 0 0]
%   P(2,:) = [positive x 0 0]
%   P(3,:) = [x positive y 0]
%
% Known coordinates must be expressed in this reference frame.

    validateattributes(d, {'numeric'}, ...
        {'real','finite','vector','nonempty','positive'});

    d = double(d(:));
    M = numel(d);

    % Infer N if all pairwise distances are supplied.
    if nargin < 2 || isempty(N)
        N = round((1 + sqrt(1 + 8*M))/2);

        if N*(N-1)/2 ~= M
            error(['N must be specified when the measurements ', ...
                   'do not contain all pairwise distances.']);
        end
    end

    validateattributes(N, {'numeric'}, ...
        {'real','finite','scalar','integer','>=',4});

    N = double(N);

    % Default: no known coordinates.
    if nargin < 3 || isempty(knownCoords)
        knownCoords = nan(N,3);
    end

    if ~isnumeric(knownCoords) || ~isreal(knownCoords) || ...
            ~isequal(size(knownCoords), [N,3]) || ...
            any(isinf(knownCoords(:)))
        error(['knownCoords must be a real N-by-3 matrix, ', ...
               'containing finite values or NaN.']);
    end

    knownCoords = double(knownCoords);

    if M < 6
        error('At least six distance measurements are required.');
    end

    if M > N*(N-1)/2
        error('Too many measurements for N points without repetition.');
    end

    if exist('lsqnonlin','file') ~= 2
        error('This function requires the Optimization Toolbox.');
    end

    % All pairs in the required ordering.
    allPairs = nchoosek(1:N, 2);
    pairs = allPairs(1:M,:);

    % Number of free parameters after removing rigid-body motion.
    nVars = 3*N - 6;

    lb = -Inf(nVars,1);
    ub =  Inf(nVars,1);

    % Fix the orientation of the reference frame.
    lb(1) = 1e-9;  % P(2,1) > 0
    lb(3) = 1e-9;  % P(3,2) > 0

    % Map each coordinate to its optimization variable.
    % P(1,:) = [0 0 0]
    % P(2,:) = [t(1) 0 0]
    % P(3,:) = [t(2) t(3) 0]
    % P(4:N,:) use t(4:end).

    for i = 1:N
        for j = 1:3

            value = knownCoords(i,j);

            if isnan(value)
                continue;
            end

            % Coordinates already fixed by the reference frame.
            if i == 1 || (i == 2 && j >= 2) || ...
                    (i == 3 && j == 3)

                if abs(value) > 1e-10
                    error(['Known coordinate P(%d,%d) conflicts ', ...
                           'with the fixed reference frame.'], i,j);
                end

                continue;
            end

            % Convert coordinate index to optimization variable.
            if i == 2
                idx = 1;
            elseif i == 3
                idx = j + 1;  % P(3,1) -> 2; P(3,2) -> 3
            else
                idx = 3*(i-4) + j + 3;
            end

            % Enforce the known coordinate exactly.
            lb(idx) = value;
            ub(idx) = value;
        end
    end

    if any(lb > ub)
        error(['Known coordinates conflict with the ', ...
               'reference-frame constraints.']);
    end

    opts = optimoptions('lsqnonlin', ...
        'Display','off', ...
        'MaxFunctionEvaluations',20000, ...
        'MaxIterations',3000);

    scale = median(d);

    if ~isfinite(scale) || scale <= 0
        scale = 1;
    end

    bestTheta = [];
    bestCost = Inf;
    nStarts = 100;

    for k = 1:nStarts

        theta0 = scale * randn(nVars,1);

        if k == 1
            theta0(:) = 0;
            theta0(1) = d(1);
            theta0(2) = d(2)/2;
            theta0(3) = sqrt(max( ...
                d(2)^2 - theta0(2)^2, 0.01*scale^2));
        end

        theta0(1) = max(abs(theta0(1)) + 0.1*scale, lb(1));
        theta0(3) = max(abs(theta0(3)) + 0.1*scale, lb(3));

        % Start within all bounds, including fixed coordinates.
        theta0 = max(theta0,lb);
        theta0 = min(theta0,ub);

        try
            [theta,resnorm,~,exitflag] = lsqnonlin( ...
                @(t) distanceResiduals(t,d,pairs,N), ...
                theta0,lb,ub,opts);

            if exitflag > 0 && isfinite(resnorm) && ...
                    resnorm < bestCost
                bestCost = resnorm;
                bestTheta = theta;
            end
        catch
            % Ignore failed initializations.
        end
    end

    if isempty(bestTheta)
        error('Optimization failed for all initializations.');
    end

    P = parametersToPoints(bestTheta,N);

    % Calculate fitted distances.
    delta = P(pairs(:,1),:) - P(pairs(:,2),:);
    dFit = sqrt(sum(delta.^2,2));

    residuals = dFit - d;
    rmse = sqrt(mean(residuals.^2));

end


function P = parametersToPoints(t,N)

    P = zeros(N,3);

    P(2,1) = t(1);
    P(3,1) = t(2);
    P(3,2) = t(3);

    if N > 3
        P(4:N,:) = reshape(t(4:end),3,N-3).';
    end

end


function r = distanceResiduals(t,d,pairs,N)

    P = parametersToPoints(t,N);

    delta = P(pairs(:,1),:) - P(pairs(:,2),:);
    r = sqrt(sum(delta.^2,2)) - d;

end
