#pragma once
#include "gameRules.h"
#include <cstdint>
#include <istream>
#include <stdexcept>
#include <string>
#include <vector>

struct GameConfig {
    Number first;
    Number second;
    std::uint32_t seed;
};

class InputError : public std::runtime_error {
public:
    explicit InputError(const std::string& message);
};

// Input format: <number1> <number2> [seed]
// Empty lines and lines starting with '#' are ignored.
class InputReader {
public:
    static GameConfig read(std::istream& in, std::uint32_t defaultSeed);

private:
    static constexpr std::int64_t kMaxStartNumber = 1000000000000;   
    static constexpr std::int64_t kMaxSeed = 4294967295;             

    static std::vector<std::string> readTokens(std::istream& in);
    static std::int64_t parseInteger(const std::string& token, std::int64_t min,
                                     std::int64_t max, const std::string& what);
};