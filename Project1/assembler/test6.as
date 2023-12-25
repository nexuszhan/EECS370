        lw      0       1       One
        lw      0       2       Two
        sw      0       2       Two2
        nor     1       2       3
        noop
        lw      0       3       Two2
        add     0       3       1
        halt
        add     1       1       1
One     .fill   1
Two     .fill   2
Two2    .fill   0