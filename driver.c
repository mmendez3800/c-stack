/*****************************************************************************

File Name:      driver.c
Description:    This program allows us to test the code that we have written
                for the stack in stack.c

*****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "mylib.h"
#include "stack.h"

int main (int argc, char * const * argv) 
{
    Stack * main_Stack = 0;         /* the test stack */
    unsigned long amount;        /* numbers of items possible go on stack */
    long command;                   /* stack command entered by user */
    long item = 0;                  /* item to go on stack */
    char option;                    /* the command line option */
    long status;                    /* return status of stack functions */
        
    /* initialize debug states */
    debug_off ();

    /* check command line options for debug display */
    while ( (option = getopt (argc, argv, "x") ) != EOF ) 
    {
        switch (option) 
        {
            case 'x': debug_on (); 
            break;
        }
    }

    while (1) 
    {
        command = 0;            /* initialize command, need for loops */
        writeline ("\nPlease enter a command:", stdout);
        writeline ("\n\t(a)llocate, (d)eallocate, ", stdout);
        writeline ("p(u)sh, (p)op, (t)op, (i)sempty, (e)mpty, ",stdout);
        writeline ("\n\tis(f)ull, (n)um_elements,", stdout);
        writeline (" (w)rite to stdout, (W)rite to stderr.\n", stdout);
        writeline ("Please enter choice:  ", stdout);
        command = getchar ();
        if (command == EOF)     /* are we done? */
        {
            break;
        }
        clrbuf (command);       /* get rid of extra characters */

        switch (command)       /* process commands */
        {
            case 'a':               /* allocate */
                writeline ("\nPlease enter the number of objects to", stdout);
                writeline (" be able to store: ", stdout);
                amount = decin ();
                
                /* If statement executed when stack already exists */
                if (main_Stack)
                {
                    delete_Stack (&main_Stack);     /* deletes stack */
                }

                main_Stack = new_Stack (amount);    /* creates a new stack */

                clrbuf (amount + ' ');
                break;

            case 'd':               /* deallocate */
                /* deallocated memory from the stack */
                delete_Stack(&main_Stack);  

                writeline ("Stack has been deleted\n", stdout);
                break;

            case 'e':               /* empty */
                empty_Stack (main_Stack);      /* empties stack */

                writeline ("Stack is empty.\n", stdout);
                break;    

            case 'f':               /* isfull */
                if ( isfull_Stack (main_Stack) )
                {
                    writeline ("Stack is full.\n",stdout);
                }
                else
                {
                    writeline ("Stack is not full.\n", stdout);
                }
                break;

            case 'i':               /* isempty */
                if ( isempty_Stack (main_Stack) )
                {
                    writeline ("Stack is empty.\n", stdout);
                }
                else
                {
                    writeline ("Stack is not empty.\n", stdout);
                }
                break;

            case 'n':               /* num_elements */
                writeline ("Number of elements on the stack is:  ", stdout);
                decout ( num_elements (main_Stack) );
                newline ();
                break;

            case 'p':               /* pop */
                status = pop (main_Stack, &item);
                if (! status)
                {
                    fprintf (stderr,"\nWARNING:  pop FAILED\n");
                }
                else 
                {
                    writeline ("Number popped from the stack is:  ", stdout);
                    decout (item);
                    newline ();
                }
                break;

            case 't':               /* top */
                status = top (main_Stack, &item);
                if (! status)
                {
                    fprintf (stderr,"\nWARNING:  top FAILED\n");
                }
                else 
                {
                    writeline ("Number at top of the stack is:  ", stdout);
                    decout (item);
                    newline ();
                }
                break;

            case 'u':               /* push */
                writeline ("\nPlease enter a number to push to stack:  ",
                           stdout);
                item = decin ();
                clrbuf (0);     /* get rid of extra characters */
                status = push (main_Stack, item);
                if (! status)
                {
                    fprintf(stderr,"\nWARNING:  push FAILED\n");
                }
                break;

            case 'w':               /* write */
                writeline ("\nThe Stack contains:\n", stdout);
                write_Stack (main_Stack, stdout);
                break;

            case 'W':               /* write */
                writeline ("\nThe Stack contains:\n", stdout);
                write_Stack (main_Stack, stderr);
                break;
        }

        if (item == EOF) /* check if done */
        {
            break;
        }
    }

    if (main_Stack)
    {
        delete_Stack (&main_Stack);     /* deallocate stack */
    }
    newline ();
    return 0;
}

