#pragma once

#include <Wt/Dbo/Dbo.h>
#include <Wt/Dbo/WtSqlTraits.h>

class Test {
public:
    template<typename Action>
    void persist(Action& a) {
        Wt::Dbo::field(a, testName_, "test_name");
    }

private:
    std::string testName_;
};