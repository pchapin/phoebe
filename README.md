
Pseudo-Code Interpreter
=======================

This repository contains a simple pseudo code interpreter. The program developed here will
accept an input file in the pseudo code described in pcode.txt and "execute it" by interacting
with the user as each action statement is evaluated. Since there is no way this program could
know what the action statements are really intended to do, the best this program can accomplish
is to ask the user to imagine that they are executing. The program also asks the user about the
truth or falsehood of conditional expressions.

There are two potentially useful applications of this program. First it can verify the syntax of
the pseudo-code given to it. Second it can be used to explore the design of a program by making
that design executable even when while being very abstract.

BUGS

+ There are features described in pcode.txt that are not implemented in the program. Similarly I
  believe there are features in the program that are not described in pcode.txt. This
  inconsistency should be fixed.

+ If you enter in p-code interactively (at standard input) and then type an EOF indication to
  terminate the input, you can't execute the pseudo code properly. The execution engine tries to
  read responses from standard input and standard input is at EOF by that time. This isn't a
  problem when reading p-code from a file.
