#include "Waste.h"

void Waste::addCard(const Card& card) {
    wastePile.push(card);
}

Card Waste::getTopCard() const {
    return wastePile.empty() ? Card(SUIT::HEARTS, 1, false): wastePile.top();
}

bool Waste::isEmpty() const {
    return wastePile.empty();
}

void Waste::removeTopCard() {
    if (!isEmpty()) {
        wastePile.pop();
    }
}

bool Waste::moveToTableau(Tableau& tableau, int tableauIndex) {
    if (!isEmpty()) {
        Card cardToMove = getTopCard();
        if (tableau.canPlaceOnTop(cardToMove)) {
            tableau.addCard(cardToMove);
            removeTopCard();
            return true;
        }
    }
    return false;
}

bool Waste::moveToFoundation(Foundation& foundation) {
    if (!isEmpty()) {
        Card cardToMove = getTopCard();
        if (foundation.canPlaceOnTop(cardToMove)) {
            foundation.addCard(cardToMove);
            removeTopCard();
            return true;
        }
    }
    return false;
}

void Waste::resetWasteToDeck(Deck& deck) {
    deck.refillFromWaste(wastePile);
}
