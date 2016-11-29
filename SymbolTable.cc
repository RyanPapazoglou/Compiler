/*
  Ryan Papazoglou
  CSE 109
  SymbolTable class
  Desc:
  Class to hold the unique symbols for each of the scopes within the program
*/


#include "SymbolTable.h"
#include "stack.h"
#include "hashTable.h"
#include "hash.h"
#include "parser.h"


SymbolTable::SymbolTable(){
  index = 0;
  symbolTable = Stack<HashTable>();
}

void SymbolTable::enterScope(){
  symbolTable.push(HashTable(59));
}

void SymbolTable::exitScope(){
  symbolTable.pop();
}

int SymbolTable::addSymbol(string sym){
  HashTable& topScope = symbolTable.peek();
  if(topScope.inTable(sym)){
      return 0;
  }
  symbolTable.peek().add(sym, (sym + "$" + itos(++index)));
  return 1;
}


string SymbolTable::getUniqueSymbol(string sym){
  for(int i = symbolTable.tos; i >= 0; --i){
    HashTable& curScope = symbolTable[i];
    if(curScope.inTable(sym)){
      return curScope[sym];
    }
  }
  return "";
}

string itos(int i){
  stringstream ss;
  ss << i;
  string res = ss.str();
  return res;
}
