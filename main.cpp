#include "playerTask.h"
#include <iostream>
#include <vector>

PlayerTask counter(int id) {
    for (int i = 1; i <= 3; ++i) {
        std::cout << "Player " << id << ", action " << i << "\n";
        co_await std::suspend_always{};
    }
}

int main() {
    std::vector<PlayerTask> players;
    players.push_back(counter(1));
    players.push_back(counter(2));
    std::cout << "Створено\n";

    for (int round = 0; round < 4; ++round) {
        for (int i = 0; i < 2; ++i) {
            players[i].resume();
        }
    }
}