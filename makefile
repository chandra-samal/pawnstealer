all:
	g++ -std=c++20 -oFast bitboard.cpp -o engine

debug:
	g++ -std=c++20 bitboard.cpp -o engine 
# compiles faster than released version :D
