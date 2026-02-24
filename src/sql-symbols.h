#pragma once

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
