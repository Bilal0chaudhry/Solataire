#include "Display.h"
#include "Deck.h"
#include "Waste.h"
#include "Tableau.h"
#include <iostream>
#include <cstdlib>
#include <Windows.h>  
using namespace std;

void clearConsole() {
    system("cls");
}

void setColorForSuit(SUIT suit) {
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
    }
}

void resetConsoleColor() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);  
}

void Display::displayBoard(Game& game) {
    clearConsole();
    displayDeck(game.getDeck());
    displayWaste(game.getWaste());
    displayTableau(game.getTableau(), 7);
    displayFoundations(game.getFoundations(), 4);
}

void Display::displayDeck(const Deck& deck) {
    cout << "Deck: ";
    if (deck.isEmpty()) {
        cout << "[Empty]" << endl;
    }
    else {
        cout << "[Cards Remaining: " << deck.size() << "]" << endl;
    }
}

void Display::displayWaste(const Waste& waste) {
    cout << "Waste: ";
    if (waste.isEmpty()) {
        cout << "[Empty]" << endl;
    }
    else {
        setColorForSuit(waste.getTopCard().getSuit());
        cout << waste.getTopCard().toString();
        resetConsoleColor();
        cout << endl;
    }
}

void Display::displayTableau(const Tableau tableau[], int numColumns) {
    cout << "Tableau: " << endl;
    for (int i = 0; i < numColumns; ++i) {
        cout << "Column " << i + 1 << ": ";
        tableau[i].displayAllCards();  
    }
}

void Display::displayFoundations(const Foundation foundations[], int numFoundations) {
    cout << "Foundations: ";
    for (int i = 0; i < numFoundations; ++i) {
        if (foundations[i].isEmpty()) {
            cout << "[Empty] ";
        }
        else {
            setColorForSuit(foundations[i].getTopCard().getSuit());
            cout << foundations[i].getTopCard().toString();
            resetConsoleColor();
            cout << " ";
        }
    }
    cout << endl;
}
