function listener_timer = Setup_Auto_Listener(s)

    listener_timer = timer( ...
        'ExecutionMode', 'fixedRate', ...
        'Period', 0.2, ...
        'BusyMode', 'drop', ...
        'TimerFcn', @(~,~) Auto_Listen(s));
    
    start(listener_timer);
    
    disp("Auto listener started.");
end

function Auto_Listen(s)

    if ~isvalid(s)
        return;
    end

    if s.NumBytesAvailable > 0
        line = strtrim(readline(s));
        disp(line);
    end

end