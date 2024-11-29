#pragma once
#include "Deck.h"
#include "Waste.h"
#include "Foundation.h"
#include "Tableau.h"
#include "Action.h"

class Game {
public:
    Game();
    void drawCardFromDeck();
    Deck& getDeck();
    Waste& getWaste();
    Tableau* getTableau();
    Foundation* getFoundations();
    bool moveCard(int fromColumn, int toColumn, int startIndex);
    bool moveCardFromWasteToTableau(int tableauIndex); 
    bool moveCardFromWasteToFoundation(int foundationIndex);
    void setGameOver(bool status);
    bool moveTableauToFoundation(int tableauIndex, int foundationIndex);
    bool isGameOver() const;
    bool undo();
    bool redo();
    void setDifficulty(const std::string& level);

private:
    std::stack<Action> undoStack; 
    std::stack<Action> redoStack; 
    bool isEasyDifficulty;
    Deck deck;
    Waste waste;
    Tableau tableau[7];
    Foundation foundations[4];
    bool gameOver;
};
