OPTS = -g -c -Wall -Werror

test: lexer.o token.o lextest.o
	g++ -o test lexer.o token.o lextest.o

lexer.o: lexer.h lexer.cc token.h token.cc
	g++ $(OPTS) lexer.cc

token.o: token.h token.cc
	g++ $(OPTS) token.cc

lextest.o: lextest.cc token.cc lexer.cc
	g++ $(OPTS) lextest.cc

clean:
	rm -f testtrie *.o
