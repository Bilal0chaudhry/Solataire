#pragma once
#include <stack>
#include "Card.h"
#include "Deck.h"
#include "Tableau.h"     
#include "Foundation.h"  
class Waste {
public:
    void addCard(const Card& card);
    Card getTopCard() const;
    bool isEmpty() const;
    void removeTopCard(); 
    void resetWasteToDeck(Deck& deck);
    bool moveToTableau(Tableau& tableau, int tableauIndex);
    bool moveToFoundation(Foundation& foundation);


private:
    std::stack<Card> wastePile; 
};
