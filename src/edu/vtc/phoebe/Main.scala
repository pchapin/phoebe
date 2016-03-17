package edu.vtc.phoebe

import org.antlr.v4.runtime._
import org.antlr.v4.runtime.tree.ParseTreeWalker

object Main {

  def main(args: Array[String]): Unit = {
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
  }

}
