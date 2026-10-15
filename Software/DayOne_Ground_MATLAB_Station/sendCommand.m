function sendCommand(c, s, values)
    tx = sprintf('%d,', int32(c));
    if (~isempty(values))
        tx = [tx, sprintf('%0.2f,', values(:))];
    end
    tx(end) = [];

    writeline(s, tx);

    fprintf('Send command:\n%s\n', tx);

end