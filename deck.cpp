#include "deck.h"
#include <cstdlib>
#include <ctime>
#include <vector>
#include "card.h"

Deck::Deck() 
{
    for (int suit = static_cast<int>(SUIT::HEARTS); suit <= static_cast<int>(SUIT::SPADES); ++suit) {
        for (int rank = 1; rank <= 13; ++rank) {
            cards.push(Card(static_cast<SUIT>(suit), rank, false)); 
        }
    }
    std::srand(static_cast<unsigned>(std::time(0)));

    shuffle();
}

void Deck::shuffle() {
    std::vector<Card> cardArray;
    while (!cards.empty()) {
        cardArray.push_back(cards.top());
        cards.pop();
    }

    for (int i = cardArray.size() - 1; i > 0; --i) 
    {
        int randomIndex = std::rand() % (i + 1); 

        Card temp = cardArray[i];
        cardArray[i] = cardArray[randomIndex];
        cardArray[randomIndex] = temp;
    }

    for (const auto& card : cardArray) {
        cards.push(card);
    }
}

Card Deck::drawCard() {
    if (isEmpty()) {
        throw ("Cannot draw from an empty deck!");
    }
    Card topCard = cards.top(); 
    cards.pop();
    return topCard; 
}

bool Deck::isEmpty() const {
    return cards.empty();
}

int Deck::size() const {
    return cards.size();
}

void Deck::refillFromWaste(std::stack<Card>& wastePile) 
{
    while (!wastePile.empty()) 
    {
        Card card = wastePile.top();
        card.flip();
        cards.push(card);
        wastePile.pop();
    }
    shuffle();  
}

void Deck::returnCard(Card card) {
    card.flip();
    cards.push(card); 
}