        lw      0       2       mcand
        lw      0       3       mplier
        lw      0       4       judge
        lw      0       5       goal
        lw      0       7       subtr
start   beq     4       5       end     //end when judge=2^15
        lw      0       6       cmp
        add     6       7       6
        add     7       7       7
        nor     3       6       6
        beq     4       6       skip
        add     2       1       1       //add mcand to the result
skip    add     2       2       2
        add     4       4       4
        beq     0       0       start
end     halt
mcand   .fill   1103
mplier  .fill   7043
cmp     .fill   -1
subtr   .fill   -1
judge   .fill   1
goal    .fill   32768
