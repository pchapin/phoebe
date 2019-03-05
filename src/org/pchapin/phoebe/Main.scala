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
    val input = CharStreams.fromFileName(args(0))

    // Tokenize the input file.
    val lexer  = new PhoebeLexer(input)
    val tokens = new CommonTokenStream(lexer)
    tokens.fill()

    val tokenList = tokens.getTokens
    val scalaTokenList: List[Token] = tokenList.asScala.toList
    val (messages, parseTree) = parse(scalaTokenList)
    println("Messages:\n" + messages.reverse)
    println("\nParse Tree:\n" + parseTree)
  }


  // TODO: Finish me!
  def parse(tokenList: List[Token]): (List[String], TreeNode.StatementListNode) = {
    import TreeNode._
    type TokenListType = List[Token]
    type MessageListType = List[String]


    def statement_list(tokenList: TokenListType, messages: MessageListType):
      (TokenListType, MessageListType, StatementListNode) = {

      // TODO: Fix me!
      (tokenList, messages, StatementListPassThrough(EPStatement("Hello, World!")))
    }


    def statement(tokenList: TokenListType, messages: MessageListType):
      (TokenListType, MessageListType, StatementNode) = {

      // What kind of statement are we dealing with?
      tokenList.head.getType match {
        case PhoebeLexer.WHILE =>
          val (afterConditionalTokens, afterConditionalMessages, conditionalExpression) =
            conditional_expr(tokenList.drop(1), messages)

          if (afterConditionalTokens.head.getType != PhoebeLexer.LOOP) {
            (afterConditionalTokens,
              "Unexpected token where LOOP was expected" :: afterConditionalMessages,
              WhileStatement(conditionalExpression, StatementListPassThrough(EPStatement("NULL BODY"))))
          }
          else {
            val (afterStatementsTokens, afterStatementsMessages, statementList) =
              statement_list(afterConditionalTokens.drop(1), afterConditionalMessages)

            if (afterStatementsTokens.head.getType != PhoebeLexer.END) {
              (afterStatementsTokens,
                "Unexpected token where END was expected" :: afterStatementsMessages,
                WhileStatement(conditionalExpression, statementList))
            }
            else {
              (afterStatementsTokens.drop(1),
                afterStatementsMessages,
                WhileStatement(conditionalExpression, statementList))
            }
          }

        case _ =>
          val badToken = tokenList.head
          (tokenList.drop(1),
            "Unrecognized or unsupported token found" :: messages,
            EPStatement("BAD TOKEN: " + badToken.getText))
      }
    }


    def conditional_expr(tokenList: TokenListType, messages: MessageListType):
      (TokenListType, MessageListType, ConditionalExpressionNode) = {

      // TODO: Fix me!
      (tokenList, messages, ConditionalPassThroughExpression(AndPassThroughExpression(SimpleEPExpression(EPNode("Hi")))))
    }


    // Calling the start symbol and return the results.
    val (finalTokens, finalMessages, finalTree) = statement_list(tokenList, List())
    if (finalTokens.nonEmpty)
      ("Unprocessed tokens at end of input" :: finalMessages, finalTree)
    else
      (finalMessages, finalTree)
  }

}
