#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * SqlToken - Enumeration of SQL token types
 *
 * Only the tokens needed for DML statement validation are included.
 * Parser-state symbols have been removed; transitions are encoded in
 * expected_table[] instead.
 */

typedef enum : unsigned short
{
    SELECT,
    FROM,
    WHERE,
    UPDATE,
    DELETE,
    INSERT,
    INTO,
    VALUES,
    SET,
    JOIN,

    COMMA,
    SEMICOLON,
    EQUALS,
    STAR,

    NUMBER,
    DOUBLE_QUOTED_VALUE,
    SINGLE_QUOTED_VALUE,
    SQL_IDENTIFIER,

    AND,
    OR,

    ROUND_BRACKETS_OPEN,
    ROUND_BRACKETS_CLOSE,

    END
} SqlSymbols;

/*
 * symbol_to_str - usage by indexing with enum value from SqlSymbols
 */
const char* symbol_to_str[] = {"SELECT",
                               "FROM",
                               "WHERE",
                               "UPDATE",
                               "DELETE",
                               "INSERT",
                               "INTO",
                               "VALUES",
                               "SET",
                               "JOIN",

                               "COMMA",
                               "SEMICOLON",
                               "EQUALS",
                               "STAR",

                               "NUMBER",
                               "DOUBLE_QUOTED_VALUE",
                               "SINGLE_QUOTED_VALUE",
                               "SQL_IDENTIFIER",

                               "AND",
                               "OR",

                               "ROUND_BRACKETS_OPEN",
                               "ROUND_BRACKETS_CLOSE",

                               "END"};

typedef struct
{
    char* value;
    SqlSymbols type;
} Token;

void print_token(Token token)
{
    printf("Token { value: < %s >, type: %s}\n",
           token.value,
           symbol_to_str[token.type]);
}

/**
 * struct Arena - Simple bump allocator used by the tokenizer
 * @data: backing buffer
 * @size: bytes currently used
 * @capacity: total bytes available
 */
typedef struct
{
    unsigned char* data;
    size_t size;
    size_t capacity;
} Arena;

typedef struct
{
    Token* elems; // Pointer to an array of Token elements, representing the
                  // stack's contents.
    int len; // The current length of the stack, indicating how many tokens are
             // currently in it.
    int cap; // The capacity of the stack, which indicates the maximum number of
             // tokens it can hold before needing to be resized.
} TokenStack;

/**
 * append - Append a Token to the TokenStack
 * @list: Pointer to the TokenStack to append to
 * @token: Token to append
 *
 * Return: Nothing
 */
void append(TokenStack* list, Token token)
{
    if (list && list->elems && list->len < list->cap)
    {
        list->elems[list->len] = token;
        list->len += 1;
    }
}

/**
 * init_static_arena - Allocate a bump arena of given capacity
 * @capacity: number of bytes to reserve
 *
 * Return: initialized Arena with allocated buffer
 */
Arena init_static_arena(size_t capacity)
{
    unsigned char* data = malloc(capacity);
    Arena a             = {.data = data, .size = 0, .capacity = capacity};
    return a;
}

/**
 * static_arena_alloc - Allocate a slice from the arena without freeing
 * @arena: arena to allocate from
 * @size: bytes requested
 *
 * Return: pointer to allocated slice or NULL if insufficient space
 */
static void* static_arena_alloc(Arena* arena, size_t size)
{
    if (!arena || !arena->data)
        return NULL;

    if (arena->size + size <= arena->capacity)
    {
        unsigned char* data = &arena->data[arena->size];
        arena->size += size;
        return data;
    }
    return NULL;
}

/**
 * arena_free - Release arena backing memory
 * @arena: arena to free
 */
void arena_free(Arena* arena)
{
    if (!arena)
        return;
    free(arena->data);
    arena->data     = NULL;
    arena->size     = 0;
    arena->capacity = 0;
}

