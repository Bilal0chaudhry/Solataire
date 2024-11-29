#include "Display.h"
#include "Game.h"
#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#define NOMINMAX
#include <Windows.h>
#include <limits>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

using namespace chrono;
int score = 0;
bool first = true;

void playSound() {
    sf::SoundBuffer buffer;
    if (!buffer.loadFromFile("card-sounds-35956.wav")) {
        std::cerr << "Error loading sound file.\n";
        return;
    }
    sf::Sound sound;
    sound.setBuffer(buffer);
    sound.play();

    while (sound.getStatus() == sf::Sound::Playing) {
        sf::sleep(sf::milliseconds(100)); 
    }
}

void displayMessage(const std::string& message) {
    std::cout << message << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(2));
}

bool getIntegerInput(int& value, const std::string& prompt) {
    std::cout << prompt;
    std::cin >> value;
    if (std::cin.fail()) {
        std::cin.clear();

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return false;
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return true;
}

void getUserInput(Game& game) 
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE);

    std::string input;
    displayMessage("Enter command:\n1: move between tableau\n2: waste to tableau\n3: waste to foundation\n4: tableau to foundation\n5: draw card\n6: undo\n7: redo\n8: quit");

    std::cin >> input;

    if (input == "1") {
        int fromColumn, toColumn, startIndex;
        if (!getIntegerInput(fromColumn, "From column (1-7): ") || fromColumn < 1 || fromColumn > 7) {
            displayMessage("Invalid column input. Please enter a number between 1 and 7.");
            return;
        }
        if (!getIntegerInput(startIndex, "Start index of card to move (0-based): ") || startIndex < 0) {
            displayMessage("Invalid start index input. Please enter a non-negative integer.");
            return;
        }
        if (!getIntegerInput(toColumn, "To column (1-7): ") || toColumn < 1 || toColumn > 7) {
            displayMessage("Invalid column input. Please enter a number between 1 and 7.");
            return;
        }
        if (game.moveCard(fromColumn - 1, toColumn - 1, startIndex)) {
            displayMessage("Move successful!");
            playSound();
        }
        else {
            displayMessage("Invalid move between tableau columns!");
        }
    }
    else if (input == "2") {
        int tableauIndex;
        if (!getIntegerInput(tableauIndex, "Move to tableau column (1-7): ") || tableauIndex < 1 || tableauIndex > 7) {
            displayMessage("Invalid tableau column index. Please enter a number between 1 and 7.");
            return;
        }
        if (game.getWaste().moveToTableau(game.getTableau()[tableauIndex - 1], tableauIndex - 1)) {
            displayMessage("Card moved from waste to tableau!");
			score += 5;
            playSound();
        }
        else {
            displayMessage("Invalid move from waste to tableau!");
        }
    }
    else if (input == "3") {
        int foundationIndex;
        if (!getIntegerInput(foundationIndex, "Move to foundation (1-4): ") || foundationIndex < 1 || foundationIndex > 4) {
            displayMessage("Invalid foundation index. Please enter a number between 1 and 4.");
            return;
        }
        if (game.getWaste().moveToFoundation(game.getFoundations()[foundationIndex - 1])) {
            displayMessage("Card moved from waste to foundation!");
            score += 10;
            playSound();
        }
        else {
            displayMessage("Invalid move from waste to foundation!");
        }
    }
    else if (input == "4") {
        int tableauIndex, foundationIndex;
        if (!getIntegerInput(tableauIndex, "From tableau column (1-7): ") || tableauIndex < 1 || tableauIndex > 7) {
            displayMessage("Invalid tableau index. Please enter a number between 1 and 7.");
            return;
        }
        if (!getIntegerInput(foundationIndex, "To foundation (1-4): ") || foundationIndex < 1 || foundationIndex > 4) {
            displayMessage("Invalid foundation index. Please enter a number between 1 and 4.");
            return;
        }
        if (game.moveTableauToFoundation(tableauIndex - 1, foundationIndex - 1)) {
            displayMessage("Card moved from tableau to foundation!");
            score += 10;
            playSound();
        }
        else {
            displayMessage("Invalid move from tableau to foundation!");
        }
    }
    else if (input == "5") {
        game.drawCardFromDeck();
        displayMessage("Drew a card from the deck.");
		if (first == false) {
            score -= 10;
			first = true;
		}
		first = false;
        playSound();
    }
    else if (input == "6") {
        if (game.undo()) {
            displayMessage("Last action undone.");
            score -= 5;
            playSound();
        }
        else {
            displayMessage("Nothing to undo!");
        }
    }
    else if (input == "7") { 
        if (game.redo()) {
            displayMessage("Last action redone.");
            playSound();
        }
        else {
            displayMessage("Nothing to redo!");
        }
    }
    else if (input == "8") { 
        game.setGameOver(true);
        displayMessage("Quitting the game...");
    }
    else {
        displayMessage("Invalid command! Please try again.");
    }
    SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);
}

