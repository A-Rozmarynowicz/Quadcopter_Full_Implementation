function C_Change_UWB_Settings(s, receiver, channel, reset_time, response_delay)
    Send_Command(Commands.LGHS_UWB_SETTINGS_SET, s, [receiver, channel, response_delay, reset_time]);
end