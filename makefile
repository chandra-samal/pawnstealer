all:
	g++ -oFast bitboard.cpp -o engine

debug:
	g++ bitboard.cpp -o engine 
# compiles faster than released version :D
