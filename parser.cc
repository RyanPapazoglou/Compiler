/*
  Ryan Papazoglou
  rpp219
  CSE109
  Program 5
  Desc: Parser class to build the tree for assembly use
*/

#include "parser.h"
#include <cstring>


Parser::Parser(Lexer& lexerx, ostream& outx): lexer(lexerx), out(outx), lindex(1), tindex(1) {
  token = lexer.nextToken();
}

Parser::~Parser() {
}

void Parser::error(string message) {
  cerr << message << " Found " << token.lexeme() << " at line " << token.line() << " position " << token.pos() << endl;
  exit(1);
}

void Parser::check(int tokenType, string message) {
  if (token.type() != tokenType)
    error(message);
}

Parser::TreeNode* Parser::factor() {
  token = lexer.nextToken();
  int tokenType = Token.getType();
  TreeNode* node = NULL;
  
  switch(tokenType){
  case Token::IDENT:
    node = new TreeNode(PUSHV, token.getLexeme());
    token = lexer.nextToken();
    return node;
    break;
  case Token::INTLIT:
    node = new TreeNode(PUSHL, token.getLexeme());
    token = lexer.nextToken();
    return node;
    break;
  case Token::LPAREN:
    check(Token::LPAREN, "Expecting (");
    node = expression();
    token = lexer.nextToken();
    check(Token::RPAREN, "Expecting )");
    return node;
  }
  error("Not a factor token");
}

Parser::TreeNode* Parser::term() {
  TreeNode* node = NULL;
  node = factor();
  int tokenType = Token.getType();
  while((tokenType == Token::TIMES) || (tokenType == Token::DIVIDE)){
    tokenType = Token.getType();
    token = lexer.nextToken();
    switch(tokenType){
    case Token::TIMES:
      node = new TreeNode(MULT, node, factor());
      break;
    case Token::DIVIDE:
      node = new TreeNode(DIV, node, factor());
      break;
    }
  }
  token = lexer.nextToken();
  return node;
}

Parser::TreeNode* Parser::expression() {
  TreeNode* node = NULL;
  node = term();
  int tokenType = Token.getType();
  while((tokenType == Token::PLUS) || (tokenType == Token::MINUS)){
    tokenType = Token.getType();
    token = lexer.nextToken();
    switch(tokenType){
    case Token::PLUS:
      node = new TreeNode(ADD, node, term());
      break;
    case Token::MINUS:
      node = new TreeNode(SUB, node, term());
      break;
    }
  }
  token = lexer.nextToken();
  return node;
}

Parser::TreeNode* Parser::relationalExpression() {
  TreeNode* node = expression();
  token = lexer.nextToken();
  int tokenType = Token.getType();
  if((tokenType == Token::EQ) || (tokenType == Token::LT) || (tokenType == Token::LE) || (tokenType == Token::GT) || (tokenType == Token::GE) || (tokenType == Token::NE){
      token = lexer.nextToken();
      switch(tokenType){
      case Token::EQ:
	node = new TreeNode(ISEQ, node, expression());
	break;
      case Token::LT:
	node = new TreeNode(ISLT, node, expression());
	break;
      case Token::LE:
	node = new TreeNode(ISLE, node, expression());
	break;
      case Token::GT:
	node = new TreeNode(ISGT, node, expression());
	break;
      case Token::GE:
	node = new TreeNode(ISGE, node, expression());
	break;
      case Token::NE:
	node = new TreeNode(ISNE, node, expression());
	break;
      }
    }
    return node; 
}

Parser::TreeNode* Parser::logicalExpression() {

}
