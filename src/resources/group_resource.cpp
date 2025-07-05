#include "group_resource.h"
#include "root_requirements.h"
#include "cf_service.h"

void GroupResource::getRequirements(const HttpRequest& request, Session& session) const {
    RootRequirements::requireAuth(request, session);
}

void GroupResource::postRequirements(const HttpRequest& request, Session& session) const {
    RootRequirements::requireAuth(request, session);
}

void GroupResource::getIdRequirements(const HttpRequest& request, Session& session, const Ptr<Group>& group) const {
    RootRequirements::requireAuth(request, session);
}

void GroupResource::deleteRequirements(const HttpRequest& request, Session& session, const Ptr<Group>& group) const {
    RootRequirements::requireTeacherRoots(request, session);
}

void GroupResource::processPatch(const HttpRequest& request, Session& session, const Ptr<Group>& group) const {
    RootRequirements::requireTeacherRoots(request, session);

    for (const auto& [key, value] : request.body()) {
        if (key == Str::name) {
            if (session.exist(&Session::getByName<Group>, value)) {
                throw UnprocessableEntityException("Name already exists");
            }

            group.modify()->setName(value);
        }
        else if (key == Str::cfGroupCode) {
            CfService::setGroupCode(value, group);
        }
        else if (key == Str::cfMaxPoint) {
            group.modify()->setCfMaxPoint(value);
        }
        else if (key == Str::atcMaxPoint) {
            group.modify()->setAtcMaxPoint(value);
        }
    }
}
