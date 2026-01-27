#include "sql-symbols.h"
#include "sql-token-stack.c"
#include "sql-token-stack.h"
#include "sql-token.c"
#include "sql-token.h"

#include <assert.h>
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

/**
 * init_static_arena - Allocate a simple bump arena of given capacity
 * @capacity: number of bytes to reserve
 *
 * Return: initialized Arena with allocated buffer
 */
Arena init_static_arena(size_t capacity)
{
    u8* data = malloc(capacity);
    Arena a  = {.data = data, .size = 0, .capacity = capacity};
    return a;
}

/**
 * static_arena_alloc - Allocate a slice from the arena without freeing
 * @arena: arena to allocate from
 * @size: bytes requested
 *
 * Return: pointer to allocated slice or NULL if insufficient space
 */
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

/**
 * arena_free - Release arena backing memory
 * @arena: arena to free
 */
void arena_free(Arena* arena)
{
    free(arena->data);
    arena->size     = 0;
    arena->capacity = 0;
}

TokenStack get_Tokens(char* txt, Arena* arena)
{
    size_t txt_len    = strlen(txt);
    size_t tokenCount = txt_len;

    TokenStack tokenList;
    tokenList.elems = malloc(tokenCount * sizeof(Token));
    tokenList.len   = 0;
    tokenList.cap   = tokenCount;

    int index      = 0;
    int last_index = (int)txt_len - 1;
    while (index < last_index) // TODO : tokenizer needs to split by
                               // mutiple things not just spaces like for
                               // cases where no spaces are there e.g
                               // ("h","a")
    {
        Token token;

        char c = txt[index];
        switch (c)
        {
            case ' ':
                index++;
                continue;
            case 'S':
            case 's':
                if (index == 0)
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
                    token.type  = COMMA;
                    token.value = static_arena_alloc(arena, 2);
                    token.value = ",\0";
                    append(&tokenList, token);
                    index++;
                    continue;
                }
            case 'f':
            case 'F':
                {
                    if (txt[index + 1] == 'r' || txt[index + 1] == 'R')
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
                token.type  = SEMICOLON;
                token.value = static_arena_alloc(arena, 2);
                token.value = ";\0";
                append(&tokenList, token);
                index++;
                continue;
            case '"':
            case '\'':
                token.type = STRING_VALUE;

                char seperator = '\xcd';
                if (c == '\'')
                {
                    seperator = '\'';
                }
                else
                {
                    seperator = '"';
                }

                int start = index;

                index++;
                c = txt[index];

                while (c != seperator && index < last_index) // until
                                                             // seperators
                {
                    c = txt[index];
                    index++;
                }
                index++; // include seperator

                int end = index;
                assert(end > start && "end should be bigger than start");

                token.value = static_arena_alloc(arena, (end - start) + 1);
                strncpy(token.value, txt + start, end - start);
                token.value[end - start] = '\0';
                printf("%d\n", (int)strlen(token.value));

                append(&tokenList, token);
                continue;
            default:
                {
                    token.type = STRING;
                }
                break;
        }

        int start = index;
        while (c != ' ' && c != '\0' && c != ';' && c != ',' &&
               index < last_index) // until
                                   // seperators
        {
            index++;
            c = txt[index];
        }
        index++; // include seperator
        int end = index;
        assert(end > start && "end should be bigger than start");

        token.value = static_arena_alloc(arena, (end - start) + 1);
        strncpy(token.value, txt + start, end - start);
        token.value[end - start] = '\0';
        append(&tokenList, token);
    }

    return tokenList;
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
    char* txt      = "\'from\'";
    size_t txt_len = strlen(txt);

    Arena arena = init_static_arena(2 * txt_len);

    TokenStack tokenList = get_Tokens(txt, &arena);

    for (int i = 0; i < tokenList.len; i++)
    {
        print_token(tokenList.elems[i]);
    }

    return 0;
}
