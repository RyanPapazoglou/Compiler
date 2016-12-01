/*
  Ryan Papazoglou
  rpp219
  CSE109
  Program 5
  Desc: Parser class to build the tree for assembly use
*/


#include "parser.h"
#include "SymbolTable.h"
#include <cstring>
#include <string>
#include <iostream>


const string Parser::ops[] = {"ADD", "SUB", "MULT", "DIV",
			      "ISEQ", "ISNE", "ISLT", "ISLE", "ISGT", "ISGE",
			      "AND", "OR",
			      "PUSHL", "PUSHV", "STORE",
			      "JUMP", "JUMPF", "JUMPT", "CALL", "RET",
			      "PRINTF",
			      "LABEL", "SEQ",
			      "FUNC" };



Parser::Parser(Lexer& lexerx, ostream& outx): lexer(lexerx), out(outx), lindex(1), tindex(1) {
  token = lexer.nextToken();
  symbolTable = new SymbolTable();
}

Parser::~Parser() {
}

void Parser::error(string message) {
  cerr << message << " found " << token.getLexeme() << " at line " << token.getLine() << " position " << token.getPos() << endl;
  exit(1);
}

void Parser::check(int tokenType, string message) {
  if (token.getType() != tokenType)
    error(message);
}

void Parser::emit(string asmb){
  cout << asmb << endl;
}

void Parser::geninst(TreeNode *node){

}

void Parser::genasm(TreeNode *node){
  emit("  global main");
  emit("  extern printf");
  emit("  segment .bss");
}

Parser::TreeNode* Parser::factor() {
  TreeNode* node = NULL;
  string sym = "";
  switch(token.getType()){
  case Token::IDENT:
    sym = token.getLexeme();
    if(symbolTable->getUniqueSymbol(sym) == ""){
      error("Var already exists");
    }
    sym = symbolTable->getUniqueSymbol(sym);
    token = lexer.nextToken();
    if(token.getType() == Token::LPAREN){
      node = funcall(sym);
    }else if(token.getType() == Token::ASSIGN){
      node = statement();
    }else{
      node = new TreeNode(Parser::PUSHV, sym);
    }
    return node;
    break;
  case Token::INTLIT:
    node = new TreeNode(Parser::PUSHL, token.getLexeme());
    token = lexer.nextToken();
    return node;
    break;
  case Token::LPAREN:
    token = lexer.nextToken();
    node = expression();
    check(Token::RPAREN, "Expecting )");
    return node;
    break;
  default:
    token = lexer.nextToken();
    error("Not a factor token");
    break;
  }
  
  return node;
}

Parser::TreeNode* Parser::term() {
  TreeNode* node = factor();
  
  while((token.getType() == Token::TIMES) || (token.getType() == Token::DIVIDE)){
    switch(token.getType()){
    case Token::TIMES:
      node = new TreeNode(Parser::MULT, node, factor());
      break;
    case Token::DIVIDE:
      node = new TreeNode(Parser::DIV, node, factor());
      break;
    }
    token = lexer.nextToken();
  }
  
  return node;
}

Parser::TreeNode* Parser::expression() {
  TreeNode* node = term();
  
  while((token.getType() == Token::PLUS) || (token.getType() == Token::MINUS)){
    switch(token.getType()){
    case Token::PLUS:
      node = new TreeNode(Parser::ADD, node, term());
      break;
    case Token::MINUS:
      node = new TreeNode(Parser::SUB, node, term());
      break;
    }
    token = lexer.nextToken();
  }
  return node;
}

