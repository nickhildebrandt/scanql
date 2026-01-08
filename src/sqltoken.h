#pragma once

/*
 * SqlToken - Enumeration of SQL token types
*/
typedef enum : unsigned short
{
    SELECT,
    FROM,
    WHERE,
    COMMA,
    EQUALS,
    NEGATION,
    NUMBER,
    STRING,
    NOT,
    IN,
    SQL_NULL,
    AND,
    OR,
    ASTERISK,
    SEMICOLON,
    OPEN_PAREN,
    CLOSE_PAREN,
} SqlToken;
