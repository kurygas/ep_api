#include "json.h"

std::string JsonObject::getString(const std::string& key) const {
    return at(key);
}

int JsonObject::getInt(const std::string &key) const {
    return at(key);
}

const Wt::Json::Value& JsonObject::at(const std::string& key) const {
    return const_cast<JsonObject*>(this)->at(key);
}

Wt::Json::Value& JsonObject::at(const std::string& key) {
    if (!contains(key)) {
        throw std::runtime_error("Does not contain key " + key);
    }

    return Wt::Json::Object::at(key);
}
