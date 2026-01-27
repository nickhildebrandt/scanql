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
    ValidationError errs[8];
    ValidationResult res = {.errors = errs, .error_capacity = 8};
    expect_true(&failures,
                !validate_query_with_errors(&s, &res),
                __FILE__,
                __LINE__,
                "SELECT a t; should be invalid (missing FROM)");
    expect_true(&failures,
                res.error_count >= 1,
                __FILE__,
                __LINE__,
                "error count recorded");
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
    ValidationError errs[8];
    ValidationResult res = {.errors = errs, .error_capacity = 8};
    expect_true(&failures,
                !validate_query_with_errors(&s, &res),
                __FILE__,
                __LINE__,
                "Trailing tokens after ';' should be invalid");
    expect_true(&failures,
                res.error_count >= 1,
                __FILE__,
                __LINE__,
                "error count recorded");
}

static void test_accumulates_multiple_errors(void)
{
    Token toks[] = {
        {.value = "FROM", .type = FROM}, /* wrong start */
        {.value = "SELECT", .type = SELECT},
        {.value = "FROM", .type = FROM}, /* missing select item before FROM */
        {.value = "t", .type = STRING},
        {.value = "WHERE", .type = WHERE},
        {.value = "=", .type = EQUALS}, /* missing lhs */
        {.value = "1", .type = NUMBER},
        {.value = ";", .type = SEMICOLON},
    };
    TokenStack s = make_stack(toks, (int)(sizeof(toks) / sizeof(toks[0])));
    ValidationError errs[16];
    ValidationResult res = {.errors = errs, .error_capacity = 16};
    bool ok = validate_query_with_errors(&s, &res);
    expect_true(&failures,
                !ok,
                __FILE__,
                __LINE__,
                "query with multiple issues should fail");
    expect_true(&failures,
                res.error_count >= 3,
                __FILE__,
                __LINE__,
                "multiple errors should be accumulated");
}

int main(void)
{
    test_valid_simple_select_star();
    test_valid_where_and_or();
    test_invalid_missing_from();
    test_invalid_trailing_after_semicolon();
    test_accumulates_multiple_errors();

    if (failures == 0)
        return 0;

    fprintf(stderr, "%d test(s) failed\n", failures);
    return 1;
}
