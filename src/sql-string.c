#include "sql-string.h"

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

SqlString* sql_string_new(const char* value)
{
    SqlString* sql_str = (SqlString*)malloc(sizeof(SqlString));
    if (!sql_str)
        return NULL;

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

SqlString* sql_string_new_nocopy(char* value)
{
    SqlString* sql_str = (SqlString*)malloc(sizeof(SqlString));
    if (!sql_str)
        return NULL;

    sql_str->value      = value;
    sql_str->length     = strlen(value);
    sql_str->owned_data = false;
    sql_str->next       = NULL;
    return sql_str;
}

void free_sql_string(SqlString* sql_str)
{
    if (!sql_str)
        return;

    if (sql_str->owned_data && sql_str->value)
        free(sql_str->value);

    free(sql_str);
}

SqlString* append_sql_string(SqlString* base_string, const char* value)
{
    if (!base_string || !value)
        return base_string;

    SqlString* next_string = base_string->owned_data
                                 ? sql_string_new(value)
                                 : sql_string_new_nocopy((char*)value);

    if (!next_string)
        return base_string;

    base_string->next = next_string;
    return base_string;
}

char sql_string_get_char(const SqlString* sql_str, int index)
{
    if (!sql_str || index < 0)
        return '\0';

    size_t idx               = (size_t)index;
    const SqlString* current = sql_str;
    while (current)
    {
        if (idx < current->length)
            return current->value[idx];
        idx -= current->length;
        current = current->next;
    }
    return '\0';
}

size_t sql_string_length(const SqlString* sql_str)
{
    if (!sql_str)
        return 0;

    size_t total_length      = 0;
    const SqlString* current = sql_str;
    while (current)
    {
        total_length += current->length;
        current = current->next;
    }
    return total_length;
}
