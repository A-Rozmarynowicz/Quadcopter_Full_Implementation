function EL(s)
% Emergency Landing command: short name because it needs to be quickly
% accessible by human.
    Send_Command(Commands.QUADCOPTER_EMERGENCY_LANDING, s, []);
end