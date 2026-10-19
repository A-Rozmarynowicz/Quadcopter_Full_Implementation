function d = Read_Distances_ESP32(s, N)
% d : vector of pairwise distances, ordered as:
%     d01, d02, ..., d0(N-1), d12, ..., d(N-2)(N-1)
%
% Number of measurements: N*(N-1)/2

    if N < 2 || N ~= floor(N)
        error('N must be an integer >= 2.');
    end

    numDistances = N * (N - 1) / 2;

    % Request measurements
    tx = sprintf('%d,', Commands.REQUEST_MEASUREMENTS);
    tx(end) = [];

    writeline(s, tx);

    fprintf('Requested measurements:\n%s\n', tx);

    while true
        line = strtrim(readline(s));

        if isempty(line)
            continue;
        end

        % Parse command ID and all distance values
        values = sscanf(line, '%f,').';

        if numel(values) == numDistances + 1 ...
                && all(isfinite(values)) ...
                && all(values >= 0)

            if values(1) ~= Commands.SEND_DISTANCES
                fprintf('Wrong command received in SEND_DISTANCES format.\n');
                continue;
            end

            d = values(2:end).';

            % Display distances in the expected order
            fprintf('Received averaged distances:\n');

            k = 1;
            for i = 0:N-2
                for j = i+1:N-1
                    fprintf('d%d%d = %.4f\n', i, j, d(k));
                    k = k + 1;
                end
            end

            return;

        else
            fprintf('Message received: %s\n', line);
        end
    end
end