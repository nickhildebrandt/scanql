#include "sql-symbols.h"
#include "sql-token-stack.c"
#include "sql-token-stack.h"
#include "sql-token.c"
#include "sql-token.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned char u8;
typedef struct
{
    u8* data;
    size_t size;
    size_t capacity;
} Arena;

Arena init_static_arena(size_t capacity)
{
    u8* data = malloc(capacity);
    Arena a  = {.data = data, .size = 0, .capacity = capacity};
    return a;
}

void* static_arena_alloc(Arena* arena, size_t size)
{
    if (arena->size + size < arena->capacity)
    {
        u8* data = &arena->data[arena->size];
        arena->size += size;
        return data;
    }
    return NULL;
}

void arena_free(Arena* arena)
{
    free(arena->data);
    arena->size     = 0;
    arena->capacity = 0;
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
    // if (argc != 2)
    // {
    //     printf("Usage: %s <SQL-String>\n", argv[0]);
    //     return 1;
    // }
    // printf("Received SQL String: %s\n", argv[1]);

    // char* txt         = argv[1];
    char* txt         = "select h,a";
    size_t txt_len    = strlen(txt);
    size_t tokenCount = txt_len;

    Arena arena = init_static_arena(2 * txt_len);

    TokenStack tokenList;
    tokenList.elems = malloc(tokenCount * sizeof(Token));
    tokenList.len   = 0;
    tokenList.cap   = tokenCount;

    int i = 0;
    while (i < (int)txt_len) // TODO : tokenizer needs to split by mutiple
                             // things not just spaces like for cases where no
                             // spaces are there e.g ("h","a")
    {
        Token token;

        char c = txt[i];
        switch (c)
        {
            case ' ':
                i++;
                continue;
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
            case ';':
                token.type = SEMICOLON;
                break;
            default:
                {
                    token.type = STRING;
                }
                break;
        }

        if (c == ';') // TODO: Maybe handle the exceptions here in the switch
                      // case
        {
            token.value = static_arena_alloc(&arena, 2);
            token.value = ";\0";
            append(&tokenList, token);
            i++;
            continue;
        }

        if (c == ',')
        {
            token.value = static_arena_alloc(&arena, 2);
            token.value = ",\0";
            append(&tokenList, token);
            i++;
            continue;
        }

        int start = i;
        while (c != ' ' && c != '\0' && c != ';' && c != ',') // until
                                                              // seperators
        {
            i++;
            c = txt[i];
        }
        int end = i;

        token.value = static_arena_alloc(&arena, (end - start) + 1);
        strncpy(token.value, txt + start, end - start);
        token.value[end - start] = '\0';
        append(&tokenList, token);
    }

    for (int i = 0; i < tokenList.len; i++)
    {
        print_token(tokenList.elems[i]);
    }

    return 0;
}
