#include "Card.h"
#include <string>

Card::Card() : suit(SUIT::HEARTS), rank(1), faceUp(false) {}

Card::Card(SUIT suit, int rank, bool faceUp) : suit(suit), rank(rank), faceUp(faceUp) {}

std::string Card::toString() const 
{
    if (!faceUp) return "[*]"; 

    char suitChar;
    switch (suit) {
    case SUIT::HEARTS:   suitChar = 'H'; break;
    case SUIT::DIAMONDS: suitChar = 'D'; break;
    case SUIT::CLUBS:    suitChar = 'C'; break;
    case SUIT::SPADES:   suitChar = 'S'; break;
    default:             suitChar = '?'; break;
    }

    return "[" + std::to_string(rank) + suitChar + "]";
}

SUIT Card::getSuit() const { return suit; }

int Card::getRank() const { return rank; }

bool Card::isFaceUp() const 
{
    return faceUp; 
}

void Card::flip() 
{
    faceUp = !faceUp; 
}

bool Card::isRed() const { return suit == SUIT::HEARTS || suit == SUIT::DIAMONDS; }
