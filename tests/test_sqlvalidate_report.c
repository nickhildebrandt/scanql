#include "sql-validate.h"
#include "sql-validate-report.h"
#include "sql-token.h"

#include <assert.h>
#include <stdio.h>

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

    assert(!ok);
    assert(res.error_count >= 1);

    /* Smoke-test the printer (no assertions on stdout content) */
    print_validation_result(&res);
}

static void test_report_formats_new_symbols(void)
{
    /* Manually craft a result using newly added symbols to exercise symbol_to_str */
    ValidationError errs[2];
    errs[0].token    = &(Token){.value = "UPDATE", .type = UPDATE};
    errs[0].position = 0;
    errs[0].expected = SELECT_ITEM; /* arbitrary state */
    errs[0].message  = "unexpected token";

    errs[1].token    = &(Token){.value = "JOIN", .type = JOIN};
    errs[1].position = 1;
    errs[1].expected = CONDITION_LHS;
    errs[1].message  = "unexpected token";

    ValidationResult res = {
        .ok = false,
        .error_count = 2,
        .error_capacity = 2,
        .errors = errs,
    };

    /* Smoke-test that printer handles new enum entries without falling back to '?' */
    print_validation_result(&res);
}

/**
 * main - Run all unit tests for SqlValidateReport
 */
int main(void)
{
    test_report_formats_errors();
    test_report_formats_new_symbols();
    return 0;
}
