#include <bits/stdc++.h>
#include <bit>
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

const vector<string> squareToCoordinates = {
    "a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8",
    "a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
    "a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
    "a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
    "a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
    "a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
    "a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
    "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1"
};


enum {
    white, black
};

#define getBit(bitboard, position) ((bitboard&(1ULL << position)) ? 1 : 0)
#define setBit(bitboard, position) bitboard |= (1ULL << position)
#define eraseBit(bitboard, position) (getBit(bitboard, position) ? bitboard ^= (1ULL<<position) : 0)

int countBits(U64 bitboard){
    return __popcount(bitboard);
}

int getLSBIndex(U64 bitboard){
    if (!bitboard) return -1;
    int position = 0;
    while (position<64){
        if (bitboard & (1ULL<<position)) break;
        position++;
    }
    return position;
}


/* for out of border testing */
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

// FOR KING
U64 kingAttacks[64];

U64 maskKingAttacks(int square){
    U64 attacks = 0ULL;

    U64 pieceBitboard = 0ULL;
    setBit(pieceBitboard, square);
    // offsets = 1, 7, 8, 9
    // we can reuse the pawn codes for offset 7 and 9
    attacks |= maskPawnAttack(white, square);
    attacks |= maskPawnAttack(black, square);

    // for offset 1 
    if((pieceBitboard >> 1) & notH) attacks |= (pieceBitboard >> 1);
    if((pieceBitboard << 1) & notA) attacks |= (pieceBitboard << 1);

    // for offset 8
    attacks |= (pieceBitboard>>8);
    attacks |= (pieceBitboard<<8);

    return attacks;
}

// FOR BISHOP
U64 bishopAttacks[64];

// relevant bits lookup table for each square
const vector<int> bihsopRelevantBits = {
    6, 5, 5, 5, 5, 5, 5, 6, 
    5, 5, 5, 5, 5, 5, 5, 5, 
    5, 5, 7, 7, 7, 7, 5, 5, 
    5, 5, 7, 9, 9, 7, 5, 5, 
    5, 5, 7, 9, 9, 7, 5, 5, 
    5, 5, 7, 7, 7, 7, 5, 5, 
    5, 5, 5, 5, 5, 5, 5, 5, 
    6, 5, 5, 5, 5, 5, 5, 6, 
};

U64 maskBishopAttacks(int square){
    U64 attacks = 0ULL;

    int targetRank = square/8;
    int targetFile = square%8;
 
    for (int i = targetRank+1,j = targetFile+1; i<7 && j<7; i++, j++) setBit(attacks, 8*i+j);
    for (int i = targetRank-1,j = targetFile+1; i>0 && j<7; i--, j++) setBit(attacks, 8*i+j);
    for (int i = targetRank+1,j = targetFile-1; i<7 && j>0; i++, j--) setBit(attacks, 8*i+j);
    for (int i = targetRank-1,j = targetFile-1; i>0 && j>0; i--, j--) setBit(attacks, 8*i+j);

    return attacks;
}

U64 generateBishopAttacks(int square, U64 block){
    U64 attacks = 0ULL;

    int targetRank = square/8;
    int targetFile = square%8;
 
    for (int i = targetRank+1,j = targetFile+1; i<8 && j<8; i++, j++) {
        setBit(attacks, 8*i+j);
        if (1ULL << (8*i+j) & block) break;
    }
    for (int i = targetRank-1,j = targetFile+1; i>=0 && j<8; i--, j++) {
        setBit(attacks, 8*i+j);
        if (1ULL << (8*i+j) & block) break;
    }
    for (int i = targetRank+1,j = targetFile-1; i<8 && j>=0; i++, j--) {
        setBit(attacks, 8*i+j);
        if (1ULL << (8*i+j) & block) break;
    }
    for (int i = targetRank-1,j = targetFile-1; i>=0 && j>=0; i--, j--) {
        setBit(attacks, 8*i+j);
        if (1ULL << (8*i+j) & block) break;
    }

    return attacks;
}


