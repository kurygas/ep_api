#include "json.h"

const Wt::Json::Value& JsonObject::at(const std::string& key) const {
    return const_cast<JsonObject*>(this)->at(key);
}

Wt::Json::Value& JsonObject::at(const std::string& key) {
    if (!contains(key)) {
        throw std::runtime_error("JsonObject does not contain key " + key);
    }

    return Wt::Json::Object::at(key);
}
