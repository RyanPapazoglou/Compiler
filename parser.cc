/*
  Ryan Papazoglou
  rpp219
  CSE109
  Program 5
  Desc: Parser class to build the tree for assembly use
*/


#include "parser.h"
#include <cstring>


const string ops[] = {"ADD", "SUB", "MULT", "DIV",

		      "ISEQ", "ISNE", "ISLT", "ISLE", "ISGT", "ISGE",

		      "AND", "OR",

		      "PUSHL", "PUSHV", "STORE",

		      "JUMP", "JUMPF", "JUMPT", "CALL", "RET",

		      "PRINTF",

		      "LABEL", "SEQ" };


Parser::Parser(Lexer& lexerx, ostream& outx): lexer(lexerx), out(outx), lindex(1), tindex(1) {
  token = lexer.nextToken();
}

Parser::~Parser() {
}

void Parser::error(string message) {
  cerr << message << " Found " << token.getLexeme() << " at line " << token.getLine() << " position " << token.getPos() << endl;
  exit(1);
}

void Parser::check(int tokenType, string message) {
  if (token.getType() != tokenType)
    error(message);
}

Parser::TreeNode* Parser::factor() {
  token = lexer.nextToken();
  int tokenType = token.getType();
  TreeNode* node = NULL;
  
  switch(tokenType){
  case Token::IDENT:
    node = new TreeNode(Parser::PUSHV, token.getLexeme());
    token = lexer.nextToken();
    return node;
    break;
  case Token::INLIT:
    node = new TreeNode(Parser::PUSHL, token.getLexeme());
    token = lexer.nextToken();
    return node;
    break;
  case Token::LPAREN:
    check(Token::LPAREN, "Expecting (");
    node = expression();
    token = lexer.nextToken();
    check(Token::RPAREN, "Expecting )");
    return node;
    break;
  }
  //Might need to return something
  return node;
  error("Not a factor token");
}

Parser::TreeNode* Parser::term() {
  TreeNode* node = NULL;
  node = factor();
  int tokenType = token.getType();
  while((tokenType == Token::TIMES) || (tokenType == Token::DIVIDE)){
    tokenType = token.getType();
    token = lexer.nextToken();
    switch(tokenType){
    case Token::TIMES:
      node = new TreeNode(Parser::MULT, node, factor());
      break;
    case Token::DIVIDE:
      node = new TreeNode(Parser::DIV, node, factor());
      break;
    }
  }
  token = lexer.nextToken();
  return node;
}

Parser::TreeNode* Parser::expression() {
  TreeNode* node = NULL;
  node = term();
  int tokenType = token.getType();
  while((tokenType == Token::PLUS) || (tokenType == Token::MINUS)){
    tokenType = token.getType();
    token = lexer.nextToken();
    switch(tokenType){
    case Token::PLUS:
      node = new TreeNode(Parser::ADD, node, term());
      break;
    case Token::MINUS:
      node = new TreeNode(Parser::SUB, node, term());
      break;
    }
  }
  token = lexer.nextToken();
  return node;
}

Parser::TreeNode* Parser::relationalExpression() {
  TreeNode* node = expression();
  token = lexer.nextToken();
  int tokenType = token.getType();
  if((tokenType == Token::EQ) || (tokenType == Token::LT) || (tokenType == Token::LE) || (tokenType == Token::GT) || (tokenType == Token::GE) || (tokenType == Token::NE)){
      token = lexer.nextToken();
      switch(tokenType){
      case Token::EQ:
	node = new TreeNode(Parser::ISEQ, node, expression());
	break;
      case Token::LT:
	node = new TreeNode(Parser::ISLT, node, expression());
	break;
      case Token::LE:
	node = new TreeNode(Parser::ISLE, node, expression());
	break;
      case Token::GT:
	node = new TreeNode(Parser::ISGT, node, expression());
	break;
      case Token::GE:
	node = new TreeNode(Parser::ISGE, node, expression());
	break;
      case Token::NE:
	node = new TreeNode(Parser::ISNE, node, expression());
	break;
      }
    }
  token = lexer.nextToken();
  return node; 
}

