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
#include <sstream>


const string Parser::ops[] = {"ADD", "SUB", "MULT", "DIV",
			      "ISEQ", "ISNE", "ISLT", "ISLE", "ISGT", "ISGE",
			      "AND", "OR",
			      "PUSHL", "PUSHV", "STORE",
			      "JUMP", "JUMPF", "JUMPT", "CALL", "RET",
			      "PRINTF",
			      "LABEL", "SEQ",
			      "FUNC" };



Parser::Parser(Lexer& lexerx, ostream& outx): lexer(lexerx), out(outx), lindex(1), tindex(1), jindex(0) {
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

string currentFunc;
int nfmts = 0;
string fmts[100];
string fmt;
int nparams;

void Parser::geninst(TreeNode *node){
  /*if(node->leftChild != NULL){
    geninst(node->leftChild);
  }
  if(node->rightChild != NULL){
    geninst(node->rightChild);
    }*/

  if(node == NULL){
    return;
  }
  geninst(node->leftChild);
  geninst(node->rightChild);
  
  switch(node->op){
  case Parser::ADD:
    emit("  pop rbx");
    emit("  pop rax");
    emit("  add rax,rbx");
    emit("  push rax");
    break;
  case Parser::SUB:
    emit("  pop rbx");
    emit("  pop rax");
    emit("  sub rax,rbx");
    emit("  push rax");
    break;
  case Parser::AND:
    emit("  pop rbx");
    emit("  pop rax");
    emit("  and rax,rbx");
    emit("  push rax");
    break;
  case Parser::OR:
    emit("  pop rbx");
    emit("  pop rax");
    emit("  or rax,rbx");
    emit("  push rax");
    break;
  case Parser::MULT:
    emit("  pop rbx");
    emit("  pop rax");
    emit("  imul rbx");
    emit("  push rax");
    break;
  case Parser::DIV:
    emit("  mov rdx,0");
    emit("  pop rbx");
    emit("  pop rax");
    emit("  idiv rbx");
    emit("  push rax");
    break;
  case Parser::LABEL:
    emit(node->val + ":");
    break;
  case Parser::ISEQ:
    isLabelGen("je");
    break;
  case Parser::ISNE:
    isLabelGen("jne");
    break;
  case Parser::ISLT:
    isLabelGen("jl");
    break;
  case Parser::ISLE:
    isLabelGen("jle");
    break;
  case Parser::ISGT:
    isLabelGen("jg");
    break;
  case Parser::ISGE:
    isLabelGen("jge");
    break;
  case Parser::JUMP:
    emit("  jmp " + node->val);
    break;
  case Parser::JUMPF:
    emit("  pop rax");
    emit("  cmp rax,0");
    emit("  je " + node->val);
    break;
  case Parser::JUMPT:
    emit("  pop rax");
    emit("  cmp rax,0");
    emit("  jne " + node->val);
    break;
  case Parser::PUSHV:
    emit("  push qword[" + node->val + "]");
    break;
  case Parser::PUSHL:
    emit("  mov rax," + node->val);
    emit("  push rax");
    break;
  case Parser::STORE:
    emit("  pop qword[" + node->val + "]");
    break;
  case Parser::PRINTF:
    fmt = node->val;
    nparams = fmt.at(0) - '0';
    fmt = "`" + fmt.substr(1) + "`";
    fmts[nfmts++] = fmt;
    emit("  mov    rdi,fmt" + itos(nfmts));
    if (nparams == 5) {
      emit("  pop   r9");
      --nparams;
    }
    if (nparams == 4) {
      emit("  pop   r8");
      --nparams;
    }
    if (nparams == 3) {
      emit("  pop   rcx");
      --nparams;
    }
    if (nparams == 2) {
      emit("  pop   rdx");
      --nparams;
    }
    if (nparams == 1) {
      emit("  pop   rsi");
    }
    emit("  mov    rax,0");
    emit("  push   rbp");
    emit("  call   printf");
    emit("  pop    rbp");
    break;
  case Parser::CALL:
    emit("  call " + node->val);
    emit("  push rax");
    break;
  case Parser::FUNC:
    currentFunc = node->val + ":";
    emit(currentFunc);
    if (currentFunc != "main:"){
      emit("  pop r15");
    }
    break;
  case Parser::RET:
    emit("  pop rax");
    if (currentFunc != "main:"){
      emit("  push r15");
    }
    emit("  ret");
    break;
  default:
    break;
  }
}

int counter = 0;
void Parser::vardefs(TreeNode *node){
  if(node->leftChild != NULL){
    vardefs(node->leftChild);
  }
  if(node->rightChild != NULL){
    vardefs(node->rightChild);
  }
  
  switch(node->op){
  case Parser::PUSHV:
    if(checkString(node->val)){
	vars[counter] = node->val;
	counter++;
      }
    break;
  case Parser::STORE:
    if(checkString(node->val)){
	  vars[counter] = node->val;
	  counter++;
	}
    break;
  default:
    break;
  }
}

bool Parser::checkString(string str){
  for(int i = counter; i >= 0 ; i--){
    if(str == vars[i]){
      return false;
    }
  }
  return true;
}

void Parser::genasm(TreeNode *node){
  emit("global main");
  emit("extern printf");
  emit("segment .bss");
  vardefs(node);
  for(int i = 0; i < counter; i++){
    emit("  " + vars[i] + " resq 1");
  }
  emit("section .text");
  geninst(node);
  
  cout << endl << "section .data" << endl;

  for (int i=0; i < nfmts; ++i) {
    cout << "  fmt" << i+1 << ": db " << fmts[i] << ", 0" << endl;
  }
}

Parser::TreeNode* Parser::factor() {
  TreeNode* node = NULL;
  string prevtok;
  string unique;
  switch(token.getType()){
  case Token::IDENT:
    prevtok =  token.getLexeme();
    token = lexer.nextToken();
    if(token.getType() == Token::LPAREN){
      node = funcall(prevtok);
      return node;
    }else{
      unique = symbolTable->getUniqueSymbol(prevtok);
      node = new TreeNode(Parser::PUSHV, unique);
      if(symbolTable->getUniqueSymbol(prevtok) == ""){
	error("Variable not declared");
      }
      return node;
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
    token = lexer.nextToken();
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
      token = lexer.nextToken();
      node = new TreeNode(Parser::MULT, node, factor());
      break;
    case Token::DIVIDE:
      token = lexer.nextToken();
      node = new TreeNode(Parser::DIV, node, factor());
      break;
    }
    
  }
  
  return node;
}

Parser::TreeNode* Parser::expression() {
  TreeNode* node = term();
  
  while((token.getType() == Token::PLUS) || (token.getType() == Token::MINUS)){
    switch(token.getType()){
    case Token::PLUS:
      token = lexer.nextToken();
      node = new TreeNode(Parser::ADD, node, term());
      break;
    case Token::MINUS:
      token = lexer.nextToken();
      node = new TreeNode(Parser::SUB, node, term());
      break;
    }
    //token = lexer.nextToken();
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
	token = lexer.nextToken();
	node = new TreeNode(Parser::ISLT, node, expression());
	break;
      case Token::LE:
	token = lexer.nextToken();
	node = new TreeNode(Parser::ISLE, node, expression());
	break;
      case Token::GT:
	token = lexer.nextToken();
	node = new TreeNode(Parser::ISGT, node, expression());
	break;
      case Token::GE:
	token = lexer.nextToken();
	node = new TreeNode(Parser::ISGE, node, expression());
	break;
      case Token::NE:
	token = lexer.nextToken();
	node = new TreeNode(Parser::ISNE, node, expression());
	break;
    }
    //token = lexer.nextToken();
  }
 
  return node; 
}

