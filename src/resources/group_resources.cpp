#include "group_resources.h"
#include "session.h"
#include "json.h"

void CreateGroupResource::processPost(const JsonObject& requestContent, JsonObject& responseContent, Session& session) const {
    session.addGroup(requestContent.getString("group_name"));
}