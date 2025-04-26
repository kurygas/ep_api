#include "group_resources.h"
#include "session.h"
#include "user.h"
#include "request.h"

using namespace GroupResources;

void Create::processPost(const HttpRequest& request, Wt::Json::Object& response, Session& session) const {
    session.addGroup(request.getBody().at("group_name"));
}
