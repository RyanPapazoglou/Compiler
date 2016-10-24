OPTS = -g -c -Wall -Werror

parsetest: parser.o lexer.o token.o microc.o
	g++ -o parsetest lexer.o token.o parser.o microc.o

lexer.o: lexer.h lexer.cc token.h token.cc
	g++ $(OPTS) lexer.cc

token.o: token.h token.cc
	g++ $(OPTS) token.cc

microc.o: microc.cc token.cc lexer.cc parser.cc 
	g++ $(OPTS) microc.cc

parser.o: parser.cc parser.h token.cc token.h lexer.cc lexer.h
	g++ $(OPTS) parser.cc 

clean:
	rm -f parsetest *.o