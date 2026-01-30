#include "sql-string.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * test_new_and_length_and_get_char - Validate creation, length, and indexing
 */
static void test_new_and_length_and_get_char(void)
{
    SqlString* s = sql_string_new("SELECT");
    assert(s != NULL);
    assert(sql_string_length(s) == 6);

    assert(sql_string_get_char(s, 0) == 'S');
    assert(sql_string_get_char(s, 1) == 'E');
    assert(sql_string_get_char(s, 5) == 'T');

    /* out of bounds */
    assert(sql_string_get_char(s, -1) == '\0');
    assert(sql_string_get_char(s, 6) == '\0');

    free_sql_string(s);
}

/**
 * test_append_and_length - Validate append and total length calculation
 */
static void test_append_and_length(void)
{
    SqlString* s = sql_string_new("A");
    assert(s != NULL);

    append_sql_string(s, "BC");
    assert(sql_string_length(s) == 3);

    /* indexing must work across appended nodes */
    assert(sql_string_get_char(s, 0) == 'A');
    assert(sql_string_get_char(s, 1) == 'B');
    assert(sql_string_get_char(s, 2) == 'C');
    assert(sql_string_get_char(s, 3) == '\0');

    free_sql_string(s);
}

/**
 * test_nocopy_append_does_not_crash - Ensure nocopy strings remain valid
 */
static void test_nocopy_append_does_not_crash(void)
{
    char* buf = (char*)malloc(4);
    assert(buf != NULL);
    strcpy(buf, "XYZ");

    SqlString* s = sql_string_new_nocopy(buf);
    assert(s != NULL);

    /* append must not crash; caller keeps buf alive */
    append_sql_string(s, "1");
    assert(sql_string_length(s) == 4);

    assert(sql_string_get_char(s, 0) == 'X');
    assert(sql_string_get_char(s, 1) == 'Y');
    assert(sql_string_get_char(s, 2) == 'Z');
    assert(sql_string_get_char(s, 3) == '1');

    /* free_sql_string must not free buf in nocopy mode */
    free_sql_string(s);

    /* if buf was incorrectly freed, this may crash under ASAN/valgrind; keep it
     * simple */
    assert(buf[0] == 'X');

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
    return 0;
}
