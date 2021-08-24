/******************************************************************************

File Name:      stack.c
Description:    This program implements an array-based stack of longs that can
                deallocated all memory associated with the stack, empty the 
                stack of all its elements, check to see if the stack is empty
                or full, allocate memory into a new stack, count the number of
                elements in the stack, remove and add items on the top of
                the stack, and tells the user the top item in the stack.
                Program terminates when user enters ^D.

******************************************************************************/

#include <malloc.h>
#include <stdio.h>
#include "mylib.h"
#include "stack.h"

#define STACK_POINTER_INDEX (-1)        /* Index of last used space */
#define STACK_SIZE_INDEX (-2)           /* Index of size of the stack */
#define STACK_COUNT_INDEX (-3)          /* Index of which stack allocated */
#define STACK_OFFSET 3  /* offset from allocation to where user info begins */

/* catastrophic error messages */
static const char DELETE_NONEXIST[] = "Deleting a non-existent stack!!!\n";
static const char EMPTY_NONEXIST[] = "Emptying a non-existent stack!!!\n";
static const char INCOMING_NONEXIST[] = 
                        "Incoming parameter does not exist!!!\n";
static const char ISEMPTY_NONEXIST[] = 
                        "Isempty check from a non-existent stack!!!\n";
static const char ISFULL_NONEXIST[] = 
                        "Isfull check from a non-existent stack!!!\n";
static const char NUM_NONEXIST[] = 
                        "Num_elements check from a non-existent stack!!!\n";
static const char POP_NONEXIST[] = "Popping from a non-existent stack!!!\n";
static const char POP_EMPTY[] = "Popping from an empty stack!!!\n"; 
static const char PUSH_NONEXIST[] = "Pushing to a non-existent stack!!!\n";
static const char PUSH_FULL[] = "Pushing to a full stack!!!\n";
static const char TOP_NONEXIST[] = "Topping from a non-existent stack!!!\n";
static const char TOP_EMPTY[] = "Topping from an empty stack!!!\n";
static const char WRITE_NONEXIST_FILE[] = 
                    "Attempt to write using non-existent file pointer!!!\n";
static const char WRITE_NONEXIST_STACK[] = 
                        "Attempt to write to a non-existent stack!!!\n";

/* Debug messages. HEX messages used for negative numbers on the stack. */
static const char ALLOCATED[] = "[Stack %ld has been allocated]\n";
static const char DEALLOCATE[] = "[Stack %ld has been deallocated]\n";
static const char HEXPOP[] = "[Stack %ld - Popping 0x%lx]\n";
static const char HEXPUSH[] = "[Stack %ld - Pushing 0x%lx]\n";
static const char HEXTOP[] = "[Stack %ld - Topping 0x%lx]\n";
static const char POP[] = "[Stack %ld - Popping %ld]\n";
static const char PUSH[] = "[Stack %ld - Pushing %ld]\n";
static const char TOP[] = "[Stack %ld - Topping %ld]\n";

/* static variable allocation */
static int debug = FALSE; /* allocation of debug flag */
static int stack_counter = 0; /* number of stacks allocated so far */

/* Debug state methods */
void debug_off (void) 
{
        debug = FALSE;
}


void debug_on (void) 
{
        debug = TRUE;
}


/*----------------------------------------------------------------------------
Function Name:          delete_Stack
Purpose:                This function deletes a created stack
Description:            This function checks to see if the stack has not 
                        been set yet or if the pointer to the stack has not
                        yet been set yet. If so, an error message is printed.
                        If not, we then deallocate the memory to free the
                        stack
Input:                  spp: the stack from which we will deallocate memory
Result:                 Deletes the created stack or prints an error message
----------------------------------------------------------------------------*/
void delete_Stack (Stack ** spp) 
{
    /* If statement is executed if spp is a null pointer or if the pointer 
     * pointing to spp is not a null pointer */
    if (!spp || !*spp)
    {
        writeline (DELETE_NONEXIST, stderr);   /* error message printed */
        return;
    }

    /* If statement is executed if debug mode is on */
    if (debug)
    {
        fprintf (stderr, DEALLOCATE, (long)stack_counter);
    }

    /* code to deallocate the memory, set the pointer being pointed to NULL,
     * and decrement stack_counter */
    free (*spp - STACK_OFFSET);
    *spp = NULL;
    stack_counter--;
}


/*----------------------------------------------------------------------------
Function Name:          empty_Stack
Purpose:                This function empties a stack
Description:            This function check to see if the stack has been set
                        yet. If not, an error message is printed and the 
                        function ends. If the stack has been set, then the
                        while loop loops through all of the indexes of the 
                        stack, deleting every value until it reaches its 
                        initial index
Input:                  this_Stack: the stack which will be emptied
Result:                 Empties the items in the stack or prints an error 
                        message
----------------------------------------------------------------------------*/
void empty_Stack (Stack * this_Stack) 
{
    long pointerIndex = 0;  /* variable for original index of pointer index */

    /* If statement is executed when stack has not been set yet */
    if (!this_Stack)
    {
        writeline (EMPTY_NONEXIST, stderr);     /* error message printed */
        return;
    }

    /* While loop loops through all of the indexes in stack */
    while (this_Stack[STACK_POINTER_INDEX] != -1)
    {
        /* code used to remove item from the top of stack and to move to the
         * next index in the stack */ 
        pointerIndex = this_Stack[STACK_POINTER_INDEX];
        this_Stack[pointerIndex] = 0;
        this_Stack[STACK_POINTER_INDEX]--;
    }
}


