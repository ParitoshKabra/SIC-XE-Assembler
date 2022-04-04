SIC-XE-Assembler
Author: Paritosh Kabra

B.Tech C.S.E. 2nd Year

Indian Institute of Technology Roorkee.

This is the project for the Systems Software course, Spring 2022.

The requirement was to build a two-pass assembler for the SIC-XE machine in C/C++. This is a SIC/XE Assembler with implementation of :

Basic Assembler.
Input format free.
Error handling.
Literals/org/equ support
Program Blocks.
Control Sections.
Input :

Freely formatted SIC/XE assembly program.
Output:

Object Code
List File
Symbol Table
Error Report (if neccessary)
Current Error:

Addressing mode.
Current Section name is not written in object record.
Instructions :

# NOTE:

    - For instructions like LDCH BUFFER, X --> there should be a , and a space between BUFFER and X.
    - No spaces in EQU BUFFEND-BUFFER+SOME_LABEL (No spaces in the expression!)

# Clone this repository

git clone https://github.com/ParitoshKabra/SIC-XE-Assembler.git

# Go into repository

cd SIC-XE-Assembler/

# Compile Files into executable

g++ .\transpile.cpp .\parser.cpp .\opTab.cpp .\symTab.cpp .\utils.cpp .\Pass1.cpp
.\Pass2.cpp -o Assembler

Now use assembler to convert your SIC-XE program :)

# Example

./Assembler.exe assemblyCode.in

# Output will be generated in assemblyCode.out

It will include _intermediate_, _listing_ and _object program_ headings.

TODO:
Freely Formatting in Operations in EQU.
Handle invalid prepended hex characters in Byte and WORD.
Handle certain small errors mentioned in comment.
Handle Control Sections.

This is compiled using `gcc version 8.1.0 (i686-posix-dwarf-rev0, Built by MinGW-W64 project)`

Issue
If you find any error, feel free to open an issue.

Contribute
To contribute to this repository, you may make a pull request.
