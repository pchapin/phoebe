lexer grammar PhoebeLexer;

//@lexer::header {
//    package org.pchapin.phoebe;
//}

/* ======= */
/* Grammar */
/* ======= */

// No grammar rules allowed!

/* =========== */
/* Lexer rules */
/* =========== */

// --------------
// Reserved Words
// --------------

AND      : 'AND';
BEGIN    : 'BEGIN';
BREAK    : 'BREAK';
CASE     : 'CASE';
CONTINUE : 'CONTINUE';
DECLARE  : 'DECLARE';
DOMAIN   : 'DOMAIN';
ELSE     : 'ELSE';
END      : 'END';
FOR      : 'FOR';
FOREACH  : 'FOREACH';
FUNCTION : 'FUNCTION';
IF       : 'IF';
IS       : 'IS';
LOOP     : 'LOOP';
NOT      : 'NOT';
OF       : 'OF';
OR       : 'OR';
PROMISES : 'PROMISES';
RANGE    : 'RANGE';
REPEAT   : 'REPEAT';
REQUIRES : 'REQUIRES';
RETURN   : 'RETURN';
RETURNS  : 'RETURNS';
SWITCH   : 'SWITCH';
THEN     : 'THEN';
TYPE     : 'TYPE';
UNTIL    : 'UNTIL';
VOID     : 'VOID';
WHILE    : 'WHILE';


IDENTIFIER
    :   [a-zA-Z][a-zA-Z0-9_]*;

EP
    :   '[' .*? ']';

WHITESPACE
    :   [ \t\f\r\n]+  -> skip;

COMMENT
    :   '#' .*? [\r\n] -> skip;

NUMERIC_LITERAL
    :   (DECIMAL | BASED) ( ('E' | 'e') DIGIT+ )?;

fragment DECIMAL
    :   DIGIT ('_'? DIGIT)*;

fragment BASED
    :   DIGIT+ '#' HDIGIT ('_'? HDIGIT)* '#';

fragment DIGIT
    :   [0-9];

fragment HDIGIT
    :   [0-9a-fA-F];
