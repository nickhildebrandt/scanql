#include "sql-token.h"
#include "sql-validate.h"
#include "tokenizer.h"

#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/**
 * test_tokenizes_basic_select - Ensure tokenizer produces expected tokens
 */
static void test_tokenizes_basic_select(void)
{
    const char* sql = "SELECT name,age FROM users;";
    size_t sql_len  = strlen(sql);
    Arena arena     = init_static_arena(sql_len * 2 + sql_len * sizeof(Token));
    TokenStack toks = get_tokens(sql, &arena);

    assert(toks.len == 7);
    assert(toks.elems[0].type == SELECT);
    assert(strcmp(toks.elems[1].value, "name") == 0);
    assert(toks.elems[2].type == COMMA);
    assert(strcmp(toks.elems[3].value, "age") == 0);
    assert(toks.elems[4].type == FROM);
    assert(strcmp(toks.elems[5].value, "users") == 0);
    assert(toks.elems[6].type == SEMICOLON);

    arena_free(&arena);
}

/**
 * test_tokenizer_integrates_with_validator - Tokenizer output is accepted by
 * validator
 */
static void test_tokenizer_integrates_with_validator(void)
{
    const char* sql = "SELECT a FROM t;";
    size_t sql_len  = strlen(sql);
    Arena arena     = init_static_arena(sql_len * 2 + sql_len * sizeof(Token));
    TokenStack toks = get_tokens(sql, &arena);

    ValidationError errs[8];
    ValidationResult res = {.errors = errs, .error_capacity = 8};
    bool ok              = validate_query_with_errors(&toks, &res);

    assert(ok);

    arena_free(&arena);
}

int main(void)
{
    test_tokenizes_basic_select();
    test_tokenizer_integrates_with_validator();
    return 0;
}
