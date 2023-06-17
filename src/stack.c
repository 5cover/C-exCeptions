/** \brief stack.h implementation
 * \author Scover
 */

#include <stdlib.h>
#include <stdio.h>

#include "stack.h"
#include "assert.h"

#define ASSERT_THIS_NOT_NULL(funcName) assert(this != NULL && "The stack argument for " #funcName " is NULL")
#define ASSERT_THIS_NOT_EMPTY(funcName) assert(this->size > 0 && "The stack argument for " #funcName " is an empty stack")

typedef struct StackNode StackNode;
struct StackNode
{
    void *value;
    StackNode *next;
};

void pushStack(Stack *this, void *value)
{
    ASSERT_THIS_NOT_NULL(pushStack);

    StackNode *pNewNode = malloc(sizeof *pNewNode);
    if(pNewNode == NULL)
    {
        fprintf(stderr, "Stack node heap allocation failed: malloc(%u) returned NULL\n", (unsigned)sizeof *pNewNode);
        abort();
    }

    pNewNode->value = value;

    pNewNode->next = this->_first;
    this->_first = pNewNode;

    ++this->size;

}

void popStack(Stack *this)
{
    ASSERT_THIS_NOT_NULL(popStack);
    ASSERT_THIS_NOT_EMPTY(popStack);

    StackNode *first = this->_first;
    this->_first = first->next;
    free(first);

    --this->size;
}

void *peekStack(Stack *this)
{
    ASSERT_THIS_NOT_NULL(peekStack);
    ASSERT_THIS_NOT_EMPTY(peekStack);

    return ((StackNode*)this->_first)->value;
}
