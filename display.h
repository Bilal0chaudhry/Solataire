#pragma once
#include "Game.h"

class Display {
public:
    void displayBoard( Game& game);
    void displayDeck(const Deck& deck);
    void displayWaste(const Waste& waste);
    void displayTableau(const Tableau tableau[], int numColumns);
    void displayFoundations(const Foundation foundations[], int numFoundations);
};