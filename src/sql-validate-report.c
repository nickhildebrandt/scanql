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
    case SELECT: return "SELECT";
    case SELECT_ITEM: return "SELECT_ITEM";
    case SELECT_CONT: return "SELECT_CONT";
    case TABLE: return "TABLE";
    case WHERE_OR_END: return "WHERE_OR_END";
    case CONDITION_LHS: return "COND_LHS";
    case CONDITION_OP: return "COND_OP";
    case CONDITION_RHS: return "COND_RHS";
    case CONDITION_CONT: return "COND_CONT";
    case END: return "END";
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
    case UPDATE: return "UPDATE";
    case DELETE: return "DELETE";
    case FROM: return "FROM";
    case WHERE: return "WHERE";
    case CREATE: return "CREATE";
    case TABLE: return "TABLE";
    case DROP: return "DROP";
    case IF: return "IF";
    case EXISTS: return "EXISTS";
    case INSERT: return "INSERT";
    case VALUES: return "VALUES";
    case SET: return "SET";
    case RETURNING: return "RETURNING";
    case DISTINCT: return "DISTINCT";
    case ALL_SYM: return "ALL";
    case UNION: return "UNION";
    case INTERSECT: return "INTERSECT";
    case EXCEPT: return "EXCEPT";
    case JOIN: return "JOIN";
    case INNER: return "INNER";
    case LEFT: return "LEFT";
    case RIGHT: return "RIGHT";
    case FULL: return "FULL";
    case OUTER: return "OUTER";
    case CROSS: return "CROSS";
    case ON: return "ON";
    case AS: return "AS";
    case GROUP: return "GROUP";
    case BY: return "BY";
    case HAVING: return "HAVING";
    case ORDER: return "ORDER";
    case ASC: return "ASC";
    case DESC: return "DESC";
    case LIMIT: return "LIMIT";
    case OFFSET: return "OFFSET";
    case FETCH: return "FETCH";
    case ROWS: return "ROWS";
    case ONLY: return "ONLY";
    case TOP: return "TOP";
    case BETWEEN: return "BETWEEN";
    case LIKE: return "LIKE";
    case ILIKE: return "ILIKE";
    case IS: return "IS";
    case ANY_SYM: return "ANY";
    case SOME: return "SOME";
    case ALL_PRED: return "ALL_PRED";
    case EXISTS_PRED: return "EXISTS_PRED";
    case TRUE_SYM: return "TRUE";
    case FALSE_SYM: return "FALSE";
    case CASE: return "CASE";
    case WHEN: return "WHEN";
    case THEN: return "THEN";
    case ELSE_SYM: return "ELSE";
    case END_KW: return "END_KW";
    case TYPE_TEXT: return "TYPE_TEXT";
    case TYPE_ENUM: return "TYPE_ENUM";
    case TYPE_INT: return "TYPE_INT";
    case TYPE_INTEGER: return "TYPE_INTEGER";
    case TYPE_BIGINT: return "TYPE_BIGINT";
    case TYPE_SMALLINT: return "TYPE_SMALLINT";
    case TYPE_DECIMAL: return "TYPE_DECIMAL";
    case TYPE_NUMERIC: return "TYPE_NUMERIC";
    case TYPE_REAL: return "TYPE_REAL";
    case TYPE_DOUBLE: return "TYPE_DOUBLE";
    case TYPE_FLOAT: return "TYPE_FLOAT";
    case TYPE_BOOLEAN: return "TYPE_BOOLEAN";
    case TYPE_CHAR: return "TYPE_CHAR";
    case TYPE_VARCHAR: return "TYPE_VARCHAR";
    case TYPE_DATE: return "TYPE_DATE";
    case TYPE_TIME: return "TYPE_TIME";
    case TYPE_TIMESTAMP: return "TYPE_TIMESTAMP";
    case TYPE_BLOB: return "TYPE_BLOB";
    case INTO: return "INTO";
    case NOT: return "NOT";
    case IN: return "IN";
    case null: return "NULL";
    case COMMA: return "COMMA";
    case DOT: return "DOT";
    case SEMICOLON: return "SEMICOLON";
    case PLUS: return "PLUS";
    case MINUS_OP: return "MINUS";
    case STAR_OP: return "STAR";
    case SLASH: return "SLASH";
    case PERCENT: return "PERCENT";
    case CARET: return "CARET";
    case CONCAT_OP: return "CONCAT";
    case EQUALS: return "EQUALS";
    case NOT_EQUAL: return "NOT_EQUAL";
    case LESS: return "LESS";
    case GREATER: return "GREATER";
    case LESS_EQUAL: return "LESS_EQUAL";
    case GREATER_EQUAL: return "GREATER_EQUAL";
    case NEGATION: return "NEGATION";
    case NUMBER: return "NUMBER";
    case STRING: return "STRING";
    case SQL_NULL: return "SQL_NULL";
    case PARAMETER: return "PARAM";
    case IDENTIFIER: return "IDENTIFIER";
    case AND: return "AND";
    case OR: return "OR";
    case ROUND_BRACKETS_OPEN: return "RO(";
    case ROUND_BRACKETS_CLOSE: return "RO)";
    case SQUARE_BRACKETS_OPEN: return "SO[";
    case SQUARE_BRACKETS_CLOSE: return "SO]";
    case INLINE_COMMENT_MINUS: return "INLINE_COMMENT";
    case MULTILINE_COMMENT_OPEN: return "ML_COMMENT_OPEN";
    case MULTILINE_COMMENT_CLOSE: return "ML_COMMENT_CLOSE";
    case SELECT_CONT: return "EXPECT_SELECT_CONT";
    case SELECT_ITEM: return "EXPECT_SELECT_ITEM";
    case WHERE_OR_END: return "EXPECT_WHERE_OR_END";
    case CONDITION_LHS: return "EXPECT_COND_LHS";
    case CONDITION_OP: return "EXPECT_COND_OP";
    case CONDITION_RHS: return "EXPECT_COND_RHS";
    case CONDITION_CONT: return "EXPECT_COND_CONT";
    case END: return "EXPECT_END";
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
