/*
  ryan papazoglou
  rpp219
  CSE 109
  progam 4
  desc:
*/

#include <iosfwd>
#include "token.h"
#include <istream>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "lexer.h"

using namespace std;

Lexer::Lexer(istream& st){
  stream = st;
  line = 1;
  pos = 1;
  ch = nextChar();
}

Lexer::~Lexer(){
  
}

char Lexer::nextChar(){
  //Lexer* lex = new Lexer(stream);
  char nextChar = stream.get();
    if(nextChar == '\n'){
      line++;
      pos = 0;
      return ' ';
    }else if(nextChar == EOF){
      return '#';
    }else{
      //pos++; -- don't want here b/c we're going to use nextChar() in conditionals
      return nextChar;
    }
}

Token Lexer::nextToken(){
  //Token tok;
  Token* tok;
  int lineNum;
  string lex;
  
  //Arrays that hold same index for comparison of keywords
  string words[] = {"if", "else", "while", "function", "var", "printf", "return"};
  int possTok[] = {Token::IF, Token::ELSE, Token::WHILE, Token::FUNCTION, Token::VAR, Token::PRINTF, Token::RETURN};
  
  while(ch == ' '){
    ch = nextChar();
    pos++;
  }  
  
  //OPERATORS
  if(ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '(' || ch == ')' || ch == '{' || ch == '}' || ch == ',' || ch == ';'){
    pos++;
    lineNum = line;
    lex += ch;
    switch(ch){
    case '+':
      //Do this for rest of cases
      ch = nextChar();
      return new Token(Token::PLUS, lex, lineNum, pos);
      break;
    case '-': return new Token(Token::MINUS, lex, lineNum, pos);
      break;
    case '*': return new Token(Token::TIMES, lex, lineNum, pos);
      break;
    case '/': return new Token(Token::DIVIDE, lex, lineNum, pos);
      break;
    case '(': return new Token(Token::LPAREN, lex, lineNum, pos);
      break;
    case ')': return new Token(Token::RPAREN, lex, lineNum, pos);
      break;
    case '{': return new Token(Token::LBRACE, lex, lineNum, pos);
      break;
    case '}': return new Token(Token::RBRACE, lex, lineNum, pos);
      break;
    case ',': return new Token(Token::COMMA, lex, lineNum, pos);
      break;
    case ';': return new Token(Token::SEMICOLON, lex, lineNum, pos);
      break;
    }
  }

  if(ch == '='){
    pos++;
    lineNum = line;
    lex+=ch;
    ch = nextChar();
    if(ch == '='){
      lex+=ch;
      tok = new Token(Token::EQ, lex, lineNum, pos);
      return tok;
    }else{
      tok = new Token(Token::ASSIGN, lex, lineNum, pos);
      ch = nextChar();
      return tok;
    }
    //ch = nextChar();
  }

  if(ch == '<'){
    pos++;
    lineNum = line;
    lex+=ch;
    ch = nextChar();
    if(ch == '='){
      lex+=ch;
      tok = new Token(Token::LE, lex, lineNum, pos);
      return tok;
    }else{
      tok = new Token(Token::LT, lex, lineNum, pos);
      ch = nextChar();
      return tok;
    }
  }

  if(ch == '>'){
    pos++;
    lineNum = line;
    lex+=ch;
    ch = nextChar();
    if(ch == '='){
      lex+=ch;
      tok = new Token(Token::GE, lex, lineNum, pos);
      //ch = nextChar(); -- Do we need?
      return tok;
    }else{
      tok = new Token(Token::GT, lex, lineNum, pos);
      ch = nextChar();
      return tok;
    }
  }

  //Checkpoint: Need && || !=
  if(ch == '&'){
    pos++;
    lex+=ch;
    ch = nextChar();
    if(ch == '&'){
      lex+=ch;
      tok = new Token(Token::AND, lex, lineNum, pos);
      ch == nextChar();
      return tok;
    }else{
      //Check this for pos errors
      //Do we need to write to cout and return an error token?
      cout << "Invalid Token: " << lex << " " << lineNum << " " << pos << endl;
      return new Token(Token::ERROR, lex, pos, lineNum);
    }
  }

  if(ch == '|'){
    pos++;
    lex+=ch;
    lineNum = line;
    ch = nextChar();
    if(ch == '|'){
      lex+=ch;
      tok = new Token(Token::OR, lex, lineNum, pos);
      //ch = nextChar();
      return tok;
    }else{
      cout << "Invalid Token: "<< lex << " " << lineNum << " " + pos << endl;
      return new Token(Token::ERROR, lex, pos, lineNum);
    }
  }

  if(ch == '!'){
    pos++;
    lex+=ch;
    lineNum=line;
    ch=nextChar();
    if(ch == '='){
      lex+=ch;
      tok = new Token(Token::NE, lex, lineNum, pos);
      //ch = nextChar();
      return tok;
    }else{
      cout << "Invalid Token: " << lex << " " << lineNum << " " + pos << endl;
      return new Token(Token::ERROR, lex, pos, lineNum);
    }
  }
  
  //STRINGLIT
  if(ch = '"'){
    ch=nextChar();
    pos++;
    while(ch != '"'){
      lex += ch;
      pos++;
      lineNum = line;
      ch=nextChar();
    }
    tok = new Token(Token::STRINGLIT, lex, lineNum, pos);
    return tok;
  } 

  //IDENT
  if((ch>='a' && ch<='z') || (ch >= 'A' || ch <= 'Z')){   
    while((ch>='a' && ch<='z') || (ch >= 'A' || ch <= 'Z') || (ch >= 0 && ch <= 9)){
      lex+=ch;
      pos++;
      lineNum=line;
      ch=nextChar();
    }
    //KEYWORDS
    if(lex == "if"){
      tok = new Token(Token::IF, lex, lineNum, pos);
      return tok;
    }

    if(lex == "else"){
      tok = new Token(Token::ELSE, lex, lineNum, pos);
      return tok;
    }

    if(lex == "while"){
      tok = new Token(Token::WHILE, lex, lineNum, pos);
      return tok;
    }

    if(lex == "function"){
      tok = new Token(Token::FUNCTION, lex, lineNum, pos);
      return tok;
    }

    if(lex == "var"){
      tok = new Token(Token::VAR, lex, lineNum, pos);
      return tok;
    }

    if(lex == "printf"){
      tok = new Token(Token::PRINTF, lex, lineNum, pos);
      return tok;
    }

    if(lex == "return"){
      tok = new Token(Token::RETURN, lex, lineNum, pos);
      return tok;
    }
    
    tok = new Token(Token::IDENT, lex, lineNum, pos);
    return tok;
  }


  //INLIT
  if(ch >= 0 && ch <= 9){
    while(ch >= 0 && ch <= 9){
      pos++;
      lineNum = line;
      lex+= ch;
      ch = nextChar();
    }
    tok = new Token(Token::INLIT, lex, lineNum, pos);
    return tok;
  }

  //Check if we've received the endfile char

  //if all fails, return an error
  cout << "Invalid Token: " << lex << " " << lineNum << " " << pos << endl;
  exit(1);
  return new Token(Token::ERROR, lex, lineNum, pos);
  
}
