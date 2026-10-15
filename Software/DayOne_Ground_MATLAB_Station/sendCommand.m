function sendCommand(c, s)
    tx = sprintf('%d,', int32(c));
    tx(end) = [];

    writeline(s, tx);

    fprintf('Send command:\n%s\n', tx);

end