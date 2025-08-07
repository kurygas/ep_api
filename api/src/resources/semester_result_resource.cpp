#include "semester_result_resource.h"
#include "root_requirements.h"
#include "cf_service.h"
#include "atc_service.h"

void SemesterResultResource::processPatch(const HttpRequest &request, Session& session, const Ptr<SemesterResult>& semesterResult) const {
    RootRequirements::requireTeacherRoots(request, session);

    for (const auto& [key, value] : request.body()) {
        if (key == Str::semesterId) {
            auto semester = session.load<Semester>(value);

            if (session.exist(&Session::getSemesterResult, semester, semesterResult->getUser())) {
                throw UnprocessableEntityException("Already exists");
            }

            semesterResult.modify()->setSemester(std::move(semester));
        }
        else if (key == Str::userId) {
            auto user = session.load<User>(value);

            if (session.exist(&Session::getSemesterResult, semesterResult->getSemester(), user)) {
                throw UnprocessableEntityException("Already exists");
            }

            semesterResult.modify()->setUser(std::move(user));
        }
    }
}

void SemesterResultResource::getRequirements(const HttpRequest &request, Session &session) const {
    RootRequirements::requireAuth(request);
}

void SemesterResultResource::postRequirements(const HttpRequest& request, Session& session) const {
    RootRequirements::requireTeacherRoots(request, session);
}

void SemesterResultResource::getIdRequirements(const HttpRequest& request, Session& session, 
    const Ptr<SemesterResult>& semesterResult) const {
    RootRequirements::requireAuthId(request, session, semesterResult->getUser());
}

void SemesterResultResource::deleteRequirements(const HttpRequest& request, Session& session, 
    const Ptr<SemesterResult>& semesterResult) const {
        RootRequirements::requireTeacherRoots(request, session);
}
