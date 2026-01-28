#include "tokenizer.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

/**
 * init_static_arena - Allocate a bump arena of given capacity
 * @capacity: number of bytes to reserve
 *
 * Return: initialized Arena with allocated buffer
 */
Arena init_static_arena(size_t capacity)
{
    unsigned char* data = malloc(capacity);
    Arena a             = {.data = data, .size = 0, .capacity = capacity};
    return a;
}

/**
 * static_arena_alloc - Allocate a slice from the arena without freeing
 * @arena: arena to allocate from
 * @size: bytes requested
 *
 * Return: pointer to allocated slice or NULL if insufficient space
 */
static void* static_arena_alloc(Arena* arena, size_t size)
{
    if (!arena || !arena->data)
        return NULL;

    if (arena->size + size <= arena->capacity)
    {
        unsigned char* data = &arena->data[arena->size];
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
    if (!arena)
        return;
    free(arena->data);
    arena->data     = NULL;
    arena->size     = 0;
    arena->capacity = 0;
}

/**
 * get_tokens - Tokenize a SQL string into a TokenStack
 * @sql: input SQL text
 * @arena: arena for storing token lexeme strings
 *
 * Note: This is a very small tokenizer tailored to the validator. It is not a
 * full SQL lexer.
 */
TokenStack get_tokens(const char* sql, Arena* arena)
{
    assert(sql != NULL);
    assert(arena != NULL);

    size_t txt_len    = strlen(sql);
    size_t tokenCount = txt_len ? txt_len : 1;

    TokenStack tokenList = {
        .elems = (Token*)static_arena_alloc(arena, tokenCount * sizeof(Token)),
        .len   = 0,
        .cap   = (int)tokenCount,
    };

    int index      = 0;
    int last_index = (int)txt_len;
    while (index < last_index)
    {
        Token token;
        char c = sql[index];

        switch (c)
        {
            case ' ': /* skip whitespace */
                index++;
                continue;
            case 'S':
            case 's':
                token.type = (index == 0) ? SELECT : STRING;
                break;
            case ',':
                token.type     = COMMA;
                token.value    = static_arena_alloc(arena, 2);
                token.value[0] = ',';
                token.value[1] = '\0';
                append(&tokenList, token);
                index++;
                continue;
            case 'f':
            case 'F':
                if ((index + 1 < last_index) &&
                    (sql[index + 1] == 'r' || sql[index + 1] == 'R'))
                    token.type = FROM;
                else
                    token.type = STRING;
                break;
            case '=':
                token.type = EQUALS;
                break;
            case '!':
                token.type = NEGATION;
                break;
            case ';':
                token.type     = SEMICOLON;
                token.value    = static_arena_alloc(arena, 2);
                token.value[0] = ';';
                token.value[1] = '\0';
                append(&tokenList, token);
                index++;
                continue;
            case '"':
            case '\'':
                {
                    token.type     = STRING;
                    char separator = c;

                    int start = index;
                    index++; /* skip opening quote */
                    c = sql[index];

                    while (index < last_index && c != separator)
                    {
                        index++;
                        c = sql[index];
                    }
                    if (index < last_index)
                        index++; /* consume closing quote */

                    int end = index;
                    assert(end > start && "end should be bigger than start");

                    token.value =
                        static_arena_alloc(arena, (size_t)(end - start) + 1);
                    strncpy(token.value, sql + start, (size_t)(end - start));
                    token.value[end - start] = '\0';
                    append(&tokenList, token);
                    continue;
                }
            default:
                token.type = STRING;
                break;
        }

        int start = index;
        while (index < last_index && sql[index] != ' ' && sql[index] != '\0' &&
               sql[index] != ';' && sql[index] != ',')
        {
            index++;
        }
        int end = index;
        assert(end > start && "end should be bigger than start");

        token.value = static_arena_alloc(arena, (size_t)(end - start) + 1);
        strncpy(token.value, sql + start, (size_t)(end - start));
        token.value[end - start] = '\0';
        append(&tokenList, token);

        /* consume separator if present */
        if (index < last_index && (sql[index] == ' ' || sql[index] == '\0'))
            index++;
    }

    return tokenList;
}
