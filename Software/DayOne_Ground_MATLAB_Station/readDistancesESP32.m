function d = readDistancesESP32(s)
%   d : [d12; d13; d14; d23; d24; d34]

    tx = sprintf('%d,', Commands.REQUEST_MEASUREMENTS);
    tx(end) = [];

    writeline(s, tx);

    fprintf('Requested measurements:\n%s\n', tx);

    while true
        line = strtrim(readline(s));

        if isempty(line)
            continue;
        end

        values = sscanf(line, '%f,%f,%f,%f,%f,%f');

        if numel(values) == 6 && all(isfinite(values)) ...
                && all(values > 0)

            d = values(:);

            fprintf('Received averaged distances:\n');
            fprintf('d01 = %.4f\n', d(1));
            fprintf('d02 = %.4f\n', d(2));
            fprintf('d03 = %.4f\n', d(3));
            fprintf('d12 = %.4f\n', d(4));
            fprintf('d13 = %.4f\n', d(5));
            fprintf('d23 = %.4f\n', d(6));

            return;
        else
            fprintf('Ignoring invalid line: %s\n', line);
        end
    end
end