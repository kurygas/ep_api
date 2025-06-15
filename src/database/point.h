#pragma once

#include <Wt/Dbo/Dbo.h>
#include <Wt/Dbo/WtSqlTraits.h>

#include "json.h"
#include "str.h"

class User;

class Point {
public:
    template<typename Action>
    void persist(Action& a) {
        Wt::Dbo::field(a, reason_, Str::reason);
        Wt::Dbo::field(a, amount_, Str::amount);
        Wt::Dbo::belongsTo(a, user_, "user");
    }

    operator Wt::Json::Object() const;
    static const std::string& getListName();

    Point() = default;
    Point(const Wt::WString& reason, int amount, const Wt::Dbo::ptr<User>& user);

    void setReason(const Wt::WString& reason);
    void setAmount(int amount);
    void setUser(const Wt::Dbo::ptr<User>& user);

    const Wt::WString& getReason() const;
    int getAmount() const;
    const Wt::Dbo::ptr<User>& getUser() const;

private:
    Wt::WString reason_;
    int amount_;

    Wt::Dbo::ptr<User> user_;
};
