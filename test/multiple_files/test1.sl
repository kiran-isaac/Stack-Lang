func factorial_recur
// n -- n`
    dup 1 >= branch end
        dup -1 + :factorial_recur *
    end:
endfunc