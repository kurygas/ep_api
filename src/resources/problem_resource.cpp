#include "problem_resource.h"
#include "root_requirements.h"

void ProblemResource::getRequirements(const HttpRequest& request, Session& session) const {
    RootRequirements::requireTeacherRoots(request, session);
}

void ProblemResource::getIdRequirements(const HttpRequest& request, Session& session, int id) const {
    const auto user = session.getByToken<User>(request.token());

    if (user->getUserType() == UserType::Student && session.getCurrentProblem(user).id() != id) {
        throw ForbiddenException("No roots");
    }
}

void ProblemResource::deleteRequirements(const HttpRequest& request, Session& session, int id) const {
    RootRequirements::requireTeacherRoots(request, session);
}

void ProblemResource::processPatch(const HttpRequest& request, Session& session, const Wt::Dbo::ptr<Problem>& group) const {
    for (const auto& [key, value] : request.body()) {
        
    }
}