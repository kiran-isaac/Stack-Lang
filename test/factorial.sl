// Factorial solved using recursion
func factorial_recur
// n -- n`
    dup 1 >= branch end
        dup -1 + :factorial_recur *
    end:
endfunc

// Factorial solved using a loop
func factorial_loop
// n -- n`
    &cs accumulate 1
    loop: &sd dup 1 >= branch end 
        &s accumulate copy default *
        &sd -1 +
        goto loop
    end:
        &sd pop bring accumulate
endfunc

func main
    10 :factorial_recur &num :print &newline
    10 :factorial_loop &num :print &newline
endfunc