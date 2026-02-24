#include "tokenizer.h"

#include <assert.h>
#include <ctype.h>
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

bool fuzzy_match(const char* to_compare, const char* compare_to)
{
    // NOTE: Bad fuzzy match algo, but is fuzzy only on bigger length
    int differences = 0;

    int to_compare_len = strlen(to_compare);
    int compare_to_len = strlen(compare_to);

    if (to_compare_len < compare_to_len)
    {
        return false;
    }

    int shorter_len =
        compare_to_len < to_compare_len ? compare_to_len : to_compare_len;

    for (int i = 0; i < shorter_len; i++)
    {
        int to_compare_char = toupper(to_compare[i]);

        bool in = false;
        for (int j = i; j < shorter_len; j++)
        {
            if (to_compare_char == toupper(compare_to[j]) && i == j)
            {
                in = true;
                break;
            }
        }

        if (!in)
        {
            differences++;
        }
    }

    if (differences < 2)
    {
        return true;
    }

    return false;
}

bool match(const char* to_compare, const char* compare_to)
{
    unsigned long compare_to_len = strlen(compare_to);
    if (strlen(to_compare) == compare_to_len)
    {
        for (int i = 0; i < (int)compare_to_len; i++)
        {
            if (toupper(to_compare[i]) != toupper(compare_to[i]))
            {
                return false;
            }
        }
        return true;
    }
    return false;
}

typedef Token Keyword;
Keyword keywords[] = {
    {"select", SELECT},
    {"from", FROM},
    {"where", WHERE},
    {"table", TABLE},
    {"drop", DROP},
    {"if", IF},
    {"exists", EXISTS},
    {"insert", INSERT},
    {"into", INTO},
    {"not", NOT},
    {"in", IN},
    {"update", UPDATE},
    {"delete", DELETE},
    {"values", VALUES},
    {"set", SET},
    {"returning", RETURNING},
    {"join", JOIN},
    {"create", CREATE},
};
const int keyword_count = sizeof(keywords) / sizeof(keywords[0]);

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

    char c = ' ';
    while (index < last_index)
    {
        char seperators[255] = " \t\n";
        c                    = sql[index];
        Token token          = {};
        bool is_single       = false;

        switch (c)
        {
            case ' ':
            case '\t':
            case '\n':
                index++;
                continue;

            case ',':
                token.type = COMMA;
                is_single  = true;
                break;

            case ';':
                token.type = SEMICOLON;
                is_single  = true;
                break;

            case '=':
                token.type = EQUALS;
                is_single  = true;
                break;
            case '(':
                token.type = ROUND_BRACKETS_OPEN;
                is_single  = true;
                break;
            case ')':
                token.type = ROUND_BRACKETS_CLOSE;
                is_single  = true;
                break;

            case '"':
                token.type = DOUBLE_QUOTED_VALUE;
                strcpy(seperators, "\"");
                index++; // skip current seperator
                break;
            case '\'':
                token.type = SINGLE_QUOTED_VALUE;
                strcpy(seperators, "'");
                index++; // skip current seperator
                break;

            default:
                if (isalpha(c) || c == '_')
                {
                    token.type = SQL_IDENTIFIER;
                    strcat(seperators, ",;()");
                }
                else if (isdigit(c))
                {
                    token.type = NUMBER;
                }
        }

        int start = index;
        if (is_single)
        {
            index++;
        }
        else
        {

            // NOTE: +1 include the \0 terminator for absolute end else crash at
            // the end because of the justified assert
            for (int char_idx = index; char_idx < (int)txt_len + 1; char_idx++)
            {
                bool is_seperator = false;
                char current_char = toupper(sql[char_idx]);
                // check for seperators
                for (int sep_idx = 0; sep_idx < (int)strlen(seperators);
                     sep_idx++)
                {
                    if (current_char == toupper(seperators[sep_idx]) ||
                        current_char == '\0')
                    {
                        is_seperator = true;
                        break;
                    }
                }

                if (is_seperator)
                {
                    index = char_idx;
                    break;
                }
            }
        }
        int end = index;
        assert(end > start && "end should be bigger than start");

        if (sql[index] == '"' || sql[index] == '\'') // consume specific
                                                     // string seperators
        {
            index++;
        }

        token.value = static_arena_alloc(arena, (size_t)(end - start) + 1);
        strncpy(token.value, sql + start, (size_t)(end - start));
        token.value[end - start] = '\0';

        // keyword check
        for (int i = 0; i < keyword_count; i++)
        {
            Keyword keyword = keywords[i];
            if (match(token.value, keyword.value))
            {
                token.type = keyword.type;
                break;
            }
        }

        append(&tokenList, token);
    }
    return tokenList;
}
