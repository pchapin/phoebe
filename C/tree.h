/****************************************************************************
FILE          : tree.h
LAST REVISION : 2003-03-22
SUBJECT       : Declarations of tree handling functions.
PROGRAMMER    : (C) Copyright 2003 by Peter Chapin

This file declares types and functions that pertain to the parse tree.

Please send comments or bug reports to

     Peter Chapin
     Vermont Technical College
     Randolph Center, VT 05061
     pchapin@ecet.vtc.edu
****************************************************************************/

#ifndef TREE_H
#define TREE_H

#include "vtcstr.h"

// Used to indicate the different statement types.
enum statement_type
  { BREAKtype,  CONTINUEtype, EPtype,     FORtype,    IFtype,
    IFELSEtype, REPEATtype,   RETURNtype, SWITCHtype, WHILEtype };

// Used with the different expressions. PASSop means send the left
// parameter as the value of this expression. PROMPTop means ask the
// user.
//
enum operation { ORop, ANDop, NOTop, PASSop, PROMPTop };

// ---------------
// The structures!
// ---------------

// Forward declarations just to be nice.
struct case_branch;
struct case_list;
struct expression;
struct statement;
struct statement_list;

// Used to represent one branch of a case statement.
struct case_branch {
  struct statement_list *first;
  vtc_string            *case_condition;
};

// Used to represent a list of case branches.
struct case_list {
  struct case_list      *first;
  struct case_branch    *second;
};

// Used to represent the various expression types.
struct expression {
  struct expression     *first;
  struct expression     *second;
  enum   operation       op;
  vtc_string            *ep;
};

// Used to represent the various statement types.
struct statement {
  enum   statement_type  type;
  struct expression     *conditional;
  struct statement_list *first;
  struct statement_list *second;
  vtc_string            *ep;
  struct case_list      *cl;
};

// Used to represent statement lists.
struct statement_list {
  struct statement_list *first;
  struct statement      *second;
};

// --------------
// The functions!
// --------------

struct case_branch *new_case_branch_node(
  struct statement_list *first,
  vtc_string            *case_condition);

struct case_list *new_case_list_node(
  struct case_list      *first,
  struct case_branch    *second);

struct expression *new_expression_node(
  struct expression     *first,
  struct expression     *second,
  enum   operation       op,
  vtc_string            *ep);

struct statement *new_statement_node(
  enum   statement_type  type,
  struct expression     *conditional,
  struct statement_list *first,
  struct statement_list *second,
  vtc_string            *ep,
  struct case_list      *cl);

struct statement_list *new_statement_list_node(
  struct statement_list *first,
  struct statement      *second);


enum abort_type { NORMAL, fromBREAK, fromCONTINUE };

// This function performs the actions of the statment list.
enum abort_type execute_statement_list(struct statement_list *sub);

// This function performs the action of the indicated statement.
enum abort_type execute_statement(struct statement *sub);

// This function executes a case list in a switch statement.
enum abort_type execute_case_list(struct case_list *cl);

// This function returns TRUE or FALSE.
int evaluate_expression(struct expression *sub);

#endif
