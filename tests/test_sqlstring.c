#include "sqlstring.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int failures = 0;

/**
 * expect_true - Assert a boolean condition
 * @ok: Condition to check (non-zero means true)
 * @file: Source file name for diagnostics
 * @line: Source line number for diagnostics
 * @msg: Message describing the expectation
 *
 * Return: Nothing
 */
static void expect_true(int ok, const char* file, int line, const char* msg)
{
    if (ok)
    {
        return;
    }

    failures++;
    fprintf(stderr, "FAIL %s:%d: %s\n", file, line, msg);
}

/**
 * expect_eq_size_t - Assert two size_t values are equal
 * @got: Actual value
 * @want: Expected value
 * @file: Source file name for diagnostics
 * @line: Source line number for diagnostics
 * @msg: Message describing the expectation
 *
 * Return: Nothing
 */
static void expect_eq_size_t(size_t got,
                             size_t want,
                             const char* file,
                             int line,
                             const char* msg)
{
    if (got == want)
    {
        return;
    }

    failures++;
    fprintf(stderr,
            "FAIL %s:%d: %s (got %zu, want %zu)\n",
            file,
            line,
            msg,
            got,
            want);
}

/**
 * expect_eq_char - Assert two char values are equal
 * @got: Actual character
 * @want: Expected character
 * @file: Source file name for diagnostics
 * @line: Source line number for diagnostics
 * @msg: Message describing the expectation
 *
 * Return: Nothing
 */
static void
expect_eq_char(char got, char want, const char* file, int line, const char* msg)
{
    if (got == want)
    {
        return;
    }

    failures++;
    fprintf(stderr,
            "FAIL %s:%d: %s (got '%c'/%d, want '%c'/%d)\n",
            file,
            line,
            msg,
            got,
            (int)got,
            want,
            (int)want);
}

/**
 * test_new_and_length_and_get_char - Test creation, length and indexing
 *
 * Return: Nothing
 */
static void test_new_and_length_and_get_char(void)
{
    SqlString* s = sql_string_new("SELECT");
    expect_true(s != NULL, __FILE__, __LINE__, "sql_string_new returned NULL");
    expect_eq_size_t(sql_string_length(s),
                     6,
                     __FILE__,
                     __LINE__,
                     "sql_string_length(\"SELECT\") == 6");

    expect_eq_char(sql_string_get_char(s, 0),
                   'S',
                   __FILE__,
                   __LINE__,
                   "sql_string_get_char(s, 0) == 'S'");
    expect_eq_char(sql_string_get_char(s, 1),
                   'E',
                   __FILE__,
                   __LINE__,
                   "sql_string_get_char(s, 1) == 'E'");
    expect_eq_char(sql_string_get_char(s, 5),
                   'T',
                   __FILE__,
                   __LINE__,
                   "sql_string_get_char(s, 5) == 'T'");

    /* out of bounds */
    expect_eq_char(sql_string_get_char(s, -1),
                   '\0',
                   __FILE__,
                   __LINE__,
                   "sql_string_get_char(s, -1) == \\0");
    expect_eq_char(sql_string_get_char(s, 6),
                   '\0',
                   __FILE__,
                   __LINE__,
                   "sql_string_get_char(s, 6) == \\0");

    free_sql_string(s);
}

/**
 * test_append_and_length - Test appending nodes and indexing across nodes
 *
 * Return: Nothing
 */
static void test_append_and_length(void)
{
    SqlString* s = sql_string_new("A");
    expect_true(s != NULL, __FILE__, __LINE__, "sql_string_new returned NULL");

    append_sql_string(s, "BC");
    expect_eq_size_t(sql_string_length(s),
                     3,
                     __FILE__,
                     __LINE__,
                     "sql_string_length(\"A\" + \"BC\") == 3");

    /* indexing must work across appended nodes */
    expect_eq_char(sql_string_get_char(s, 0),
                   'A',
                   __FILE__,
                   __LINE__,
                   "sql_string_get_char(\"A\"+\"BC\", 0) == 'A'");
    expect_eq_char(sql_string_get_char(s, 1),
                   'B',
                   __FILE__,
                   __LINE__,
                   "sql_string_get_char(\"A\"+\"BC\", 1) == 'B'");
    expect_eq_char(sql_string_get_char(s, 2),
                   'C',
                   __FILE__,
                   __LINE__,
                   "sql_string_get_char(\"A\"+\"BC\", 2) == 'C'");
    expect_eq_char(sql_string_get_char(s, 3),
                   '\0',
                   __FILE__,
                   __LINE__,
                   "sql_string_get_char(\"A\"+\"BC\", 3) == \\0");

    free_sql_string(s);
}

/**
 * test_nocopy_append_does_not_crash - Test nocopy mode and append behavior
 *
 * Return: Nothing
 */
static void test_nocopy_append_does_not_crash(void)
{
    char* buf = (char*)malloc(4);
    expect_true(buf != NULL, __FILE__, __LINE__, "malloc(4) returned NULL");
    strcpy(buf, "XYZ");

    SqlString* s = sql_string_new_nocopy(buf);
    expect_true(s != NULL,
                __FILE__,
                __LINE__,
                "sql_string_new_nocopy returned NULL");

    /* append must not crash; caller keeps buf alive */
    append_sql_string(s, "1");
    expect_eq_size_t(sql_string_length(s),
                     4,
                     __FILE__,
                     __LINE__,
                     "sql_string_length(\"XYZ\" + \"1\") == 4");

    expect_eq_char(sql_string_get_char(s, 0),
                   'X',
                   __FILE__,
                   __LINE__,
                   "sql_string_get_char(\"XYZ\"+\"1\", 0) == 'X'");
    expect_eq_char(sql_string_get_char(s, 1),
                   'Y',
                   __FILE__,
                   __LINE__,
                   "sql_string_get_char(\"XYZ\"+\"1\", 1) == 'Y'");
    expect_eq_char(sql_string_get_char(s, 2),
                   'Z',
                   __FILE__,
                   __LINE__,
                   "sql_string_get_char(\"XYZ\"+\"1\", 2) == 'Z'");
    expect_eq_char(sql_string_get_char(s, 3),
                   '1',
                   __FILE__,
                   __LINE__,
                   "sql_string_get_char(\"XYZ\"+\"1\", 3) == '1'");

    /* free_sql_string must not free buf in nocopy mode */
    free_sql_string(s);

    /* if buf was incorrectly freed, this may crash under ASAN/valgrind; keep it
     * simple */
    expect_eq_char(buf[0],
                   'X',
                   __FILE__,
                   __LINE__,
                   "buf[0] == 'X' after free_sql_string(nocopy)");

    free(buf);
}

/**
 * main - Run all unit tests for SqlString
 *
 * Return: 0 if all tests passed, non-zero otherwise
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
