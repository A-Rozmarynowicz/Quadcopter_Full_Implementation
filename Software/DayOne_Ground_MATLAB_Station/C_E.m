function C_E(s)
% Emergency Total Stop command: short name because it needs to be quickly
% accessible by human.
    Send_Command(Commands.QUADCOPTER_EMERGENCY_TOTAL_STOP, s, []);
end