OPTS = -g -c -Wall -Werror

parsetest: parser.o lexer.o token.o microc.o hash.o stack.o hashTable.o SymbolTable.o link.o
	g++ -o parsetest lexer.o token.o parser.o microc.o hash.o stack.o hashTable.o SymbolTable.o link.o

lexer.o: lexer.h lexer.cc token.h
	g++ $(OPTS) lexer.cc

token.o: token.h token.cc
	g++ $(OPTS) token.cc

parser.o: parser.cc parser.h token.h lexer.h SymbolTable.h
	g++ $(OPTS) parser.cc

microc.o: microc.cc token.cc lexer.cc parser.cc
	g++ $(OPTS) microc.cc

hashTable.o: hashTable.cc hashTable.h hash.h link.h
	g++ $(OPTS) hashTable.cc

hash.o: hash.cc hash.h
	g++ $(OPTS) hash.cc

stack.o: stack.cc stack.h
	g++ $(OPTS) stack.cc

SymbolTable.o: SymbolTable.cc SymbolTable.h hashTable.h hash.h stack.h
	g++ $(OPTS) SymbolTable.cc

link.o: link.cc link.h
	g++ $(OPTS) link.cc

clean:
	rm -f parsetest *.o