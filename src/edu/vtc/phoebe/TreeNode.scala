package edu.vtc.phoebe

object TreeNode {

  class Node

  case class EPNode(text: String) extends Node

  class StatementListNode extends Node
  case class MultipleStatementList(remaining: StatementListNode, statement: StatementNode)
    extends StatementListNode
  case class StatementListPassThrough(statement: StatementNode)
    extends StatementListNode

  class StatementNode extends Node
  case class EPStatement(text: String) extends StatementNode
  case object ReturnStatement extends StatementNode
  case class IfStatement(condition: ConditionalExpressionNode, statements: StatementListNode)
    extends StatementNode
  case class IfElseStatement(condition: ConditionalExpressionNode, ifStatements: StatementListNode, elseStatements: StatementListNode)
    extends StatementNode
  case class WhileStatement(condition: ConditionalExpressionNode, statements: StatementListNode)
    extends StatementNode
  case class RepeatStatement(statements: StatementListNode, condition: ConditionalExpressionNode)

  class ConditionalExpressionNode extends Node
  case class ConditionalBinaryExpression(left: ConditionalExpressionNode, right: AndExpressionNode)
    extends ConditionalExpressionNode
  case class ConditionalPassThroughExpression(child: AndExpressionNode)
    extends ConditionalExpressionNode

  class AndExpressionNode extends Node
  case class AndBinaryExpression(left: AndExpressionNode, right: SimpleExpressionNode)
    extends AndExpressionNode
  case class AndPassThroughExpression(child: SimpleExpressionNode)
    extends AndExpressionNode

  class SimpleExpressionNode extends Node
  case class SimpleNotExpression(child: SimpleExpressionNode) extends SimpleExpressionNode
  case class SimpleNestedExpression(child: ConditionalExpressionNode) extends SimpleExpressionNode
  case class SimpleEPExpression(child: EPNode) extends SimpleExpressionNode
}
