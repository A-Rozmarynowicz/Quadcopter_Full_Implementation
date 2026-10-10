d = readDistancesESP32("COM5", 115200);

[P, dFit, residuals, rmse] = estimatePoints3D(d);

disp('Estimated coordinates (X, Y, Z):');
disp(P);

fprintf('Distance RMSE: %.6f\n', rmse);