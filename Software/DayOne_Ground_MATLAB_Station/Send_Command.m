function Send_Command(c, s, values)
    tx = sprintf('%d,', int32(c));
    if (~isempty(values))
        tx = [tx, sprintf('%0.2f,', values(:))];
    end
    tx(end) = [];

    if ~isempty(s) && isvalid(s)
        writeline(s, tx);
        fprintf('Send command:\n%s\n', tx);
    else
        fprintf('Serial not avaiable');
    end
   
end