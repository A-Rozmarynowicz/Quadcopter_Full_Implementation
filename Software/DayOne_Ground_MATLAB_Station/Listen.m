function Listen(s)

    fprintf("Start listening...\n");
    while true
        line = strtrim(readline(s));

        if isempty(line)
            continue;
        end

        fprintf('Message received: %s\n', line);

    end

end