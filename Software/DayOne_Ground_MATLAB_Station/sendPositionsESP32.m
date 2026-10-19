function sendPositionsESP32(P, s, N)
% Send 4 points (4x3 matrix) to ESP32 over UART.

    validateattributes(P, {'numeric'}, ...
        {'real','finite','size',[N,3]});

    % Flatten coordinates row by row
    values = reshape(P.', 1, []);

    % Format and transmit
    tx = sprintf('%d,', int32(Commands.SEND_POSITIONS));
    tx = [tx, sprintf('%.2f,', values(1:3*N))];
    tx(end) = [];

    writeline(s, tx);

    fprintf('Positions sent to ESP32:\n%s\n', tx);

    while true
        line = strtrim(readline(s));

        if isempty(line)
            continue;
        end

        values = sscanf(line, '%d');

        if isscalar(values) && all(isfinite(values)) ...
                && all(values > 0)

            if values == Commands.ACK_RECEIVED_POSITIONS
                fprintf("Received acknowledgement of position transfer\n");
            else
                fprintf('ERROR: Acknowledgement of position transfer went wrong: %d\n', values);
            end

            return;
        else
            fprintf('Ignoring invalid line: %s\n', line);
        end
    end
end