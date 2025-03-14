#include "group_resources.h"
#include "session.h"
#include "json.h"

void CreateGroupResource::processPost(const Wt::Http::Request& request, JsonObject& responseContent, Session& session) const {
    session.addGroup(getBodyContent(request).getString("group_name"));
}