all:
	g++ -std=c++11 axis.cpp tile.cpp main.cpp -o main

debug:
	g++ -std=c++1 axis.cpp tile.cpp main.cpp -o main -g3
