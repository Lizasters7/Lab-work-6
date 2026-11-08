#pragma once
#include "gameRules.h"
#include <optional>
#include <random>

class RandomStrategy {
public:
    explicit RandomStrategy(std::mt19937& rng);

    Move chooseMove(const GameRules& rule, std::optional<Action> lastAction);

private:
    std::mt19937& rng;
};