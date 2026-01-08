#include <sqltoken.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    char* value;
    SqlToken type;
} Token;

void printToken(Token t)
{
    printf("Token { value: %s, type: %d}\n", t.value, t.type);
}

typedef struct
{
    Token* elems; // Pointer to an array of Token elements, representing the
                  // stack's contents.
    int len; // The current length of the stack, indicating how many tokens are
             // currently in it.
    int cap; // The capacity of the stack, which indicates the maximum number of
             // tokens it can hold before needing to be resized.
} TokenStack;

void append(TokenStack* list, Token t)
{
    if (list->len <= list->cap)
    {
        list->elems[list->len] = t;
        list->len += 1;
    }
}

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

    char* txt         = "select t from b"; // TODO: use argv here
    size_t tokenCount = strlen(txt);

    TokenStack tokenList;
    tokenList.elems = malloc(tokenCount * sizeof(Token));
    tokenList.cap   = tokenCount;

    int i = 0;
    while (i <= (int)strlen(txt))
    {
        Token token;

        char c = txt[i];
        printf("%c\n", c);
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
                    if (txt[i + 1] == 'r')
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
            case ' ':
                break;
            default:
                {
                    token.type = STRING;
                }
                break;
        }

        int start = i;
        while (c != ' ')
        {
            i++;
            c = txt[i];
        }
        int end     = i;
        token.value = malloc(end - start);
        strncpy(token.value, txt + start, end - start);
        append(&tokenList, token);
        i++;
    }

    for (int i = 0; i < tokenList.len; i++)
    {
        printToken(tokenList.elems[i]);
    }

    return 0;
}
