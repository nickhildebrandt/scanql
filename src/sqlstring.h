#pragma once

#include <stddef.h>

/* Opaque structure representing a SQL string */
typedef struct SqlString SqlString;

/**
 * sql_string_new - Create a new SqlString and copy the input value
 * @value: NUL-terminated string to copy into the new SqlString
 *
 * Return: New SqlString on success, NULL on allocation failure
 */
SqlString* sql_string_new(const char* value);

/**
 * sql_string_new_nocopy - Create a new SqlString without copying the input
 * value
 * @value: NUL-terminated string buffer owned by the caller
 *
 * Return: New SqlString on success, NULL on allocation failure
 */
SqlString* sql_string_new_nocopy(char* value);

/**
 * free_sql_string - Free a SqlString node
 * @sql_str: SqlString to free (may be NULL)
 *
 * Return: Nothing
 */
void free_sql_string(SqlString* sql_str);

/**
 * append_sql_string - Append a new node to a chained SqlString
 * @base_string: Base node to append to
 * @value: NUL-terminated string to append as a new node
 *
 * Return: base_string (unchanged pointer), even on allocation failure
 */
SqlString* append_sql_string(SqlString* base_string, const char* value);

/**
 * sql_string_get_char - Get character at a global index across the chain
 * @sql_str: Base SqlString (may be NULL)
 * @index: Zero-based character index across all nodes
 *
 * Return: Character at index, or '\0' if out of bounds
 */
char sql_string_get_char(const SqlString* sql_str, int index);

/**
 * sql_string_length - Get the total length of a chained SqlString
 * @sql_str: SqlString to measure (may be NULL)
 *
 * Return: Total number of characters across this node and all next nodes
 */
size_t sql_string_length(const SqlString* sql_str);
