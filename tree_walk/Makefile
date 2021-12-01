CXX = g++
CXX_FLAGS = -std=c++2a -Wall -g -Wno-psabi

lox: main.o tree_walk.o scanner.o parser.o token.o interpreter.o environment.o expr.o
	$(CXX) $(CXX_FLAGS) -o lox main.o tree_walk.o scanner.o parser.o token.o interpreter.o environment.o expr.o

main.o: main.cpp
	$(CXX) $(CXX_FLAGS) -c main.cpp

scanner.o: scanner.cpp
	$(CXX) $(CXX_FLAGS) -c scanner.cpp

parser.o: parser.cpp
	$(CXX) $(CXX_FLAGS) -c parser.cpp

tree_walk.o: tree_walk.cpp
	$(CXX) $(CXX_FLAGS) -c tree_walk.cpp

token.o: token.cpp
	$(CXX) $(CXX_FLAGS) -c token.cpp

interpreter.o: interpreter.cpp
	$(CXX) $(CXX_FLAGS) -c interpreter.cpp

environment.o: environment.cpp
	$(CXX) $(CXX_FLAGS) -c environment.cpp

expr.o: expr.cpp
	$(CXX) $(CXX_FLAGS) -c expr.cpp

ast_printer: ast_printer_main.cpp
	$(CXX) $(CXX_FLAGS) -o ast_printer ast_printer_main.cpp

clean:
	rm lox ast_printer *.o