Parser::TreeNode* Parser::logicalExpression() {
  TreeNode* node = relationalExpression();

  while((token.getType() == Token::AND) || (token.getType() == Token::OR)){
    switch(token.getType()){
    case Token::AND:
      token = lexer.nextToken();
      node = new TreeNode(Parser::AND, node, relationalExpression());
      break;
    case Token::OR:
      token = lexer.nextToken();
      node = new TreeNode(Parser::OR, node, relationalExpression());
      break;
    }
    //token = lexer.nextToken();
  }

  return node;
}

Parser::TreeNode* Parser::assignStatement() {
  TreeNode* node = NULL;
  check(Token::IDENT, "Expecting identifier");
  
  if(symbolTable->getUniqueSymbol(token.getLexeme()) == ""){
    error("Variable not declared");
  }
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
  
  check(Token::WHILE, "Expecting While");
  token = lexer.nextToken();
  check(Token::LPAREN, "Expecting (");
  string l1 = makeLabel();
  string l2 = makeLabel();
  TreeNode* lab1 = new TreeNode(Parser::LABEL, l1);
  TreeNode* lab2 = new TreeNode(Parser::LABEL, l2);
  token = lexer.nextToken();
  //node = logicalExpression();
  node = new TreeNode(Parser::SEQ, lab1, logicalExpression());
  node = new TreeNode(Parser::SEQ, node, new TreeNode(Parser::JUMPF, l2));
  token = lexer.nextToken();
  symbolTable->enterScope();
  node = new TreeNode(Parser::SEQ, node, block());
  symbolTable->exitScope();
  node = new TreeNode(Parser::SEQ, node, new TreeNode(Parser::JUMP, l1));
  node = new TreeNode(Parser::SEQ, node, lab2);
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
  token = lexer.nextToken();
  symbolTable->enterScope();
  node = new TreeNode(Parser::SEQ, node, block());
  symbolTable->exitScope();
  if(token.getType() == Token::ELSE){
    node = new TreeNode(Parser::SEQ, node, new TreeNode(Parser::JUMP, l2));
    node = new TreeNode(Parser::SEQ, node, new TreeNode(Parser::LABEL, l1));
    token = lexer.nextToken();
    symbolTable->enterScope();
    node = new TreeNode(Parser::SEQ, node, block());
    symbolTable->exitScope();
    node = new TreeNode(Parser::SEQ, node, new TreeNode(Parser::LABEL, l2));
  }else{
     node = new TreeNode(Parser::SEQ, node, new TreeNode(Parser::LABEL, l1));
  }
  //token = lexer.nextToken();
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
    break;
  case Token::PRINTF:
    node = printfStatement();
    break;
  default:
    error("Not a statement type");
    break;
  }
  return node;
}


