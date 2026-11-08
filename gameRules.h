#pragma once

#include <cstdint>
#include <string>

using Number = std::int64_t;

enum class Action { AddOne, Double, DivideByThree };

struct Move {
    Action action;
    int index;  
};

std::string describe(const Move& move);

class GameRules {
public:
    GameRules(Number first, Number second);

    Number getNumber(int index) const;
    Number getSum() const;
    Number getTarget() const;

    bool canApply(const Move& move) const;
    void apply(const Move& move);
    bool isWon() const;

    std::string toString() const;

private:
    Number numbers[2];
    Number target;
};