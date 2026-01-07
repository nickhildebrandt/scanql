#pragma once

#include "sqlstring.h"

typedef struct SqlStringList SqlStringList;

/**
 * sql_string_list_new - Create a new empty SqlStringList
 *
 * Return: New SqlStringList node, or NULL on allocation failure
 */
SqlStringList* sql_string_list_new(void);

/**
 * sql_string_list_free - Free a SqlStringList and all its nodes
 * @list: List head (may be NULL)
 *
 * Return: Nothing
 */
void sql_string_list_free(SqlStringList* list);

/**
 * sql_string_list_append - Append a SqlString item to the end of a list
 * @list: List head to append to
 * @str: SqlString to store (pointer is stored, not copied)
 *
 * Return: Nothing
 */
void sql_string_list_append(SqlStringList* list, SqlString* str);

/**
 * sql_string_list_get - Get the SqlString at a zero-based position
 * @list: List head (may be NULL)
 * @index: Zero-based index to retrieve
 *
 * Return: SqlString* at index, or NULL if out of bounds
 */
SqlString* sql_string_list_get(SqlStringList* list, int index);