Parser::TreeNode* Parser::logicalExpression() {
  TreeNode* node = relationalExpression();
  token = lexer.nextToken();
  int tokenType = token.getType();
  while((tokenType == Token::AND) || (tokenType == Token::OR)){
    tokenType = token.getType();
    token = lexer.nextToken();
    switch(tokenType){
    case Token::AND:
      node = new TreeNode(Parser::AND, node, relationalExpression());
      break;
    case Token::OR:
      node = new TreeNode(Parser::OR, node, relationalExpression());
      break;
    }
  }
  token = lexer.nextToken();
  return node;
}

Parser::TreeNode* Parser::assignStatement() {
  TreeNode* node = NULL;
  token = lexer.nextToken();
  int tokenType = token.getType();
  check(Token::IDENT, "Expected token of type IDENT");
  token = lexer.nextToken();
  tokenType = token.getType();
  check(Token::ASSIGN, "Expected token of type ASSIGN");
  node = logicalExpression();
  token = lexer.nextToken();
  check(Token::SEMICOLON, "Expected ;");
  token = lexer.nextToken();
  return node;
}

/*Parser::TreeNode* Parser::printfStatement() {
  }*/

Parser::TreeNode* Parser::whileStatement() {
  TreeNode* node = NULL;
  token = lexer.nextToken();
  //int tokenType = token.getType();
  check(Token::WHILE, "Expecting While");
  token = lexer.nextToken();
  check(Token::RPAREN, "Expecting (");
  string l1 = makeLabel();
  string l2 = makeLabel();
  node = new TreeNode(Parser::SEQ, new TreeNode(Parser::LABEL, l1), logicalExpression());
  node = new TreeNode(Parser::SEQ, node, new TreeNode(Parser::JUMPF, l2));
  node = new TreeNode(Parser::SEQ, node, block());
  node = new TreeNode(Parser::SEQ, node, new TreeNode(Parser::JUMP, l1));
  node = new TreeNode(Parser::SEQ, node, new TreeNode(Parser::LABEL, l2));
  token = lexer.nextToken();
  return node;
}

Parser::TreeNode* Parser::ifStatement() {
  TreeNode* node = NULL;
  token = lexer.nextToken();
  int tokenType = token.getType();
  check(Token::IF, "Expecting keyword IF");
  token = lexer.nextToken();
  tokenType = token.getType();
  check(Token::LPAREN, "Expecting (");
  string l1 = makeLabel();
  string l2 = makeLabel();
  node = new TreeNode(Parser::SEQ, logicalExpression(), new TreeNode(Parser::JUMPF, l1));
  node = new TreeNode(Parser::SEQ, node, block());
  if(token.getType() == Token::ELSE){
    node = new TreeNode(Parser::SEQ, node, new TreeNode(Parser::JUMP, l2));
    node = new TreeNode(Parser::SEQ, node, new TreeNode(Parser::LABEL, l1));
    node = new TreeNode(Parser::SEQ, node, block());
    node = new TreeNode(Parser::SEQ, node, new TreeNode(Parser::LABEL, l2));
  }else{
     node = new TreeNode(Parser::SEQ, node, new TreeNode(Parser::JUMP, l2));
  }
  return node;
}
/*Parser::TreeNode* Parser::returnStatement() {
  }*/

Parser::TreeNode* Parser::statement() {
  TreeNode* node = NULL;
  token = lexer.nextToken();
  int tokenType = token.getType();
  if(tokenType == Token::EQ){
    node = assignStatement();
  }else if(tokenType == Token::IF){
    node = ifStatement();
  }else if(tokenType == Token::WHILE){
    node = whileStatement();
  }else{
    node = new TreeNode(Parser::SEQ);
  }
  token = lexer.nextToken();
  return node;
}

//Check this method
Parser::TreeNode* Parser::block() {
  TreeNode* node = NULL;
  token = lexer.nextToken();
  int tokenType = token.getType();
  check(Token::LBRACE, "Expected {");
  token = lexer.nextToken();
  node = statement();
  if(token.getType() == Token::RBRACE){
    check(Token::RBRACE, "Expected }");
    return node = new TreeNode(Parser::SEQ);
  }else{
    while(token.getType() != Token::RBRACE){
      node = new TreeNode(Parser::SEQ, node, statement());
      tokenType = token.getType();
    }
  }
  token = lexer.nextToken();
  return node;
}

/*Parser::TreeNode* Parser::function() {
}

Parser::TreeNode* Parser::compilationunit() {
}*/
