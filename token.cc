/*
  ryan papazoglou
  rpp219
  CSE 109
  Program 4
  Desc:
*/

#include <cstdlib>
#include "token.h"
#include <string>

using namespace std;

Token::Token(int ty, string lex, int curLine, int curPos){
  type = ty;
  lexeme = lex;
  line = curLine;
  pos = curPos;
}

Token::Token(){
  type = 0;
  lexeme = "";
  line = 1;
  pos = 1;
}

int Token::getType() {
  return type;
}

string Token::getLexeme(){
  return lexeme;
}

int Token::getLine(){
  return line;
}

int Token::getPos(){
  return pos;
}

Token::~Token(){

}
