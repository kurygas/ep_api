#include "work_result_resource.h"
#include "root_requirements.h"

void WorkResultResource::processPatch(const HttpRequest& request, Session& session, const Ptr<WorkResult>& workResult) const {
    for (const auto& [key, value] : request.body()) {
        if (key == Str::filename) {
            RootRequirements::requireAuthId(request, session, workResult->getUser());

            if (!workResult->getFilename().empty()) {
                throw ForbiddenException("Another solution has been already pinned");
            }

            const auto* solution = request.file(Str::solution);

            if (!solution) {
                throw BadRequestException("No solution file");
            }

            workResult.modify()->setFilename(value);
            std::rename(solution->spoolFileName().c_str(), workResult->getSolutionPath().c_str());
        }
        else if (key == Str::mark) {
            RootRequirements::requireTeacherRoots(request, session);
            workResult.modify()->setMark(value);
        }
        else if (key == Str::workId) {
            RootRequirements::requireTeacherRoots(request, session);
            const auto work = session.getById<Work>(value);

            if (session.exist(&Session::getWorkResult, work, workResult->getUser())) {
                throw UnprocessableEntityException("Already exists");
            }

            workResult.modify()->setWork(work);
        }
        else if (key == Str::problemId) {
            RootRequirements::requireTeacherRoots(request, session);
            workResult.modify()->setProblem(session.getById<Problem>(value));
        }
        else if (key == Str::userId) {
            RootRequirements::requireTeacherRoots(request, session);
            const auto user = session.getById<User>(value);

            if (session.exist(&Session::getWorkResult, workResult->getWork(), user)) {
                throw UnprocessableEntityException("Already exists");
            }

            workResult.modify()->setUser(user);
        }
    }
}

void WorkResultResource::getRequirements(const HttpRequest& request, Session& session) const {
    RootRequirements::requireAuth(request, session);
}

void WorkResultResource::postRequirements(const HttpRequest& request, Session& session) const {
    RootRequirements::requireAuthId(request, session, session.getById<User>(request.body().at(Str::userId)));
}

void WorkResultResource::getIdRequirements(const HttpRequest& request, Session& session, 
    const Ptr<WorkResult>& workResult) const {
    RootRequirements::requireAuth(request, session);
}

void WorkResultResource::deleteRequirements(const HttpRequest& request, Session& session, 
    const Ptr<WorkResult>& workResult) const {
    RootRequirements::requireTeacherRoots(request, session);
}

void WorkResultResource::processGetMethod(const HttpRequest& request, Wt::Json::Object& response, Session& session, 
    const Ptr<WorkResult>& workResult, const std::string& method) const {
    if (method == Str::problemId) {
        RootRequirements::requireTeacherRoots(request, session);
        response[Str::problemId] = workResult->getProblem().id();
    }
    else if (method == Str::statement) {
        const auto caller = session.getByToken<User>(request.token());

        if (caller->getUserType() == UserType::Student && caller != workResult->getUser()) {
            throw ForbiddenException("");
        }

        response[Str::statement] = workResult->getProblem()->getStatement();
    }
    else if (method == Str::mark) {
        const auto caller = session.getByToken<User>(request.token());

        if (caller->getUserType() == UserType::Student && caller != workResult->getUser()) {
            throw ForbiddenException("");
        }

        response[Str::mark] = workResult->getMark();
    }
    else {
        throw NotFoundException("");
    }
}
