#pragma once

#include <Wt/WString.h>

#include "types.h"

class User;
class Group;

namespace CfService {
    inline std::string key;
    inline std::string secret;

    void setCfName(const Wt::WString& cfName, const Ptr<User>& user);
    void setGroupCode(const Wt::WString& groupCode, const Ptr<Group>& group);
}
