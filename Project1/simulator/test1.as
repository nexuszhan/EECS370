        lw      0       1       A
        lw      0       2       Num
        lw      0       3       B
        jalr    3       7       
        halt
Five    lw      0       6       neg1
        add     0       2       5
start   add     2       6       2
        beq     3       0       skip
        add     5       6       5
skip    beq     0       2       end
        beq     0       0       start
end     jalr    7       3
A       .fill   4
neg1    .fill   -1
Num     .fill   5
B       .fill   Five
