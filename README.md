# Slang

Slang is a bytecode interpreted stack based programming language. It is very complicated to write, and full of bad design decisions! It is not recommended to use this for anything other than messing around. However it was very fun to create! This was my first large c++ project, and I was 17 at the time, hence the code being a mess. However it is a fun language to mess around with.

# Setup
## Support
Slang is only supported on Linux. It may work on other platforms, but it has not been tested.
## Building and Installation
Slang uses CMake to build. Run this to install all the dependencies (on ubuntu):
```bash
sudo apt install cmake build-essential
```

The following commands can be used to build the project.

```bash
mkdir build
cd build
cmake ..
make
```

To install it:
```bash
cd build
sudo ln -s slang /usr/bin/slang
```

### Building the Standard Library
Slang has a standard library compiled as Slang bytecode. This can be achieved by using the -std argument, or by putting the lib at /usr/lib/Slang/stdlib where the program will automatically search.

The standard library can be compiled using:
```bash
cd std
slang compile -o stdlib stack.sl str.sl
sudo mv stdlib /usr/lib/slang/stdlib
```

More standard library modules will be added in future

# Usage

A Slang program is compiled to bytecode which is interpreted by the Slang virtual machine. A Slang program called "main.sl" would be compiled and output to "a.out", and then run by the virtual machine using the command below. 

```bash
slang compile main.sl -o a.out
```

More information on the Slang CLI can be found by running:
```bash
slang help
```

# Programming in Slang
## Syntax

Slang is a stack based programming language. This means that all operations are performed on a stack. The stack is a LIFO (last in first out) data structure. The last thing to be pushed onto the stack is the first thing to be popped off of the stack.

Slang is a whitespace insensitive language. This means that whitespace is ignored, and the only way to separate tokens is by using a space. 

Slang is a postfix language. This means that the operator comes after the operands. For example, in the expression 1 + 2, the operator is +, and the operands are 1 and 2. In Slang, this would be written as 1 2 +. The only exception to this is the name of any stacks being operated on, which comes after the operator (I know this is confusing, but it makes sense when you see it in action).

A Slang program consists of a set of functions. Everything outside a function definition is treated as if it is part of the main function. There cannot be two main functions, or two functions of the same name. Below is a basic Slang program.

```
10 dup * &num :print 
```
- 10 : push 10 onto the stack
- dup : duplicate the last thing on the stack
- \* : multiply the last thing on the stack
- &num : is a macro, that expands to the data type value code, which in this case is 0. This (of course) is pushed to the stack.  See the section on data types for more information.
- :print : a function call to the inbuilt "print" function, which will pop the last value off of the stack, and as it is 0 (representing a number) it pops another item off the stack and prints it as if it was a number.

The program outputs 100.

### Keywords
Stack manipulation:
- create [stackname] : creates a new stack
- switch [stackname] : switches to the specified stack
- Bring [stackname] : brings the top item in the specified stack to the current stack
- copy [stackname] : copies the top item in the specified stack to the current stack
- clone [stackname] : clones all the items in the specified stack and puts them the current stack in the same order they were in in the original stack
- pop [stackname] : pops the top item off of the specified stack
- dup : duplicates the top item on the stack

Control flow:
- branch [label] : if the top item on the stack is not 0 (i.e. it evaluates to true), jump to the specified label.
- goto [label] : jump to the specified label

### Data Types
The first argument to many programms is the data type it is intended to operate on. This is because Slang is a dynamically typed language, and the data type of the last thing on the stack is not known at compile time. 

The 4 data types are:
- 0 | &num : num
- 1 | &char : char
- 2 | &bool : bool
- 3 | &string : string

The numbers themselves can be used instead of the macros, but the macros are recommended as they are easier to read. (See the section on macros for more information)

## Stacks
Slang (stack lang) suprisinly is mostly based on stacks. The default stack is the one that is used when no other stack is specified. A stack can contain any of the 4 data types, and can be created using the &c macro.

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