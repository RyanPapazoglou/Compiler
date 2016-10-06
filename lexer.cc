/*
  ryan papazoglou
  rpp219
  CSE 109
  progam 4
  desc:
*/

#include "token.h"
#include <iostream>
#include "lexer.h"
#include <cstdio>
#include <cstdlib>
#include <string.h>

using namespace std;

Lexer::Lexer(istream& st):stream(st){
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
  int lineNum;
  string lex;
  
  //Arrays that hold same index for comparison of keywords
  //string words[] = {"if", "else", "while", "function", "var", "printf", "return"};
  //int possTok[] = {Token::IF, Token::ELSE, Token::WHILE, Token::FUNCTION, Token::VAR, Token::PRINTF, Token::RETURN};
  
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
      return Token(Token::PLUS, lex, lineNum, pos);
      break;
    case '-': return Token(Token::MINUS, lex, lineNum, pos);
      break;
    case '*': return Token(Token::TIMES, lex, lineNum, pos);
      break;
    case '/': return Token(Token::DIVIDE, lex, lineNum, pos);
      break;
    case '(': return Token(Token::LPAREN, lex, lineNum, pos);
      break;
    case ')': return Token(Token::RPAREN, lex, lineNum, pos);
      break;
    case '{': return Token(Token::LBRACE, lex, lineNum, pos);
      break;
    case '}': return Token(Token::RBRACE, lex, lineNum, pos);
      break;
    case ',': return Token(Token::COMMA, lex, lineNum, pos);
      break;
    case ';': return Token(Token::SEMICOLON, lex, lineNum, pos);
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
      return Token(Token::EQ, lex, lineNum, pos);
    }else{
      ch = nextChar();
      return Token(Token::ASSIGN, lex, lineNum, pos);
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
      return Token(Token::LE, lex, lineNum, pos);
    }else{
      ch = nextChar();
      return Token(Token::LT, lex, lineNum, pos);
    }
  }

  if(ch == '>'){
    pos++;
    lineNum = line;
    lex+=ch;
    ch = nextChar();
    if(ch == '='){
      lex+=ch;
      return Token(Token::GE, lex, lineNum, pos);
      //ch = nextChar(); -- Do we need?
    }else{
      ch = nextChar();
      return Token(Token::GT, lex, lineNum, pos);
    }
  }

  //Checkpoint: Need && || !=
  if(ch == '&'){
    pos++;
    lex+=ch;
    ch = nextChar();
    if(ch == '&'){
      lex+=ch;
      ch = nextChar();
      return Token(Token::AND, lex, lineNum, pos);
    }else{
      //Check this for pos errors
      //Do we need to write to cout and return an error token?
      cout << "Invalid Token: " << lex << " " << lineNum << " " << pos << endl;
      return Token(Token::ERROR, lex, pos, lineNum);
    }
  }

  if(ch == '|'){
    pos++;
    lex+=ch;
    lineNum = line;
    ch = nextChar();
    if(ch == '|'){
      lex+=ch;
      return Token(Token::OR, lex, lineNum, pos);
      //ch = nextChar();
    }else{
      cout << "Invalid Token: "<< lex << " " << lineNum << " " + pos << endl;
      return Token(Token::ERROR, lex, pos, lineNum);
    }
  }

  if(ch == '!'){
    pos++;
    lex+=ch;
    lineNum=line;
    ch=nextChar();
    if(ch == '='){
      lex+=ch;
      return Token(Token::NE, lex, lineNum, pos);
      //ch = nextChar();
    }else{
      cout << "Invalid Token: " << lex << " " << lineNum << " " + pos << endl;
      return Token(Token::ERROR, lex, pos, lineNum);
    }
  }
  
  //STRINGLIT
  if(ch == '"'){
    ch=nextChar();
    pos++;
    while(ch != '"'){
      lex += ch;
      pos++;
      lineNum = line;
      ch=nextChar();
    }
    return Token(Token::STRINGLIT, lex, lineNum, pos);
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
      pos++;
      lineNum = line;
      lex+= ch;
      ch = nextChar();
    }
    return Token(Token::INLIT, lex, lineNum, pos);
  }

  //Check if we've received the endfile char

  //if all fails, return an error
  cout << "Invalid Token: " << lex << " " << lineNum << " " << pos << endl;
  exit(1);
  return Token(Token::ERROR, lex, lineNum, pos);
  
}
