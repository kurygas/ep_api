#include "group_resources.h"
#include "session.h"
#include "user.h"
#include "request.h"

using namespace GroupResources;

void CreateGroupResource::processPost(const HttpRequest& request, Wt::Json::Object& response, Session& session) const {
    session.addGroup(request.getBody().at("group_name"));
}

void AddUserResource::processPost(const HttpRequest& request, Wt::Json::Object& response, Session& session) const {
    const auto bodyContent = request.getBody();
    auto user = session.getUserByTgId(bodyContent.at("tg_id"));

    if (user->getGroup()) {
        throw std::runtime_error("User already in group");
    }

    user.modify()->setGroup(session.getGroupByGroupId(bodyContent.at("group_id")));
}
