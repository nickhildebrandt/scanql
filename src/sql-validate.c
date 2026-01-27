#include "sql-validate.h"

#include <stdio.h>
#include <string.h>

#include "sql-symbols.h"
#include "sql-token-expected.h"

typedef struct {
    SqlTokenExpected expected;
} parser_t;
/* Helpers to interpret existing Token/SqlSymbols into semantic roles */
static bool token_is_select(const Token *t)     { return t && t->type == SELECT; }
static bool token_is_from(const Token *t)       { return t && t->type == FROM; }
static bool token_is_where(const Token *t)      { return t && t->type == WHERE; }
static bool token_is_comma(const Token *t)      { return t && t->type == COMMA; }
static bool token_is_equals(const Token *t)     { return t && t->type == EQUALS; }
static bool token_is_and(const Token *t)        { return t && t->type == AND; }
static bool token_is_or(const Token *t)         { return t && t->type == OR; }
static bool token_is_semicolon(const Token *t)  { return t && t->type == SEMICOLON; }

static bool token_is_star(const Token *t)
{
    return t && t->type == STRING && t->value && t->value[0] == '*' && t->value[1] == '\0';
}

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

/* accept: core state machine using existing Token/SqlSymbols. */
static bool accept(parser_t *p, const Token *tok, bool is_eof)
{
    switch (p->expected)
    {
    case EXP_SELECT:
        if (!is_eof && token_is_select(tok)) { p->expected = EXP_SELECT_ITEM; return true; }
        return false;

    case EXP_SELECT_ITEM:
        if (!is_eof && (token_is_identifier(tok) || token_is_star(tok)))
        { p->expected = EXP_SELECT_CONT; return true; }
        return false;

    case EXP_SELECT_CONT:
        if (!is_eof && token_is_comma(tok)) { p->expected = EXP_SELECT_ITEM; return true; }
        if (!is_eof && token_is_from(tok))  { p->expected = EXP_TABLE; return true; }
        return false;

    case EXP_TABLE:
        if (!is_eof && token_is_identifier(tok)) { p->expected = EXP_WHERE_OR_END; return true; }
        return false;

    case EXP_WHERE_OR_END:
        if (!is_eof && token_is_where(tok))     { p->expected = EXP_CONDITION_LHS; return true; }
        if (!is_eof && token_is_semicolon(tok)) { p->expected = EXP_END; return true; }
        return false;

    case EXP_CONDITION_LHS:
        if (!is_eof && token_is_identifier(tok)) { p->expected = EXP_CONDITION_OP; return true; }
        return false;

    case EXP_CONDITION_OP:
        if (!is_eof && token_is_equals(tok)) { p->expected = EXP_CONDITION_RHS; return true; }
        return false;

    case EXP_CONDITION_RHS:
        if (!is_eof && token_is_literal(tok)) { p->expected = EXP_CONDITION_CONT; return true; }
        return false;

    case EXP_CONDITION_CONT:
        if (!is_eof && (token_is_and(tok) || token_is_or(tok))) { p->expected = EXP_CONDITION_LHS; return true; }
        if (!is_eof && token_is_semicolon(tok))                 { p->expected = EXP_END; return true; }
        return false;

    case EXP_END:
        if (is_eof) return true;
        return false;

    default:
        return false;
    }
}

static void record_error(ValidationResult* r,
                         const Token* tok,
                         int pos,
                         SqlTokenExpected expected,
                         const char* msg)
{
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

bool validate_query_with_errors(const TokenStack* tokens, ValidationResult* result)
{
    if (!tokens || !result)
        return false;

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
