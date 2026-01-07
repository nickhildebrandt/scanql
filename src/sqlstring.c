#include <sqlstring.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

struct SqlString
{
    char* value;
    size_t length;
    bool owned_data;
    SqlString* next;
};

/**
 * sql_string_new - Create a new SqlString and copy the input value
 * @value: NUL-terminated string to copy into the new SqlString
 *
 * Return: New SqlString on success, NULL on allocation failure
 */
SqlString* sql_string_new(const char* value)
{
    SqlString* sql_str = (SqlString*)malloc(sizeof(SqlString));

    if (!sql_str)
    {
        return NULL;
    }

    sql_str->length = strlen(value);
    sql_str->value  = (char*)malloc(sql_str->length + 1);

    if (!sql_str->value)
    {
        free(sql_str);
        return NULL;
    }

    strcpy(sql_str->value, value);
    sql_str->owned_data = true;
    sql_str->next       = NULL;

    return sql_str;
}

/**
 * sql_string_new_nocopy - Create a new SqlString without copying the input
 * value
 * @value: NUL-terminated string buffer owned by the caller
 *
 * Return: New SqlString on success, NULL on allocation failure
 */
SqlString* sql_string_new_nocopy(char* value)
{
    SqlString* sql_str = (SqlString*)malloc(sizeof(SqlString));

    if (!sql_str)
    {
        return NULL;
    }

    sql_str->value      = value;
    sql_str->length     = strlen(value);
    sql_str->owned_data = false;
    sql_str->next       = NULL;

    return sql_str;
}

/**
 * free_sql_string - Free a SqlString node
 * @sql_str: SqlString to free (may be NULL)
 *
 * Return: Nothing
 */
void free_sql_string(SqlString* sql_str)
{
    if (!sql_str)
    {
        return;
    }

    if (sql_str->owned_data && sql_str->value)
    {
        free(sql_str->value);
    }

    free(sql_str);
}

/**
 * append_sql_string - Append a new node to a chained SqlString
 * @base_string: Base node to append to
 * @value: NUL-terminated string to append as a new node
 *
 * Return: base_string (unchanged pointer), even on allocation failure
 */
SqlString* append_sql_string(SqlString* base_string, const char* value)
{
    if (!base_string || !value)
    {
        return base_string;
    }

    SqlString* next_string = NULL;

    if (base_string->owned_data)
    {
        next_string = sql_string_new(value);
    }
    else
    {
        next_string = sql_string_new_nocopy((char*)value);
    }

    // Check if allocation was successful
    if (!next_string)
    {
        return base_string;
    }

    base_string->next = next_string;
    return base_string;
}

/**
 * sql_string_get_char - Get character at a global index across the chain
 * @sql_str: Base SqlString (may be NULL)
 * @index: Zero-based character index across all nodes
 *
 * Return: Character at index, or '\0' if out of bounds
 */
char sql_string_get_char(const SqlString* sql_str, int index)
{
    if (!sql_str || index < 0)
    {
        return '\0';
    }

    size_t idx               = (size_t)index;
    const SqlString* current = sql_str;

    while (current)
    {
        if (idx < current->length)
        {
            return current->value[idx];
        }

        idx -= current->length;
        current = current->next;
    }

    return '\0';
}

/**
 * sql_string_length - Get the total length of a chained SqlString
 * @sql_str: SqlString to measure (may be NULL)
 *
 * Return: Total number of characters across this node and all next nodes
 */
size_t sql_string_length(const SqlString* sql_str)
{
    if (!sql_str)
    {
        return 0;
    }

    size_t total_length      = 0;
    const SqlString* current = sql_str;

    while (current)
    {
        total_length += current->length;
        current = current->next;
    }

    return total_length;
}
