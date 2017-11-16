all:
	g++ -std=c++11 axis.cpp tile.cpp board.cpp main.cpp -o main

debug:
	g++ -std=c++11 axis.cpp tile.cpp board.cpp main.cpp -o main -g3
