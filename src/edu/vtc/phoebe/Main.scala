package edu.vtc.phoebe

import org.antlr.v4.runtime._
import org.antlr.v4.runtime.tree.ParseTreeWalker

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

    def statement_list(tokenList: List[Token]): (List[Token], TreeNode.StatementListNode) = {
      // TODO: Fix me!
      (tokenList, StatementListPassThrough(EPStatement("Hello, World!")))
    }

    // Start things off by calling the start symbol.
    val (remainingTokens, finalTree) = statement_list(tokenList)
    if (remainingTokens.nonEmpty) {
      // TODO: Handle the error! Unprocessed tokens at the end of input.
    }
    finalTree
  }

}
