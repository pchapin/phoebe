/****************************************************************************
FILE          : tree.c
LAST REVISION : 2003-03-22
SUBJECT       : Helper functions for parse tree building.
PROGRAMMER    : (C) Copyright 2003 by Peter Chapin

This file implements the functions declared in tree.h

To Do:

+ Fix execute_case_list so that the default case is properly handled.

Please send comments or bug reports to

     Peter Chapin
     Vermont Technical College
     Randolph Center, VT 05061
     pchapin@ecet.vtc.edu
****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "tree.h"

struct case_branch *new_case_branch_node(
  struct statement_list *first,
  vtc_string            *case_condition)
{
  // Allocate space for the structure.
  struct case_branch *p =
    (struct case_branch *)malloc(sizeof(struct case_branch));
  if (p == NULL) {
    // COMPLAIN!
  }

  // Fill it in.
  p->first          = first;
  p->case_condition = case_condition;

  return p;
} 


struct case_list *new_case_list_node(
  struct case_list   *first,
  struct case_branch *second)
{
  // Allocate space for the structure.
  struct case_list *p =
    (struct case_list *)malloc(sizeof(struct case_list));
  if (p == NULL) {
    // COMPLAIN!
  }

  // Fill it in.
  p->first  = first;
  p->second = second;

  return p;
} 


struct expression *new_expression_node(
  struct expression *first,
  struct expression *second,
  enum   operation   op,
  vtc_string        *ep)
{
  // Allocate space for the structure.
  struct expression *p =
    (struct expression *)malloc(sizeof(struct expression));
  if (p == NULL) {
    // COMPLAIN!
  }

  // Fill it in.
  p->first  = first;
  p->second = second;
  p->op     = op;
  p->ep     = ep;

  return p;
} 


struct statement *new_statement_node(
  enum   statement_type  type,
  struct expression     *conditional,
  struct statement_list *first,
  struct statement_list *second,
  vtc_string            *ep,
  struct case_list      *cl)
{
  // Allocate space for the structure.
  struct statement *p =
    (struct statement *)malloc(sizeof(struct statement));
  if (p == NULL) {
    // COMPLAIN!
  }

  // Fill it in.
  p->type        = type;
  p->conditional = conditional;
  p->first       = first;
  p->second      = second;
  p->ep          = ep;
  p->cl          = cl;

  return p;
}


struct statement_list *new_statement_list_node(
  struct statement_list *first,
  struct statement      *second)
{
  // Allocate space for the structure.
  struct statement_list *p =
    (struct statement_list *)malloc(sizeof(struct statement_list));
  if (p == NULL) {
    // COMPLAIN!
  }

  // Fill it in.
  p->first  = first;
  p->second = second;

  return p;
}


enum abort_type execute_statement_list(struct statement_list *list)
{
  enum abort_type result = NORMAL;

  if (list->first) result = execute_statement_list(list->first);
  if (result == NORMAL) result = execute_statement(list->second);
  return result;
}


enum abort_type execute_statement(struct statement *statement)
{
  enum abort_type result = NORMAL;

  switch (statement->type) {
    case BREAKtype:
      result = fromBREAK;
      break;

    case CONTINUEtype:
      result = fromCONTINUE;
      break;

    case EPtype:
      printf("%s\n", vtc_string_getcharp(statement->ep));
      while (getchar() != '\n') ;
      break;

    case FORtype:
    case WHILEtype:
      while (evaluate_expression(statement->conditional)) {
        result = execute_statement_list(statement->first);
        if (result == fromBREAK) break;
        if (result == fromCONTINUE) continue;
      }
      result = NORMAL;
      break;

    case IFtype:
      if (evaluate_expression(statement->conditional)) {
        result = execute_statement_list(statement->first);
      }
      break;

    case IFELSEtype:
      if (evaluate_expression(statement->conditional)) {
        result = execute_statement_list(statement->first);
      }
      else {
        result = execute_statement_list(statement->second);
      }
      break;

    case REPEATtype:
      do {
        result = execute_statement_list(statement->first);
        if (result == fromBREAK) break;
        if (result == fromCONTINUE) continue;
      } while (!evaluate_expression(statement->conditional));
      result = NORMAL;
      break;

    case RETURNtype:
      printf("\nRETURN not implemented!\n");
      break;

    case SWITCHtype:
      printf("Which of the following is %s?\n",
        vtc_string_getcharp(statement->ep));
      result = execute_case_list(statement->cl);
      break;
  }

  return result;
}


enum abort_type execute_case_list(struct case_list *cl)
{
  enum abort_type result = NORMAL;
  int ch;

  // The NULL ep is the default case. I don't handle that right.
  if (cl->second->case_condition == NULL) return result;

  printf("%s Match? [y/n] ", vtc_string_getcharp(cl->second->case_condition));
  ch = getchar();
  while (getchar() != '\n') ;
  if (ch == 'Y' || ch == 'y') {
    result = execute_statement_list(cl->second->first);
  }
  else if (cl->first != NULL) {
    result = execute_case_list(cl->first);
  }
  return result;
}


int evaluate_expression(struct expression *sub)
{
  int result = 0;
  int ch;

  switch (sub->op) {
    case PASSop:
      result = evaluate_expression(sub->first);
      break;

    case NOTop:
      result = !evaluate_expression(sub->first);
      break;

    case ANDop:
      result = evaluate_expression(sub->first) &&
               evaluate_expression(sub->second);
      break;

    case ORop:
      result = evaluate_expression(sub->first) ||
               evaluate_expression(sub->second);
      break;

    case PROMPTop:
      printf("%s\n", vtc_string_getcharp(sub->ep));
      printf("True or False? ");
      ch = getchar();
      while (getchar() != '\n') ;
      if (ch == 'T' || ch == 't') result = 1;
      break;
  }

  return result;
}
