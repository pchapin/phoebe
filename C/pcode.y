%{
/****************************************************************************
FILE          : pcode-simple.y
LAST REVISION : 2003-05-15
SUBJECT       : Simple YACC grammar for my p-code.
PROGRAMMER    : (C) Copyright 2005 by Peter C. Chapin

See pcode.txt for the documenation (such as it is).

Please send comments or bug reports to

     Peter Chapin
     Vermont Technical College
     Randolph Center, VT 05061
     pchapin@ecet.vtc.edu
****************************************************************************/

#include "tree.h"

struct statement_list *top_node;

%}

%union {
  struct statement_list *statementlistp;
  struct statement      *statementp;
  struct expression     *exprp;
  vtc_string            *stringp;
  struct case_branch    *casebranchp;
  struct case_list      *caselistp;
};

%token AND
%token pBEGIN
%token BREAK
%token CASE
%token CONTINUE
%token DECLARE
%token DEFAULT
%token DOMAIN
%token ELSE
%token END
%token <stringp> EP
%token FOR
%token FOREACH
%token FUNCTION
%token IF
%token IS
%token LOOP
%token NOT
%token OF
%token OR
%token PROMISES
%token RANGE
%token REPEAT
%token REQUIRES
%token RETURN
%token RETURNS
%token SWITCH
%token THEN
%token TYPE
%token UNTIL
%token VOID
%token WHILE

/* %type function_list    */
/* %type function         */
/* %type function_header  */
/* %type param_list       */
/* %type param            */
/* %type result_clause    */
/* %type result_list      */
/* %type result           */
%type <statementlistp> statement_list
%type <statementp>     statement
%type <statementp>     switch_statement
%type <caselistp>      case_list
%type <casebranchp>    case
%type <exprp> conditional_expr
%type <exprp> and_expr
%type <exprp> simple_expr

%start program

%%

/* For now, disable the function syntax...
function_list:
     function_list function
   | function
   | statement_list
   ;

function:
     FUNCTION function_header '(' param_list ')'
       result_clause pBEGIN statement_list END
   ;

function_header:
     EP
   | EP REQUIRES EP
   ;

param_list:
     param_list ',' param
   | param
   ;

param:
     EP
   | EP DOMAIN EP
   ;

result_clause:
     RETURNS result_list
   | PROMISES EP RETURNS result_list
   ;

result_list:
     result_list ',' result
   | result
   ;

result:
     EP
   | EP RANGE EP
   | VOID
   ;
*/

program:
     statement_list
     { top_node = $1; }
   | declare_block statement_list
     { top_node = $2; }
   ;

declare_block:
     DECLARE type_definition_list END
   ;

type_definition_list:
     type_definition_list type_definition
   | type_definition
   ;

type_definition:
     TYPE EP IS type_descriptor
   ;

type_descriptor:
     EP
   | EP ':' EP
   | EP OF type_descriptor_list END
   ;

type_descriptor_list:
     type_descriptor_list ',' type_descriptor
   | type_descriptor
   ;

statement_list:
     statement_list statement
     { $$ = new_statement_list_node($1, $2); }
   | statement
     { $$ = new_statement_list_node(NULL, $1); }
   ;

statement:
     EP
     { $$ = new_statement_node(EPtype, NULL, NULL, NULL, $1, NULL); }
   | BREAK
     { $$ = new_statement_node(BREAKtype, NULL, NULL, NULL, NULL, NULL); }
   | CONTINUE
     { $$ = new_statement_node(CONTINUEtype, NULL, NULL, NULL, NULL, NULL); }
   | RETURN
     { $$ = new_statement_node(RETURNtype, NULL, NULL, NULL, NULL, NULL); }
   | IF conditional_expr THEN statement_list END
     { $$ = new_statement_node(IFtype, $2, $4, NULL, NULL, NULL); }
   | IF conditional_expr THEN statement_list ELSE statement_list END
     { $$ = new_statement_node(IFELSEtype, $2, $4, $6, NULL, NULL); }
   | FOR conditional_expr LOOP statement_list END
     { $$ = new_statement_node(FORtype, $2, $4, NULL, NULL, NULL); }
   | FOREACH conditional_expr LOOP statement_list END
     { $$ = new_statement_node(FORtype, $2, $4, NULL, NULL, NULL); }
   | WHILE conditional_expr LOOP statement_list END
     { $$ = new_statement_node(WHILEtype, $2, $4, NULL, NULL, NULL); }
   | REPEAT statement_list UNTIL conditional_expr
     { $$ = new_statement_node(REPEATtype, $4, $2, NULL, NULL, NULL); }
   | switch_statement
     { $$ = $1; }
   ;

switch_statement:
     SWITCH EP case_list END
     { $$ = new_statement_node(SWITCHtype, NULL, NULL, NULL, $2, $3); }
   ;

case_list:
     case_list case
     { $$ = new_case_list_node($1, $2); }
   | case
     { $$ = new_case_list_node(NULL, $1); }
   ;

case:
     CASE EP ':' statement_list END
     { $$ = new_case_branch_node($4, $2); }
   | DEFAULT ':' statement_list END
     { $$ = new_case_branch_node($3, NULL); }
   ;

conditional_expr:
     conditional_expr OR and_expr
     { $$ = new_expression_node($1, $3, ORop, NULL); }
   | and_expr
     { $$ = new_expression_node($1, NULL, PASSop, NULL); }
   ;

and_expr:
     and_expr AND simple_expr
     { $$ = new_expression_node($1, $3, ANDop, NULL); }
   | simple_expr
     { $$ = new_expression_node($1, NULL, PASSop, NULL); }
   ;

simple_expr:
     NOT simple_expr
     { $$ = new_expression_node($2, NULL, NOTop, NULL); }
   | '(' conditional_expr ')'
     { $$ = new_expression_node($2, NULL, PASSop, NULL); }
   | EP
     { $$ = new_expression_node(NULL, NULL, PROMPTop, $1); }
   ;

%%
