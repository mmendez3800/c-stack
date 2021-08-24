# Java Turtles
This project implements a stack data structure using C and allows the user to perform multiple actions

## General
When the user starts the project, they will be able to perform the below operations through the terminal. To close the program, the user will enter `^D` in terminal.
 * a - allocate memory to the stack
 * d - deallocate memory to the stack
 * u - pushes to the stack by adding a new element to it
 * p - pops the stack by removing the top element
 * t - displays the top element of the stack
 * i - checks if the stack is empty
 * e - deletes every element in the stack
 * f - checks if the stack is full
 * n - displays the number of elements in the stack
 * w - displays the elements of the stack in `stdout`
 * W - displays the elements of the stack in `stderr`

## Technologies
The programs in this project were run using the following:
* GCC 9.3

## Setup
After cloning or forking the repository, you can run the program through the command line in the below manner:
1. You will want to `cd` into the repository
2. Compile all of the `*.c` files present
   - `gcc driver.c stack.c mylib.c`
3. Run the executable created
   - `./a.out`
