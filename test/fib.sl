func main
    &cs x 0
    &cs y 1
    &cs z 0
    &cs i 0

    #MACRO &n 20

    loop: &sd copy i &n < branch endloop
        &s i dup &num :print [+ 1]
        " : " &string :print
        copy z &num :print &newline
        &s z pop copy y
        &s y copy x +
        &s x pop copy z
        goto loop
    endloop:
        "Done!" &string :print &newline
endfunc