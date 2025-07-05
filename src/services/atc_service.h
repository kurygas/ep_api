#pragma once

#include <Wt/WString.h>

#include "types.h"

class User;

namespace AtcService {
    void setAtcName(const Wt::WString& atcName, const Ptr<User>& user);
}
