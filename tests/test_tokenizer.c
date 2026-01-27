#include "tokenizer.h"
#include "sql-token.h"
#include "sql-validate.h"
#include "test_asserts.h"

#include <stdlib.h>
#include <string.h>

static int failures = 0;

/**
 * test_tokenizes_basic_select - Ensure tokenizer produces expected tokens
 */
static void test_tokenizes_basic_select(void)
{
    const char* sql = "SELECT name,age FROM users;";
    Arena arena     = init_static_arena(128);
    TokenStack toks = get_tokens(sql, &arena);

    expect_eq_size_t(&failures, toks.len, 7, __FILE__, __LINE__, "token count");
    expect_true(&failures, toks.elems[0].type == SELECT, __FILE__, __LINE__, "first token SELECT");
    expect_true(&failures, strcmp(toks.elems[1].value, "name") == 0, __FILE__, __LINE__, "second token name");
    expect_true(&failures, toks.elems[2].type == COMMA, __FILE__, __LINE__, "third token comma");
    expect_true(&failures, strcmp(toks.elems[3].value, "age") == 0, __FILE__, __LINE__, "fourth token age");
    expect_true(&failures, toks.elems[4].type == FROM, __FILE__, __LINE__, "fifth token FROM");
    expect_true(&failures, strcmp(toks.elems[5].value, "users") == 0, __FILE__, __LINE__, "sixth token table name");
    expect_true(&failures, toks.elems[6].type == SEMICOLON, __FILE__, __LINE__, "seventh token semicolon");

    free(toks.elems);
    arena_free(&arena);
}

/**
 * test_tokenizer_integrates_with_validator - Tokenizer output is accepted by validator
 */
static void test_tokenizer_integrates_with_validator(void)
{
    const char* sql = "SELECT a FROM t;";
    Arena arena     = init_static_arena(64);
    TokenStack toks = get_tokens(sql, &arena);

    ValidationError errs[8];
    ValidationResult res = {.errors = errs, .error_capacity = 8};
    bool ok = validate_query_with_errors(&toks, &res);

    expect_true(&failures, ok, __FILE__, __LINE__, "validator accepts tokenizer output");

    free(toks.elems);
    arena_free(&arena);
}

int main(void)
{
    test_tokenizes_basic_select();
    test_tokenizer_integrates_with_validator();

    if (failures == 0)
        return 0;

    fprintf(stderr, "%d test(s) failed\n", failures);
    return 1;
}
