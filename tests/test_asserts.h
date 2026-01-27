#pragma once

#include <stddef.h>
#include <stdio.h>

/**
 * expect_true - Assert a boolean condition
 * @failures: Pointer to failure counter to increment on failure
 * @ok: Condition to check (non-zero means true)
 * @file: Source file name for diagnostics
 * @line: Source line number for diagnostics
 * @msg: Message describing the expectation
 *
 * Return: Nothing
 */
static inline void
expect_true(int* failures, int ok, const char* file, int line, const char* msg)
{
    if (ok)
    {
        return;
    }

    (*failures)++;
    fprintf(stderr, "FAIL %s:%d: %s\n", file, line, msg);
}

/**
 * expect_eq_size_t - Assert two size_t values are equal
 * @failures: Pointer to failure counter to increment on failure
 * @got: Actual value
 * @want: Expected value
 * @file: Source file name for diagnostics
 * @line: Source line number for diagnostics
 * @msg: Message describing the expectation
 *
 * Return: Nothing
 */
static inline void expect_eq_size_t(int* failures,
                                    size_t got,
                                    size_t want,
                                    const char* file,
                                    int line,
                                    const char* msg)
{
    if (got == want)
    {
        return;
    }

    (*failures)++;
    fprintf(stderr,
            "FAIL %s:%d: %s (got %zu, want %zu)\n",
            file,
            line,
            msg,
            got,
            want);
}

/**
 * expect_eq_char - Assert two char values are equal
 * @failures: Pointer to failure counter to increment on failure
 * @got: Actual character
 * @want: Expected character
 * @file: Source file name for diagnostics
 * @line: Source line number for diagnostics
 * @msg: Message describing the expectation
 *
 * Return: Nothing
 */
static inline void expect_eq_char(int* failures,
                                  char got,
                                  char want,
                                  const char* file,
                                  int line,
                                  const char* msg)
{
    if (got == want)
    {
        return;
    }

    (*failures)++;
    fprintf(stderr,
            "FAIL %s:%d: %s (got '%c'/%d, want '%c'/%d)\n",
            file,
            line,
            msg,
            got,
            (int)got,
            want,
            (int)want);
}

/**
 * expect_eq_ptr - Assert two pointers are equal
 * @failures: Pointer to failure counter to increment on failure
 * @got: Actual pointer
 * @want: Expected pointer
 * @file: Source file name for diagnostics
 * @line: Source line number for diagnostics
 * @msg: Message describing the expectation
 *
 * Return: Nothing
 */
static inline void expect_eq_ptr(int* failures,
                                 const void* got,
                                 const void* want,
                                 const char* file,
                                 int line,
                                 const char* msg)
{
    if (got == want)
    {
        return;
    }

    (*failures)++;
    fprintf(stderr,
            "FAIL %s:%d: %s (got %p, want %p)\n",
            file,
            line,
            msg,
            got,
            want);
}
