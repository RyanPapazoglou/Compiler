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
  //int curPos = 1;
  pos++;
  if(next == '\n'){
    line++;
    pos = 1;
    return ' ';
  }else if(stream.eof()){
      return '#';
  }else{
    //curPos++;
    
    return next;
  }
}

Token Lexer::nextToken(){
  int lineNum;
  string lex;
  //ch = nextChar();
  while((ch == ' ') || (ch == '\t')){
    ch = nextChar();
  }
    
  
  //OPERATORS
  if(ispunct(ch)){
    lineNum = line;
    if(ch != '"'){
      lex += ch;
    }

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
      //lex += ch;
      ch = nextChar();
      if(ch != '='){
	//lex+=ch;
	return Token(Token::ASSIGN, lex, lineNum, pos);
	break;
      }else{	
	lex+=ch;
	ch = nextChar();
	return Token(Token::EQ, lex, lineNum, pos);
	break;
      }
      break;
    case '>':
      //lex += ch;
      ch = nextChar();
      if(ch != '='){
	//lex+=ch;
	return Token(Token::GT, lex, lineNum, pos);
	break;
      }else{
	lex+=ch;
	ch = nextChar();
	return Token(Token::GE, lex, lineNum, pos);
	break;
      }
      break;
    case '<':
      //lex += ch;
      ch = nextChar();
      if(ch != '='){
	//lex+=ch;
	return Token(Token::LT, lex, lineNum, pos);
	break;
      }else{
	lex+=ch;
	ch = nextChar();
	return Token(Token::LE, lex, lineNum, pos);
	break;
      }
      break;
    case '&':
      //lex += ch;
      ch = nextChar();
      if(ch != '&'){
	//lex+=ch;
	//return Token(Token::ERROR, lex, lineNum, pos);
	break;
      }else{
	lex+=ch;
	ch = nextChar();
	//cout << "Invalid Token: " << lex << " " << lineNum << " " << pos << endl;
	return Token(Token::AND, lex, lineNum, pos);
	break;
      }
      break;
    case '|':
      //lex += ch;
      ch = nextChar();
      if(ch != '|'){
	//lex+=ch;
	//return Token(Token::OR, lex, lineNum, pos);
	break;
      }else{
	lex+=ch;
	ch = nextChar();
	//cout << "Invalid Token: "<< lex << " " << lineNum << " " + pos << endl;
	return Token(Token::OR, lex, lineNum, pos);
	break;
      }
      break;
    case '!':
      //lex += ch;
      ch=nextChar();
      if(ch != '='){
	//lex+=ch;
	//return Token(Token::NE, lex, lineNum, pos);
	break;
      }else{
	lex+=ch;
	ch = nextChar();
	//cout << "Invalid Token: " << lex << " " << lineNum << " " + pos << endl;
	return Token(Token::NE, lex, lineNum, pos);
      }
      break;
    case '"':
      ch=nextChar();
      while(ch != '"'){
	lex += ch;
	lineNum = line;
	ch=nextChar();
      }
      //lex += ch;
      ch = nextChar();
      return Token(Token::STRINGLIT, lex, lineNum, pos);
      break;
    }
    //ch = nextChar();
    /*switch(ch){
    case '+':
      //lex += ch;
      ch = nextChar();
      return Token(Token::PLUS, lex, lineNum, pos);
      break;
    case '-':
      //lex += ch;
      ch = nextChar();
      return Token(Token::MINUS, lex, lineNum, pos);
      break;
    case '*':
      //lex += ch;
      ch = nextChar();
      return Token(Token::TIMES, lex, lineNum, pos);
      break;
    case '/':
      //lex += ch;
      ch = nextChar();
      return Token(Token::DIVIDE, lex, lineNum, pos);
      break;
    case '(':
      //lex += ch;
      ch = nextChar();
      return Token(Token::LPAREN, lex, lineNum, pos);
      break;
    case ')':
      //lex += ch;
      ch = nextChar();
      return Token(Token::RPAREN, lex, lineNum, pos);
      break;
    case '{':
      //lex += ch;
      ch = nextChar();
      return Token(Token::LBRACE, lex, lineNum, pos);
      break;
    case '}':
      //lex += ch;
      ch = nextChar();
      return Token(Token::RBRACE, lex, lineNum, pos);
      break;
    case ',':
      //lex += ch;
      ch = nextChar();
      return Token(Token::COMMA, lex, lineNum, pos);
      break;
    case ';':
      //lex += ch;
      ch = nextChar();
      return Token(Token::SEMICOLON, lex, lineNum, pos);
      break;
    case '=':
      //lex += ch;
      ch = nextChar();
      if(ch != '='){
	//lex+=ch;
	return Token(Token::ASSIGN, lex, lineNum, pos);
	break;
      }else{	
	lex+=ch;
	ch = nextChar();
	return Token(Token::EQ, lex, lineNum, pos);
	break;
      }
      break;
    case '>':
      //lex += ch;
      ch = nextChar();
      if(ch != '='){
	//lex+=ch;
	return Token(Token::GT, lex, lineNum, pos);
	break;
      }else{
	lex+=ch;
	ch = nextChar();
	return Token(Token::GE, lex, lineNum, pos);
	break;
      }
      break;
    case '<':
      //lex += ch;
      ch = nextChar();
      if(ch != '='){
	//lex+=ch;
	return Token(Token::LT, lex, lineNum, pos);
	break;
      }else{
	lex+=ch;
	ch = nextChar();
	return Token(Token::LE, lex, lineNum, pos);
	break;
      }
      break;
    case '&':
      //lex += ch;
      ch = nextChar();
      if(ch != '&'){
	//lex+=ch;
	//return Token(Token::ERROR, lex, lineNum, pos);
	break;
      }else{
	lex+=ch;
	ch = nextChar();
	//cout << "Invalid Token: " << lex << " " << lineNum << " " << pos << endl;
	return Token(Token::AND, lex, lineNum, pos);
	break;
      }
      break;
    case '|':
      //lex += ch;
      ch = nextChar();
      if(ch != '|'){
	//lex+=ch;
	//return Token(Token::OR, lex, lineNum, pos);
	break;
      }else{
	lex+=ch;
	ch = nextChar();
	//cout << "Invalid Token: "<< lex << " " << lineNum << " " + pos << endl;
	return Token(Token::OR, lex, lineNum, pos);
	break;
      }
      break;
    case '!':
      //lex += ch;
      ch=nextChar();
      if(ch != '='){
	//lex+=ch;
	//return Token(Token::NE, lex, lineNum, pos);
	break;
      }else{
	lex+=ch;
	ch = nextChar();
	//cout << "Invalid Token: " << lex << " " << lineNum << " " + pos << endl;
	return Token(Token::NE, lex, lineNum, pos);
      }
      break;
    case '"':
      ch=nextChar();
      while(ch != '"'){
	lex += ch;
	lineNum = line;
	ch=nextChar();
      }
      //lex += ch;
      ch = nextChar();
      return Token(Token::STRINGLIT, lex, lineNum, pos);
      break;
      }*/
  }

  if(ch == '#'){
    //ch = nextChar();
    return Token(Token::ENDOFFILE, lex, lineNum, pos);
  }

  //IDENT
  if((ch>='a' && ch<='z') || (ch >= 'A' && ch <= 'Z')){
    while(isalpha(ch) || isdigit(ch)){
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
    // ch = nextChar();
    return Token(Token::IDENT, lex, lineNum, pos);
  }


  //INTLIT
  if(ch >= '0' && ch <= '9'){
    while(ch >= '0' && ch <= '9'){
      lineNum = line;
      lex+= ch;
      ch = nextChar();
    }
    //ch = nextChar();
    return Token(Token::INTLIT, lex, lineNum, pos);
  }
  ch = nextChar();
  return Token(Token::ERROR, lex, lineNum, pos);
}
