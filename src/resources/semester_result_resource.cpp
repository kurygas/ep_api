#include "semester_result_resource.h"
#include "root_requirements.h"

void SemesterResultResource::processPatch(const HttpRequest &request, Session& session, const Ptr<SemesterResult>& semesterResult) const {
    RootRequirements::requireTeacherRoots(request, session);

    for (const auto& [key, value] : request.body()) {
        if (key == Str::semesterId) {
            const auto semester = session.getById<Semester>(value);

            if (session.exist(&Session::getSemesterResult, semester, semesterResult->getUser())) {
                throw UnprocessableEntityException("Already exists");
            }

            semesterResult.modify()->setSemester(semester);
        }
        else if (key == Str::userId) {
            const auto user = session.getById<User>(value);

            if (session.exist(&Session::getSemesterResult, semesterResult->getSemester(), user)) {
                throw UnprocessableEntityException("Already exists");
            }

            semesterResult.modify()->setUser(user);
        }
    }
}

void SemesterResultResource::getRequirements(const HttpRequest &request, Session &session) const {
    RootRequirements::requireAuth(request, session);
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
