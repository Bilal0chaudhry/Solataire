#include "Tableau.h"
#include <iostream>
#include <Windows.h>


void setColorForSuittableau(SUIT suit) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    int backgroundColor = BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE;

    switch (suit) {
    case SUIT::HEARTS:
    case SUIT::DIAMONDS:
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY | backgroundColor); 
        break;
    case SUIT::CLUBS:
    case SUIT::SPADES:
        SetConsoleTextAttribute(hConsole, 0 | backgroundColor);  
        break;
    }
}

void resetConsoleColortableau() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED); 
}

void Tableau::addCards(const std::vector<Card>& cardsToAdd) 
{
    cards.insert(cards.end(), cardsToAdd.begin(), cardsToAdd.end());
}

void Tableau::addCard(const Card& card) {
    cards.push_back(card);
}

Card Tableau::removeTopCard() {
    if (!cards.empty()) {
        Card topCard = cards.back();
        cards.pop_back();
        revealTopCard(); 
        return topCard;
    }
    return Card(SUIT::HEARTS, 1, false);
}

void Tableau::removeCardsFromIndex(int index) {
    if (index >= 0 && index < cards.size()) {
        cards.erase(cards.begin() + index, cards.end());
    }
}


std::vector<Card> Tableau::getCardsFromIndex(int index) {
    if (index < 0 || index >= cards.size()) {
        return {};  
    }
    return std::vector<Card>(cards.begin() + index, cards.end());
}

bool Tableau::isEmpty() const {
    return cards.empty();
}

void Tableau::displayTopCard() const {
    if (!cards.empty()) {
        std::cout << cards.back().toString();
    }
    else {
        std::cout << "[Empty]";
    }
}

void Tableau::revealTopCard() {
    if (cards.size() > 0) {
        cards.back().flip();
    }
}

void Tableau::displayAllCards() const 
{
    if (cards.empty()) {
        std::cout << "[Empty]";
    }
    else 
    {
        for (size_t i = 0; i < cards.size(); ++i) {
            if (!cards[i].isFaceUp()) {
                std::cout << "[*] "; 
            }
            else {
                setColorForSuittableau(cards[i].getSuit());  
                std::cout << cards[i].toString() << " ";
            }
        }
    }
    resetConsoleColortableau();  
    std::cout << std::endl; 
}

Card Tableau::topCard() const {
    if (!cards.empty()) {
        return cards.back();
    }
    return Card(SUIT::HEARTS, 1, false);
}

Card& Tableau::topCards(){
    if (!cards.empty()) {
        return cards.back(); 
    }
}

int Tableau::getCardCount() const {
    return cards.size();
}

bool Tableau::canPlaceOnTop(const Card& card) const 
{
    if (cards.empty()) {
        return card.getRank() == 13;
    }

    Card topCard = cards.back();
    bool differentColors = (card.isRed() != topCard.isRed());
    bool descendingOrder = (card.getRank() == topCard.getRank() - 1);

    return differentColors && descendingOrder;
}

bool Tableau::empty() const {
    return cards.empty();
}

bool Tableau::moveToFoundation(Foundation& foundation) {
    if (!isEmpty()) {
        Card cardToMove = topCard();
        if (foundation.canPlaceOnTop(cardToMove)) {
            foundation.addCard(cardToMove);
            removeTopCard();
            return true;
        }
    }
    return false;
}

Card* Tableau::getTopCard() {
    if (!cards.empty()) {
        return &cards.back(); 
    }
    return nullptr;
}