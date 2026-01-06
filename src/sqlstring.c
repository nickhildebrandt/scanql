#include <sqlstring.h>
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

/* Create a new SqlString with the given value and copy the value.
 * The SqlString will manage the memory of value.
 * The caller can free or modify the original value.
 * In this case on every append_sql_string or free_sql_string call,
 * the SqlString will have its own copy of the value.
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

/* Create a new SqlString with the given value without copying the value
 * The caller is responsible for managing the memory of value
 * In this case on every append_sql_string or free_sql_string call,
 * the caller must ensure that the value remains valid
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

/* Free the SqlString and its value if owned_data is true */
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

/* Append the given value to the SqlString by setting the next pointer.
 * If the SqlString does not own its data, it will create a copy of the value
 * and set owned_data to true.
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

/* Get the character at the given index in the SqlString.
 * Returns '\0' if index is out of bounds.
 */
char sql_string_get_char(const SqlString* sql_str, int index)
{
    if (!sql_str || index < 0 || (size_t)index >= sql_str->length)
    {
        return '\0';
    }

    char* current_value = sql_str->value;
    SqlString* next     = sql_str->next;

    for (int i = 0; i < index; i++)
    {
        current_value++;
        if (!current_value)
        {
            if (!next)
            {
                return '\0';
            }
            current_value = next->value;
        }
        current_value++;
    }

    return *current_value;
}
