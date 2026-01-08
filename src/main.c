#include "sql-token-stack.h"
#include "sql-token.h"
#include "sql-token.c"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * main - Program entry point
 * @argc: Number of command line arguments
 * @argv: Array of command line argument strings
 *
 * Return: 0 on success, non-zero on usage error
 */
int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s <SQL-String>\n", argv[0]);
        return 1;
    }
    printf("Received SQL String: %s\n", argv[1]);

    char* txt         = argv[1];
    size_t txt_len    = strlen(txt);
    size_t tokenCount = txt_len;

    TokenStack tokenList;
    tokenList.elems = malloc(tokenCount * sizeof(Token));
    tokenList.len   = 0;
    tokenList.cap   = tokenCount;

    int i = 0;
    while (i < (int)txt_len)
    {
        Token token;

        char c = txt[i];
        if (c == ' ')
        {
            i++;
            continue;
        }
        switch (c)
        {
            case 'S':
            case 's':
                if (i == 0)
                {
                    token.type = SELECT;
                }
                else
                {
                    token.type = STRING;
                }
                break;
            case ',':
                {
                    token.type = COMMA;
                    break;
                }
            case 'f':
            case 'F':
                {
                    if (txt[i + 1] == 'r' || txt[i + 1] == 'R')
                    { // TODO: implement point compare system && look
                      // if from is already set
                        token.type = FROM;
                    }
                    else
                    {
                        token.type = STRING;
                    }
                }
                break;
            case '=':
                {
                    token.type = EQUALS;
                }
                break;
            case '!':
                {
                    token.type = NEGATION;
                }
                break;
            default:
                {
                    token.type = STRING;
                }
                break;
        }

        int start = i;
        while (c != ' ' && c != '\0')
        {
            i++;
            c = txt[i];
        }
        int end     = i;
        token.value = malloc((end - start) + 1);
        if (token.value)
        {
            strncpy(token.value, txt + start, end - start);
            token.value[end - start] = '\0';
        }
        if (tokenList.len < tokenList.cap)
        {
            tokenList.elems[tokenList.len] = token;
            tokenList.len += 1;
        }
        while (txt[i] == ' ')
        {
            i++;
        }
    }

    for (int i = 0; i < tokenList.len; i++)
    {
        print_token(tokenList.elems[i]);
    }

    return 0;
}
