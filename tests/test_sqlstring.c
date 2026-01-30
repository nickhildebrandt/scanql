#include "sql-string.h"
#include "test_asserts.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int failures = 0;

/**
 * test_new_and_length_and_get_char - Validate creation, length, and indexing
 */
static void test_new_and_length_and_get_char(void)
{
    SqlString* s = sql_string_new("SELECT");
    expect_true(&failures,
                s != NULL,
                __FILE__,
                __LINE__,
                "sql_string_new returned NULL");
    expect_eq_size_t(&failures,
                     sql_string_length(s),
                     6,
                     __FILE__,
                     __LINE__,
                     "sql_string_length(\"SELECT\") == 6");

    expect_eq_char(&failures,
                   sql_string_get_char(s, 0),
                   'S',
                   __FILE__,
                   __LINE__,
                   "sql_string_get_char(s, 0) == 'S'");
    expect_eq_char(&failures,
                   sql_string_get_char(s, 1),
                   'E',
                   __FILE__,
                   __LINE__,
                   "sql_string_get_char(s, 1) == 'E'");
    expect_eq_char(&failures,
                   sql_string_get_char(s, 5),
                   'T',
                   __FILE__,
                   __LINE__,
                   "sql_string_get_char(s, 5) == 'T'");

    /* out of bounds */
    expect_eq_char(&failures,
                   sql_string_get_char(s, -1),
                   '\0',
                   __FILE__,
                   __LINE__,
                   "sql_string_get_char(s, -1) == \\0");
    expect_eq_char(&failures,
                   sql_string_get_char(s, 6),
                   '\0',
                   __FILE__,
                   __LINE__,
                   "sql_string_get_char(s, 6) == \\0");

    free_sql_string(s);
}

/**
 * test_append_and_length - Validate append and total length calculation
 */
static void test_append_and_length(void)
{
    SqlString* s = sql_string_new("A");
    expect_true(&failures,
                s != NULL,
                __FILE__,
                __LINE__,
                "sql_string_new returned NULL");

    append_sql_string(s, "BC");
    expect_eq_size_t(&failures,
                     sql_string_length(s),
                     3,
                     __FILE__,
                     __LINE__,
                     "sql_string_length(\"A\" + \"BC\") == 3");

    /* indexing must work across appended nodes */
    expect_eq_char(&failures,
                   sql_string_get_char(s, 0),
                   'A',
                   __FILE__,
                   __LINE__,
                   "sql_string_get_char(\"A\"+\"BC\", 0) == 'A'");
    expect_eq_char(&failures,
                   sql_string_get_char(s, 1),
                   'B',
                   __FILE__,
                   __LINE__,
                   "sql_string_get_char(\"A\"+\"BC\", 1) == 'B'");
    expect_eq_char(&failures,
                   sql_string_get_char(s, 2),
                   'C',
                   __FILE__,
                   __LINE__,
                   "sql_string_get_char(\"A\"+\"BC\", 2) == 'C'");
    expect_eq_char(&failures,
                   sql_string_get_char(s, 3),
                   '\0',
                   __FILE__,
                   __LINE__,
                   "sql_string_get_char(\"A\"+\"BC\", 3) == \\0");

    free_sql_string(s);
}

/**
 * test_nocopy_append_does_not_crash - Ensure nocopy strings remain valid
 */
static void test_nocopy_append_does_not_crash(void)
{
    char* buf = (char*)malloc(4);
    expect_true(&failures,
                buf != NULL,
                __FILE__,
                __LINE__,
                "malloc(4) returned NULL");
    strcpy(buf, "XYZ");

    SqlString* s = sql_string_new_nocopy(buf);
    expect_true(&failures,
                s != NULL,
                __FILE__,
                __LINE__,
                "sql_string_new_nocopy returned NULL");

    /* append must not crash; caller keeps buf alive */
    append_sql_string(s, "1");
    expect_eq_size_t(&failures,
                     sql_string_length(s),
                     4,
                     __FILE__,
                     __LINE__,
                     "sql_string_length(\"XYZ\" + \"1\") == 4");

    expect_eq_char(&failures,
                   sql_string_get_char(s, 0),
                   'X',
                   __FILE__,
                   __LINE__,
                   "sql_string_get_char(\"XYZ\"+\"1\", 0) == 'X'");
    expect_eq_char(&failures,
                   sql_string_get_char(s, 1),
                   'Y',
                   __FILE__,
                   __LINE__,
                   "sql_string_get_char(\"XYZ\"+\"1\", 1) == 'Y'");
    expect_eq_char(&failures,
                   sql_string_get_char(s, 2),
                   'Z',
                   __FILE__,
                   __LINE__,
                   "sql_string_get_char(\"XYZ\"+\"1\", 2) == 'Z'");
    expect_eq_char(&failures,
                   sql_string_get_char(s, 3),
                   '1',
                   __FILE__,
                   __LINE__,
                   "sql_string_get_char(\"XYZ\"+\"1\", 3) == '1'");

    /* free_sql_string must not free buf in nocopy mode */
    free_sql_string(s);

    /* if buf was incorrectly freed, this may crash under ASAN/valgrind; keep it
     * simple */
    expect_eq_char(&failures,
                   buf[0],
                   'X',
                   __FILE__,
                   __LINE__,
                   "buf[0] == 'X' after free_sql_string(nocopy)");

    free(buf);
}

/**
 * main - Run all unit tests for SqlString
 */
int main(void)
{
    test_new_and_length_and_get_char();
    test_append_and_length();
    test_nocopy_append_does_not_crash();

    if (failures == 0)
    {
        return 0;
    }

    fprintf(stderr, "%d test(s) failed\n", failures);
    return 1;
}
