#include "Foundation.h"
#include "card.h"
#include <stdexcept>

void Foundation::addCard(const Card& card) {
    foundationPile.push(card);
}

Card Foundation::getTopCard() const {
    if (isEmpty()) {
        throw std::runtime_error("Foundation is empty.");
    }
    return foundationPile.top();
}

bool Foundation::isEmpty() const {
    return foundationPile.empty();
}

void Foundation::removeTopCard() {
    if (!isEmpty()) {
        foundationPile.pop();
    }
    else {
        throw std::runtime_error("Cannot remove card. Foundation is empty.");
    }
}

bool Foundation::canPlaceOnTop(const Card& card) const {
    if (isEmpty()) {
        return card.getRank() == 1;
    }
    Card topCard = getTopCard();
    return (card.getSuit() == topCard.getSuit() && card.getRank() == topCard.getRank() + 1);
}

int Foundation::getCardCount() const {
    return foundationPile.size(); 
}
