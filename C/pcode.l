%{
/****************************************************************************
FILE          : pcode.l
LAST REVISION : 2005-05-15
SUBJECT       : Lex specification for my p-code language.
PROGRAMMER    : (C) Copyright 2005 by Peter C. Chapin

Please send comments or bug reports to

     Peter C. Chapin
     Vermont Technical College
     Randolph Center, VT 05061
     pchapin@ecet.vtc.edu
****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "vtcstr.h"
#include "pcode.tab.h"

int current_line = 1;

%}

%%
[ \t\f\r\n]  { if (yytext[0] == '\n') current_line++; }
#.*          { /* Do nothing */  }
AND          { return AND;       }
BEGIN        { return pBEGIN;    }
BREAK        { return BREAK;     }
CASE         { return CASE;      }
CONTINUE     { return CONTINUE;  }
DECLARE      { return DECLARE;   }
DEFAULT      { return DEFAULT;   }
DOMAIN       { return DOMAIN;    }
ELSE         { return ELSE;      }
END          { return END;       }
\[           {
               int ch;
               vtc_string *accumulator =
                 (vtc_string *)malloc(sizeof(vtc_string));

               vtc_string_init(accumulator);
               vtc_string_appendchar(accumulator, '[');
               while ((ch = input()) != EOF && ch != ']') {
                 vtc_string_appendchar(accumulator, ch);
                 if (ch == '\n') current_line++;
               }
               vtc_string_appendchar(accumulator, ']');
               yylval.stringp = accumulator;
               return EP;        
             }
FOR          { return FOR;       }
FOREACH      { return FOREACH;   }
FUNCTION     { return FUNCTION;  }
IF           { return IF;        }
IS           { return IS;        }
LOOP         { return LOOP;      }
NOT          { return NOT;       }
OF           { return OF;        }
OR           { return OR;        }
PROMISES     { return PROMISES;  }
RANGE        { return RANGE;     }
REPEAT       { return REPEAT;    }
REQUIRES     { return REQUIRES;  }
RETURN       { return RETURN;    }
RETURNS      { return RETURNS;   }
SWITCH       { return SWITCH;    }
THEN         { return THEN;      }
TYPE         { return TYPE;      }
UNTIL        { return UNTIL;     }
VOID         { return VOID;      }
WHILE        { return WHILE;     }
.            { return yytext[0]; }
%%
