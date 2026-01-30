#include "sql-string-list.h"
#include "sql-string.h"

#include <stdlib.h>

struct SqlStringList
{
    SqlString* item;
    SqlStringList* next;
};

SqlStringList* sql_string_list_new(void)
{
    SqlStringList* list = (SqlStringList*)malloc(sizeof(SqlStringList));
    if (!list)
        return NULL;

    list->item = NULL;
    list->next = NULL;
    return list;
}

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

void sql_string_list_append(SqlStringList* list, SqlString* str)
{
    if (!list)
        return;

    SqlStringList* current = list;
    while (current->next)
        current = current->next;

    if (current->item == NULL)
    {
        current->item = str;
        return;
    }

    SqlStringList* node = (SqlStringList*)malloc(sizeof(SqlStringList));
    if (!node)
        return;

    node->item    = str;
    node->next    = NULL;
    current->next = node;
}

SqlString* sql_string_list_get(SqlStringList* list, int index)
{
    if (!list || index < 0)
        return NULL;

    int i = 0;
    SqlStringList* current = list;
    while (current)
    {
        if (i == index)
            return current->item;
        ++i;
        current = current->next;
    }
    return NULL;
}
