#pragma once

#include <random>
#include <string>
#include <Wt/Dbo/collection.h>

const std::string alphanumCharacters = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
constexpr std::string numCharacters = "0123456789";

namespace Random {
    std::mt19937 createGenerator();
    std::string generateString(const std::string& pool, int length);
    std::string generateRandomString(int length);
    std::string generateRandomNumString(int length);
    int randInt(int from, int to);
    
    template<typename T>
    T pickRandom(const Wt::Dbo::collection<T>& collection) {
        auto it = collection.begin();
        auto ind = randInt(0, collection.size() - 1);

        while (ind--) {
            ++it;
        }

        return *it;
    }
}
