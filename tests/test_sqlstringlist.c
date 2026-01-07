#include "sqlstringlist.h"
#include "test_asserts.h"

#include <stdio.h>
#include <stdlib.h>

static int failures = 0;

/**
 * test_new_empty_get - Test new list is empty and get behaves
 *
 * Return: Nothing
 */
static void test_new_empty_get(void)
{
    SqlStringList* list = sql_string_list_new();
    expect_true(&failures,
                list != NULL,
                __FILE__,
                __LINE__,
                "sql_string_list_new returned NULL");

    /* empty head should return NULL for any index */
    expect_eq_ptr(&failures,
                  sql_string_list_get(list, 0),
                  NULL,
                  __FILE__,
                  __LINE__,
                  "sql_string_list_get(empty, 0) == NULL");
    expect_eq_ptr(&failures,
                  sql_string_list_get(list, 1),
                  NULL,
                  __FILE__,
                  __LINE__,
                  "sql_string_list_get(empty, 1) == NULL");

    /* negative index must return NULL */
    expect_eq_ptr(&failures,
                  sql_string_list_get(list, -1),
                  NULL,
                  __FILE__,
                  __LINE__,
                  "sql_string_list_get(list, -1) == NULL");

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
    expect_true(&failures,
                list != NULL,
                __FILE__,
                __LINE__,
                "sql_string_list_new returned NULL");

    SqlString* s1 = sql_string_new("A");
    SqlString* s2 = sql_string_new("B");
    SqlString* s3 = sql_string_new("C");

    expect_true(&failures,
                s1 != NULL,
                __FILE__,
                __LINE__,
                "sql_string_new(\"A\") returned NULL");
    expect_true(&failures,
                s2 != NULL,
                __FILE__,
                __LINE__,
                "sql_string_new(\"B\") returned NULL");
    expect_true(&failures,
                s3 != NULL,
                __FILE__,
                __LINE__,
                "sql_string_new(\"C\") returned NULL");

    /* first append should populate head item */
    sql_string_list_append(list, s1);
    expect_eq_ptr(&failures,
                  sql_string_list_get(list, 0),
                  s1,
                  __FILE__,
                  __LINE__,
                  "sql_string_list_get(list, 0) == s1 after first append");

    /* subsequent appends should create new nodes */
    sql_string_list_append(list, s2);
    sql_string_list_append(list, s3);

    expect_eq_ptr(&failures,
                  sql_string_list_get(list, 0),
                  s1,
                  __FILE__,
                  __LINE__,
                  "sql_string_list_get(list, 0) == s1");
    expect_eq_ptr(&failures,
                  sql_string_list_get(list, 1),
                  s2,
                  __FILE__,
                  __LINE__,
                  "sql_string_list_get(list, 1) == s2");
    expect_eq_ptr(&failures,
                  sql_string_list_get(list, 2),
                  s3,
                  __FILE__,
                  __LINE__,
                  "sql_string_list_get(list, 2) == s3");

    /* out of bounds */
    expect_eq_ptr(&failures,
                  sql_string_list_get(list, 3),
                  NULL,
                  __FILE__,
                  __LINE__,
                  "sql_string_list_get(list, 3) == NULL (oob)");

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

    expect_eq_ptr(&failures,
                  sql_string_list_get(NULL, 0),
                  NULL,
                  __FILE__,
                  __LINE__,
                  "sql_string_list_get(NULL, 0) == NULL");
}

/**
 * main - Run all unit tests for SqlStringList
 *
 * Return: 0 if all tests passed, non-zero otherwise
 */
int main(void)
{
    test_new_empty_get();
    test_append_and_get();
    test_null_list_is_safe();

    if (failures == 0)
    {
        return 0;
    }

    fprintf(stderr, "%d test(s) failed\n", failures);
    return 1;
}
