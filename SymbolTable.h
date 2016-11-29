/*
  Ryan Papazoglou
  CSE 109
  SymbolTable header
  Desc:
  Set ups the SymbolTable.cc class
*/

#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include "stack.h"
#include "stack.cc"
#include "hashTable.h"
#include "hash.h"
#include <string>
#include <stdlib.h>
#include <sstream>

using namespace std;

class SymbolTable{
public:
  // SymbolTable(int index);
  SymbolTable();
  void enterScope();
  void exitScope();
  int addSymbol(string sym);
  string getUniqueSymbol(string sym);
  Stack<HashTable> symbolTable;
  string itos(int i){
    stringstream ss;
    ss << i;
    string res = ss.str();
    return res;
  }
  
private:
  //Stack<HashTable>::Stack symbolTable;
  int index;
};

#endif
