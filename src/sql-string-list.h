#pragma once

#include "sql-string.h"

typedef struct SqlStringList SqlStringList;

SqlStringList* sql_string_list_new(void);
void sql_string_list_free(SqlStringList* list);
void sql_string_list_append(SqlStringList* list, SqlString* str);
SqlString* sql_string_list_get(SqlStringList* list, int index);
