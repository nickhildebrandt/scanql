#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// TODO: remove a lot of Symbols(select_item) and so on just the rudimentary
// ones

//
/*
 * SqlToken - Enumeration of SQL token types
 */

typedef enum : unsigned short
{
    // Core DML/DDL keywords
    SELECT,
    FROM,
    WHERE,
    CREATE,
    TABLE,
    DROP,
    IF,
    EXISTS,
    INSERT,
    INTO,
    NOT,
    IN,
    null,
    UPDATE,
    DELETE,
    VALUES,
    SET,
    RETURNING,

    // Query modifiers & set operations
    DISTINCT,
    ALL_SYM,
    UNION,
    INTERSECT,
    EXCEPT,

    // Joins
    JOIN,
    INNER,
    LEFT,
    RIGHT,
    FULL,
    OUTER,
    CROSS,
    ON,
    AS,

    // Grouping / ordering / pagination
    GROUP,
    BY,
    HAVING,
    ORDER,
    ASC,
    DESC,
    LIMIT,
    OFFSET,
    FETCH,
    ROWS,
    ONLY,
    TOP,

    // Conditions & predicates
    BETWEEN,
    LIKE,
    ILIKE,
    IS,
    ANY_SYM,
    SOME,
    ALL_PRED,
    EXISTS_PRED,
    TRUE_SYM,
    FALSE_SYM,

    // CASE expression
    CASE,
    WHEN,
    THEN,
    ELSE_SYM,
    END_KW,

    // Types
    TYPE_TEXT,
    TYPE_ENUM,
    TYPE_INT,
    TYPE_INTEGER,
    TYPE_BIGINT,
    TYPE_SMALLINT,
    TYPE_DECIMAL,
    TYPE_NUMERIC,
    TYPE_REAL,
    TYPE_DOUBLE,
    TYPE_FLOAT,
    TYPE_BOOLEAN,
    TYPE_CHAR,
    TYPE_VARCHAR,
    TYPE_DATE,
    TYPE_TIME,
    TYPE_TIMESTAMP,
    TYPE_BLOB,

    // Operators & symbols
    COMMA,
    DOT,
    SEMICOLON,
    PLUS,
    MINUS_OP,
    STAR_OP,
    SLASH,
    PERCENT,
    CARET,
    CONCAT_OP,
    EQUALS,
    NOT_EQUAL,
    LESS,
    GREATER,
    LESS_EQUAL,
    GREATER_EQUAL,
    NEGATION,

    // Literals / identifiers / placeholders
    NUMBER,
    DOUBLE_QUOTED_VALUE,
    SINGLE_QUOTED_VALUE,
    SQL_IDENTIFIER,
    STRING,
    STRING_VALUE,
    SQL_NULL,
    PARAMETER,
    IDENTIFIER,

    // Logical operators
    AND,
    OR,

    // Delimiters & comments
    ROUND_BRACKETS_OPEN,
    ROUND_BRACKETS_CLOSE,
    SQUARE_BRACKETS_OPEN,
    SQUARE_BRACKETS_CLOSE,
    INLINE_COMMENT_MINUS,
    MULTILINE_COMMENT_OPEN,
    MULTILINE_COMMENT_CLOSE,

    // Parser expectation states (shared enum, reusing existing names when
    // possible)
    SELECT_ITEM,    // column | *
    SELECT_CONT,    // , | FROM
    WHERE_OR_END,   // WHERE | ;
    CONDITION_LHS,  // identifier
    CONDITION_OP,   // =
    CONDITION_RHS,  // literal
    CONDITION_CONT, // AND | OR | ;
    END
} SqlSymbols;

/*
 * symbol_to_str - usage by indexing with enum value from SqlSymbols
 */
