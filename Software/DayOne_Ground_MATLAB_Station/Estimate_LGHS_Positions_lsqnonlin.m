function [P, dFit, residuals, rmse] = Estimate_LGHS_Positions_lsqnonlin(d)
% Estimate N 3D points from measured pairwise distances.
%
% INPUT:
%   d       M-element vector of distances, in the order:
%           d01, d02, ..., d0(N-1), d12, d13, ..., d(N-2)(N-1)
%
%   N       Number of points (at least 4).
%           Optional only when d contains all pairwise distances.
%
% OUTPUT:
%   P         N-by-3 estimated coordinates
%   dFit      M-by-1 fitted distances
%   residuals M-by-1 residuals (fitted - measured)
%   rmse      Root mean square distance residual
%
% Requires Optimization Toolbox.

    validateattributes(d, {'numeric'}, ...
        {'real','finite','vector','nonempty','positive'});

    d = double(d(:));
    M = numel(d);

    % Infer N only when all pairwise distances are supplied.
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
    % MATLAB indices 1,...,N correspond to point labels 0,...,N-1.
    allPairs = nchoosek(1:N, 2);
    pairs = allPairs(1:M, :);

    % Remove the six rigid-body degrees of freedom:
    % P(1,:) = [0 0 0]
    % P(2,:) = [x2 0 0]
    % P(3,:) = [x3 y3 0]
    % Remaining points have unrestricted 3D coordinates.
    nVars = 3*N - 6;

    lb = -Inf(nVars,1);
    ub =  Inf(nVars,1);

    lb(1) = 1e-9;  % x-coordinate of point 2
    lb(3) = 1e-9;  % y-coordinate of point 3

    opts = optimoptions('lsqnonlin', ...
        'Display','off', ...
        'MaxFunctionEvaluations',20000, ...
        'MaxIterations',3000);

    % Multiple initializations to reduce local-minimum sensitivity.
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
            theta0(3) = sqrt( ...
                max(d(2)^2 - theta0(2)^2, 0.01*scale^2));
        end

        theta0(1) = abs(theta0(1)) + 0.1*scale;
        theta0(3) = abs(theta0(3)) + 0.1*scale;

        try
            [theta, resnorm, ~, exitflag] = lsqnonlin( ...
                @(t) distanceResiduals(t,d,pairs,N), ...
                theta0,lb,ub,opts);

            if exitflag > 0 && isfinite(resnorm) ...
                    && resnorm < bestCost
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
    dFit = zeros(M,1);

    for k = 1:M
        i = pairs(k,1);
        j = pairs(k,2);
        dFit(k) = norm(P(i,:) - P(j,:));
    end

    residuals = dFit - d;
    rmse = sqrt(mean(residuals.^2));

end


function P = parametersToPoints(t,N)

    P = zeros(N,3);

    % Fixed reference frame.
    P(2,1) = t(1);
    P(3,1) = t(2);
    P(3,2) = t(3);

    % Remaining points: [x4 y4 z4 x5 y5 z5 ...].
    if N > 3
        P(4:N,:) = reshape(t(4:end),3,N-3).';
    end

end


function r = distanceResiduals(t,d,pairs,N)

    P = parametersToPoints(t,N);

    delta = P(pairs(:,1),:) - P(pairs(:,2),:);
    r = sqrt(sum(delta.^2,2)) - d;

end