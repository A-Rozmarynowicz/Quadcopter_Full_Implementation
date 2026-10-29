clear;

port = "COM8";
baudrate = 115200;

s = openPort(port, baudrate);

pause(1);

Listen(s)
Enable_Distance_Requests(s)
d = readDistancesESP32(s)
[P, dFit, residuals, rmse] = estimatePoints3D(d)
Visualize_Lighthouses(P, d, 5)

Send_Positions_ESP32(P, s, 5)

MATLAB_Transmission_Correct(s)

% d = readDistancesESP32(s);
% 
% [P, dFit, residuals, rmse] = estimatePoints3D(d);
% 
% disp('Estimated coordinates (X, Y, Z):');
% disp(P);
% fprintf('Distance RMSE: %.6f\n', rmse);
% 
% sendPositionsESP32(P, s);
% 
% closePort(s);