Parser::TreeNode* Parser::printfStatement() {
  TreeNode* paramList = NULL;
  int nparams = 0;
  check(Token::PRINTF, "Expecting printf");
  token = lexer.nextToken();
  check(Token::LPAREN, "Expecting (");
  token = lexer.nextToken();
  check(Token::STRINGLIT, "Expecting string literal");
  string formatString = token.getLexeme();
  token = lexer.nextToken();
  if (token.getType() == Token::COMMA) {
    token = lexer.nextToken();
    paramList = expression();
    ++nparams;
    while (token.getType() == Token::COMMA) {
      token = lexer.nextToken();
      paramList = new TreeNode(SEQ, paramList, expression());
      ++nparams;
    }
  }
  check(Token::RPAREN, "Expecting )");
  token = lexer.nextToken();
  check(Token::SEMICOLON, "Expecting ;");
  token = lexer.nextToken();
  TreeNode* printStatement =
    new TreeNode(SEQ, paramList, new TreeNode(PRINTF, itos(nparams) + formatString));
  return printStatement;
}

Parser::TreeNode* Parser::block() {
  TreeNode* node = NULL;
  check(Token::LBRACE, "Expected {");
  token = lexer.nextToken();
  node = statement();
  if(token.getType() == Token::ENDOFFILE){
    return node;
  }else{
    while(token.getType() != Token::RBRACE){
      node = new TreeNode(Parser::SEQ, node, statement());
    }
    check(Token::RBRACE, "Expecting }");
    token = lexer.nextToken();
  }
  
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
    token = lexer.nextToken();
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
  //genasm(node);
  return node;
}

Parser::TreeNode* Parser::funcall(string functionName){
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
  symbolTable->addSymbol(param);
  param = symbolTable->getUniqueSymbol(token.getLexeme());
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
  check(Token::RPAREN, "Expecting )");
  token = lexer.nextToken();
  return params;
}

void Parser::isLabelGen(string op){
  string jump1 = makeJumpLabel();
  emit("  pop rbx");
  emit("  pop rax");
  emit("  cmp rax,rbx");
  emit("  " + op + " " + jump1);
  emit("  mov rax,0");
  string jump2 = makeJumpLabel();
  emit("  jmp " + jump2);
  emit(jump1 + ":");
  emit("  mov rax,1");
  emit(jump2 + ":");
  emit("  push rax");
  labelCounter++;
}
