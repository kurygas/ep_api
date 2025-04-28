#pragma once

#include <Wt/Dbo/ptr.h>
#include <Wt/Dbo/collection.h>
#include <Wt/Json/Array.h>
#include <Wt/Json/Object.h>
#include <Wt/Json/Value.h>
#include <Wt/Json/Serializer.h>
#include <Wt/Json/Parser.h>

template<typename T>
Wt::Json::Array getAllId(const Wt::Dbo::collection<Wt::Dbo::ptr<T>>& collection) {
    Wt::Json::Array array;
    
    for (const auto& elem : collection) {
        array.emplace_back(elem.id());
    }

    return array;
}

template<typename T>
T parse(const Wt::Json::Value& value) {
    return static_cast<T>(static_cast<int>(value));
}
