#include "game.h"
#include <iostream>

int main() {
    Game game(3, 5, 42, std::cout);
    std::optional<int> winner = game.play();

    if (winner.has_value()) {
        std::cout << "\n=== Winner: " << game.getPlayerName(winner.value()) << " ===\n";
    } else {
        std::cout << "\n=== Tie ===\n";
    }
}