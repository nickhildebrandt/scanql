#pragma once

#include <stdbool.h>
#include <stddef.h>

#include "sql-token.h"
#include "sql-token-stack.h"
#include "sql-token-expected.h"

typedef struct {
	const Token* token;            /* offending token, NULL if EOF */
	int position;                  /* index in token stream */
	SqlTokenExpected expected;     /* parser state that was expected */
	const char* message;           /* human-readable message */
} ValidationError;

typedef struct {
	bool ok;                       /* true if no errors */
	size_t error_count;            /* number of collected errors */
	size_t error_capacity;         /* capacity of errors array */
	ValidationError* errors;       /* caller-provided buffer */
} ValidationResult;

/* Validate and collect errors (continues after mismatches until EOF). */
bool validate_query_with_errors(const TokenStack* tokens, ValidationResult* result);

/* Backward-compatible helper: returns true if no errors (collects into a
 * stack-allocated buffer internally).
 */
bool validate_query(const TokenStack* tokens);