/**
 * fuzzy_match - Check whether two strings are similar (1 char tolerance)
 * @to_compare: the input string to test
 * @compare_to: the reference string to match against
 *
 * Returns false immediately when @to_compare is shorter than @compare_to.
 * Otherwise counts character mismatches up to the shorter length and returns
 * true when fewer than 2 differences are found.
 *
 * NOTE: Simple algorithm; only fuzzy for strings of similar length.
 *
 * Return: true if strings are considered a match, false otherwise.
 */
bool fuzzy_match(const char* to_compare, const char* compare_to)
{
    // NOTE: Bad fuzzy match algo, but is fuzzy only on bigger length
    int differences = 0;

    int to_compare_len = strlen(to_compare);
    int compare_to_len = strlen(compare_to);

    if (to_compare_len < compare_to_len)
    {
        return false;
    }

    int shorter_len =
        compare_to_len < to_compare_len ? compare_to_len : to_compare_len;

    for (int i = 0; i < shorter_len; i++)
    {
        int to_compare_char = toupper(to_compare[i]);

        bool in = false;
        for (int j = i; j < shorter_len; j++)
        {
            if (to_compare_char == toupper(compare_to[j]) && i == j)
            {
                in = true;
                break;
            }
        }

        if (!in)
        {
            differences++;
        }
    }

    if (differences < 2)
    {
        return true;
    }

    return false;
}

/**
 * match - Case-insensitive exact string comparison
 * @to_compare: the input string to test
 * @compare_to: the reference string to match against
 *
 * Return: true if both strings have equal length and identical characters
 *         (ignoring case), false otherwise.
 */
bool match(const char* to_compare, const char* compare_to)
{
    unsigned long compare_to_len = strlen(compare_to);
    if (strlen(to_compare) == compare_to_len)
    {
        for (int i = 0; i < (int)compare_to_len; i++)
        {
            if (toupper(to_compare[i]) != toupper(compare_to[i]))
            {
                return false;
            }
        }
        return true;
    }
    return false;
}

typedef Token Keyword;
Keyword keywords[] = {
    {"select", SELECT},
    {"from", FROM},
    {"where", WHERE},
    {"insert", INSERT},
    {"into", INTO},
    {"update", UPDATE},
    {"delete", DELETE},
    {"values", VALUES},
    {"set", SET},
    {"join", JOIN},
    {"and", AND},
    {"or", OR},
};
const int keyword_count = sizeof(keywords) / sizeof(keywords[0]);

/**
 * get_tokens - Tokenize a SQL string into a TokenStack
 * @sql: input SQL text
 * @arena: arena for storing token lexeme strings
 *
 * Note: This is a very small tokenizer tailored to the validator. It is not a
 * full SQL lexer.
 */
