grammar Phoebe;

@parser::header {
    package edu.vtc.phoebe;
}

@lexer::header {
    package edu.vtc.phoebe;
}


/* ======= */
/* Grammar */
/* ======= */

program
    :   BEGIN END;

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

// Various operator symbols.
DIVIDE   : '/';
MINUS    : '-';
MULTIPLY : '*';
PLUS     : '+';

IDENTIFIER
    :   [a-zA-Z][a-zA-Z0-9_]*;

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
