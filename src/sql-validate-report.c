#include "sql-validate-report.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "sql-symbols.h"

#define CLR_RED   "\033[31m"
#define CLR_YEL   "\033[33m"
#define CLR_DIM   "\033[90m"
#define CLR_RESET "\033[0m"

/**
 * expected_to_str - Convert expected state to string label
 */
static const char* expected_to_str(SqlSymbols e)
{
    switch (e)
    {
    case EXP_SELECT: return "SELECT";
    case EXP_SELECT_ITEM: return "SELECT_ITEM";
    case EXP_SELECT_CONT: return "SELECT_CONT";
    case EXP_TABLE: return "TABLE";
    case EXP_WHERE_OR_END: return "WHERE_OR_END";
    case EXP_CONDITION_LHS: return "COND_LHS";
    case EXP_CONDITION_OP: return "COND_OP";
    case EXP_CONDITION_RHS: return "COND_RHS";
    case EXP_CONDITION_CONT: return "COND_CONT";
    case EXP_END: return "END";
    default: return "?";
    }
}

/**
 * symbol_to_str - Convert lexer symbol to human-readable label
 */
static const char* symbol_to_str(SqlSymbols s)
{
    switch (s)
    {
    case SELECT: return "SELECT";
    case FROM: return "FROM";
    case WHERE: return "WHERE";
    case CREATE: return "CREATE";
    case TABLE: return "TABLE";
    case DROP: return "DROP";
    case IF: return "IF";
    case EXISTS: return "EXISTS";
    case INSERT: return "INSERT";
    case TYPE_TEXT: return "TYPE_TEXT";
    case TYPE_ENUM: return "TYPE_ENUM";
    case INTO: return "INTO";
    case NOT: return "NOT";
    case IN: return "IN";
    case null: return "NULL";
    case COMMA: return "COMMA";
    case EQUALS: return "EQUALS";
    case NEGATION: return "NEGATION";
    case NUMBER: return "NUMBER";
    case STRING: return "STRING";
    case SQL_NULL: return "SQL_NULL";
    case AND: return "AND";
    case OR: return "OR";
    case SEMICOLON: return "SEMICOLON";
    case ROUND_BRACKETS_OPEN: return "RO(";
    case ROUND_BRACKETS_CLOSE: return "RO)";
    case INLINE_COMMENT_MINUS: return "INLINE_COMMENT";
    case MULTILINE_COMMENT_OPEN: return "ML_COMMENT_OPEN";
    case MULTILINE_COMMENT_CLOSE: return "ML_COMMENT_CLOSE";
    default: return "?";
    }
}

/**
 * describe_token - Render a token into a small textual description
 * @e: validation error holding the token
 * @buf: output buffer
 * @n: buffer size
 */
static void describe_token(const ValidationError* e, char* buf, size_t n)
{
    if (!e || !buf || n == 0)
        return;

    const Token* t = e->token;
    if (!t)
    {
        snprintf(buf, n, "<EOF>");
        return;
    }

    const char* kind = symbol_to_str(t->type);
    const char* val  = (t->value && t->value[0]) ? t->value : "";

    if (val[0])
        snprintf(buf, n, "%s \"%s\"", kind, val);
    else
        snprintf(buf, n, "%s", kind);
}

/**
 * print_validation_result - Pretty-print validation outcome with ANSI colors
 * @result: validation result to print (must not be NULL)
 */
void print_validation_result(const ValidationResult* result)
{
    assert(result != NULL);

    if (result->ok)
    {
        printf(CLR_DIM "validation: ok" CLR_RESET "\n");
        return;
    }

    printf(CLR_RED "validation failed (%zu error%s)" CLR_RESET "\n",
           result->error_count,
           result->error_count == 1 ? "" : "s");

    for (size_t i = 0; i < result->error_count; ++i)
    {
        const ValidationError* e = &result->errors[i < result->error_capacity ? i : result->error_capacity - 1];
        char tokbuf[128];
        describe_token(e, tokbuf, sizeof(tokbuf));

        printf("  %s%-5s%s at token %d: expected %s, got %s%s%s (%s)\n",
               CLR_RED, "error", CLR_RESET,
               e->position,
               expected_to_str(e->expected),
               CLR_YEL, tokbuf, CLR_RESET,
               e->message ? e->message : "");

        /* Prevent out-of-bounds if errors overflowed capacity */
        if (i >= result->error_capacity && result->error_capacity > 0)
            break;
    }
}
