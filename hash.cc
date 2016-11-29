#include "hash.h"

Hash::Hash(string str) {
  int tmp = 0;
  const char *p = str.c_str();
  while(*p) {
    tmp += *p++;  //  (*p)++  *(p++)
  }
  key = abs(tmp);
}

int Hash::hash() const{
  return key;
}

