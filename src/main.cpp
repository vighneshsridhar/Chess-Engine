#include <SFML/Graphics.hpp>
#include <string>
#include <utility>
#include <iostream>
#include <filesystem> // Requires C++17 or later
#include <windows.h>
#include <vector>


#include "Play.h"



int main() {
    ChessGame::Play play;
    play.playGame();
    return 0;
}