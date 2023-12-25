one     noop    1       2       3
two     jalr    0       1       5
start   beq     2       3       end
        beq     1       0       done
three   add     1       2       1
        beq     0       1       start
done    halt    3       7       0
end     .fill   -100
