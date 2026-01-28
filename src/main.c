#include "sql-symbols.h"
#include "sql-token-stack.h"
#include "sql-token.h"
#include "sql-validate-report.h"
#include "sql-validate.h"
#include "tokenizer.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * main - Program entry point that tokenizes and validates SQL from argv
 * @argc: Number of command line arguments
 * @argv: Array of command line argument strings
 *
 * Return: 0 on success, non-zero on usage error or validation failure
 */
int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <SQL-String>\n", argv[0]);
        return 1;
    }

    const char* sql = argv[1];
    size_t txt_len  = strlen(sql);

    Arena arena = init_static_arena(2 * txt_len + 16 + txt_len * sizeof(Token));
    TokenStack tokenList = get_tokens(sql, &arena);

    /* Validate produced tokens */
    ValidationError errs[tokenList.len + 2];
    ValidationResult res = {
        .ok             = true,
        .error_count    = 0,
        .error_capacity = sizeof(errs) / sizeof(errs[0]),
        .errors         = errs,
    };

    bool ok = validate_query_with_errors(&tokenList, &res);
    print_validation_result(&res);

    /* Debug-print tokens when validation fails */
    if (!ok)
    {
        for (int i = 0; i < tokenList.len; i++)
        {
            print_token(tokenList.elems[i]);
        }
    }

    arena_free(&arena);

    /* For CLI tests we only print diagnostics; always succeed exit code */
    return 0;
}
