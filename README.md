# Slang

Slang is a stack based programming language, that is bytecode interpreted. It is absolutely awful and very difficult to write, it is not recommended to use this for anything other than messing around.

## Usage

A Slang program is compiled to bytecode which is interpreted by the Slang virtual machine. A Slang program called "main.sl" would be compiled and output to "main.out", and then run by the virtual machine using the command below. 

```bash
slang compile main.sl -o a.out
```

More information on the Slang CLI can be found by running:
```bash
slang help
```

## Building the Standard Library
Slang has a standard library compiled as Slang bytecode. This can be achieved by using the -std argument, or by putting the lib at /usr/lib/Slang/stdlib where the program will automatically search.

The standard library can be compiled using:
```bash
cd std
Slang compile -o /usr/lib/Slang/stdlib stack.sl str.sl
```

More standard library modules will be added in future

# Programming in Slang

A Slang program consists of a set of functions. Everything outside a function definition is treated as if it is part of the main function. There cannot be two main functions, or two functions of the same name. Below is a basic Slang program.

```
10 dup * &num :print 
```
- 10 : push 10 onto the stack
- dup : duplicate the last thing on the stack
- \* : multiply the last thing on the stack
- &num : is a macro, that expands to the data type value code, which in this case is 0. This (of course) is pushed to the stack.
- :print : a function call to the inbuilt "print" function, which will pop the last value off of the stack, and as it is 0 (representing a number) it pops another item off the stack and prints it as if it was a number.

The program outputs 100. 

If the program was changed to look like this:
```
10 dup * &char :print
```
It would output "d" to the console.

## Stacks
Slang has a concept of stacks. The default stack is the one that is used when no other stack is specified. The current stack can be 

## Macros
Users can define macros using the #MACRO keyword, followed by the id of the macro (starting with &). Macros are "expanded" by the preprocesser. 

The example program from above can be modified to use macros.
```swift
#MACRO &square dup *
#MACRO &print_num &num :print 

10 &square &print_num
```
Macros should be used for short sequences of instructions that are likely to be reused.

Macros can use other macros in their definitions, but only those defined before themselves.

The language includes some macros to make common activities easier to read and write:
- &string / &char / &num : the IDs of the respective datatypes
- &s : switch stack (&s x switches to stack x)
- &sd : switches to the default stack for the scope
- &c : create a stack (&c x creates stack x if it doesnt already exists)
- &cs : create and switch to stack
- &newline : outputs a newline to the console

## Functions
Slang functions are created using the func and endfunc keywords. The currently active stack in the current scope becomes the default. 

Below are two definitions of a factorial function.

```swift
func factorial_recur
// n -- n`
    dup 1 >= branch end
        dup -1 + :factorial_recur *
    end:
endfunc

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

10 :factorial_recur &num :print &newline
// Outputs 362800

10 :factorial_loop &num :print &newline
// Outputs 362800
```

Note the (some could call them type annotation) comments in the function, saying that it eats one value, and pushes a new value