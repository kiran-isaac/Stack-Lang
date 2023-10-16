func fib_recur
    dup 2 >= branch small
        &cs res
            1 copy default - dup &num :print " => " &string :print :fib_recur dup &num :print &newline
            2 copy default - dup &num :print " => " &string :print :fib_recur dup &num :print &newline
            +
        &sd bring res
        goto end
    small: 1
end: endfunc  

4 :fib_recur &num :print &newline

// &cs i 0
// loop_start: &s i 1 + &sd copy i dup 10 < branch end
// :fib_recur &num :print &newline
// goto loop_start
// end: :dump 0 :exit