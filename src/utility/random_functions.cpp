#include "random_functions.h"

std::mt19937 Random::createGenerator() {
    return std::mt19937((std::random_device())());
}

Wt::WString Random::generateRandomString(const int length) {
    auto generator = Random::createGenerator();
    std::uniform_int_distribution<int> distribution(0, alphanumCharacters.size() - 1);
    std::string randomString(16, '\0');

    for (auto& ch : randomString) {
        ch = alphanumCharacters[distribution(generator)];
    }

    return randomString;
}

int Random::randInt(const int from, const int to) {
    auto generator = createGenerator();
    return (std::uniform_int_distribution<int>(from, to))(generator);
}