TokenStack get_tokens(const char* sql, Arena* arena)
{
    assert(sql != NULL);
    assert(arena != NULL);

    size_t txt_len    = strlen(sql);
    size_t tokenCount = txt_len ? txt_len : 1;

    TokenStack tokenList = {
        .elems = (Token*)static_arena_alloc(arena, tokenCount * sizeof(Token)),
        .len   = 0,
        .cap   = (int)tokenCount,
    };

    int index      = 0;
    int last_index = (int)txt_len;

    char c = ' ';
    while (index < last_index)
    {
        char seperators[255] = " \t\n";
        c                    = sql[index];
        Token token          = {};
        bool is_single       = false;

        switch (c)
        {
            case ' ':
            case '\t':
            case '\n':
                index++;
                continue;

            case ',':
                token.type = COMMA;
                is_single  = true;
                break;

            case ';':
                token.type = SEMICOLON;
                is_single  = true;
                break;

            case '=':
                token.type = EQUALS;
                is_single  = true;
                break;
            case '(':
                token.type = ROUND_BRACKETS_OPEN;
                is_single  = true;
                break;
            case ')':
                token.type = ROUND_BRACKETS_CLOSE;
                is_single  = true;
                break;

            case '*':
                token.type = STAR;
                is_single  = true;
                break;

            case '"':
                token.type = DOUBLE_QUOTED_VALUE;
                strcpy(seperators, "\"");
                index++; // skip current seperator
                break;
            case '\'':
                token.type = SINGLE_QUOTED_VALUE;
                strcpy(seperators, "'");
                index++; // skip current seperator
                break;

            default:
                if (isalpha(c) || c == '_')
                {
                    token.type = SQL_IDENTIFIER;
                    strcat(seperators, ",;()");
                }
                else if (isdigit(c))
                {
                    token.type = NUMBER;
                    strcat(seperators, ",;()");
                }
                else
                {
                    /* Unknown character: skip it and continue */
                    index++;
                    continue;
                }
        }

        int start = index;
        if (is_single)
        {
            index++;
        }
        else
        {

            // NOTE: +1 include the \0 terminator for absolute end else crash at
            // the end because of the justified assert
            for (int char_idx = index; char_idx < (int)txt_len + 1; char_idx++)
            {
                bool is_seperator = false;
                char current_char = toupper(sql[char_idx]);
                // check for seperators
                for (int sep_idx = 0; sep_idx < (int)strlen(seperators);
                     sep_idx++)
                {
                    if (current_char == toupper(seperators[sep_idx]) ||
                        current_char == '\0')
                    {
                        is_seperator = true;
                        break;
                    }
                }

                if (is_seperator)
                {
                    index = char_idx;
                    break;
                }
            }
        }
        int end = index;
        assert(end > start && "end should be bigger than start");

        if (sql[index] == '"' || sql[index] == '\'') // consume specific
                                                     // string seperators
        {
            index++;
        }

        token.value = static_arena_alloc(arena, (size_t)(end - start) + 1);
        strncpy(token.value, sql + start, (size_t)(end - start));
        token.value[end - start] = '\0';

        // keyword check
        for (int i = 0; i < keyword_count; i++)
        {
            Keyword keyword = keywords[i];
            if (match(token.value, keyword.value))
            {
                token.type = keyword.type;
                break;
            }
        }

        append(&tokenList, token);
    }
    return tokenList;
}

/**
 * struct Valid_Symbols - Represents a set of valid expected symbols
 * @valids: array of valid symbols
 * @len: number of valid symbols
 */
typedef struct
{
    SqlSymbols valids[15]; // max symbols
    unsigned char len; // The current length of the Valid_Symbols, indicating
                       // how many valids there actually are
} Valid_Symbols;

/**
 * struct ValidationError - A single validation error detail
 * @token: offending token (NULL when the error relates to EOF)
 * @position: index in the token stream (0-based)
 * @expected: parser state that was expected at this position
 * @message: human-readable diagnostic message
 */
typedef struct
{
    const Token* token;
    int position;
    Valid_Symbols expected;
    const char* message;
} ValidationError;

/**
 * struct ValidationResult - Aggregated validation outcome
 * @ok: true when no errors were found
 * @error_count: total number of errors detected (may exceed capacity)
 * @error_capacity: capacity of the caller-provided @errors buffer
 * @errors: pointer to caller-provided buffer to store up to @error_capacity
 * errors
 */
typedef struct
{
    bool ok;
    size_t error_count;
    size_t error_capacity;
    ValidationError* errors;
} ValidationResult;

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
                         Valid_Symbols expected,
                         const char* msg)
{
    assert(r != NULL);
    r->ok = false;
    if (r->error_count < r->error_capacity)
    {
        ValidationError* e = &r->errors[r->error_count];
        e->token           = tok;
        e->position        = pos;
        e->expected        = expected;
        e->message         = msg;
    }
    r->error_count++;
}

/*
 * expected_table - Transition table for the table-driven validator
 *
 * Indexed by SqlSymbols enum value. Each entry holds the set of valid token
 * types that may follow the given token. The validator walks the token stream
 * and looks up this table after every accepted token to advance the expected
 * set. END is included in terminal positions so that a query may end without
 * a trailing semicolon where appropriate.
 */
