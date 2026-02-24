/**
 * @file tokenizer.h
 * @brief Simple SQL tokenizer producing a TokenStack for validation.
 */

#pragma once

#include "sql-token-stack.h"
#include "sql-token.h"

#include <stddef.h>

/**
 * struct Arena - Simple bump allocator used by the tokenizer
 * @data: backing buffer
 * @size: bytes currently used
 * @capacity: total bytes available
 */
typedef struct
{
    unsigned char* data;
    size_t size;
    size_t capacity;
} Arena;

/**
 * init_static_arena - Allocate a bump arena of given capacity
 * @capacity: number of bytes to reserve
 *
 * Return: initialized Arena with allocated buffer (must be freed with
 * arena_free).
 */
Arena init_static_arena(size_t capacity);

/**
 * arena_free - Release arena backing memory
 * @arena: arena to free (safe to call with NULL fields)
 */
void arena_free(Arena* arena);

/**
 * get_tokens - Tokenize a SQL string into a TokenStack
 * @sql: input SQL text
 * @arena: arena for storing token lexeme strings (must outlive returned stack)
 *
 * The caller owns the returned TokenStack.elems array (allocated with malloc)
 * and must free it. Lexeme storage lives inside @arena and is freed via
 * arena_free.
 */
TokenStack get_tokens(const char* sql, Arena* arena);
