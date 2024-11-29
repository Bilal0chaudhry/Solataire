#pragma once
#include "card.h"
#include <stack>

class Deck {
public:
    Deck(); 
    void shuffle();
    Card drawCard(); 
    bool isEmpty() const;
    int size() const; 
    void refillFromWaste(std::stack<Card>& wastePile);
    void returnCard(Card card);

private:
    std::stack<Card> cards; 
};