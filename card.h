#pragma once
#include <string>
using namespace std;
enum class SUIT {
    HEARTS=0,
    DIAMONDS,
    CLUBS,
    SPADES
};

class Card {
public:
    Card(SUIT suit, int rank, bool faceUp);
    Card();
    string toString() const;
    SUIT getSuit() const;
    int getRank() const;
    bool isFaceUp() const;
    void flip();
    bool isRed() const;

private:
    SUIT suit;    
    int rank;     
    bool faceUp = true;  
};