void setConsoleColors() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED | FOREGROUND_BLUE);
    system("cls");
}

void showscore(int score) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY | BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);

    std::cout << "Score: " << score << std::endl;

    SetConsoleTextAttribute(hConsole, BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED | FOREGROUND_BLUE);
}

void showMainMenu() 
{
    system("cls");

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY | BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);
    std::cout << "======================================" << std::endl;

    SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY | BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);
    std::cout << "        Welcome to Console Solitaire  " << std::endl;

    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY | BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);
    std::cout << "           by Bilal Chaudhry          " << std::endl;

    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY | BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);
    std::cout << "======================================" << std::endl;

    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE);

    std::cout << "\nChoose your difficulty level:" << std::endl;
    std::cout << "1. Nalla" << std::endl;
    std::cout << "2. Shapatar" << std::endl;
	std::cout << "3. Quit" << std::endl;
    std::cout << "Enter your choice (1 or 2 , 3 if You r a Quitter): ";
    SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);

    int choice = 0;
    while (true) {
        std::cin >> choice;
        Game game;
        if (choice == 1) {
            game.setDifficulty("Easy");
            displayMessage("Nalla hi rahi sari zindagi !!");
            break;
        }
        else if (choice == 2) {
            game.setDifficulty("Hard");
            displayMessage("Shapatar Chapri hi rahi sari zindagi !!");
            break;
        }
        else if (choice ==3){
            sf::Texture texture;
            if (!texture.loadFromFile("exit_image.png")) 
            {  
                std::cout << "Failed to load image!" << std::endl;
                exit(1);  
            }
            sf::Vector2u imageSize = texture.getSize();
            sf::RenderWindow window(sf::VideoMode(imageSize.x, imageSize.y), "Exiting...");
            sf::Sprite sprite(texture);
            sf::Clock clock;
            while (window.isOpen()) {
                sf::Event event;
                while (window.pollEvent(event)) {
                    if (event.type == sf::Event::Closed)
                        window.close();
                }
                if (clock.getElapsedTime().asSeconds() >= 2.0f) 
                {
                    window.close(); 
                }

                window.clear(sf::Color::White);
                window.draw(sprite);
                window.display();
            }

			displayMessage("TF MAN !!");
            exit(0);
        }
        else {
            std::cout << "Invalid choice. Please enter 1 for Easy or 2 for Hard: ";
        }
    }
    system("cls");
}

int main() {
    int elapsed_Seconds = 0;
    setConsoleColors();
    Game game;
    Display display;
    showMainMenu();
    auto start = std::chrono::steady_clock::now();
	playSound();
    while (!game.isGameOver()) {
        auto now = std::chrono::steady_clock::now();
        elapsed_Seconds = std::chrono::duration_cast<std::chrono::seconds>(now - start).count();
		if (elapsed_Seconds >= 10) {
			break;
		}
        display.displayBoard(game);
		showscore(score);
        getUserInput(game);
    }
    displayMessage("Game Over!");
    int seconds = elapsed_Seconds % 60;
	int minutes = elapsed_Seconds / 60;
	if (elapsed_Seconds >= 10)
	{
		std::cout << "Time: " << minutes << " minutes " << seconds << " seconds" << std::endl;
	}
	else
	{
		std::cout << "Time: " << seconds << " seconds" << std::endl;
	}
    std::this_thread::sleep_for(std::chrono::seconds(2));
    return 0;
}
