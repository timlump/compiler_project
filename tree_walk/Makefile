CXX = g++
CXX_FLAGS = -std=c++2a -Wall -g -Wno-psabi

lox: main.o tree_walk.o scanner.o parser.o
	$(CXX) $(CXX_FLAGS) -o lox main.o tree_walk.o scanner.o parser.o

main.o: main.cpp
	$(CXX) $(CXX_FLAGS) -c main.cpp

scanner.o: scanner.cpp
	$(CXX) $(CXX_FLAGS) -c scanner.cpp

parser.o: parser.cpp
	$(CXX) $(CXX_FLAGS) -c parser.cpp

tree_walk.o: tree_walk.cpp
	$(CXX) $(CXX_FLAGS) -c tree_walk.cpp

ast_printer: ast_printer_main.cpp
	$(CXX) $(CXX_FLAGS) -o ast_printer ast_printer_main.cpp

clean:
	rm lox ast_printer *.o
