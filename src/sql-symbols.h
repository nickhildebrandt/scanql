#pragma once

/*
 * SqlToken - Enumeration of SQL token types
 */
typedef enum : unsigned short
{
    // Keywords
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

    // Types
    TYPE_TEXT,
    TYPE_ENUM,

    // special character
    COMMA,
    EQUALS,
    NEGATION,
    NUMBER,
    STRING,
    STRING_VALUE,
    SQL_NULL,
    AND,
    OR,
    SEMICOLON,
    ROUND_BRACKETS_OPEN,
    ROUND_BRACKETS_CLOSE,
    INLINE_COMMENT_MINUS,
    MULTILINE_COMMENT_OPEN,
    MULTILINE_COMMENT_CLOSE,
    SELECT_ITEM,
    SELECT_CONT,
    WHERE_OR_END,
    CONDITION_LHS,
    CONDITION_OP,
    CONDITION_RHS,
    CONDITION_CONT,
    END
} SqlSymbols;