/*----------------------------------------------------------------------------
Function Name:          isempty_Stack
Purpose:                This function checks to see if the stack is empty
Description:            This function acquires a stack and checks to see if it
                        is empty or not. It returns true if it is and returns
                        false if it does not
Input:                  this_Stack: the stack being checked
Result:                 True if the stack is empty, false if the stack is not
                        empty or an error occurs, and an error message prints
                        if an error is found
----------------------------------------------------------------------------*/
long isempty_Stack (Stack * this_Stack) 
{
    /* If statement is executed if the stack is not yet set */
    if (!this_Stack)
    {
        writeline (ISEMPTY_NONEXIST, stderr);      /* error message printed */
        return 1;
    }

    /* If statement is executed if the stack is empty */
    if (this_Stack[STACK_POINTER_INDEX] == -1)
    {
        return 1;
    }
    
    /* If neither of the two if statements were met, then the stack is not 
     * empty and we return false */
    return 0;
}


/*-----------------------------------------------------------------------------
Function Name:          isfull_Stack
Purpose:                This function checks to see if the stack is full
Description:            This function acquires a stack and checks to see if it
                        is full or not. It returns true if it is and returns
                        false if it does not
Input:                  this_Stack: the stack being checked
Result:                 True if the stack is full, false if the stack is not
                        full or an error occurs, and an error message prints
                        if an error is found
-----------------------------------------------------------------------------*/
long isfull_Stack (Stack * this_Stack) 
{     
    /* If statement is executed if the stack is not yet set */
    if (!this_Stack)
    {
        writeline (ISFULL_NONEXIST, stderr);   /* error message printed */
        return 0;
    }

    /* If statement is executed if the stack is full */
    if (this_Stack[STACK_POINTER_INDEX] >= this_Stack[STACK_SIZE_INDEX] - 1)
    {
        return 1;
    }
    
    /* If neither of the two if statements were met, then the stack is not
     * full and we return false */
    return 0;
}


/*-----------------------------------------------------------------------------
Function Name:          new_Stack
Purpose:                This function allocates memory to hold stacksize number
                        of longs, initializes the stack infrastructure, and
                        returns a pointer to the first storage space in the
                        stack
Description:            This function creates a pointer which takes the number 
                        of elements you want to hold and multiplies it by the 
                        size of long. A second pointer is then created because
                        of the offset where our previous pointer is added to
                        STACK_OFFSET. We then initialize the elements
                        corresponding to our stack and increment its counter to
                        track how many Stack data structures are allocated
Input:                  stacksize: number of longs allocated into memory
Result:                 A pointer that points to the address of the array where
                        user data allotment begins
-----------------------------------------------------------------------------*/
Stack * new_Stack (unsigned long stacksize) 
{
    /* void pointer used to allocate memory for the stack using the formula */
    void * memory = malloc ( (stacksize + STACK_OFFSET) * sizeof(long) );

    /* Stack pointer used for returning in function because of offset  */
    Stack * this_Stack = (Stack *)memory + STACK_OFFSET;

    /* new stack starts at an index of -1  */
    this_Stack[STACK_POINTER_INDEX] = -1;

    /* stores the size of stack  */
    this_Stack[STACK_SIZE_INDEX] = stacksize;

    /* increases the counter of stack */
    this_Stack[STACK_COUNT_INDEX]++;

    /* incrementation of stack counter to keep track of how many data 
     * structures are allocated */
    stack_counter++;

    /* If statement is executed when debug mode is on */
    if (debug)
    {
        fprintf (stderr, ALLOCATED, this_Stack[STACK_COUNT_INDEX]);
    }

    return this_Stack;
}


/*----------------------------------------------------------------------------
Function Name:          num_elements
Purpose:                This function returns the number of elements in stack
Description:            This function first checks to see if stack has been 
                        set yet. If not, it prints an error message and
                        returns false. If the stack passes the check, then we
                        return the number of elements using the stack pointer
                        index
Input:                  this_Stack: the stack we used to check the number of
                        elements in
Result:                 The number of elements if the stack has been set.
                        False if not and an error message is printed
----------------------------------------------------------------------------*/
long num_elements (Stack * this_Stack) 
{
    /* If statement is executed if the stack has not been set yet */
    if (!this_Stack)
    {
        writeline (NUM_NONEXIST, stderr);     /* error message printed */
        return 0;
    }

    return this_Stack[STACK_POINTER_INDEX] + 1;
}


