University of Maryland, Baltimore County 
CMPE/ENEE 491/691 
Hardware Security 
Spring 2023 
HW 5: Differential Fault Attack (DFA)
Jordan Kieltyka

Prerequisites: gcc with C11 compiler using Linux

Build and Test:
    1) To build, use command 'make dfa' in the 'project root' directory.
    2) To perform parsing, use command:
       './build/bin/dfa ./res/sbox.txt ./data/in.txt ./data/out.txt' 
       in the 'project root' directory.
    3) Output will be in 'out.txt' in the 'data' directory.

Notes
    * Simply using the command 'make' will build everything.
    * To clean the 'build/bin' directory, use the command 'make clean'.
    * confirm results using: https://www.cryptool.org/en/cto/aes-step-by-step