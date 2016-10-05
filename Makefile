OPTS = -g -c -Wall -Werror

test: lexer.o token.o
	g++ -o test lexer.o token.o

lexer.o: lexer.h lexer.cc token.h token.cc
	g++ $(OPTS) lexer.cc

token.o: token.h token.cc lexer.h lexer.cc
	g++ $(OPTS) token.cc

clean:
	rm -f testtrie *.o