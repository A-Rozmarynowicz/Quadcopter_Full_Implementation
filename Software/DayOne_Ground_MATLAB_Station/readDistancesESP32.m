function d = readDistancesESP32(s)
%   d : [d12; d13; d14; d23; d24; d34]

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
            fprintf('d12 = %.4f\n', d(1));
            fprintf('d13 = %.4f\n', d(2));
            fprintf('d14 = %.4f\n', d(3));
            fprintf('d23 = %.4f\n', d(4));
            fprintf('d24 = %.4f\n', d(5));
            fprintf('d34 = %.4f\n', d(6));

            return;
        else
            fprintf('Ignoring invalid line: %s\n', line);
        end
    end
end