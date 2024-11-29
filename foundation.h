#pragma once
#include <stack>
#include "Card.h"

class Foundation {
public:
    void addCard(const Card& card);
    Card getTopCard() const;
    bool isEmpty() const;
    void removeTopCard();
    bool canPlaceOnTop(const Card& card) const;
    int getCardCount() const;

private:
    std::stack<Card> foundationPile;
};
