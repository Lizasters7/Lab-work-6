#include "gameRules.h"
#include <stdexcept>

constexpr Number kTargetFactor = 10;   

std::string describe(const Move& move) 
{
    std::string numberIndex;
    if (move.index == 0) 
    {
        numberIndex = "first";
    } else 
    {
        numberIndex = "second";
    }

    std::string actionText;
    if (move.action == Action::AddOne) 
    {
        actionText = "+1 to the number";
    } else if (move.action == Action::Double) 
    {
        actionText = "x2 the number";
    } else 
    {
        actionText = "/3 the number";
    }

    return actionText + " (" + numberIndex + ")";
}

GameRules::GameRules(Number first, Number second)
    : numbers{first, second}, target{kTargetFactor * (first + second)} {
    if (first <= 0 || second <= 0) {
        throw std::invalid_argument("The numbers must be positive");
    }
}

Number GameRules::getNumber(int index) const 
{
    return numbers[index];
}

Number GameRules::getSum() const {
    return numbers[0] + numbers[1];
}

Number GameRules::getTarget() const 
{
    return target;
}

bool GameRules::canApply(const Move& move) const 
{
    if (move.index != 0 && move.index != 1) {
        return false;
    }
    if (move.action == Action::DivideByThree) {
        return numbers[move.index] % 3 == 0;
    }
    return true;
}

void GameRules::apply(const Move& move) 
{
    if (!canApply(move)) 
    {
        throw std::logic_error("You cannot do: " + describe(move));
    }
    Number& value = numbers[move.index];
    if (move.action == Action::AddOne) 
    {
        value += 1;
    } else if (move.action == Action::Double) 
    {
        value *= 2;
    } else 
    {
        value /= 3;
    }
}

bool GameRules::isWon() const 
{
    return getSum() >= getTarget();
}

std::string GameRules::toString() const 
{
    return "(" + std::to_string(getNumber(0)) + ", " + std::to_string(getNumber(1)) + ")";
}