Parser::TreeNode* Parser::relationalExpression() {
  TreeNode* node = expression();
  
  if((token.getType() == Token::EQ) || (token.getType() == Token::LT) || (token.getType() == Token::LE) || (token.getType() == Token::GT) || (token.getType() == Token::GE) || (token.getType() == Token::NE)){
    switch(token.getType()){
      case Token::EQ:
	token = lexer.nextToken();
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
    token = lexer.nextToken();
  }
 
  return node; 
}

Parser::TreeNode* Parser::logicalExpression() {
  TreeNode* node = relationalExpression();

  while((token.getType() == Token::AND) || (token.getType() == Token::OR)){
    switch(token.getType()){
    case Token::AND:
      node = new TreeNode(Parser::AND, node, relationalExpression());
      break;
    case Token::OR:
      node = new TreeNode(Parser::OR, node, relationalExpression());
      break;
    }
    token = lexer.nextToken();
  }

  return node;
}

Parser::TreeNode* Parser::assignStatement() {
  TreeNode* node = NULL;
  check(Token::IDENT, "Expecting identifier");
  token = lexer.nextToken();
  check(Token::ASSIGN, "Expected token of type ASSIGN");
  token = lexer.nextToken();
  node = logicalExpression();
  check(Token::SEMICOLON, "Expected ;");
  token = lexer.nextToken();
  return node;
}

Parser::TreeNode* Parser::whileStatement() {
  TreeNode* node = NULL;
  token = lexer.nextToken();
  check(Token::WHILE, "Expecting While");
  token = lexer.nextToken();
  check(Token::RPAREN, "Expecting (");
  string l1 = makeLabel();
  string l2 = makeLabel();
  node = new TreeNode(Parser::SEQ, new TreeNode(Parser::LABEL, l1), logicalExpression());
  node = new TreeNode(Parser::SEQ, node, new TreeNode(Parser::JUMPF, l2));
  symbolTable->enterScope();
  node = new TreeNode(Parser::SEQ, node, block());
  symbolTable->exitScope();
  node = new TreeNode(Parser::SEQ, node, new TreeNode(Parser::JUMP, l1));
  node = new TreeNode(Parser::SEQ, node, new TreeNode(Parser::LABEL, l2));
  token = lexer.nextToken();
  return node;
}

Parser::TreeNode* Parser::ifStatement() {
  TreeNode* node = NULL;
  check(Token::IF, "Expecting keyword IF");
  token = lexer.nextToken();
  check(Token::LPAREN, "Expecting (");
  string l1 = makeLabel();
  string l2 = makeLabel();
  token = lexer.nextToken();
  node = new TreeNode(Parser::SEQ, logicalExpression(), new TreeNode(Parser::JUMPF, l1));
  symbolTable->enterScope();
  node = new TreeNode(Parser::SEQ, node, block());
  symbolTable->exitScope();
  if(token.getType() == Token::ELSE){
    node = new TreeNode(Parser::SEQ, node, new TreeNode(Parser::JUMP, l2));
    node = new TreeNode(Parser::SEQ, node, new TreeNode(Parser::LABEL, l1));
    symbolTable->enterScope();
    node = new TreeNode(Parser::SEQ, node, block());
    symbolTable->exitScope();
    node = new TreeNode(Parser::SEQ, node, new TreeNode(Parser::LABEL, l2));
  }else{
     node = new TreeNode(Parser::SEQ, node, new TreeNode(Parser::JUMP, l2));
  }
  token = lexer.nextToken();
  return node;
}

Parser::TreeNode* Parser::returnStatement() {
  TreeNode* retNode = new TreeNode(Parser::RET);
  check(Token::RETURN, "invalid return statement");
  token = lexer.nextToken();
  TreeNode* node = logicalExpression();
  check(Token::SEMICOLON, "Missing ;");
  node = new TreeNode(Parser::SEQ, node, retNode);
  token = lexer.nextToken();
  return node;
}

Parser::TreeNode* Parser::statement() {
  TreeNode* node = NULL;
  
  switch(token.getType()){
  case Token::IDENT:
    node = assignStatement();
    break;
  case Token::IF:
    node = ifStatement();
    break;
  case Token::WHILE:
    node = whileStatement();
    break;
  case Token::VAR:
    node = vardefStatement();
    break;
  case Token::RETURN:
    node = returnStatement();
  default:
    error("Not a statement type");
    break;
  }
  return node;
}

Parser::TreeNode* Parser::block() {
  TreeNode* node = NULL;
  check(Token::LBRACE, "Expected {");
  token = lexer.nextToken();
  node = statement();
  if(token.getType() == Token::ENDOFFILE){
    return node;
    /*}else if(token.getType() == Token::RBRACE){
    check(Token::RBRACE, "Expected }");
    return node = new TreeNode(Parser::SEQ);*/
  }else{
    while(token.getType() != Token::RBRACE){
      node = new TreeNode(Parser::SEQ, node, statement());
    }
  }
  check(Token::RBRACE, "Expecting }");
  token = lexer.nextToken();
  return node;
}

Parser::TreeNode* Parser::function() {
  check(Token::FUNCTION, "Missing function keyword");
  token = lexer.nextToken();
  check(Token::IDENT, "Missing function name");
  string funcname = token.getLexeme();
  TreeNode* funcNode = new TreeNode(Parser::FUNC, funcname);
  token = lexer.nextToken();
  check(Token::LPAREN, "Missing (");
  token = lexer.nextToken();
  if(token.getType() == Token::RPAREN){
    symbolTable->enterScope();
    funcNode = new TreeNode(Parser::SEQ, funcNode, block());
    symbolTable->exitScope();
  }else{
    //Read in parameters
    funcNode = new TreeNode(Parser::SEQ, funcNode, parameterdefs());
    funcNode = new TreeNode(Parser::SEQ, funcNode, block());
  }
  
  return funcNode;
}

Parser::TreeNode* Parser::compilationUnit() {
  if(token.getType() != Token::FUNCTION){
    return new TreeNode(Parser::SEQ);
  }
  TreeNode* node = function();
  while(token.getType() != Token::ENDOFFILE){
    //check(Token::FUNCTION, "Invalid function declaration");
    node = new TreeNode(Parser::SEQ, node, function());
  }
  return node;
}

Parser::TreeNode* Parser::funcall(string functionName){
  //curToken ='('
  TreeNode* node = new TreeNode(Parser::CALL, functionName);
  token = lexer.nextToken();
  while(token.getType() != Token::RPAREN){
    if(token.getType() == Token::COMMA){
      token = lexer.nextToken();
    }else{
      node = new TreeNode(Parser::SEQ, node, expression());
    }
  }
  token = lexer.nextToken();
  return node;
}

Parser::TreeNode* Parser::vardefStatement() {
  TreeNode* node = new TreeNode(Parser::SEQ);
  check(Token::VAR, "Expected keyword var");
  token = lexer.nextToken();
  string sym = token.getLexeme();
  symbolTable->addSymbol(sym);
  token = lexer.nextToken();
  while(token.getType() == Token::COMMA){
    token = lexer.nextToken();
    sym = token.getLexeme();
    symbolTable->addSymbol(sym);
    token = lexer.nextToken();
  }
  check(Token::SEMICOLON, "Missing ;");
  token = lexer.nextToken();
  return node;
}

Parser::TreeNode* Parser::parameterdef(){
  check(Token::IDENT, "Invalid parameter");
  string param = token.getLexeme();
  TreeNode* paramNode = new TreeNode(Parser::STORE, param);
  token = lexer.nextToken();
  return paramNode;
}

Parser::TreeNode* Parser::parameterdefs(){
  TreeNode* params = parameterdef();
  if(token.getType() == Token::COMMA){
    while(token.getType() == Token::COMMA){
    token = lexer.nextToken();
    params = new TreeNode(Parser::SEQ, params, parameterdef());
    }
    token = lexer.nextToken();
  }
  return params;
}
