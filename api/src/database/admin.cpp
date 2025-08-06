#include "admin.h"

Admin::Admin(std::string name) {
    setName(std::move(name));
}

void Admin::setName(std::string name) {
    name_ = std::move(name);
}

const std::string& Admin::getName() const {
    return name_;
}
