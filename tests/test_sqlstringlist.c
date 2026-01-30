#include "sql-string-list.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * test_new_empty_get - Test new list is empty and get behaves
 *
 * Return: Nothing
 */
static void test_new_empty_get(void)
{
    SqlStringList* list = sql_string_list_new();
    assert(list != NULL);

    /* empty head should return NULL for any index */
    assert(sql_string_list_get(list, 0) == NULL);
    assert(sql_string_list_get(list, 1) == NULL);

    /* negative index must return NULL */
    assert(sql_string_list_get(list, -1) == NULL);

    sql_string_list_free(list);
}

/**
 * test_append_and_get - Test append fills head, then adds nodes, and get works
 *
 * Return: Nothing
 */
static void test_append_and_get(void)
{
    SqlStringList* list = sql_string_list_new();
    assert(list != NULL);

    SqlString* s1 = sql_string_new("A");
    SqlString* s2 = sql_string_new("B");
    SqlString* s3 = sql_string_new("C");

    assert(s1 != NULL);
    assert(s2 != NULL);
    assert(s3 != NULL);

    /* first append should populate head item */
    sql_string_list_append(list, s1);
    assert(sql_string_list_get(list, 0) == s1);

    /* subsequent appends should create new nodes */
    sql_string_list_append(list, s2);
    sql_string_list_append(list, s3);

    assert(sql_string_list_get(list, 0) == s1);
    assert(sql_string_list_get(list, 1) == s2);
    assert(sql_string_list_get(list, 2) == s3);

    /* out of bounds */
    assert(sql_string_list_get(list, 3) == NULL);

    /* cleanup: list does not own SqlString objects */
    sql_string_list_free(list);
    free_sql_string(s1);
    free_sql_string(s2);
    free_sql_string(s3);
}

/**
 * test_null_list_is_safe - Test NULL list behavior
 *
 * Return: Nothing
 */
static void test_null_list_is_safe(void)
{
    sql_string_list_append(NULL, NULL);

    assert(sql_string_list_get(NULL, 0) == NULL);
}

/**
 * main - Run all unit tests for SqlStringList
 */
int main(void)
{
    test_new_empty_get();
    test_append_and_get();
    test_null_list_is_safe();
    return 0;
}
