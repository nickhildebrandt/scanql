#pragma once

/*
 * SqlToken - Enumeration of SQL token types
 */
#include <stdio.h>

typedef enum : unsigned short
{

    // KEYWORDS
    SELECT,
    FROM,
    WHERE,
    CREATE,
    TABLE,
    DROP,
    IF,
    EXISTS,
    INSERT,
    ENUM,
    INTO,
    NOT,
    IN,
    null,

    // special character
    COMMA,
    EQUALS,
    NEGATION,
    NUMBER,
    STRING,
    SQL_NULL,
    AND,
    OR,
    SEMICOLON,
    PAREN,
    ROUND_BRACKETS_OPEN,
    ROUND_BRACKETS_CLOSE,
    INLINE_COMMENT_MINUS,
    MULTILINE_COMMENT_OPEN,
    MULTILINE_COMMENT_CLOSE,
} SqlToken;

typedef struct
{
    char* value;
    SqlToken type;
} Token;

void printToken(Token t)
{
    printf("Token { value: %s, type: %d}\n", t.value, t.type);
}

typedef struct
{
    Token* elems; // Pointer to an array of Token elements, representing the
                  // stack's contents.
    int len; // The current length of the stack, indicating how many tokens are
             // currently in it.
    int cap; // The capacity of the stack, which indicates the maximum number of
             // tokens it can hold before needing to be resized.
} TokenStack;

void append(TokenStack* list, Token t)
{
    if (list->len <= list->cap)
    {
        list->elems[list->len] = t;
        list->len += 1;
    }
}
