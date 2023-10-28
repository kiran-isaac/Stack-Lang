func main
    "HELLO WORLD!" &string :print '\n' &char :print
    // Each character is pushed to the default stack (in reverse order)
    // The code for a string is then pushed
    // The string is then printed
    // The newline character is then printed

    // Macros find and replace text in the source code
    #MACRO &HELLO "HELLO FROM A MACRO!" &string :print '\n' &char :print
    &HELLO
endfunc