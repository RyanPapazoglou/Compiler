/*
  ryan papazoglou
  rpp219
  CSE 109
  progam 4
  desc: lexical analyzer
*/

#include "token.h"
#include <iostream>
#include "lexer.h"
#include <cstdio>
#include <cstdlib>
#include <string.h>
#include <cctype>

using namespace std;

Lexer::Lexer(istream& st):stream(st){
  line = 1;
  pos = 1;
  ch = nextChar();
}

Lexer::~Lexer(){
}

char Lexer::nextChar(){
  char next = stream.get();
    if(next == '\n'){
      line++;
      pos = 1;
      return ' ';
    }else if(next == EOF){
      return '#';
    }else if(next == ' '){
      pos++;
      return '*';
    }else{
      pos++;
      return next;
    }
}

Token Lexer::nextToken(){
  int lineNum;
  string lex;

  if(ch == '*'){
    ch = nextChar();
  }
  
  //OPERATORS
  if(ispunct(ch)){
    lineNum = line;
    lex += ch;
    switch(ch){
    case '+':
      ch = nextChar();
      return Token(Token::PLUS, lex, lineNum, pos);
      break;
    case '-':
      ch = nextChar();
      return Token(Token::MINUS, lex, lineNum, pos);
      break;
    case '*':
      ch = nextChar();
      return Token(Token::TIMES, lex, lineNum, pos);
      break;
    case '/':
      ch = nextChar();
      return Token(Token::DIVIDE, lex, lineNum, pos);
      break;
    case '(':
      ch = nextChar();
      return Token(Token::LPAREN, lex, lineNum, pos);
      break;
    case ')':
      ch = nextChar();
      return Token(Token::RPAREN, lex, lineNum, pos);
      break;
    case '{':
      ch = nextChar();
      return Token(Token::LBRACE, lex, lineNum, pos);
      break;
    case '}':
      ch = nextChar();
      return Token(Token::RBRACE, lex, lineNum, pos);
      break;
    case ',':
      ch = nextChar();
      return Token(Token::COMMA, lex, lineNum, pos);
      break;
    case ';':
      ch = nextChar();
      return Token(Token::SEMICOLON, lex, lineNum, pos);
      break;
    case '=':
      ch = nextChar();
      if(ch == '='){
	lex+=ch;
	return Token(Token::EQ, lex, lineNum, pos);
      }else{
	return Token(Token::ASSIGN, lex, lineNum, pos);
      }
      break;
    case '>':
      ch = nextChar();
      if(ch == '='){
	lex+=ch;
	return Token(Token::LE, lex, lineNum, pos);
      }else{
	return Token(Token::LT, lex, lineNum, pos);
      }
      break;
    case '<':
      ch = nextChar();
      if(ch == '='){
	lex+=ch;
	return Token(Token::GE, lex, lineNum, pos);
      }else{
	return Token(Token::GT, lex, lineNum, pos);
      }
      break;
    case '&':
      ch = nextChar();
      if(ch == '&'){
	lex+=ch;
	return Token(Token::AND, lex, lineNum, pos);
      }else{
	ch = nextChar();
	cout << "Invalid Token: " << lex << " " << lineNum << " " << pos << endl;
      }
      break;
    case '|':
      ch = nextChar();
      if(ch == '|'){
	lex+=ch;
	return Token(Token::OR, lex, lineNum, pos);
      }else{
	ch = nextChar();
	cout << "Invalid Token: "<< lex << " " << lineNum << " " + pos << endl;
      }
      break;
    case '!':
      ch=nextChar();
      if(ch == '='){
	lex+=ch;
	return Token(Token::NE, lex, lineNum, pos);
      }else{
	ch = nextChar();
	cout << "Invalid Token: " << lex << " " << lineNum << " " + pos << endl;
      }
      break;
    case '"':
      ch=nextChar();
      while(ch != '"'){
	lex += ch;
	lineNum = line;
	ch=nextChar();
      }
      ch = nextChar();
      return Token(Token::STRINGLIT, lex, lineNum, pos);
      break;
    }
  }

  if(ch == '#'){
    ch = nextChar();
    return Token(Token::ENDOFFILE, lex, lineNum, pos);
  }

  //IDENT
  if((ch>='a' && ch<='z') || (ch >= 'A' && ch <= 'Z')){
    while(isalpha(ch)){
      lex+=ch;
      lineNum=line;
      ch=nextChar();
    }
    //KEYWORDS
    if(lex == "if"){
      return Token(Token::IF, lex, lineNum, pos);
    }

    if(lex == "else"){
      return Token(Token::ELSE, lex, lineNum, pos);
    }

    if(lex == "while"){
      return Token(Token::WHILE, lex, lineNum, pos);
    }

    if(lex == "function"){
      return Token(Token::FUNCTION, lex, lineNum, pos);
    }

    if(lex == "var"){
      return Token(Token::VAR, lex, lineNum, pos);
    }

    if(lex == "printf"){
      return Token(Token::PRINTF, lex, lineNum, pos);
    }

    if(lex == "return"){
      return Token(Token::RETURN, lex, lineNum, pos);
    }
    
    return Token(Token::IDENT, lex, lineNum, pos);
  }


  //INLIT
  if(ch >= 0 && ch <= 9){
    while(ch >= 0 && ch <= 9){
      lineNum = line;
      lex+= ch;
      ch = nextChar();
      }
    return Token(Token::INLIT, lex, lineNum, pos);
  }
  ch = nextChar();
  return Token(Token::ERROR, lex, lineNum, pos);
}
