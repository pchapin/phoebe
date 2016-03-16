package edu.vtc.phoebe

import org.antlr.v4.runtime._
import org.antlr.v4.runtime.tree.ParseTreeWalker

object Main {

  def main(args: Array[String]): Unit = {
    // Analyze the command line.
    if (args.length != 2) {
      println("Usage: java -jar Phoebe.jar source-file.pcd")
      System.exit(1)
    }

    // Create a stream that reads from the specified file.
    val input = new ANTLRFileStream(args(1))

    // Parse the input file as Ada.
    val lexer  = new PhoebeLexer(input)
    val tokens = new CommonTokenStream(lexer)
    val parser = new PhoebeParser(tokens)
    val tree   = parser.program

    // Do nothing more (semantic analysis is all that is necessary).
    val parseTree = tree.toStringTree(parser)
    println("*** AST ==> " + parseTree + "\n")
  }

}
