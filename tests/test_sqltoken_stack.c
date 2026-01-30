#include "sql-token-stack.h"

#include <assert.h>
#include <stddef.h>
#include <string.h>

static Token make_token(const char* val, SqlSymbols type)
{
    Token t = {.value = (char*)val, .type = type};
    return t;
}

static void test_append_increments_len_until_capacity(void)
{
    Token buf[3];
    TokenStack s = {.elems = buf, .len = 0, .cap = 3};

    append(&s, make_token("a", STRING));
    append(&s, make_token("b", STRING));
    append(&s, make_token("c", STRING));

    assert(s.len == 3);
    assert(strcmp(s.elems[0].value, "a") == 0);
    assert(strcmp(s.elems[1].value, "b") == 0);
    assert(strcmp(s.elems[2].value, "c") == 0);
}

static void test_append_does_not_overflow_capacity(void)
{
    Token buf[2];
    TokenStack s = {.elems = buf, .len = 0, .cap = 2};

    append(&s, make_token("x", STRING));
    append(&s, make_token("y", STRING));
    /* This should be ignored because len == cap */
    append(&s, make_token("z", STRING));

    assert(s.len == 2);
    assert(strcmp(s.elems[0].value, "x") == 0);
    assert(strcmp(s.elems[1].value, "y") == 0);
}

static void test_append_null_stack_is_safe(void)
{
    append(NULL, make_token("x", STRING));
}

int main(void)
{
    test_append_increments_len_until_capacity();
    test_append_does_not_overflow_capacity();
    test_append_null_stack_is_safe();
    return 0;
}
