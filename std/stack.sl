func dump
    loop_start: :stack_len 0 == branch endloop pop goto loop_start endloop:
endfunc

func shift 
    &cs temp bring default 
    &sd :reverse bring temp :reverse
endfunc

func shift_n
    &cs n bring default 1 +
    &cs temp

    loop_start: (copy n :stack_len == branch reverse_temp)
        &s temp bring default
        goto loop_start
    reverse_temp: &s temp :reverse endloop: (&s temp 1 :stack_len == branch end)
        &sd :reverse bring temp :reverse
        goto endloop
    end:
endfunc