const Valid_Symbols expected_table[] = {
    [SELECT] = {{SQL_IDENTIFIER, STAR}, 2},
    [FROM]   = {{SQL_IDENTIFIER}, 1},
    [WHERE]  = {{SQL_IDENTIFIER}, 1},
    [UPDATE] = {{SQL_IDENTIFIER}, 1},
    [DELETE] = {{FROM}, 1},
    [INSERT] = {{INTO}, 1},
    [INTO]   = {{SQL_IDENTIFIER}, 1},
    [VALUES] = {{ROUND_BRACKETS_OPEN}, 1},
    [SET]    = {{SQL_IDENTIFIER}, 1},
    [JOIN]   = {{SQL_IDENTIFIER}, 1},

    [COMMA]     = {{SQL_IDENTIFIER,
                    STAR,
                    NUMBER,
                    SINGLE_QUOTED_VALUE,
                    DOUBLE_QUOTED_VALUE},
                   5},
    [SEMICOLON] = {{END}, 1},
    [EQUALS] =
        {{NUMBER, SINGLE_QUOTED_VALUE, DOUBLE_QUOTED_VALUE, SQL_IDENTIFIER}, 4},
    [STAR] = {{COMMA, FROM, END}, 3},

    [NUMBER] = {{COMMA, SEMICOLON, AND, OR, WHERE, ROUND_BRACKETS_CLOSE, END},
                7},
    [DOUBLE_QUOTED_VALUE] =
        {{COMMA, SEMICOLON, AND, OR, WHERE, ROUND_BRACKETS_CLOSE, END}, 7},
    [SINGLE_QUOTED_VALUE] =
        {{COMMA, SEMICOLON, AND, OR, WHERE, ROUND_BRACKETS_CLOSE, END}, 7},
    [SQL_IDENTIFIER] = {{COMMA,
                         FROM,
                         WHERE,
                         EQUALS,
                         SEMICOLON,
                         AND,
                         OR,
                         JOIN,
                         ROUND_BRACKETS_CLOSE,
                         SET,
                         VALUES,
                         END},
                        12},

    [AND] = {{SQL_IDENTIFIER}, 1},
    [OR]  = {{SQL_IDENTIFIER}, 1},

    [ROUND_BRACKETS_OPEN] =
        {{SQL_IDENTIFIER, NUMBER, SINGLE_QUOTED_VALUE, DOUBLE_QUOTED_VALUE}, 4},
    [ROUND_BRACKETS_CLOSE] = {{COMMA, SEMICOLON, AND, OR, WHERE}, 5},

    [END] = {{END}, 1}};

/**
 * validate_query_with_errors - Validate and collect all errors
 * @tokens: token stack to validate
 * @result: output accumulator (caller provides storage)
 *
 * Returns: true if no errors, false otherwise. Continues after mismatches.
 */
bool validate_query_with_errors(const TokenStack* tokens,
                                ValidationResult* result)
{
    assert(tokens != NULL);
    assert(result != NULL);

    result->ok          = true;
    result->error_count = 0;

    if (tokens->len == 0)
    {
        return true;
    }

    Valid_Symbols expected = {{SELECT, UPDATE, DELETE, INSERT}, 4};

    for (int i = 0; i <= tokens->len; i++)
    {
        bool is_eof       = (i == tokens->len);
        const Token* t    = is_eof ? NULL : &tokens->elems[i];
        SqlSymbols t_type = is_eof ? END : t->type;

        bool is_valid = false;
        for (int j = 0; j < expected.len; j++)
        {
            if (t_type == expected.valids[j])
            {
                is_valid = true;
                break;
            }
        }

        if (!is_valid)
        {
            record_error(result, t, i, expected, "unexpected token");
            continue; // Continue parsing to accumulate errors
        }

        if (!is_eof)
        {
            expected = expected_table[t_type];
        }
    }

    return result->ok;
}

/**
 * validate_query - Convenience wrapper that only reports success/failure
 * @tokens: token stack to validate
 *
 * Returns: true if query is valid, false otherwise.
 */
bool validate_query(const TokenStack* tokens)
{
    if (!tokens)
        return false;

    ValidationError buf[tokens->len + 2];
    ValidationResult res = {
        .ok             = true,
        .error_count    = 0,
        .error_capacity = sizeof(buf) / sizeof(buf[0]),
        .errors         = buf,
    };

    return validate_query_with_errors(tokens, &res);
}

