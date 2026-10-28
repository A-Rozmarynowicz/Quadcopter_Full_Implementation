function s = Open_Port(port, baudrate)

    s = serialport(port, baudrate);
    configureTerminator(s, "LF");
    s.Timeout = 50;
    
    flush(s);
    
    % cleanupObj = onCleanup(@() delete(s));
end