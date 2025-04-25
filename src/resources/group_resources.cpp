#include "group_resources.h"
#include "session.h"
#include "json.h"
#include "user.h"

using namespace GroupResources;

void CreateGroupResource::processPost(const Wt::Http::Request& request, JsonObject& responseContent, Session& session) const {
    session.addGroup(getBodyContent(request)["group_name"]);
}

void AddUserResource::processPost(const Wt::Http::Request& request, JsonObject& responseContent, Session& session) const {
    const auto bodyContent = getBodyContent(request);
    auto user = session.getUserByTgId(bodyContent.at("tg_id"));

    if (user->getGroup()) {
        throw std::runtime_error("User already in group");
    }

    user.modify()->setGroup(session.getGroupByGroupId(bodyContent.at("group_id")));
}