#define CLR_RED "\033[31m"
#define CLR_YEL "\033[33m"
#define CLR_GRN "\033[32m"
#define CLR_DIM "\033[90m"
#define CLR_RESET "\033[0m"

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

    const char* kind = symbol_to_str[t->type];
    const char* val  = (t->value && t->value[0]) ? t->value : "";

    if (val[0])
        snprintf(buf, n, "%s \"%s\"", kind, val);
    else
        snprintf(buf, n, "%s", kind);
}

/**
 * token_name - Write only the type name of a token's offending error into buf
 * @e: validation error holding the token (may be NULL)
 * @buf: output buffer
 * @n: buffer size
 */
static void token_name(const ValidationError* e, char* buf, size_t n)
{
    if (!buf || n == 0)
        return;

    const Token* t = e ? e->token : NULL;
    if (!t)
    {
        snprintf(buf, n, "<EOF>");
        return;
    }

    snprintf(buf, n, "%s", symbol_to_str[t->type]);
}

/**
 * expected_to_str - Convert expected state to string label
 */
static void expected_to_str(Valid_Symbols e, char* buf, size_t n)
{
    if (!buf || n == 0)
        return;

    buf[0] = '\0';
    for (int i = 0; i < e.len; i++)
    {
        strncat(buf, symbol_to_str[e.valids[i]], n - strlen(buf) - 1);
        if (i < e.len - 1)
        {
            strncat(buf, " | ", n - strlen(buf) - 1);
        }
    }
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
        printf(CLR_GRN "validation: ok" CLR_RESET "\n");
        return;
    }

    const ValidationError* e0 = &result->errors[0];
    char tokbuf[128];
    char namebuf[64];
    char expectedbuf[256];
    describe_token(e0, tokbuf, sizeof(tokbuf));
    token_name(e0, namebuf, sizeof(namebuf));
    expected_to_str(e0->expected, expectedbuf, sizeof(expectedbuf));

    printf(CLR_RED "validation failed" CLR_RESET " at token %s: expected %s, "
                   "got %s%s%s",
           namebuf,
           expectedbuf,
           CLR_YEL,
           tokbuf,
           CLR_RESET);
    if (e0->message && e0->message[0])
        printf(" (%s)", e0->message);

    if (result->error_count > 1)
        printf(" [and %zu more]", result->error_count - 1);

    printf("\n");
}

// NOTE: for developing tests and triggering treesitter to highlight
//<-- test dev-->
// #define TEST_MODE false
// #if TEST_MODE
//<-- test dev-->

#ifndef TEST_MODE
/**
 * main - Program entry point: tokenizes and validates a SQL string
 * @argc: number of command-line arguments
 * @argv: argument vector; argv[1] is the SQL string to validate
 *
 * When invoked with exactly one argument the SQL is tokenized and validated.
 * A human-readable result is printed to stdout. The exit code is always 0
 * so that the program can be used as a diagnostic tool in scripts.
 *
 * Return: 0 always (use the printed diagnostic for pass/fail information)
 */
int main(int argc, char* argv[])
{
    const char* sql;

    if (argc == 2)
    {
        sql = argv[1];
    }
    else
    {
        /* Fallback demo query when no argument is provided */
        sql = "SELECT a, b FROM c WHERE id = 1;";
    }

    size_t txt_len = strlen(sql);

    Arena arena = init_static_arena(2 * txt_len + 16 + txt_len * sizeof(Token));
    TokenStack tokenList = get_tokens(sql, &arena);

    /* Validate produced tokens */
    ValidationError errs[tokenList.len + 2];
    ValidationResult res = {
        .ok             = true,
        .error_count    = 0,
        .error_capacity = sizeof(errs) / sizeof(errs[0]),
        .errors         = errs,
    };

    validate_query_with_errors(&tokenList, &res);
    print_validation_result(&res);

    arena_free(&arena);

    return res.ok ? 0 : 1;
}
#else

/**
 * make_stack - Wrap a Token array into a read-only TokenStack view
 * @toks: array of tokens
 * @len: number of elements in @toks
 *
 * Return: TokenStack pointing into the caller's array (no allocation).
 */
