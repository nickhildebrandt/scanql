#pragma once

#include <stddef.h>

/* Opaque structure representing a SQL string */
typedef struct SqlString SqlString;

/* Create a new SqlString with the given value and copy the value.
 * The SqlString will manage the memory of value.
 * The caller can free or modify the original value.
 * In this case on every append_sql_string or free_sql_string call,
 * the SqlString will have its own copy of the value.
 */
SqlString* sql_string_new(const char* value);

/* Create a new SqlString with the given value without copying the value
 * The caller is responsible for managing the memory of value
 * In this case on every append_sql_string or free_sql_string call,
 * the caller must ensure that the value remains valid
 */
SqlString* sql_string_new_nocopy(char* value);

/* Free the SqlString and its value if owned_data is true */
void free_sql_string(SqlString* sql_str);

/* Append the given value to the SqlString by setting the next pointer.
 * If the SqlString does not own its data, it will create a copy of the value
 * and set owned_data to true.
 */
SqlString* append_sql_string(SqlString* base_string, const char* value);

/* Get the character at the given index in the SqlString.
 * Returns '\0' if index is out of bounds.
 */
char sql_string_get_char(const SqlString* sql_str, int index);

/* Get the total length of the SqlString counting all characters
 */
size_t sql_string_length(const SqlString* sql_str);
