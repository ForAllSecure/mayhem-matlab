function [] = mayhemit(word)
    % Convert the input to a char array if it's a string
    if isstring(word)
        word = char(word);
    end
 
    % Check if the input is a char array
    assert(ischar(word), 'Input must be a char array or string.');
 
    if length(word) >= 1 && word(1) == 'b'
        if length(word) >= 2 && word(2) == 'u'
            if length(word) >= 3 && word(3) == 'g'
                fprintf('Bug here!\n');
                len = 1e7;
                for i = 1:len
                    m = mod(length(word), i) + 1;
                    word(i) = word(m);
                end
            end
        end
    end
    fprintf('%s\n', word);
end
