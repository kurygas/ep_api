#include "cf_service.h"
#include "user.h"
#include "session.h"

#include <Wt/Http/Client.h>

void CfService::setCfName(const Wt::WString& cfName, const Ptr<User>& user) {
    Wt::Http::Client client;
    client.get(std::format("https://codeforces.com/profile/{}", cfName.toUTF8()));
    client.done().connect([cfName, user](const Wt::AsioWrapper::error_code& err, const Wt::Http::Message& response) {
        if (!err) {
            Session session;
            const Wt::Dbo::Transaction tr(session);
            user.modify()->setCfName(cfName);
            std::cout << "good cf name" << std::endl;
        }
        else {
            std::cout << "bad cf name" << std::endl;
        }
    });
}

void CfService::setGroupCode(const Wt::WString& groupCode, const Ptr<Group>& group) {
    
}