static TokenStack make_stack(Token* toks, int len)
{
    TokenStack s = {.elems = toks, .len = len, .cap = len};
    return s;
}

/**
 * make_token - Construct a Token with given value and type
 * @val: lexeme string (must outlive the token)
 * @type: SqlSymbols type tag
 *
 * Return: initialized Token.
 */
static Token make_token(const char* val, SqlSymbols type)
{
    Token t = {.value = (char*)val, .type = type};
    return t;
}

/**
 * test_append_increments_len_until_capacity - append fills the stack in order
 */
static void test_append_increments_len_until_capacity(void)
{
    Token buf[3];
    TokenStack s = {.elems = buf, .len = 0, .cap = 3};

    append(&s, make_token("a", SQL_IDENTIFIER));
    append(&s, make_token("b", SQL_IDENTIFIER));
    append(&s, make_token("c", SQL_IDENTIFIER));

    assert(s.len == 3);
    assert(strcmp(s.elems[0].value, "a") == 0);
    assert(strcmp(s.elems[1].value, "b") == 0);
    assert(strcmp(s.elems[2].value, "c") == 0);
}

/**
 * test_append_does_not_overflow_capacity - append beyond capacity is silently
 * ignored
 */
static void test_append_does_not_overflow_capacity(void)
{
    Token buf[2];
    TokenStack s = {.elems = buf, .len = 0, .cap = 2};

    append(&s, make_token("x", SQL_IDENTIFIER));
    append(&s, make_token("y", SQL_IDENTIFIER));
    /* This should be ignored because len == cap */
    append(&s, make_token("z", SQL_IDENTIFIER));

    assert(s.len == 2);
    assert(strcmp(s.elems[0].value, "x") == 0);
    assert(strcmp(s.elems[1].value, "y") == 0);
}

/**
 * test_append_null_stack_is_safe - append to NULL stack must not crash
 */
static void test_append_null_stack_is_safe(void)
{
    append(NULL, make_token("x", SQL_IDENTIFIER));
}

/**
 * test_report_formats_errors - smoke-test that the printer handles a real error
 * result without crashing and renders at least one error line.
 */
static void test_report_formats_errors(void)
{
    Token toks[] = {
        {.value = "FROM", .type = FROM}, /* invalid start */
        {.value = "SELECT", .type = SELECT},
        {.value = "FROM", .type = FROM},
        {.value = "t", .type = SQL_IDENTIFIER},
        {.value = ";", .type = SEMICOLON},
    };
    TokenStack s = make_stack(toks, (int)(sizeof(toks) / sizeof(toks[0])));

    ValidationError errs[8];
    ValidationResult res = {.errors = errs, .error_capacity = 8};
    bool ok              = validate_query_with_errors(&s, &res);

    assert(!ok);
    assert(res.error_count >= 1);

    /* Smoke-test the printer (no assertions on stdout content) */
    print_validation_result(&res);
}

/**
 * test_report_formats_new_symbols - smoke-test that symbol_to_str covers
 * every token type reachable from the current enum (UPDATE, JOIN, etc.).
 */
static void test_report_formats_new_symbols(void)
{
    /* Manually craft a result using newly added symbols to exercise
     * symbol_to_str */
    ValidationError errs[2];
    errs[0].token    = &(Token){.value = "UPDATE", .type = UPDATE};
    errs[0].position = 0;
    errs[0].expected = (Valid_Symbols){{SELECT}, 1}; /* arbitrary state */
    errs[0].message  = "unexpected token";

    errs[1].token    = &(Token){.value = "JOIN", .type = JOIN};
    errs[1].position = 1;
    errs[1].expected = (Valid_Symbols){{SQL_IDENTIFIER}, 1};
    errs[1].message  = "unexpected token";

    ValidationResult res = {
        .ok             = false,
        .error_count    = 2,
        .error_capacity = 2,
        .errors         = errs,
    };

    /* Smoke-test that printer handles new enum entries without falling back to
     * '?' */
    print_validation_result(&res);
}

/**
 * test_valid_simple_select_star - Validate SELECT *,identifier FROM
 */
