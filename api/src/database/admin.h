#pragma once

#include <Wt/Dbo/Dbo.h>
#include <Wt/Dbo/WtSqlTraits.h>

#include "str.h"

class Admin {
public:
    template<typename Action>
    void persist(Action& a) {
        Wt::Dbo::field(a, name_, Str::name);
    }

    Admin() = default;
    Admin(std::string name);

    void setName(std::string name);
    const std::string& getName() const;

private:
    std::string name_;
};
