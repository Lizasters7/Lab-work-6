#include "randomStrategy.h"
#include <stdexcept>
#include <vector>

RandomStrategy::RandomStrategy(std::mt19937& rng)
    : rng(rng) {
}

Move RandomStrategy::chooseMove(const GameRules& rule, std::optional<Action> lastAction) {
    const Action allActions[] = {Action::AddOne, Action::Double, Action::DivideByThree};
    std::vector<Move> allowed;
    for (Action action : allActions) {
        bool repeatsLast = lastAction.has_value() && lastAction.value() == action;
        if (!repeatsLast) {
            for (int index = 0; index < 2; ++index) {
                Move move{action, index};
                    if (rule.canApply(move)) 
                    {
                        allowed.push_back(move);
                    }
                }
            }
        }
    
    if (allowed.empty()) 
    {
        throw std::logic_error("No allowed moves available");
    }

    std::uniform_int_distribution<std::size_t> pick(0, allowed.size() - 1);
    return allowed[pick(rng)];
}