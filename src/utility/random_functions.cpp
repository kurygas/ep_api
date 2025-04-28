#include "random_functions.h"

std::mt19937 Random::createGenerator() {
    return std::mt19937((std::random_device())());
}

std::string Random::generateString(const std::string& pool, const int length) {
    auto generator = Random::createGenerator();
    std::uniform_int_distribution<int> distribution(0, pool.size() - 1);
    std::string randomString(length, '\0');

    for (auto& ch : randomString) {
        ch = pool[distribution(generator)];
    }

    return randomString;
}

std::string Random::generateRandomString(const int length) {
    return Random::generateString(alphanumCharacters, length);
}

std::string Random::generateRandomNumString(const int length) {
    return Random::generateString(numCharacters, length);
}

int Random::randInt(const int from, const int to) {
    auto generator = createGenerator();
    return (std::uniform_int_distribution<int>(from, to))(generator);
}
