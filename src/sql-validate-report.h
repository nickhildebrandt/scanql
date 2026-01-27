#pragma once

#include "sql-validate.h"

/**
 * print_validation_result - Pretty-print validation errors with ANSI colors
 * @result: validation result to print (must not be NULL)
 */
void print_validation_result(const ValidationResult* result);
