#pragma once

#include <stdbool.h>
#include "sql-token.h"
#include "sql-token-stack.h"

/* Public entry point: validate tokens produced by existing lexer/tokenizer.
 * Returns true if query is valid for the restricted subset, false otherwise.
 */
bool validate_query(const TokenStack *tokens);
