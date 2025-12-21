#include <bits/stdc++.h>
using namespace std;

#define U64 unsigned long long

enum {
    a8, b8, c8, d8, e8, f8, g8, h8,
    a7, b7, c7, d7, e7, f7, g7, h7,
    a6, b6, c6, d6, e6, f6, g6, h6,
    a5, b5, c5, d5, e5, f5, g5, h5,
    a4, b4, c4, d4, e4, f4, g4, h4,
    a3, b3, c3, d3, e3, f3, g3, h3,
    a2, b2, c2, d2, e2, f2, g2, h2,
    a1, b1, c1, d1, e1, f1, g1, h1
};

#define getBit(bitboard, position) ((bitboard&(1ULL << position)) ? 1 : 0)
#define setBit(bitboard, position) bitboard |= (1ULL << position)
#define eraseBit(bitboard, position) (getBit(bitboard, position) ? bitboard ^= (1ULL<<position) : 0)

void printBitBoard(U64 bitboard){
    cout << '\n';
    char ch = 'a';
    for (int i = 0; i<8; i++){
        for(int j = 0; j<8; j++){
            int position = 8*i + j; 
            // it is assumed that the white pieces will be placed on the bottom half of the board before the game starts
            if (!j) cout << "  " << 8-i << "  "; 
            cout << getBit(bitboard, position) << " ";
        }
        cout << '\n';
    }
    cout << "\n     a b c d e f g h";
    cout << "\n\n     Bitboard: " << bitboard << "\n\n";
}

int main(){
    U64 bitboard = 0ULL;
    setBit(bitboard, e2);
    setBit(bitboard, a1);
    setBit(bitboard, b1);
    eraseBit(bitboard, a1);

    printBitBoard(bitboard);

    return 0;
}