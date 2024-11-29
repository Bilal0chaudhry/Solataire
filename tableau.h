#pragma once
#include <vector>
#include "Card.h"
#include "Foundation.h" 

class Tableau {
public:
    void addCards(const std::vector<Card>& cards);
    std::vector<Card> getCardsFromIndex(int index);
    Card removeTopCard();
    void removeCardsFromIndex(int index);
    void addCard(const Card& card);
    bool isEmpty() const;
    void displayTopCard() const;
    void revealTopCard();
    void displayAllCards() const;
    Card& topCards();
    Card topCard() const;
    int getCardCount() const;
    bool canPlaceOnTop(const Card& card) const;
    bool empty() const;
    bool moveToFoundation(Foundation& foundation);
    Card* getTopCard();
       
private:
    std::vector<Card> cards;
};