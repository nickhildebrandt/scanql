#pragma once

#include "sql-token.h"

typedef struct
{
    Token* elems; // Pointer to an array of Token elements, representing the
                  // stack's contents.
    int len; // The current length of the stack, indicating how many tokens are
             // currently in it.
    int cap; // The capacity of the stack, which indicates the maximum number of
             // tokens it can hold before needing to be resized.
} TokenStack;

/**
 * append - Append a Token to the TokenStack
 * @list: Pointer to the TokenStack to append to
 * @token: Token to append
 *
 * Return: Nothing
 */
void append(TokenStack* list, Token token);