#include "sqlstringlist.h"

#include "sqlstring.h"

#include <stdlib.h>

struct SqlStringList
{
    SqlString* item;
    SqlStringList* next;
};

/**
 * sql_string_list_new - Create a new empty SqlStringList
 *
 * Return: New SqlStringList node, or NULL on allocation failure
 */
SqlStringList* sql_string_list_new(void)
{
    SqlStringList* list = (SqlStringList*)malloc(sizeof(SqlStringList));
    if (!list)
    {
        return NULL;
    }

    list->item = NULL;
    list->next = NULL;
    return list;
}

/**
 * sql_string_list_free - Free a SqlStringList and all its nodes
 * @list: List head (may be NULL)
 *
 * Return: Nothing
 */
void sql_string_list_free(SqlStringList* list)
{
    SqlStringList* current = list;
    while (current)
    {
        SqlStringList* next = current->next;
        free(current);
        current = next;
    }
}

/**
 * sql_string_list_append - Append a SqlString item to the end of a list
 * @list: List head to append to
 * @str: SqlString to store (pointer is stored, not copied)
 *
 * Return: Nothing
 */
void sql_string_list_append(SqlStringList* list, SqlString* str)
{
    if (!list)
    {
        return;
    }

    SqlStringList* current = list;
    while (current->next)
    {
        current = current->next;
    }

    /* If the head is empty, fill it rather than allocating a new node. */
    if (current->item == NULL)
    {
        current->item = str;
        return;
    }

    SqlStringList* node = (SqlStringList*)malloc(sizeof(SqlStringList));
    if (!node)
    {
        return;
    }

    node->item    = str;
    node->next    = NULL;
    current->next = node;
}

/**
 * sql_string_list_get - Get the SqlString at a zero-based position
 * @list: List head (may be NULL)
 * @index: Zero-based index to retrieve
 *
 * Return: SqlString* at index, or NULL if out of bounds
 */
SqlString* sql_string_list_get(SqlStringList* list, int index)
{
    if (!list || index < 0)
    {
        return NULL;
    }

    int i                  = 0;
    SqlStringList* current = list;
    while (current)
    {
        if (i == index)
        {
            return current->item;
        }
        i++;
        current = current->next;
    }

    return NULL;
}
