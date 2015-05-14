# LilChip3000
LilChip3000 is a C based virtual machine. It is currently very limited and uses a custom instruction set. It has 6 general purpose registers as well as 3 scratch registers, the instrustion pointer, stack pointer, and the link register. The "compiler" is written in python. It takes the assembly like syntax of the source code and translates the instructions and registers into their corresponding integers.

### Example
The virtual machine uses an assembly like syntax (non case sensitive) to write programs:

    PSH 5 ; Push 5 onto the stack
    PSH 6 ; Push 6 onto the stack
    ADD   ; Add the two numbers on the top of the stack and push the result
    POP   ; Pop the top item on the stack and print it to the console

##### Compiling
Use the `compile.py` script to "compile" the code

    python compile.py myProgram.txt

You can specify an output file name as the second argument. Otherwise, your program will be saved as `program.chip`. We call this a `chip file`.

##### Running
Compile the virtual machine using the provded makefile. You can then run your compiled program using the following:

    ./LilChip3000 program.chip

##### Hello World Program

    PSH 10  ; \n
    PSH 33  ; !
    PSH 100 ; d
    PSH 108 ; l
    PSH 114 ; r
    PSH 111 ; o
    PSH 87  ; W
    PSH 32  ; ' '
    PSH 111 ; o
    PSH 108 ; l
    PSH 108 ; l
    PSH 101 ; e
    PSH 72  ; H
    
    SET B 13 ; Loop 13 times (i)

    Loop: POPR A
          PUTC A
          SUBI B 1 ; i--
          JNZ B Loop
    
    HLT ; Goodbye...

or...

    ====
    string hello "Hello, World!"
    ====
    
    LDA A hello
    PUTS A
    HLT
    
For full documentation on the instruction set for the LilChip3000, please see the [wiki](https://github.com/SlayterDev/LilChip3000/wiki)
