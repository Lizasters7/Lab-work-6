#include "game.h"
#include "reader.h"
#include <exception>
#include <fstream>
#include <iostream>
#include <random>
#ifdef _WIN32
#define NOMINMAX
#include <windows.h>
#endif

GameConfig readConfig(int argc, char* argv[]) {
    std::uint32_t defaultSeed = std::random_device{}();

    if (argc > 2) {
        throw InputError("Usage: Lab-work-6 [input_file]");
    }
    if (argc == 2) {
        std::ifstream file(argv[1]);
        if (!file) {
            throw InputError(std::string("Failed to open file \"") + argv[1] + "\"");
        }
        return InputReader::read(file, defaultSeed);
    }

    std::cout << "Enter two positive integers and (optionally) a seed,\n"
                 "then Ctrl+Z and Enter (Windows) or Ctrl+D (Linux/macOS):\n";
    return InputReader::read(std::cin, defaultSeed);
}

void printResult(const Game& game, std::optional<int> winner) {
    if (winner.has_value()) {
        std::cout << "\n=== Result: winner is " << game.getPlayerName(winner.value()) << " ===\n";
    } else {
        std::cout << "\n=== Result: no winner ===\n";
    }
}

int main(int argc, char* argv[]) {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif
    try {
        GameConfig config = readConfig(argc, argv);
        std::cout << "Seed: " << config.seed << "\n";

        Game game(config.first, config.second, config.seed, std::cout);
        std::optional<int> winner = game.play();
        printResult(game, winner);
        return 0;
    } catch (const InputError& error) {
        std::cerr << "Input error: " << error.what() << "\n";
        return 1;
    } catch (const std::exception& error) {
        std::cerr << "Internal error: " << error.what() << "\n";
        return 2;
    }
}