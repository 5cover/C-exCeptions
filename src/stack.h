/** \brief Basic implementation of a LIFO stack.
 * \author Scover
 */

#ifndef STACK_H_INCLUDED
#define STACK_H_INCLUDED

// Initializer for an empty stack.
#define EMPTY_STACK_INITIALIZER {0, NULL}

typedef struct
{
    // Current size of the stack.
    int size;
    void *_first;

} Stack;

// Add an element at the top of the stack.
void pushStack(Stack*, void*);

// Remove the element at the top of the stack
void popStack(Stack*);

// Get the element at the top of the stack.
void *peekStack(Stack*);

#endif // STACK_H_INCLUDED
