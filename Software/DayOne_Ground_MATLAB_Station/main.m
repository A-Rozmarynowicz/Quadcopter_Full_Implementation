d = [1; 1; 1; 1; 1; 1];

[P, dFit, residuals, rmse] = estimatePoints3D(d);

disp('Estimated coordinates:');
disp(P);

disp('Fitted distances:');
disp(dFit);

disp('Residuals:');
disp(residuals);

fprintf('Distance RMSE = %.6f\n', rmse);