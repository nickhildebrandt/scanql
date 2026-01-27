#include "sql-validate.h"
#include "sql-validate-report.h"
#include "sql-token.h"
#include "test_asserts.h"

#include <stdio.h>

static int failures = 0;

/**
 * make_stack - helper to wrap a Token array into a TokenStack view
 */
static TokenStack make_stack(Token* toks, int len)
{
    TokenStack s = {.elems = toks, .len = len, .cap = len};
    return s;
}

static void test_report_formats_errors(void)
{
    Token toks[] = {
        {.value = "FROM", .type = FROM}, /* invalid start */
        {.value = "SELECT", .type = SELECT},
        {.value = "FROM", .type = FROM},
        {.value = "t", .type = STRING},
        {.value = ";", .type = SEMICOLON},
    };
    TokenStack s = make_stack(toks, (int)(sizeof(toks) / sizeof(toks[0])));

    ValidationError errs[8];
    ValidationResult res = {.errors = errs, .error_capacity = 8};
    bool ok = validate_query_with_errors(&s, &res);

    expect_true(&failures, !ok, __FILE__, __LINE__, "report test: query should fail");
    expect_true(&failures, res.error_count >= 1, __FILE__, __LINE__, "report test: errors collected");

    /* Smoke-test the printer (no assertions on stdout content) */
    print_validation_result(&res);
}

/**
 * main - Run all unit tests for SqlValidateReport
 */
int main(void)
{
    test_report_formats_errors();

    if (failures == 0)
        return 0;

    fprintf(stderr, "%d test(s) failed\n", failures);
    return 1;
}
