#include "work_result_resource.h"
#include "root_requirements.h"
#include "solutions.h"

void WorkResultResource::processPatch(const HttpRequest& request, Session& session, const Ptr<WorkResult>& workResult) const {
    for (const auto& [key, value] : request.body()) {
        if (key == Str::filename) {
            RootRequirements::requireAuthId(request, session, workResult->getSemesterResult()->getUser());
            const auto* solution = request.file(Str::solution);

            if (!solution) {
                throw BadRequestException("No solution file");
            }

            workResult.modify()->setFilename(static_cast<std::string>(value));
            solution->stealSpoolFile();
            std::rename(solution->spoolFileName().c_str(), Solutions::getSolutionDirectory(workResult).c_str());
        }
        else if (key == Str::mark) {
            RootRequirements::requireTeacherRoots(request, session);
            workResult.modify()->setMark(value);
        }
        else if (key == Str::workId) {
            RootRequirements::requireTeacherRoots(request, session);
            auto work = session.load<Work>(value);

            if (session.exist(&Session::getWorkResult, work, workResult->getSemesterResult())) {
                throw UnprocessableEntityException("Already exists");
            }

            workResult.modify()->setWork(std::move(work));
        }
        else if (key == Str::problemId) {
            RootRequirements::requireTeacherRoots(request, session);
            workResult.modify()->setProblem(session.load<Problem>(value));
        }
        else if (key == Str::semesterResultId) {
            RootRequirements::requireTeacherRoots(request, session);
            auto semesterResult = session.load<SemesterResult>(value);

            if (session.exist(&Session::getWorkResult, workResult->getWork(), semesterResult)) {
                throw UnprocessableEntityException("Already exists");
            }

            workResult.modify()->setSemesterResult(std::move(semesterResult));
        }
    }
}

void WorkResultResource::getRequirements(const HttpRequest& request, Session& session) const {
    RootRequirements::requireAuth(request);
}

void WorkResultResource::postRequirements(const HttpRequest& request, Session& session) const {
    RootRequirements::requireAuthId(request, session, session.load<User>(request.body().at(Str::userId)));
}

void WorkResultResource::getIdRequirements(const HttpRequest& request, Session& session, 
    const Ptr<WorkResult>& workResult) const {
    RootRequirements::requireAuth(request);
}

void WorkResultResource::deleteRequirements(const HttpRequest& request, Session& session, 
    const Ptr<WorkResult>& workResult) const {
    RootRequirements::requireTeacherRoots(request, session);
}

void WorkResultResource::processGetMethod(const HttpRequest& request, Wt::Http::Response& response, Session& session, 
    const Ptr<WorkResult>& workResult, const std::string& method) const {
    Wt::Json::Object json;

    if (method == Str::problemId) {
        RootRequirements::requireTeacherRoots(request, session);
        json[Str::problemId] = workResult->getProblem().id();
    }
    else if (method == Str::statement) {
        RootRequirements::requireAuthId(request, session, workResult->getSemesterResult()->getUser());
        json[Str::statement] = workResult->getProblem()->getStatement().c_str();
    }
    else if (method == Str::mark) {
        RootRequirements::requireAuthId(request, session, workResult->getSemesterResult()->getUser());
        json[Str::mark] = workResult->getMark();
    }
    else {
        throw NotFoundException("");
    }

    response.out() << Wt::Json::serialize(json);
}
