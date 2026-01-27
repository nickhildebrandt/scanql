#include "sql-validate.h"
#include "sql-token.h"
#include "test_asserts.h"

static int failures = 0;

static TokenStack make_stack(Token* toks, int len)
{
    TokenStack s = {.elems = toks, .len = len, .cap = len};
    return s;
}

static void test_valid_simple_select_star(void)
{
    Token toks[] = {
        {.value = "SELECT", .type = SELECT},
        {.value = "*", .type = STRING},
        {.value = ",", .type = COMMA},
        {.value = "a", .type = STRING},
        {.value = "FROM", .type = FROM},
        {.value = "t", .type = STRING},
        {.value = ";", .type = SEMICOLON},
    };
    TokenStack s = make_stack(toks, (int)(sizeof(toks) / sizeof(toks[0])));
    expect_true(&failures,
                validate_query(&s),
                __FILE__,
                __LINE__,
                "SELECT *,a FROM t; is valid");
}

static void test_valid_where_and_or(void)
{
    Token toks[] = {
        {.value = "SELECT", .type = SELECT},
        {.value = "name", .type = STRING},
        {.value = "FROM", .type = FROM},
        {.value = "users", .type = STRING},
        {.value = "WHERE", .type = WHERE},
        {.value = "name", .type = STRING},
        {.value = "=", .type = EQUALS},
        {.value = "'nick'", .type = STRING},
        {.value = "AND", .type = AND},
        {.value = "age", .type = STRING},
        {.value = "=", .type = EQUALS},
        {.value = "42", .type = NUMBER},
        {.value = ";", .type = SEMICOLON},
    };
    TokenStack s = make_stack(toks, (int)(sizeof(toks) / sizeof(toks[0])));
    expect_true(&failures,
                validate_query(&s),
                __FILE__,
                __LINE__,
                "SELECT ... WHERE ... AND ...; is valid");
}

static void test_invalid_missing_from(void)
{
    Token toks[] = {
        {.value = "SELECT", .type = SELECT},
        {.value = "a", .type = STRING},
        {.value = "t", .type = STRING},
        {.value = ";", .type = SEMICOLON},
    };
    TokenStack s = make_stack(toks, (int)(sizeof(toks) / sizeof(toks[0])));
    expect_true(&failures,
                !validate_query(&s),
                __FILE__,
                __LINE__,
                "SELECT a t; should be invalid (missing FROM)");
}

static void test_invalid_trailing_after_semicolon(void)
{
    Token toks[] = {
        {.value = "SELECT", .type = SELECT},
        {.value = "a", .type = STRING},
        {.value = "FROM", .type = FROM},
        {.value = "t", .type = STRING},
        {.value = ";", .type = SEMICOLON},
        {.value = "DROP", .type = STRING},
    };
    TokenStack s = make_stack(toks, (int)(sizeof(toks) / sizeof(toks[0])));
    expect_true(&failures,
                !validate_query(&s),
                __FILE__,
                __LINE__,
                "Trailing tokens after ';' should be invalid");
}

int main(void)
{
    test_valid_simple_select_star();
    test_valid_where_and_or();
    test_invalid_missing_from();
    test_invalid_trailing_after_semicolon();

    if (failures == 0)
        return 0;

    fprintf(stderr, "%d test(s) failed\n", failures);
    return 1;
}
