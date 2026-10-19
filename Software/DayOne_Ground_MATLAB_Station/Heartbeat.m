function Heartbeat(s)
    if ~isempty(s) && isvalid(s)
        writeline(s, "HEARTBEAT");
    end
end