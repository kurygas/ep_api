#pragma once

#include <Wt/Dbo/Dbo.h>
#include <Wt/Dbo/WtSqlTraits.h>

#include "json.h"
#include "str.h"
#include "types.h"

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
    Point(const Wt::WString& reason, int amount, const Ptr<User>& user);

    void setReason(const Wt::WString& reason);
    void setAmount(int amount);
    void setUser(const Ptr<User>& user);

    const Wt::WString& getReason() const;
    int getAmount() const;
    const Ptr<User>& getUser() const;

private:
    Wt::WString reason_;
    int amount_;

    Ptr<User> user_;
};
