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

    // Parser expectation states (formerly SqlTokenExpected)
    EXP_SELECT,         // SELECT
    EXP_SELECT_ITEM,    // column | *
    EXP_SELECT_CONT,    // , | FROM
    EXP_TABLE,          // table
    EXP_WHERE_OR_END,   // WHERE | ;
    EXP_CONDITION_LHS,  // identifier
    EXP_CONDITION_OP,   // =
    EXP_CONDITION_RHS,  // literal
    EXP_CONDITION_CONT, // AND | OR | ;
    EXP_END
} SqlSymbols;
