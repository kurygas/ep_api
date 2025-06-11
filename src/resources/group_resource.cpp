#include "group_resource.h"
#include "root_requirements.h"

void GroupResource::getRequirements(const HttpRequest& request, Session& session) const {
    RootRequirements::requireAuth(request, session);
}

void GroupResource::getIdRequirements(const HttpRequest& request, Session& session, const Wt::Dbo::ptr<Group>& group) const {
    RootRequirements::requireAuth(request, session);
}

void GroupResource::deleteRequirements(const HttpRequest& request, Session& session, const Wt::Dbo::ptr<Group>& group) const {
    RootRequirements::requireTeacherRoots(request, session);
}

void GroupResource::processPatch(const HttpRequest& request, Session& session, const Wt::Dbo::ptr<Group>& group) const {
    for (const auto& [key, value] : request.body()) {
        if (key == Str::name) {
            RootRequirements::requireTeacherRoots(request, session);

            if (session.exist(&Session::getByName<Group>, value)) {
                throw UnprocessableEntityException("Name already exists");
            }

            group.modify()->setName(value);
        }
    }
}
