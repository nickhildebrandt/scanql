#pragma once

/*
 * SqlToken - Enumeration of SQL token types
 */
typedef enum : unsigned short
{
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
} SqlSymbols;
