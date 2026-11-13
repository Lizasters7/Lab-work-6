#include "reader.h"
#include <charconv>
#include <sstream>

InputError::InputError(const std::string& message)
    : std::runtime_error(message) {
}

GameConfig InputReader::read(std::istream& in, std::uint32_t defaultSeed) {
    std::vector<std::string> tokens = readTokens(in);

    if (tokens.size() < 2) {
        throw InputError("Expected at least two numbers, got " +
                         std::to_string(tokens.size()));
    }
    if (tokens.size() > 3) {
        throw InputError("Too many values: expected 2 or 3, got " +
                         std::to_string(tokens.size()));
    }

    GameConfig config;
    config.first = parseInteger(tokens[0], 1, kMaxStartNumber, "First number");
    config.second = parseInteger(tokens[1], 1, kMaxStartNumber, "Second number");
    if (tokens.size() == 3) {
        config.seed = static_cast<std::uint32_t>(parseInteger(tokens[2], 0, kMaxSeed, "Seed"));
    } else {
        config.seed = defaultSeed;
    }
    return config;
}

std::vector<std::string> InputReader::readTokens(std::istream& in) {
    std::vector<std::string> tokens;
    std::string line;
    while (std::getline(in, line)) {
        bool isComment = !line.empty() && line[0] == '#';
        if (!isComment) {
            std::istringstream lineStream(line);
            std::string token;
            while (lineStream >> token) {
                tokens.push_back(token);
            }
        }
    }
    return tokens;
}

std::int64_t InputReader::parseInteger(const std::string& token, std::int64_t min,
                                       std::int64_t max, const std::string& what) {
    std::int64_t value = 0;
    const char* begin = token.data();
    const char* end = begin + token.size();
    std::from_chars_result result = std::from_chars(begin, end, value);

    if (result.ec == std::errc::result_out_of_range) {
        throw InputError(what + ": value \"" + token + "\" is too large");
    }
    if (result.ec != std::errc() || result.ptr != end) {
        throw InputError(what + ": \"" + token + "\" is not a valid integer");
    }
    if (value < min || value > max) {
        throw InputError(what + ": " + token + " is out of bounds [" + std::to_string(min) +
                         ", " + std::to_string(max) + "]");
    }
    return value;
}