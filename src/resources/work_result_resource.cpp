#include "work_result_resource.h"
#include "root_requirements.h"

void WorkResultResource::processPatch(const HttpRequest& request, Session& session, const Wt::Dbo::ptr<WorkResult>& workResult) const {
    RootRequirements::requireTeacherRoots(request, session);

    for (const auto& [key, value] : request.body()) {
        if (key == Str::filename) {
            workResult.modify()->setFilename(value);
        }
        else if (key == Str::mark) {
            workResult.modify()->setMark(value);
        }
        else if (key == Str::workId) {
            workResult.modify()->setWork(session.getById<Work>(value));
        }
        else if (key == Str::problemId) {
            workResult.modify()->setProblem(session.getById<Problem>(value));
        }
        else if (key == Str::userId) {
            workResult.modify()->setUser(session.getById<User>(value));
        }
        else if (key == Str::queued) {
            workResult.modify()->setQueued(value);
        }
    }
}

void WorkResultResource::getRequirements(const HttpRequest& request, Session& session) const {
    RootRequirements::requireTeacherRoots(request, session);
}

void WorkResultResource::getIdRequirements(const HttpRequest& request, Session& session, 
    const Wt::Dbo::ptr<WorkResult>& workResult) const {
    RootRequirements::requireTeacherRoots(request, session);
}

void WorkResultResource::deleteRequirements(const HttpRequest& request, Session& session, 
    const Wt::Dbo::ptr<WorkResult>& workResult) const {
    RootRequirements::requireTeacherRoots(request, session);
}
