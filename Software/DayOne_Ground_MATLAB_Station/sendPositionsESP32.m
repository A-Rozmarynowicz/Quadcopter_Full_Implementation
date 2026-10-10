function sendPositionsESP32(P, s)
% Send 4 points (4x3 matrix) to ESP32 over UART.

    validateattributes(P, {'numeric'}, ...
        {'real','finite','size',[4,3]});


    % Flatten coordinates row by row
    values = reshape(P.', 1, []);

    % Format and transmit
    tx = sprintf('%.6f,', values);
    tx(end) = [];

    writeline(s, tx);

    fprintf('Positions sent to ESP32:\n%s\n', tx);
end