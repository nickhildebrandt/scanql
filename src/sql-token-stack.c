#include "sql-token-stack.h"

/**
 * append - Append a Token to the TokenStack
 * @list: Pointer to the TokenStack to append to
 * @token: Token to append
 *
 * Return: Nothing
 */
void append(TokenStack* list, Token token)
{
    if (list && list->elems && list->len < list->cap)
    {
        list->elems[list->len] = token;
        list->len += 1;
    }
}