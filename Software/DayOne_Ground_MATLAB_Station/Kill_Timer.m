function Kill_Timer(t)
    % If the handle is not available, use:
    % stop(timerfind)
    stop(t);
    delete(t);
    clear t;
end