#include "atc_service.h"
#include "session.h"

#include <Wt/Http/Client.h>

void AtcService::setAtcName(const Wt::WString& atcName, const Ptr<User>& user) {
    Wt::Http::Client client;
    client.get(std::format("https://atcoder.jp/users/{}", atcName.toUTF8()));
    client.done().connect([atcName, user](const Wt::AsioWrapper::error_code& err, const Wt::Http::Message& response) {
        if (!err) {
            Session session;
            const Wt::Dbo::Transaction tr(session);
            user.modify()->setAtcName(atcName);
        }
    });
}