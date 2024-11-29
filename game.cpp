#pragma once
#include "Game.h"
#include <iostream>

Game::Game() : gameOver(false) {
    deck.shuffle();

    for (int i = 0; i < 7; ++i) {
        for (int j = 0; j <= i; ++j) {
            Card drawnCard = deck.drawCard();
            if (j == i) {
                drawnCard.flip();
            }
            tableau[i].addCard(drawnCard);
        }
    }
}

void Game::drawCardFromDeck() 
{
    if (deck.isEmpty()) {
        waste.resetWasteToDeck(deck);
    }

    int cardsToDraw = (isEasyDifficulty) ? 1 : 3;

    for (int i = 0; i < cardsToDraw && !deck.isEmpty(); i++) 
    {
        Card drawnCard = deck.drawCard();
        drawnCard.flip();
        waste.addCard(drawnCard);
        undoStack.push(Action(ActionType::DRAW_CARD, drawnCard, -1, -1));
        while (!redoStack.empty()) {
            redoStack.pop();
        }
    }
}

bool Game::moveCard(int fromColumn, int toColumn, int startIndex) 
{
    if (fromColumn < 0 || fromColumn >= 7 || toColumn < 0 || toColumn >= 7) {
        return false;
    }

    if (tableau[fromColumn].empty()) {
        return false;
    }

    if (startIndex < 0 || startIndex >= tableau[fromColumn].getCardCount()) {
        return false;
    }

    std::vector<Card> cardsToMove = tableau[fromColumn].getCardsFromIndex(startIndex);

    for (const Card& card : cardsToMove) {
        if (!card.isFaceUp()) {
            std::cout << "Cannot move hidden cards!" << std::endl;
            return false;
        }
    }

    Card topCardToMove = cardsToMove.front();

    if (tableau[toColumn].canPlaceOnTop(topCardToMove)) {

        tableau[toColumn].addCards(cardsToMove);
        tableau[fromColumn].removeCardsFromIndex(startIndex);

        if (!tableau[fromColumn].empty()) {
            Card& newTopCard = tableau[fromColumn].topCards();
            if (!newTopCard.isFaceUp()) {
                newTopCard.flip();
            }
        }

        undoStack.push(Action(ActionType::MOVE_CARD, topCardToMove, fromColumn, toColumn));

        while (!redoStack.empty()) {
            redoStack.pop();
        }

        return true;
    }

    return false;
}


bool Game::moveCardFromWasteToTableau(int tableauIndex) {
    if (tableauIndex < 0 || tableauIndex >= 7 || waste.isEmpty()) {
        return false;
    }

    Card cardToMove = waste.getTopCard();

    if (tableau[tableauIndex].canPlaceOnTop(cardToMove)) {
        tableau[tableauIndex].addCard(cardToMove);
        waste.removeTopCard();

        undoStack.push(Action(ActionType::MOVE_WASTE_TO_TABLEAU, cardToMove, -1, tableauIndex));
        while (!redoStack.empty()) {
            redoStack.pop();
        }

        return true;
    }

    return false;
}

bool Game::moveCardFromWasteToFoundation(int foundationIndex) {
    if (foundationIndex < 0 || foundationIndex >= 4 || waste.isEmpty()) {
        return false;
    }

    Card cardToMove = waste.getTopCard();

    if (foundations[foundationIndex].canPlaceOnTop(cardToMove)) {
        foundations[foundationIndex].addCard(cardToMove);
        waste.removeTopCard();

        undoStack.push(Action(ActionType::MOVE_WASTE_TO_FOUNDATION, cardToMove, -1, foundationIndex));
        while (!redoStack.empty()) {
            redoStack.pop();
        }

        return true;
    }

    return false;
}

bool Game::moveTableauToFoundation(int tableauIndex, int foundationIndex) {
    if (tableauIndex < 0 || tableauIndex >= 7 || foundationIndex < 0 || foundationIndex >= 4) {
        return false;
    }

    Card cardToMove = tableau[tableauIndex].topCard();

    if (foundations[foundationIndex].canPlaceOnTop(cardToMove)) {
        foundations[foundationIndex].addCard(cardToMove);
        tableau[tableauIndex].removeTopCard();

        if (!tableau[tableauIndex].empty()) {
            Card& topCard = tableau[tableauIndex].topCards();
            if (!topCard.isFaceUp()) {
                topCard.flip();
            }
        }

        undoStack.push(Action(
            ActionType::MOVE_TABLEAU_TO_FOUNDATION,
            cardToMove,
            tableauIndex,
            foundationIndex,
            !tableau[tableauIndex].empty() ? !tableau[tableauIndex].topCard().isFaceUp() : false
        ));
        while (!redoStack.empty()) {
            redoStack.pop();
        }

        return true;
    }

    return false;
}

