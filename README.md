# Bask

Bask is a stack based programming language, that compiles to bytecode and is then run using the bask vm (for now the compiler and the vm are part of the same executable). It does not yet have support for functions, however it does support macros. 

This project is in very early stages. It has currently only been tested on Ubuntu. I will update this in the future with windows build instructions and/or releases.

Bask has some similarities to forth, but:
- Multiple stacks
- Ability to create "scoped stacks", which are deleted after a function has finished executing

## Usage

A bask program is compiled to bytecode which is interpreted by the bask virtual machine. A bask program called "main.bsk" would be compiled and output to "main.bsk.byte" using the command below. 
```bash
bask compile main.bsk.byte main.bsk
```
"main.bsk.byte" can then be run using the virtual machine

```bash 
bask run main.bsk.byte
```


## Building Standard Library
The bask virtual machine needs a referance to the standard library, which is written in bask compiled as bask bytecode. This can be achieved by using the "bask" command with the -std 