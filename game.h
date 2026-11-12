#pragma once
#include "gameRules.h"
#include "playerTask.h"
#include "randomStrategy.h"
#include <cstdint>
#include <optional>
#include <ostream>
#include <random>
#include <string>
#include <vector>

class Game {
public:
    Game(Number first, Number second, std::uint32_t seed, std::ostream& out);
    std::optional<int> play();
    std::string getPlayerName(int id) const;

private:
    static constexpr int kPlayerCount = 3;
    static constexpr int kMaxMoves = 100000;

    PlayerTask player(int id);
    void printStart() const;
    void printMove(int id, const Move& move) const;
    void printWin(int id) const;

    GameRules rules;
    std::mt19937 rng;
    RandomStrategy strategy;
    std::ostream& out;
    std::vector<std::string> names;
    int moveNumber;
};