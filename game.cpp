#include "game.h"

Game::Game(Number first, Number second, std::uint32_t seed, std::ostream& out)
    : rules(first, second),
      rng(seed),
      strategy(rng),
      out(out),
      names{"Player A", "Player B", "Player C"},
      moveNumber(0) {
}

std::string Game::getPlayerName(int id) const {
    return names[id];
}

PlayerTask Game::player(int id) {
    std::optional<Action> lastAction;

    while (true) {
        Move move = strategy.chooseMove(rules, lastAction);
        rules.apply(move);
        lastAction = move.action;
        moveNumber++;
        printMove(id, move);

        if (rules.isWon()) {
            printWin(id);
            co_return;
        }

        co_await std::suspend_always{};
    }
}

std::optional<int> Game::play() {
    printStart();

    std::vector<PlayerTask> players;
    for (int id = 0; id < kPlayerCount; id++) {
        players.push_back(player(id));
    }

    int turn = 0;
    while (moveNumber < kMaxMoves) {
        players[turn].resume();
        if (players[turn].done()) {
            return turn;
        }
        turn = (turn + 1) % kPlayerCount;
    }

    out << "\nThe game was stopped after " << kMaxMoves << " moves.\n";
    return std::nullopt;
}

void Game::printStart() const {
    out << "Initial numbers: " << rules.toString()
         << ", sum: " << rules.getSum()
         << ", target: sum >= " << rules.getTarget() << "\n\n";
}

void Game::printMove(int id, const Move& move) const {
    out << "Move " << moveNumber << ". " << names[id] << ": " << describe(move)
         << " -> " << rules.toString() << ", sum " << rules.getSum() << "\n";
}

void Game::printWin(int id) const {
    out << "\n" << names[id] << ": I won! Sum " << rules.getSum()
         << " >= " << rules.getTarget() << ". Stopping the game.\n";
}