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

enum {
    white, black
};

#define getBit(bitboard, position) ((bitboard&(1ULL << position)) ? 1 : 0)
#define setBit(bitboard, position) bitboard |= (1ULL << position)
#define eraseBit(bitboard, position) (getBit(bitboard, position) ? bitboard ^= (1ULL<<position) : 0)

/* for out of border testing */

// columns
const U64 notA = 18374403900871474942ULL;
const U64 notH = 9187201950435737471ULL;
const U64 notAB = 18229723555195321596ULL;
const U64 notGH = 4557430888798830399ULL;

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

// ATTACKS

// FOR PAWNS
U64 pawnAttacks[2][64];

U64 maskPawnAttack(int side, int square){
    U64 attacks = 0ULL; // attack bitboard
    U64 pieceBitboard = 0ULL; // piece bitboard
    
    setBit(pieceBitboard, square);
    
    if (!side){
        // for white
        if ((pieceBitboard >> 7) & notA) attacks |= (pieceBitboard >> 7);
        if ((pieceBitboard >> 9) & notH) attacks |= (pieceBitboard >> 9);
    }
    else {
        // for black
        if ((pieceBitboard << 7) & notH) attacks |= (pieceBitboard << 7);
        if ((pieceBitboard << 9) & notA) attacks |= (pieceBitboard << 9);
    }
    return attacks;
}

// FOR KNIGHTS
U64 knightAttacks[64];

U64 maskKnightAttacks(int square){
    U64 attacks = 0ULL;
    U64 pieceBitboard = 0ULL;

    setBit(pieceBitboard, square);
    // offsets = 17, 15, 10, 6
    if ((pieceBitboard>>17) & notH) attacks |= (pieceBitboard>>17);
    if ((pieceBitboard>>15) & notA) attacks |= (pieceBitboard>>15);
    if ((pieceBitboard>>10) & notGH) attacks |= (pieceBitboard>>10);
    if ((pieceBitboard>>6) & notAB) attacks |= (pieceBitboard>>6);

    if ((pieceBitboard<<17) & notA) attacks |= (pieceBitboard<<17);
    if ((pieceBitboard<<15) & notH) attacks |= (pieceBitboard<<15);
    if ((pieceBitboard<<10) & notAB) attacks |= (pieceBitboard<<10);
    if ((pieceBitboard<<6) & notGH) attacks |= (pieceBitboard<<6);
    return attacks;
}

void initializeAttacks(){
    for (int i = 0; i<64; i++){
        // pawn attacks
        pawnAttacks[white][i] = maskPawnAttack(white, i);
        pawnAttacks[black][i] = maskPawnAttack(black, i);

        // knight attacks
        knightAttacks[i] = maskKnightAttacks(i);
    }
}

int main(){
    initializeAttacks();
    for (int i = 0; i<64; i++) printBitBoard(knightAttacks[i]);

    return 0;
}