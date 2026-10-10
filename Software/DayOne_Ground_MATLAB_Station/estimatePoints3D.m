function [P, dFit, residuals, rmse] = estimatePoints3D(d)
% INPUT:
%   d       6-element vector of measured distances, ordered as:
%           [d12; d13; d14; d23; d24; d34]
%
% OUTPUT:
%   P         4x3 matrix of estimated coordinates
%   dFit      6x1 vector of fitted distances
%   residuals 6x1 vector (fitted - measured)
%   rmse      Root mean square distance residual

    validateattributes(d, {'numeric'}, ...
        {'real','finite','vector','numel',6,'nonnegative'});

    d = double(d(:));

    if any(d <= 0)
        error('All six measured distances must be positive.');
    end

    if exist('lsqnonlin','file') ~= 2
        error('This function requires the Optimization Toolbox.');
    end

    pairs = [1 2;
             1 3;
             1 4;
             2 3;
             2 4;
             3 4];

    lb = [1e-9; -Inf; 1e-9; -Inf; -Inf; -Inf];
    ub = Inf(6,1);

    opts = optimoptions('lsqnonlin', ...
        'Display','off', ...
        'MaxFunctionEvaluations',10000, ...
        'MaxIterations',2000);

    rng(42);
    bestTheta = [];
    bestCost = Inf;

    for k = 1:100
        if k == 1
            theta0 = [d(1); 0; d(2); 0; 0; 0];
        else
            theta0 = [max(d(1),0.1); 4*randn(5,1)];
            theta0(3) = abs(theta0(3)) + 0.1;
        end

        [theta, resnorm] = lsqnonlin( ...
            @(t) distanceResiduals(t,d,pairs), ...
            theta0,lb,ub,opts);

        if resnorm < bestCost
            bestCost = resnorm;
            bestTheta = theta;
        end
    end

    t = bestTheta;

    P = [0,     0,     0;
         t(1),  0,     0;
         t(2),  t(3),  0;
         t(4),  t(5),  t(6)];

    dFit = zeros(6,1);

    for k = 1:6
        i = pairs(k,1);
        j = pairs(k,2);
        dFit(k) = norm(P(i,:) - P(j,:));
    end

    residuals = dFit - d;
    rmse = sqrt(mean(residuals.^2));

end


function r = distanceResiduals(t,d,pairs)

    P = [0,     0,     0;
         t(1),  0,     0;
         t(2),  t(3),  0;
         t(4),  t(5),  t(6)];

    r = zeros(6,1);

    for k = 1:6
        i = pairs(k,1);
        j = pairs(k,2);
        r(k) = norm(P(i,:) - P(j,:)) - d(k);
    end

end