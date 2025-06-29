#pragma once

#include <Wt/WString.h>

namespace CfService {
    std::string key;
    std::string secret;

    bool userExists(const std::string& cfUsername);
}
