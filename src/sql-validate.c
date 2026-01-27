#include "sql-validate.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "sql-symbols.h"
#include "sql-token-expected.h"

typedef struct {
    SqlTokenExpected expected;
} parser_t;
/* Helpers to interpret existing Token/SqlSymbols into semantic roles */
/**
 * token_is_select - Check if token is SELECT keyword
 */
static bool token_is_select(const Token *t)     { return t && t->type == SELECT; }
/** token_is_from - Check if token is FROM keyword */
static bool token_is_from(const Token *t)       { return t && t->type == FROM; }
/** token_is_where - Check if token is WHERE keyword */
static bool token_is_where(const Token *t)      { return t && t->type == WHERE; }
/** token_is_comma - Check if token is comma */
static bool token_is_comma(const Token *t)      { return t && t->type == COMMA; }
/** token_is_equals - Check if token is equals sign */
static bool token_is_equals(const Token *t)     { return t && t->type == EQUALS; }
/** token_is_and - Check if token is AND */
static bool token_is_and(const Token *t)        { return t && t->type == AND; }
/** token_is_or - Check if token is OR */
static bool token_is_or(const Token *t)         { return t && t->type == OR; }
/** token_is_semicolon - Check if token is semicolon */
static bool token_is_semicolon(const Token *t)  { return t && t->type == SEMICOLON; }

/**
 * token_is_star - Check if token lexeme is '*'
 */
static bool token_is_star(const Token *t)
{
    return t && t->type == STRING && t->value && t->value[0] == '*' && t->value[1] == '\0';
}

/**
 * token_is_literal - Check if token represents a literal value
 */
static bool token_is_literal(const Token *t)
{
    if (!t)
        return false;
    if (t->type == NUMBER)
        return true;
    if (t->type == STRING && t->value)
    {
        char c = t->value[0];
        if (c == '\'' || c == '"')
            return true;
        if (c >= '0' && c <= '9')
            return true;
    }
    return false;
}

/**
 * token_is_identifier - Check if token represents an identifier
 */
static bool token_is_identifier(const Token *t)
{
    if (!t)
        return false;
    if (t->type != STRING)
        return false;
    if (!t->value || t->value[0] == '\0')
        return false;
    char c = t->value[0];
    if (c == '\'' || c == '"' || c == '*')
        return false;
    if (c >= '0' && c <= '9')
        return false;
    return true;
}

/**
 * token_is_valid_lexeme - Filter out tokens we do not handle in validator
 */
static bool token_is_valid_lexeme(const Token *t)
{
    if (!t)
        return false;
    switch (t->type)
    {
    case SELECT:
    case FROM:
    case WHERE:
    case COMMA:
    case EQUALS:
    case AND:
    case OR:
    case SEMICOLON:
    case NUMBER:
    case STRING:
        return true;
    default:
        return false;
    }
}

/**
 * accept - Core deterministic state transition
 * @p: parser state holder
 * @tok: current token (NULL when is_eof is true)
 * @is_eof: true when tok is EOF sentinel
 *
 * Returns: true when transition is valid and state is advanced; false otherwise.
 */
static bool accept(parser_t *p, const Token *tok, bool is_eof)
{
    /* If this is EOF, only accept when we're in the terminal state */
    if (is_eof)
        return p->expected == EXP_END;

    switch (p->expected)
    {
    case EXP_SELECT:
        if (token_is_select(tok)) { p->expected = EXP_SELECT_ITEM; return true; }
        return false;

    case EXP_SELECT_ITEM:
        if (token_is_identifier(tok) || token_is_star(tok)) { p->expected = EXP_SELECT_CONT; return true; }
        return false;

    case EXP_SELECT_CONT:
        if (token_is_comma(tok)) { p->expected = EXP_SELECT_ITEM; return true; }
        if (token_is_from(tok))  { p->expected = EXP_TABLE; return true; }
        return false;

    case EXP_TABLE:
        if (token_is_identifier(tok)) { p->expected = EXP_WHERE_OR_END; return true; }
        return false;

    case EXP_WHERE_OR_END:
        if (token_is_where(tok))     { p->expected = EXP_CONDITION_LHS; return true; }
        if (token_is_semicolon(tok)) { p->expected = EXP_END; return true; }
        return false;

    case EXP_CONDITION_LHS:
        if (token_is_identifier(tok)) { p->expected = EXP_CONDITION_OP; return true; }
        return false;

    case EXP_CONDITION_OP:
        if (token_is_equals(tok)) { p->expected = EXP_CONDITION_RHS; return true; }
        return false;

    case EXP_CONDITION_RHS:
        if (token_is_literal(tok)) { p->expected = EXP_CONDITION_CONT; return true; }
        return false;

    case EXP_CONDITION_CONT:
        if (token_is_and(tok) || token_is_or(tok)) { p->expected = EXP_CONDITION_LHS; return true; }
        if (token_is_semicolon(tok))               { p->expected = EXP_END; return true; }
        return false;

    default:
        return false;
    }
}

/**
 * record_error - Append an error into the result buffer if space remains
 * @r: validation result accumulator
 * @tok: offending token (NULL for EOF)
 * @pos: token index
 * @expected: expected parser state
 * @msg: short diagnostic string (must outlive the result)
 */
static void record_error(ValidationResult* r,
                         const Token* tok,
                         int pos,
                         SqlTokenExpected expected,
                         const char* msg)
{
    assert(r != NULL);
    r->ok = false;
    if (r->error_count < r->error_capacity)
    {
        ValidationError* e = &r->errors[r->error_count];
        e->token          = tok;
        e->position       = pos;
        e->expected       = expected;
        e->message        = msg;
    }
    r->error_count++;
}

/**
 * validate_query_with_errors - Validate and collect all errors
 * @tokens: token stack to validate
 * @result: output accumulator (caller provides storage)
 *
 * Returns: true if no errors, false otherwise. Continues after mismatches.
 */
bool validate_query_with_errors(const TokenStack* tokens, ValidationResult* result)
{
    assert(tokens != NULL);
    assert(result != NULL);

    result->ok           = true;
    result->error_count  = 0;

    parser_t p = {.expected = EXP_SELECT};

    /* iterate through tokens, treat array end as EOF */
    for (int i = 0; i <= tokens->len; ++i)
    {
        bool is_eof = (i == tokens->len);
        const Token *t = is_eof ? NULL : &tokens->elems[i];

        if (!is_eof && !token_is_valid_lexeme(t))
        {
            record_error(result, t, i, p.expected, "invalid token");
            continue;
        }

        if (!accept(&p, t, is_eof))
        {
            record_error(result, t, i, p.expected, "unexpected token");
            continue;
        }
    }

    if (p.expected != EXP_END)
    {
        record_error(result, NULL, tokens->len, p.expected, "incomplete query");
    }

    return result->ok;
}

/**
 * validate_query - Convenience wrapper that only reports success/failure
 * @tokens: token stack to validate
 *
 * Returns: true if query is valid, false otherwise.
 */
bool validate_query(const TokenStack *tokens)
{
    if (!tokens)
        return false;

    ValidationError buf[tokens->len + 2];
    ValidationResult res = {
        .ok = true,
        .error_count = 0,
        .error_capacity = sizeof(buf) / sizeof(buf[0]),
        .errors = buf,
    };

    return validate_query_with_errors(tokens, &res);
}