static void test_valid_simple_select_star(void)
{
    Token toks[] = {
        {.value = "SELECT", .type = SELECT},
        {.value = "*", .type = STAR},
        {.value = ",", .type = COMMA},
        {.value = "a", .type = SQL_IDENTIFIER},
        {.value = "FROM", .type = FROM},
        {.value = "t", .type = SQL_IDENTIFIER},
        {.value = ";", .type = SEMICOLON},
    };
    TokenStack s = make_stack(toks, (int)(sizeof(toks) / sizeof(toks[0])));
    assert(validate_query(&s));
}

/**
 * test_valid_where_and_or - Validate WHERE with AND/OR chain
 */
static void test_valid_where_and_or(void)
{
    Token toks[] = {
        {.value = "SELECT", .type = SELECT},
        {.value = "name", .type = SQL_IDENTIFIER},
        {.value = "FROM", .type = FROM},
        {.value = "users", .type = SQL_IDENTIFIER},
        {.value = "WHERE", .type = WHERE},
        {.value = "name", .type = SQL_IDENTIFIER},
        {.value = "=", .type = EQUALS},
        {.value = "'nick'", .type = SINGLE_QUOTED_VALUE},
        {.value = "AND", .type = AND},
        {.value = "age", .type = SQL_IDENTIFIER},
        {.value = "=", .type = EQUALS},
        {.value = "42", .type = NUMBER},
        {.value = ";", .type = SEMICOLON},
    };
    TokenStack s = make_stack(toks, (int)(sizeof(toks) / sizeof(toks[0])));
    assert(validate_query(&s));
}

/**
 * test_invalid_missing_from - Ensure missing FROM is rejected
 */
static void test_invalid_missing_from(void)
{
    Token toks[] = {
        {.value = "SELECT", .type = SELECT},
        {.value = "a", .type = SQL_IDENTIFIER},
        {.value = "t", .type = SQL_IDENTIFIER},
        {.value = ";", .type = SEMICOLON},
    };
    TokenStack s = make_stack(toks, (int)(sizeof(toks) / sizeof(toks[0])));
    ValidationError errs[8];
    ValidationResult res = {.errors = errs, .error_capacity = 8};
    assert(!validate_query_with_errors(&s, &res));
    assert(res.error_count >= 1);
}

/**
 * test_invalid_trailing_after_semicolon - Ensure trailing tokens after ';' fail
 */
static void test_invalid_trailing_after_semicolon(void)
{
    Token toks[] = {
        {.value = "SELECT", .type = SELECT},
        {.value = "a", .type = SQL_IDENTIFIER},
        {.value = "FROM", .type = FROM},
        {.value = "t", .type = SQL_IDENTIFIER},
        {.value = ";", .type = SEMICOLON},
        {.value = "DROP", .type = SQL_IDENTIFIER},
    };
    TokenStack s = make_stack(toks, (int)(sizeof(toks) / sizeof(toks[0])));
    ValidationError errs[8];
    ValidationResult res = {.errors = errs, .error_capacity = 8};
    assert(!validate_query_with_errors(&s, &res));
    assert(res.error_count >= 1);
}

/**
 * test_accumulates_multiple_errors - Ensure multiple mismatches are collected
 */
static void test_accumulates_multiple_errors(void)
{
    Token toks[] = {
        {.value = "FROM", .type = FROM}, /* wrong start */
        {.value = "SELECT", .type = SELECT},
        {.value = "FROM", .type = FROM}, /* missing select item before FROM */
        {.value = "t", .type = SQL_IDENTIFIER},
        {.value = "WHERE", .type = WHERE},
        {.value = "=", .type = EQUALS}, /* missing lhs */
        {.value = "1", .type = NUMBER},
        {.value = ";", .type = SEMICOLON},
    };
    TokenStack s = make_stack(toks, (int)(sizeof(toks) / sizeof(toks[0])));
    ValidationError errs[16];
    ValidationResult res = {.errors = errs, .error_capacity = 16};
    bool ok              = validate_query_with_errors(&s, &res);
    assert(!ok);
    assert(res.error_count >= 2);
}

