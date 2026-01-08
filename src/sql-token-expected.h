#pragma once

/*
 * SqlTokenExpected - Enumeration of expected SQL token types in parsing
 */
typedef enum
{
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
} SqlTokenExpected;