const char* symbol_to_str[] = {

    // Core DML/DDL keywords
    "SELECT",
    "FROM",
    "WHERE",
    "CREATE",
    "TABLE",
    "DROP",
    "IF",
    "EXISTS",
    "INSERT",
    "INTO",
    "NOT",
    "IN",
    "null",
    "UPDATE",
    "DELETE",
    "VALUES",
    "SET",
    "RETURNING",

    // Query modifiers & set operations
    "DISTINCT",
    "ALL_SYM",
    "UNION",
    "INTERSECT",
    "EXCEPT",

    // Joins
    "JOIN",
    "INNER",
    "LEFT",
    "RIGHT",
    "FULL",
    "OUTER",
    "CROSS",
    "ON",
    "AS",

    // Grouping / ordering / pagination
    "GROUP",
    "BY",
    "HAVING",
    "ORDER",
    "ASC",
    "DESC",
    "LIMIT",
    "OFFSET",
    "FETCH",
    "ROWS",
    "ONLY",
    "TOP",

    // Conditions & predicates
    "BETWEEN",
    "LIKE",
    "ILIKE",
    "IS",
    "ANY_SYM",
    "SOME",
    "ALL_PRED",
    "EXISTS_PRED",
    "TRUE_SYM",
    "FALSE_SYM",

    // CASE expression
    "CASE",
    "WHEN",
    "THEN",
    "ELSE_SYM",
    "END_KW",

    // Types
    "TYPE_TEXT",
    "TYPE_ENUM",
    "TYPE_INT",
    "TYPE_INTEGER",
    "TYPE_BIGINT",
    "TYPE_SMALLINT",
    "TYPE_DECIMAL",
    "TYPE_NUMERIC",
    "TYPE_REAL",
    "TYPE_DOUBLE",
    "TYPE_FLOAT",
    "TYPE_BOOLEAN",
    "TYPE_CHAR",
    "TYPE_VARCHAR",
    "TYPE_DATE",
    "TYPE_TIME",
    "TYPE_TIMESTAMP",
    "TYPE_BLOB",

    // Operators & symbols
    "COMMA",
    "DOT",
    "SEMICOLON",
    "PLUS",
    "MINUS_OP",
    "STAR_OP",
    "SLASH",
    "PERCENT",
    "CARET",
    "CONCAT_OP",
    "EQUALS",
    "NOT_EQUAL",
    "LESS",
    "GREATER",
    "LESS_EQUAL",
    "GREATER_EQUAL",
    "NEGATION",

    // Literals / identifiers / placeholders
    "NUMBER",
    "DOUBLE_QUOTED_VALUE",
    "SINGLE_QUOTED_VALUE",
    "SQL_IDENTIFIER",
    "STRING",
    "STRING_VALUE",
    "SQL_NULL",
    "PARAMETER",
    "IDENTIFIER",

    // Logical operators
    "AND",
    "OR",

    // Delimiters & comments
    "ROUND_BRACKETS_OPEN",
    "ROUND_BRACKETS_CLOSE",
    "SQUARE_BRACKETS_OPEN",
    "SQUARE_BRACKETS_CLOSE",
    "INLINE_COMMENT_MINUS",
    "MULTILINE_COMMENT_OPEN",
    "MULTILINE_COMMENT_CLOSE",

    // Parser expectation states (shared enum", reusing existing names when
    // possible)
    "SELECT_ITEM",    // column | *
    "SELECT_CONT",    // ", | FROM
    "WHERE_OR_END",   // WHERE | ;
    "CONDITION_LHS",  // identifier
    "CONDITION_OP",   // =
    "CONDITION_RHS",  // literal
    "CONDITION_CONT", // AND | OR | ;
    "END",
};

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
    {"table", TABLE},
    {"drop", DROP},
    {"if", IF},
    {"exists", EXISTS},
    {"insert", INSERT},
    {"into", INTO},
    {"not", NOT},
    {"in", IN},
    {"update", UPDATE},
    {"delete", DELETE},
    {"values", VALUES},
    {"set", SET},
    {"returning", RETURNING},
    {"join", JOIN},
    {"create", CREATE},
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
    SqlSymbols expected;
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

// TODO: remove the parser_t and all the token_is_x functions

typedef struct
{
    SqlSymbols expected;
} parser_t;
/* Helpers to interpret existing Token/SqlSymbols into semantic roles */
/**
 * token_is_select - Check if token is SELECT keyword
 */
static bool token_is_select(const Token* t)
{
    return t && t->type == SELECT;
}
/** token_is_from - Check if token is FROM keyword */
static bool token_is_from(const Token* t)
{
    return t && t->type == FROM;
}
/** token_is_where - Check if token is WHERE keyword */
static bool token_is_where(const Token* t)
{
    return t && t->type == WHERE;
}
/** token_is_comma - Check if token is comma */
static bool token_is_comma(const Token* t)
{
    return t && t->type == COMMA;
}
/** token_is_equals - Check if token is equals sign */
static bool token_is_equals(const Token* t)
{
    return t && t->type == EQUALS;
}
/** token_is_and - Check if token is AND */
static bool token_is_and(const Token* t)
{
    return t && t->type == AND;
}
/** token_is_or - Check if token is OR */
static bool token_is_or(const Token* t)
{
    return t && t->type == OR;
}
/** token_is_semicolon - Check if token is semicolon */
static bool token_is_semicolon(const Token* t)
{
    return t && t->type == SEMICOLON;
}

