#include "sql-token.h"

#include <stdio.h>

/**
 * print_token - Print a Token's value and type
 * @token: Token to print
 *
 * Return: Nothing
 */
void print_token(Token token)
{
    printf("Token { value: %s, type: %d}\n", token.value, token.type);
}