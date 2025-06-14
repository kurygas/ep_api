#include "work_result_resource.h"
#include "root_requirements.h"

void WorkResultResource::processPatch(const HttpRequest& request, Session& session, const Wt::Dbo::ptr<WorkResult>& workResult) const {
    for (const auto& [key, value] : request.body()) {
        if (key == Str::filename) {
            RootRequirements::requireAuthId(request, session, workResult->getUser());

            if (!workResult->getFilename().empty()) {
                throw ForbiddenException("Another solution has been already pinned");
            }

            const auto* solution = request.file("solution");
            workResult.modify()->setFilename(value);
            std::string destPath = "/home/kuryga/projects/ep_api/solutions/";
            destPath += workResult->getUser()->getTgId().toUTF8() + '/';
            destPath += std::to_string(static_cast<int>(workResult->getWork()->getSubject())) + '/';
            destPath += std::to_string(workResult->getWork()->getSemester()) + '/';
            destPath += std::to_string(workResult->getWork()->getWorkNumber()) + '/';
            destPath += workResult->getFilename().toUTF8();
            std::rename(solution->spoolFileName().c_str(), destPath.c_str());
        }
        else if (key == Str::mark) {
            RootRequirements::requireTeacherRoots(request, session);
            workResult.modify()->setMark(value);
        }
        else if (key == Str::workId) {
            RootRequirements::requireTeacherRoots(request, session);
            workResult.modify()->setWork(session.getById<Work>(value));
        }
        else if (key == Str::problemId) {
            RootRequirements::requireTeacherRoots(request, session);
            workResult.modify()->setProblem(session.getById<Problem>(value));
        }
        else if (key == Str::userId) {
            RootRequirements::requireTeacherRoots(request, session);
            workResult.modify()->setUser(session.getById<User>(value));
        }
    }
}

void WorkResultResource::getRequirements(const HttpRequest& request, Session& session) const {
    RootRequirements::requireAuth(request, session);
}

void WorkResultResource::getIdRequirements(const HttpRequest& request, Session& session, 
    const Wt::Dbo::ptr<WorkResult>& workResult) const {
    RootRequirements::requireAuth(request, session);
}

void WorkResultResource::deleteRequirements(const HttpRequest& request, Session& session, 
    const Wt::Dbo::ptr<WorkResult>& workResult) const {
    RootRequirements::requireTeacherRoots(request, session);
}

void WorkResultResource::processGetMethod(const HttpRequest& request, Wt::Json::Object& response, Session& session, 
    const Wt::Dbo::ptr<WorkResult>& workResult, const std::string& method) const {
    if (method == Str::problemId) {
        RootRequirements::requireTeacherRoots(request, session);
        response[Str::problemId] = workResult->getProblem().id();
    }
    else if (method == Str::statement) {
        const auto caller = session.getByToken<User>(request.token());

        if (caller->getUserType() == UserType::Student && caller.id() != )

        response[Str::statement] = workResult->getProblem()->getStatement();
    }
    else if (method == S)
}