void Game::setGameOver(bool status) {
    gameOver = status;
    if (gameOver) {
        std::cout << "Game Over! You've won the game!" << std::endl;
    }
}

Deck& Game::getDeck() {
    return deck;
}

Waste& Game::getWaste() {
    return waste;
}

Tableau* Game::getTableau() {
    return tableau;
}

Foundation* Game::getFoundations() {
    return foundations;
}

bool Game::isGameOver() const 
{
    for (const Foundation& foundation : foundations) 
    {
        if (foundation.getCardCount() != 13) 
        { 
            return false;
        }
    }
    return true; 
}

bool Game::undo() {
    if (undoStack.empty()) {
        return false;
    }

    Action lastAction = undoStack.top();
    undoStack.pop();

    switch (lastAction.type) {
    case ActionType::MOVE_WASTE_TO_FOUNDATION: {
        foundations[lastAction.targetIndex].removeTopCard();
        waste.addCard(lastAction.card);
        break;
    }
    case ActionType::MOVE_TABLEAU_TO_FOUNDATION: {
        foundations[lastAction.targetIndex].removeTopCard();
        if (!tableau[lastAction.sourceIndex].empty() && lastAction.wasSourceCardFaceDown) {
            tableau[lastAction.sourceIndex].topCard().flip();
        }
        tableau[lastAction.sourceIndex].addCard(lastAction.card);
        break;
    }
    case ActionType::MOVE_CARD: {
        tableau[lastAction.targetIndex].removeTopCard();
        tableau[lastAction.sourceIndex].addCard(lastAction.card);
        break;
    }
    case ActionType::MOVE_WASTE_TO_TABLEAU: {
        waste.removeTopCard();
        tableau[lastAction.sourceIndex].addCard(lastAction.card);
        break;
    }
    case ActionType::DRAW_CARD: {
        int cardsToUndo = (isEasyDifficulty) ? 1 : 3;

        for (int i = 0; i < cardsToUndo && !waste.isEmpty(); ++i) {
            Card cardToReturn = waste.getTopCard();
            waste.removeTopCard();
            deck.returnCard(cardToReturn);

            Action redoAction(ActionType::DRAW_CARD, cardToReturn, -1, -1);
            redoStack.push(redoAction);
        }

        break;
    }
    default:
        return false;
    }

    redoStack.push(lastAction);
    return true;
}

bool Game::redo() {
    if (redoStack.empty()) {
        return false;
    }

    Action lastAction = redoStack.top();
    redoStack.pop();

    switch (lastAction.type) {
    case ActionType::MOVE_WASTE_TO_FOUNDATION: {
        waste.removeTopCard();
        foundations[lastAction.targetIndex].addCard(lastAction.card);
        break;
    }
    case ActionType::MOVE_TABLEAU_TO_FOUNDATION: {
        tableau[lastAction.sourceIndex].removeTopCard();
        foundations[lastAction.targetIndex].addCard(lastAction.card);
        break;
    }
    case ActionType::MOVE_CARD: {
        tableau[lastAction.sourceIndex].removeTopCard();
        tableau[lastAction.targetIndex].addCard(lastAction.card);
        break;
    }
    case ActionType::MOVE_WASTE_TO_TABLEAU: {
        tableau[lastAction.sourceIndex].removeTopCard();
        waste.addCard(lastAction.card);
        break;
    }
    case ActionType::DRAW_CARD: {
        int cardsToRedo = (isEasyDifficulty) ? 1 : 3;

        for (int i = 0; i < cardsToRedo && !deck.isEmpty(); ++i) {
            Card drawnCard = deck.drawCard();
            drawnCard.flip();
            waste.addCard(drawnCard);

            Action undoAction(ActionType::DRAW_CARD, drawnCard, -1, -1);
            undoStack.push(undoAction);
        }

        break;
    }

    default:
        return false;
    }

    undoStack.push(lastAction);
    return true;
}

void Game::setDifficulty(const std::string& level) {
    if (level == "Easy") {
        isEasyDifficulty = true;
    }
    else if (level == "Hard") {
        isEasyDifficulty = false;
    }
}