/*----------------------------------------------------------------------------
Function Name:          pop
Purpose:                This function removes the top item in stack
Description:            This function checks to see if stack has been set yet
                        or if it is empty. If so, an error message is printed.
                        If not, then we obtain the top item in the stack and
                        remove it by making it equal to 0
Input:                  this_Stack: the stack from which we are getting our
                                    items
                        item: the number we will remove from the stack
Result:                 True if the removal of the top item was a success.
                        False if the stack has yet to be made or is empty
                        and an error message is printed
----------------------------------------------------------------------------*/
long pop (Stack * this_Stack, long * item) 
{
    long pointerIndex = 0;   /* variable for original index of pointer index */ 

    /* If statement is executed when the stack has not been set yet or when
     * the stack is already empty */ 
    if (!this_Stack || this_Stack[STACK_POINTER_INDEX] <= -1)
    {
        writeline (POP_NONEXIST, stderr);  /* error message printed */
        return 0;
    }

    pointerIndex = this_Stack[STACK_POINTER_INDEX];

    /* If statement is executed when debug mode is on */
    if (debug)
    {
        fprintf (stderr, POP, (long)stack_counter, this_Stack[pointerIndex]);
    }

    /* code used to obtain top item in stack, remove item, and reduce the 
     * pointer index by one */
    *item = this_Stack[pointerIndex];
    this_Stack[pointerIndex] = 0;
    this_Stack[STACK_POINTER_INDEX]--;

    return 1;    
}


/*-----------------------------------------------------------------------------
Function Name:          push
Purpose:                This function adds a new element to the top of stack
Description:            This function first checks for EOF, then checks to see
                        if stack is set yet. If not, an error message is
                        printed. An error message is also printed if the stack 
                        is full. If none of the above occur, the function sets
                        item to the top of the stack and increases its index
Input:                  this_Stack: the stack in question
                        item: the long being stored to the top of stack
Result:                 True if the push can be made, an error message prints
                        if a push cannot be made or stack does not exist yet,
                        and EOF for our ^D test
-----------------------------------------------------------------------------*/
long push (Stack * this_Stack, long item) 
{
    long pointerIndex = 0;   /* variable for original index of pointer index */

    /* If statement is executed when EOF is reached */
    if (item == EOF)
    {
        return EOF;
    }
                 
    /* If statement is executed if the stack is not yet set */
    if (!this_Stack)
    {
        writeline (PUSH_NONEXIST, stderr);    /* error message printed */
        return 0;
    }

    /* If statement is executed if the stack is full */
    if ( isfull_Stack (this_Stack) )
    {
        writeline (PUSH_FULL, stderr);     /* error message printed */
        return 0;
    }

    /* If statement is executed if debug mode is on */
    if (debug)
    {
        fprintf (stderr, PUSH, (long)stack_counter, item);
    }

    /* code used to store item into the top of stack and to move to the next
     * index in the stack */
    pointerIndex = this_Stack[STACK_POINTER_INDEX];
    this_Stack[pointerIndex + 1] = item;
    this_Stack[STACK_POINTER_INDEX]++;   

    return 1;
}


/*----------------------------------------------------------------------------
Function Name:          top
Purpose:                This function prints the top element in the stack
Description:            This function first checks to see if the stack has
                        been set yet or if it is already empty. If so, an
                        error message is printed. If this is not the case,
                        then the long pointer is shown and printed
Input:                  this_Stack: the stack from which we are getting our
                                    items
                        item: the number on top of the stack
Result:                 Ture if there is a top item in the stack. False if 
                        the stack has yet to be set or is already empty and
                        an error message is printed
----------------------------------------------------------------------------*/
long top (Stack * this_Stack, long * item) 
{
    long pointerIndex = 0;  /* variable for original index of pointer index */


    /* If statement is executed when the stack has not been set yet or when
     * the stack is already empty */
    if (!this_Stack || this_Stack[STACK_POINTER_INDEX] <= -1)
    {
        writeline (TOP_NONEXIST, stderr);    /* error message printed */
        return 0;
    }

    pointerIndex = this_Stack[STACK_POINTER_INDEX];

    /* If statement is executed when debug mode is on */
    if (debug)
    {
        fprintf (stderr, TOP, (long)stack_counter, 
                this_Stack[pointerIndex]);
    }
    
    *item = this_Stack[pointerIndex];   /* obtain the top item in stack */

    return 1;
}


FILE * write_Stack (Stack * this_Stack, FILE * stream) 
{
    long index = 0;         /* index into the stack */

    if (this_Stack == NULL) 
    {
        fprintf (stderr, WRITE_NONEXIST_STACK);
        return stream;
    }

    if (stream == NULL) 
    {
        fprintf (stderr, WRITE_NONEXIST_FILE);
        return stream;
    }
                
    if (stream == stderr)
    {
        fprintf ( stream, "Stack has %ld items in it.\n", 
                num_elements (this_Stack) );
    }

    for (index = STACK_COUNT_INDEX + STACK_OFFSET;
            index < num_elements (this_Stack); index++) 
    {
        if (stream == stderr)
        {
            fprintf (stream, "Value on stack is |0x%lx|\n", this_Stack[index]);
        }

        else 
        {
            if (this_Stack[index] < 0)
            {
                fprintf (stream, "%c ", (char) this_Stack[index]);
            }
                        
            else
            {
                fprintf (stream, "%ld ", this_Stack[index]);
            }
        }
    }

    return stream;
}
