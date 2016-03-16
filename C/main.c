/****************************************************************************
FILE          : main.c
LAST REVISION : 2003-02-22
SUBJECT       : Main program of the pcode project.
PROGRAMMER    : (C) Copyright 2003 by Peter Chapin

Please send comments or bug reports to

     Peter Chapin
     Vermont Technical College
     Randolph Center, VT 05061
     pchapin@ecet.vtc.edu
****************************************************************************/

#include <stdio.h>
#include "tree.h"

extern int   yyparse();
extern FILE *yyin;
extern int   current_line;
extern struct statement_list *top_node;

#define YES 1
#define NO  0

int main(int argc, char **argv)
{
  char *input_filename = NULL;
  enum abort_type result;

  // Analyze the command line.
  while (*++argv != NULL) {
    if (**argv != '-') {
      input_filename = *argv;
    }
    else {
      switch (*++*argv) {
        default:
          printf("Unrecognized option: %c (ignored)\n", **argv);
          break;
      }
    }
  }

  // Open the input file and tell the lexical analyzer about it.
  if (input_filename != NULL) {
    yyin = fopen(input_filename, "r");
    if (yyin == NULL) {
      printf("Unable to open %s for input.\n", input_filename);
      return 1;
    }
  }

  // Parse the input.
  if (yyparse() == 0) {
    printf("Parsed successfully!\n");
    result = execute_statement_list(top_node);
    if (result == fromBREAK) {
      printf("Warning: Executed a BREAK without an enclosing loop.\n");
    }
    else if (result == fromCONTINUE) {
      printf("Warning: Executed a CONTINUE without an enclosing loop.\n");
    }
  }

  return 0;
}

void yyerror(char *message)
{
  printf("Syntax error: [line %d] %s\n", current_line, message);
}
