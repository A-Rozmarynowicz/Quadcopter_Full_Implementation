clear;

port = "COM8";
baudrate = 115200;

s = Open_Port(port, baudrate);

pause(1);

% C_Ground_Station_Continue_From_Initial_State(s)

% Listen(s)
% C_Enable_Distance_Requests(s)
% d = Read_Distances_ESP32(s, 5)
% [P, dFit, residuals, rmse] = Estimate_LGHS_Positions_lsqnonlin(d)
% Visualize_Lighthouses(P, d, 5)
% 
% Send_Positions_ESP32(P, s, 5)
% 
% C_MATLAB_Transmission_Correct(s)

% C_Change_UWB_Settings(s, 255, 5, 1000, 2000)