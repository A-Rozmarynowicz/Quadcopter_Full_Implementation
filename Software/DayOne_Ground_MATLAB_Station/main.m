clear;

port = "COM8";
baudrate = 115200;

s = openPort(port, baudrate);

pause(1);

% Listen(s)
% C_Enable_Distance_Requests(s)
% d = readDistancesESP32(s)
% [P, dFit, residuals, rmse] = estimatePoints3D(d)
% Visualize_Lighthouses(P, d, 5)
% 
% Send_Positions_ESP32(P, s, 5)
% 
% C_MATLAB_Transmission_Correct(s)