/**
 * test_invalid_token_aborts_early - Invalid lexeme should halt validation
 */
static void test_invalid_token_aborts_early(void)
{
    Token toks[] = {
        {.value = "SELECT", .type = SELECT},
        {.value = "a", .type = SQL_IDENTIFIER},
        {.value = "FROM", .type = FROM},
        {.value = "t", .type = SQL_IDENTIFIER},
        {.value = "(", .type = ROUND_BRACKETS_OPEN}, /* unsupported token */
        {.value = ";", .type = SEMICOLON},
    };
    TokenStack s = make_stack(toks, (int)(sizeof(toks) / sizeof(toks[0])));
    ValidationError errs[8];
    ValidationResult res = {.errors = errs, .error_capacity = 8};
    bool ok              = validate_query_with_errors(&s, &res);

    assert(!ok);
    assert(res.error_count >= 1);
}

/**
 * test_invalid_new_keyword_token - Newly added keyword should also be rejected
 * by validator
 */
static void test_invalid_new_keyword_token(void)
{
    Token toks[] = {
        {.value = "UPDATE", .type = UPDATE}, /* not supported by validator */
        {.value = "t", .type = SQL_IDENTIFIER},
        {.value = "SET", .type = SET},
        {.value = "a", .type = SQL_IDENTIFIER},
        {.value = "=", .type = EQUALS},
        {.value = "1", .type = NUMBER},
        {.value = ";", .type = SEMICOLON},
    };
    TokenStack s = make_stack(toks, (int)(sizeof(toks) / sizeof(toks[0])));
    ValidationError errs[8];
    ValidationResult res = {.errors = errs, .error_capacity = 8};
    bool ok              = validate_query_with_errors(&s, &res);

    assert(ok); // Now UPDATE is supported, so it should be valid
}

/**
 * test_tokenizes_basic_select - Ensure tokenizer produces expected tokens
 */
static void test_tokenizes_basic_select(void)
{
    const char* sql = "SELECT name,age FROM users;";
    size_t sql_len  = strlen(sql);
    Arena arena     = init_static_arena(sql_len * 2 + sql_len * sizeof(Token));
    TokenStack toks = get_tokens(sql, &arena);

    assert(toks.len == 7);
    assert(toks.elems[0].type == SELECT);
    assert(strcmp(toks.elems[1].value, "name") == 0);
    assert(toks.elems[2].type == COMMA);
    assert(strcmp(toks.elems[3].value, "age") == 0);
    assert(toks.elems[4].type == FROM);
    assert(strcmp(toks.elems[5].value, "users") == 0);
    assert(toks.elems[6].type == SEMICOLON);

    arena_free(&arena);
}

/**
 * test_tokenizer_integrates_with_validator - Tokenizer output is accepted by
 * validator
 */
static void test_tokenizer_integrates_with_validator(void)
{
    const char* sql = "SELECT a FROM t;";
    size_t sql_len  = strlen(sql);
    Arena arena     = init_static_arena(sql_len * 2 + sql_len * sizeof(Token));
    TokenStack toks = get_tokens(sql, &arena);

    ValidationError errs[8];
    ValidationResult res = {.errors = errs, .error_capacity = 8};
    bool ok              = validate_query_with_errors(&toks, &res);

    assert(ok);

    arena_free(&arena);
}

/**
 * main - Run all unit tests for SqlValidateReport
 */
int main(void)
{
    { // tokenizer
        test_tokenizes_basic_select();
        test_tokenizer_integrates_with_validator();
    }

    { // token stack
        test_append_increments_len_until_capacity();
        test_append_does_not_overflow_capacity();
        test_append_null_stack_is_safe();
    }

    { // sql validate
        test_valid_simple_select_star();
        test_valid_where_and_or();
        test_invalid_missing_from();
        test_invalid_trailing_after_semicolon();
        test_accumulates_multiple_errors();
        test_invalid_token_aborts_early();
        test_invalid_new_keyword_token();
    }

    { // sql validate report
        test_report_formats_errors();
        test_report_formats_new_symbols();
    }
    return 0;
}
#endif
