func str_len
    &cs n 0
    &cs input clone default
    loop_start: 0 == branch endloop
        &s n 1 + &s input
        goto loop_start
    endloop:
        &sd bring n 1 +
endfunc

func str_pop
    &cs n clone default :str_len 1 +
    &cs i 0
    &sd

    loop_start: copy i copy n == branch endloop
        &s i 1 +
        &sd pop
        goto loop_start
    endloop: 
endfunc

func str_shift
    :str_len :shift_n
endfunc