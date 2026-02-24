#pragma once

#include <stdbool.h>
#include <stddef.h>

/* Opaque structure representing a SQL string */
typedef struct SqlString SqlString;

SqlString* sql_string_new(const char* value);
SqlString* sql_string_new_nocopy(char* value);
void free_sql_string(SqlString* sql_str);
SqlString* append_sql_string(SqlString* base_string, const char* value);
char sql_string_get_char(const SqlString* sql_str, int index);
size_t sql_string_length(const SqlString* sql_str);
