#pragma once

#include "sql-symbols.h"

typedef struct
{
    char* value;
    SqlSymbols type;
} Token;

/**
 * print_token - Print a Token's value and type
 * @token: Token to print
 *
 * Return: Nothing
 */
void print_token(Token token);