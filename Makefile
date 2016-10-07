OPTS = -g -c -Wall -Werror

lextest: lexer.o token.o test.o
	g++ -o lextest lexer.o token.o lextest.o

lexer.o: lexer.h lexer.cc token.h token.cc
	g++ $(OPTS) lexer.cc

token.o: token.h token.cc
	g++ $(OPTS) token.cc

test.o: lextest.cc token.cc lexer.cc
	g++ $(OPTS) lextest.cc

clean:
	rm -f test *.o
