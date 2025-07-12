#include "work_resource.h"
#include "root_requirements.h"

void WorkResource::processPatch(const HttpRequest& request, Session& session, const Ptr<Work>& work) const {
    RootRequirements::requireTeacherRoots(request, session);

    for (const auto& [key, value] : request.body()) {
        if (key == Str::start) {
            work.modify()->setStart(Wt::WDateTime::fromString(value));
        }
        else if (key == Str::end) {
            work.modify()->setEnd(Wt::WDateTime::fromString(value));
        }
        else if (key == Str::semesterId) {
            const auto semester = session.load<Semester>(value);

            if (session.exist(&Session::getWork, semester, work->getName())) {
                throw UnprocessableEntityException("Already exists");
            }

            work.modify()->setSemester(semester);
        }
        else if (key == Str::name) {
            if (session.exist(&Session::getWork, work->getSemester(), value)) {
                throw UnprocessableEntityException("Already exists");
            }

            work.modify()->setName(value);
        }
        else if (key == Str::problemList) {
            work.modify()->setProblems(session.getByArray<Problem>(value));
        }
        else if (key == Str::isExam) {
            work.modify()->setIsExam(value);
        }
    }
}

void WorkResource::getRequirements(const HttpRequest& request, Session& session) const {
    RootRequirements::requireAuth(request, session);
}

void WorkResource::postRequirements(const HttpRequest& request, Session& session) const {
    RootRequirements::requireTeacherRoots(request, session);
}

void WorkResource::getIdRequirements(const HttpRequest& request, Session& session, const Ptr<Work>& work) const {
    RootRequirements::requireAuth(request, session);
}

void WorkResource::deleteRequirements(const HttpRequest& request, Session& session, const Ptr<Work>& work) const {
    RootRequirements::requireTeacherRoots(request, session);
}

void WorkResource::processGetMethod(const HttpRequest& request, Wt::Http::Response& response, Session& session, const Ptr<Work>& work, 
    const std::string& method) const {
    if (method == Str::problemList) {
        RootRequirements::requireTeacherRoots(request, session);
        response.out() << Wt::Json::serialize(JsonFunctions::getIdArray(work->getProblems()));
    }
    else {
        throw NotFoundException("Unknown method");
    }
}
