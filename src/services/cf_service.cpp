#include "cf_service.h"

#include <Wt/Http/Client.h>

bool CfService::userExists(const std::string& cfUsername) {
    Wt::Http::Client client;
    client.get("https://codeforces.com/profile/" + cfUsername);
    return client.done();
}