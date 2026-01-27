#pragma once

#include "sql-validate.h"

/* Pretty-print validation errors to stdout with ANSI colors. */
void print_validation_result(const ValidationResult* result);
