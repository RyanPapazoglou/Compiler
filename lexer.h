/*
  ryan papazoglou
  rpp219
  CSE 109
  Program 4
  desc:
*/


//#include <iostream>
//#include <cstdlib>
//#include <stdio.h>
//#include "token.h"

#ifndef LEXER_H
#define LEXER_H

#include <string.h>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <iostream>
#include "token.h"

using namespace std;

class Lexer{
public:
  Lexer(istream& st);
  ~Lexer();
  istream& stream;
  int line;
  int pos;
  int ch;
  Token nextToken();
  
private:
 char nextChar();
  
};

#endif
