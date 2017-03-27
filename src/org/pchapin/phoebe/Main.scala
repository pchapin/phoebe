package org.pchapin.phoebe

import org.antlr.v4.runtime._

object Main {

  def main(args: Array[String]): Unit = {
    import scala.collection.JavaConverters._

    // Analyze the command line.
    if (args.length != 1) {
      println("Usage: java -jar Phoebe.jar source-file.pcd")
      System.exit(1)
    }

    // Create a stream that reads from the specified file.
    val input = new ANTLRFileStream(args(0))

    // Tokenize the input file.
    val lexer  = new PhoebeLexer(input)
    val tokens = new CommonTokenStream(lexer)
    tokens.fill()

    val tokenList = tokens.getTokens
    val scalaTokenList: List[Token] = tokenList.asScala.toList
    val parseTree = parse(scalaTokenList)
    println("Parse Tree: " + parseTree)
  }

  // TODO: Finish me!
  def parse(tokenList: List[Token]): TreeNode.StatementListNode = {
    import TreeNode._

    def statement_list(tokenList: List[Token]): (List[Token], StatementListNode) = {
      // TODO: Fix me!
      (tokenList, StatementListPassThrough(EPStatement("Hello, World!")))
    }

    def statement(tokenList: List[Token]): (List[Token], StatementNode) = {
      // What kind of statement are we dealing with?
      tokenList.head.getType match {
        case PhoebeLexer.WHILE =>
          val (afterConditional, conditionalExpression) = conditional_expr(tokenList.drop(1))
          if (afterConditional.head.getType != PhoebeLexer.LOOP) {
            // Unexpected token where LOOP was expected.
            // TODO: Produce useful error message and improve error recovery.
            (afterConditional.drop(1),
              WhileStatement(conditionalExpression, StatementListPassThrough(EPStatement("BAD TOKEN"))))
          }
          else {
            val (afterStatements, statementList) = statement_list(afterConditional.drop(1))
            if (afterStatements.head.getType != PhoebeLexer.END) {
              // Unexpected token where END was expected.
              // TODO: Produce useful error message and improve error recovery.
              (afterStatements.drop(1),
                WhileStatement(conditionalExpression, statementList))
            }
            else {
              (afterStatements.drop(1),
                WhileStatement(conditionalExpression, statementList))
            }
          }

        case _ =>
          // Unrecognized or unsupported token found.
          // TODO: Produce useful error message and improve error recovery.
          (tokenList.drop(1), EPStatement("BAD TOKEN"))
      }
    }

    def conditional_expr(tokenList: List[Token]): (List[Token], ConditionalExpressionNode) = {
      // TODO: Fix me!
      (tokenList,
        ConditionalPassThroughExpression(AndPassThroughExpression(SimpleEPExpression(EPNode("Hi")))))
    }

    // Start things off by calling the start symbol.
    val (remainingTokens, finalTree) = statement_list(tokenList)
    if (remainingTokens.nonEmpty) {
      // Unprocessed tokens at the end of input.
      // TODO: Produce useful error message.
    }
    finalTree
  }

}
