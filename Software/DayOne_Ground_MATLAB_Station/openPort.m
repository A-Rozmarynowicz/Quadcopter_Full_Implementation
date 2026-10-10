function s = openPort(port, baudrate)

    s = serialport(port, baudrate);
    configureTerminator(s, "LF");
    s.Timeout = 30;
    
    flush(s);
    
    cleanupObj = onCleanup(@() delete(s));
end