// FOR ROOKS
U64 rookAttacks[64];

// relevant bits lookup table for each square
const vector<int> rookRelevantBits = {
    12, 11, 11, 11, 11, 11, 11, 12, 
    11, 10, 10, 10, 10, 10, 10, 11, 
    11, 10, 10, 10, 10, 10, 10, 11, 
    11, 10, 10, 10, 10, 10, 10, 11, 
    11, 10, 10, 10, 10, 10, 10, 11, 
    11, 10, 10, 10, 10, 10, 10, 11, 
    11, 10, 10, 10, 10, 10, 10, 11, 
    12, 11, 11, 11, 11, 11, 11, 12, 
};

U64 maskRookAttacks(int square){
    U64 attacks = 0ULL;
    int targetRank = square/8;
    int targetFile = square%8;
    for (int i = 1; i<7; i++){
        if (i!=targetRank) setBit(attacks, 8*i + targetFile);
        if (i!=targetFile) setBit(attacks, 8*targetRank + i);
    }

    return attacks;
}

U64 generateRookAttacks(int square, U64 block){
    U64 attacks = 0ULL;
    int targetRank = square/8;
    int targetFile = square%8;
    for (int i = targetRank+1,j = targetFile; i<8; i++) {
        setBit(attacks, 8*i+j);
        if (1ULL << (8*i+j) & block) break;
    }
    for (int i = targetRank-1,j = targetFile; i>=0; i--) {
        setBit(attacks, 8*i+j);
        if (1ULL << (8*i+j) & block) break;
    }
    for (int i = targetRank,j = targetFile-1; j>=0; j--) {
        setBit(attacks, 8*i+j);
        if (1ULL << (8*i+j) & block) break;
    }
    for (int i = targetRank,j = targetFile+1; j<8; j++) {
        setBit(attacks, 8*i+j);
        if (1ULL << (8*i+j) & block) break;
    }

    return attacks;
}

// FOR QUEEN
U64 queenAttacks[64];

U64 maskQueenAttacks(int square){
    U64 attacks = 0ULL;
    
    attacks |= maskBishopAttacks(square);
    attacks |= maskRookAttacks(square);

    return attacks;
}

// initialize all the attacks
void initializeAttacks(){
    for (int i = 0; i<64; i++){
        // pawn attacks
        pawnAttacks[white][i] = maskPawnAttack(white, i);
        pawnAttacks[black][i] = maskPawnAttack(black, i);

        // knight attacks
        knightAttacks[i] = maskKnightAttacks(i);

        // king attacks
        kingAttacks[i] = maskKingAttacks(i);

        // bihsop attacks
        bishopAttacks[i] = maskBishopAttacks(i);

        // rook attacks
        rookAttacks[i] = maskRookAttacks(i);
    }
}

// Useful fucntion to generate all possible combination of occupancies
U64 setOccupancy(int index, U64 attackMask){
    /* 
    Looping over the maximum index till 4096(can be used to generate permutations of combinations with max relevancy bit 12), 
    we will have every combination generated for every possible piece as every piece except the queen have relevancy bits less than or equal to 12 only
    */
    int bitsInMask = countBits(attackMask);
    
    U64 occupancy = 0ULL;
    // loops over every active bit of attackMask
    for (int count = 0; count<bitsInMask; count++){
        int square = getLSBIndex(attackMask);
        eraseBit(attackMask, square);

        if (index & (1 << count)) occupancy |= (1ULL << square);
    }

    return occupancy;
}

int main(){
    initializeAttacks();
    for (int i = 0; i<8; i++){
        for(int j = 0; j<8; j++){
            int sq = 8*i + j;
            cout << countBits(maskRookAttacks(sq)) << ", ";
        }
        cout << '\n';
    }

    // printBitBoard(4096);

    return 0;
}