/**
 * token_is_star - Check if token lexeme is '*'
 */
static bool token_is_star(const Token* t)
{
    return t && t->type == STRING && t->value && t->value[0] == '*' &&
           t->value[1] == '\0';
}

/**
 * token_is_literal - Check if token represents a literal value
 */
static bool token_is_literal(const Token* t)
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
static bool token_is_identifier(const Token* t)
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
static bool token_is_valid_lexeme(const Token* t)
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
 * Returns: true when transition is valid and state is advanced; false
 * otherwise.
 */
static bool accept(parser_t* p, const Token* tok, bool is_eof)
{
    /* If this is EOF, only accept when we're in the terminal state */
    if (is_eof)
        return p->expected == END;

    switch (p->expected)
    {
        case SELECT:
            if (token_is_select(tok))
            {
                p->expected = SELECT_ITEM;
                return true;
            }
            return false;

        case SELECT_ITEM:
            if (token_is_identifier(tok) || token_is_star(tok))
            {
                p->expected = SELECT_CONT;
                return true;
            }
            return false;

        case SELECT_CONT:
            if (token_is_comma(tok))
            {
                p->expected = SELECT_ITEM;
                return true;
            }
            if (token_is_from(tok))
            {
                p->expected = TABLE;
                return true;
            }
            return false;

        case TABLE:
            if (token_is_identifier(tok))
            {
                p->expected = WHERE_OR_END;
                return true;
            }
            return false;

        case WHERE_OR_END:
            if (token_is_where(tok))
            {
                p->expected = CONDITION_LHS;
                return true;
            }
            if (token_is_semicolon(tok))
            {
                p->expected = END;
                return true;
            }
            return false;

        case CONDITION_LHS:
            if (token_is_identifier(tok))
            {
                p->expected = CONDITION_OP;
                return true;
            }
            return false;

        case CONDITION_OP:
            if (token_is_equals(tok))
            {
                p->expected = CONDITION_RHS;
                return true;
            }
            return false;

        case CONDITION_RHS:
            if (token_is_literal(tok))
            {
                p->expected = CONDITION_CONT;
                return true;
            }
            return false;

        case CONDITION_CONT:
            if (token_is_and(tok) || token_is_or(tok))
            {
                p->expected = CONDITION_LHS;
                return true;
            }
            if (token_is_semicolon(tok))
            {
                p->expected = END;
                return true;
            }
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
                         SqlSymbols expected,
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

// TODO: use this struct for the expected_symbols
typedef struct
{
    SqlSymbols valids[10]; // max symbols
    unsigned char len; // The current length of the Valid_Symbols, indicating
                       // how many valids there actually are
} Valid_Symbols;

/*
 * expected_table - usage by indexing with enum value from SqlSymbols
 * NOTE: only for the possible direct next token
 * 72 is the current number of enum values
 */
const SqlSymbols expected_table[][10] = {
    {SQL_IDENTIFIER,
     ROUND_BRACKETS_OPEN,
     SINGLE_QUOTED_VALUE,
     DOUBLE_QUOTED_VALUE},                           // SELECT,
    {SQL_IDENTIFIER},                                // FROM,
    {SQL_IDENTIFIER, ROUND_BRACKETS_OPEN},           // WHERE,
    {},                                              // CREATE,
    {},                                              // TABLE,
    {},                                              // DROP,
    {},                                              // IF,
    {},                                              // EXISTS,
    {},                                              // INSERT,
    {},                                              // INTO,
    {},                                              // NOT,
    {},                                              // IN,
    {},                                              // null,
    {},                                              // UPDATE,
    {},                                              // DELETE,
    {},                                              // VALUES,
    {},                                              // SET,
    {},                                              // RETURNING,
    {},                                              // DISTINCT,
    {},                                              // ALL_SYM,
    {},                                              // UNION,
    {},                                              // INTERSECT,
    {},                                              // EXCEPT,
    {},                                              // JOIN,
    {},                                              // INNER,
    {},                                              // LEFT,
    {},                                              // RIGHT,
    {},                                              // FULL,
    {},                                              // OUTER,
    {},                                              // CROSS,
    {},                                              // ON,
    {},                                              // AS,
    {},                                              // GROUP,
    {},                                              // BY,
    {},                                              // HAVING,
    {},                                              // ORDER,
    {},                                              // ASC,
    {},                                              // DESC,
    {},                                              // LIMIT,
    {},                                              // OFFSET,
    {},                                              // FETCH,
    {},                                              // ROWS,
    {},                                              // ONLY,
    {},                                              // TOP,
    {},                                              // BETWEEN,
    {},                                              // LIKE,
    {},                                              // ILIKE,
    {},                                              // IS,
    {},                                              // ANY_SYM,
    {},                                              // SOME,
    {},                                              // ALL_PRED,
    {},                                              // EXISTS_PRED,
    {},                                              // TRUE_SYM,
    {},                                              // FALSE_SYM,
    {},                                              // CASE,
    {},                                              // WHEN,
    {},                                              // THEN,
    {},                                              // ELSE_SYM,
    {},                                              // END_KW,
    {},                                              // TYPE_TEXT,
    {},                                              // TYPE_ENUM,
    {},                                              // TYPE_INT,
    {},                                              // TYPE_INTEGER,
    {},                                              // TYPE_BIGINT,
    {},                                              // TYPE_SMALLINT,
    {},                                              // TYPE_DECIMAL,
    {},                                              // TYPE_NUMERIC,
    {},                                              // TYPE_REAL,
    {},                                              // TYPE_DOUBLE,
    {},                                              // TYPE_FLOAT,
    {},                                              // TYPE_BOOLEAN,
    {},                                              // TYPE_CHAR,
    {},                                              // TYPE_VARCHAR,
    {},                                              // TYPE_DATE,
    {},                                              // TYPE_TIME,
    {},                                              // TYPE_TIMESTAMP,
    {},                                              // TYPE_BLOB,
    {},                                              // COMMA,
    {},                                              // DOT,
    {},                                              // SEMICOLON,
    {},                                              // PLUS,
    {},                                              // MINUS_OP,
    {},                                              // STAR_OP,
    {},                                              // SLASH,
    {},                                              // PERCENT,
    {},                                              // CARET,
    {},                                              // CONCAT_OP,
    {},                                              // EQUALS,
    {},                                              // NOT_EQUAL,
    {},                                              // LESS,
    {},                                              // GREATER,
    {},                                              // LESS_EQUAL,
    {},                                              // GREATER_EQUAL,
    {},                                              // NEGATION,
    {},                                              // NUMBER,
    {},                                              // DOUBLE_QUOTED_VALUE,
    {},                                              // SINGLE_QUOTED_VALUE,
    {COMMA, ROUND_BRACKETS_CLOSE, EQUALS, NEGATION}, // SQL_IDENTIFIER,
    {},                                              // STRING,
    {},                                              // STRING_VALUE,
    {},                                              // SQL_NULL,
    {},                                              // PARAMETER,
    {},                                              // IDENTIFIER,
    {},                                              // AND,
    {},                                              // OR,
    {},                                              // ROUND_BRACKETS_OPEN,
    {},                                              // ROUND_BRACKETS_CLOSE,
    {},                                              // SQUARE_BRACKETS_OPEN,
    {},                                              // SQUARE_BRACKETS_CLOSE,
    {},                                              // INLINE_COMMENT_MINUS,
    {},                                              // MULTILINE_COMMENT_OPEN,
    {},                                              // MULTILINE_COMMENT_CLOSE,
    {}, // SELECT_ITEM,    // column | *
    {}, // SELECT_CONT,    // , | FROM
    {}, // WHERE_OR_END,   // WHERE | ;
    {}, // CONDITION_LHS,  // identifier
    {}, // CONDITION_OP,   // =
    {}, // CONDITION_RHS,  // literal
    {}, // CONDITION_CONT, // AND | OR | ;
    {}, // END
};

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

    // NOTE: maybe fuzzy match for here
    for (int i = 0; i < tokens->len; i++)
    {
    }

    // parser_t p                = {.expected = SELECT};
    // bool aborted              = false;
    // int abort_pos             = -1;
    // const Token* abort_token  = NULL;
    // SqlSymbols abort_expected = END;
    //
    // /* iterate through tokens, treat array end as EOF */
    // for (int i = 0; i <= tokens->len; ++i)
    // {
    //     bool is_eof    = (i == tokens->len);
    //     const Token* t = is_eof ? NULL : &tokens->elems[i];
    //
    //     if (!is_eof && !token_is_valid_lexeme(t))
    //     {
    //         record_error(result, t, i, p.expected, "invalid token");
    //         aborted        = true;
    //         abort_pos      = i;
    //         abort_token    = t;
    //         abort_expected = p.expected;
    //         break;
    //     }
    //
    //     if (!accept(&p, t, is_eof))
    //     {
    //         record_error(result, t, i, p.expected, "unexpected token");
    //         continue;
    //     }
    // }
    //
    // if (aborted)
    // {
    //     record_error(result,
    //                  abort_token,
    //                  abort_pos >= 0 ? abort_pos : tokens->len,
    //                  abort_expected,
    //                  "fatal: stopped parsing after invalid token");
    //     return result->ok;
    // }
    //
    // if (p.expected != END)
    // {
    //     record_error(result, NULL, tokens->len, p.expected, "incomplete
    //     query");
    // }

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
 * expected_to_str - Convert expected state to string label
 */
static const char* expected_to_str(SqlSymbols e)
{
    switch (e)
    {
        case SELECT:
            return "SELECT";
        case SELECT_ITEM:
            return "SELECT_ITEM";
        case SELECT_CONT:
            return "SELECT_CONT";
        case TABLE:
            return "TABLE";
        case WHERE_OR_END:
            return "WHERE_OR_END";
        case CONDITION_LHS:
            return "COND_LHS";
        case CONDITION_OP:
            return "COND_OP";
        case CONDITION_RHS:
            return "COND_RHS";
        case CONDITION_CONT:
            return "COND_CONT";
        case END:
            return "END";
        default:
            return "?";
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

    const char* kind = symbol_to_str[t->type];
    const char* val  = (t->value && t->value[0]) ? t->value : "";

    if (val[0])
        snprintf(buf, n, "%s \"%s\"", kind, val);
    else
        snprintf(buf, n, "%s", kind);
}

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
    describe_token(e0, tokbuf, sizeof(tokbuf));
    token_name(e0, namebuf, sizeof(namebuf));

    printf(CLR_RED "validation failed" CLR_RESET " at token %s: expected %s, "
                   "got %s%s%s",
           namebuf,
           expected_to_str(e0->expected),
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
 * main - Program entry point that tokenizes and validates SQL from argv
 * @argc: Number of command line arguments
 * @argv: Array of command line argument strings
 *
 * Return: 0 on success, non-zero on usage error or validation failure
 */
int main(int argc, char* argv[])
{
    // if (argc != 2)
    // {
    //     fprintf(stderr, "Usage: %s <SQL-String>\n", argv[0]);
    //     return 1;
    // }
    //
    // const char* sql = argv[1];
    const char* sql = "select a,b from c where a.id = 1";
    size_t txt_len  = strlen(sql);

    Arena arena = init_static_arena(2 * txt_len + 16 + txt_len * sizeof(Token));
    TokenStack tokenList = get_tokens(sql, &arena);

    // TODO: loop over the expected_table for validation use the enum indexing
    // to get the current Valid_Symbols and then use the len to loop over it for
    // (int i = 0; i < tokenList.len; i++)
    // {
    //     print_token(tokenList.elems[i]);
    // }

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

    /* For CLI tests we only print diagnostics; always succeed exit code */
    return 0;
}
#else

/**
 * make_stack - helper to wrap a Token array into a TokenStack view
 */
static TokenStack make_stack(Token* toks, int len)
{
    TokenStack s = {.elems = toks, .len = len, .cap = len};
    return s;
}

static Token make_token(const char* val, SqlSymbols type)
{
    Token t = {.value = (char*)val, .type = type};
    return t;
}

static void test_append_increments_len_until_capacity(void)
{
    Token buf[3];
    TokenStack s = {.elems = buf, .len = 0, .cap = 3};

    append(&s, make_token("a", STRING));
    append(&s, make_token("b", STRING));
    append(&s, make_token("c", STRING));

    assert(s.len == 3);
    assert(strcmp(s.elems[0].value, "a") == 0);
    assert(strcmp(s.elems[1].value, "b") == 0);
    assert(strcmp(s.elems[2].value, "c") == 0);
}

static void test_append_does_not_overflow_capacity(void)
{
    Token buf[2];
    TokenStack s = {.elems = buf, .len = 0, .cap = 2};

    append(&s, make_token("x", STRING));
    append(&s, make_token("y", STRING));
    /* This should be ignored because len == cap */
    append(&s, make_token("z", STRING));

    assert(s.len == 2);
    assert(strcmp(s.elems[0].value, "x") == 0);
    assert(strcmp(s.elems[1].value, "y") == 0);
}

static void test_append_null_stack_is_safe(void)
{
    append(NULL, make_token("x", STRING));
}

static void test_report_formats_errors(void)
{
    Token toks[] = {
        {.value = "FROM", .type = FROM}, /* invalid start */
        {.value = "SELECT", .type = SELECT},
        {.value = "FROM", .type = FROM},
        {.value = "t", .type = STRING},
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

static void test_report_formats_new_symbols(void)
{
    /* Manually craft a result using newly added symbols to exercise
     * symbol_to_str */
    ValidationError errs[2];
    errs[0].token    = &(Token){.value = "UPDATE", .type = UPDATE};
    errs[0].position = 0;
    errs[0].expected = SELECT_ITEM; /* arbitrary state */
    errs[0].message  = "unexpected token";

    errs[1].token    = &(Token){.value = "JOIN", .type = JOIN};
    errs[1].position = 1;
    errs[1].expected = CONDITION_LHS;
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
        {.value = "*", .type = STRING},
        {.value = ",", .type = COMMA},
        {.value = "a", .type = STRING},
        {.value = "FROM", .type = FROM},
        {.value = "t", .type = STRING},
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
        {.value = "name", .type = STRING},
        {.value = "FROM", .type = FROM},
        {.value = "users", .type = STRING},
        {.value = "WHERE", .type = WHERE},
        {.value = "name", .type = STRING},
        {.value = "=", .type = EQUALS},
        {.value = "'nick'", .type = STRING},
        {.value = "AND", .type = AND},
        {.value = "age", .type = STRING},
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
        {.value = "a", .type = STRING},
        {.value = "t", .type = STRING},
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
        {.value = "a", .type = STRING},
        {.value = "FROM", .type = FROM},
        {.value = "t", .type = STRING},
        {.value = ";", .type = SEMICOLON},
        {.value = "DROP", .type = STRING},
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
        {.value = "t", .type = STRING},
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
    assert(res.error_count >= 3);
}

/**
 * test_invalid_token_aborts_early - Invalid lexeme should halt validation
 */
static void test_invalid_token_aborts_early(void)
{
    Token toks[] = {
        {.value = "SELECT", .type = SELECT},
        {.value = "a", .type = STRING},
        {.value = "FROM", .type = FROM},
        {.value = "t", .type = STRING},
        {.value = "(", .type = ROUND_BRACKETS_OPEN}, /* unsupported token */
        {.value = ";", .type = SEMICOLON},
    };
    TokenStack s = make_stack(toks, (int)(sizeof(toks) / sizeof(toks[0])));
    ValidationError errs[8];
    ValidationResult res = {.errors = errs, .error_capacity = 8};
    bool ok              = validate_query_with_errors(&s, &res);

    assert(!ok);
    assert(res.error_count == 2);

    /* Second error should describe the fatal stop */
    assert(res.errors[1].message && strcmp(res.errors[1].message,
                                           "fatal: stopped parsing after "
                                           "invalid token") == 0);
}

/**
 * test_invalid_new_keyword_token - Newly added keyword should also be rejected
 * by validator
 */
static void test_invalid_new_keyword_token(void)
{
    Token toks[] = {
        {.value = "UPDATE", .type = UPDATE}, /* not supported by validator */
        {.value = "t", .type = STRING},
        {.value = "SET", .type = SET},
        {.value = "a", .type = STRING},
        {.value = "=", .type = EQUALS},
        {.value = "1", .type = NUMBER},
        {.value = ";", .type = SEMICOLON},
    };
    TokenStack s = make_stack(toks, (int)(sizeof(toks) / sizeof(toks[0])));
    ValidationError errs[8];
    ValidationResult res = {.errors = errs, .error_capacity = 8};
    bool ok              = validate_query_with_errors(&s, &res);

    assert(!ok);
    assert(res.error_count >= 2);
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
