clear;

port = "COM8";
baudrate = 115200;
s = serialport(port, baudrate);

pause(1);

d = readDistancesESP32(s);

[P, dFit, residuals, rmse] = estimatePoints3D(d);

disp('Estimated coordinates (X, Y, Z):');
disp(P);
fprintf('Distance RMSE: %.6f\n', rmse);

sendPositionsESP32(P, s);

delete(s);
clear s;