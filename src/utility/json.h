#pragma once

#include <Wt/Json/Object.h>
#include <Wt/Json/Parser.h>
#include <Wt/Json/Serializer.h>
#include <Wt/Json/Array.h>
#include <Wt/Json/Value.h>
#include <unordered_set>
#include <iostream>

class JsonObject : public Wt::Json::Object {
public:
    template<typename T>
    void putString(const std::string& key, const T& value) {
        if constexpr (std::is_constructible_v<Wt::Json::Value, const T&>) {
            operator[](key) = Wt::Json::Value(value);
        }
        else {
            put(key, Wt::WString::fromUTF8(value));
        }
    }

    template<typename T>
    void putInt(const std::string& key, T value) {
        operator[](key) = static_cast<int>(value);
    }
    
    const Wt::Json::Value& at(const std::string& key) const;
    Wt::Json::Value& at(const std::string& key);
};