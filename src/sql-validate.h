#pragma once

#include <stdbool.h>
#include <stddef.h>

#include "sql-token.h"
#include "sql-token-stack.h"
#include "sql-token-expected.h"

/**
 * struct ValidationError - A single validation error detail
 * @token: offending token (NULL when the error relates to EOF)
 * @position: index in the token stream (0-based)
 * @expected: parser state that was expected at this position
 * @message: human-readable diagnostic message
 */
typedef struct {
	const Token* token;
	int position;
	SqlTokenExpected expected;
	const char* message;
} ValidationError;

/**
 * struct ValidationResult - Aggregated validation outcome
 * @ok: true when no errors were found
 * @error_count: total number of errors detected (may exceed capacity)
 * @error_capacity: capacity of the caller-provided @errors buffer
 * @errors: pointer to caller-provided buffer to store up to @error_capacity errors
 */
typedef struct {
	bool ok;
	size_t error_count;
	size_t error_capacity;
	ValidationError* errors;
} ValidationResult;

/**
 * validate_query_with_errors - Validate a token stream and collect all errors
 * @tokens: token stack produced by the existing lexer
 * @result: output structure holding errors and summary
 *
 * Returns: true if no errors were found, false otherwise. Continues after
 * mismatches until EOF to accumulate all errors.
 */
bool validate_query_with_errors(const TokenStack* tokens, ValidationResult* result);

/**
 * validate_query - Backward-compatible validator that only reports success/failure
 * @tokens: token stack produced by the existing lexer
 *
 * Returns: true if the query is valid for the restricted subset, false otherwise.
 */
bool validate_query(const TokenStack* tokens);
