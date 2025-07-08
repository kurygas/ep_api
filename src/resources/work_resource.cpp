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
        else if (key == Str::subject) {
            const auto subject = JsonFunctions::parse<Subject::Type>(value);

            if (session.exist(&Session::getWork, subject, work->getSemesterNumber(), work->getGroup(), work->getName())) {
                throw UnprocessableEntityException("Already exists");
            }

            work.modify()->setSubject(subject);
        }
        else if (key == Str::semesterNumber) {
            if (session.exist(&Session::getWork, work->getSubject(), value, work->getGroup(), work->getName())) {
                throw UnprocessableEntityException("Already exists");
            }

            work.modify()->setSemesterNumber(value);
        }
        else if (key == Str::groupId) {
            const auto group = session.load<Group>(value);

            if (session.exist(&Session::getWork, work->getSubject(), work->getSemesterNumber(), group, work->getName())) {
                throw UnprocessableEntityException("Already exists");
            }

            work.modify()->setGroup(group);
        }
        else if (key == Str::name) {
            if (session.exist(&Session::getWork, work->getSubject(), work->getSemesterNumber(), work->getGroup(), value)) {
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

void WorkResource::processGetMethod(const HttpRequest& request, Wt::Json::Object& response, Session& session, 
    const Ptr<Work>& work, const std::string& method) const {
    if (method == Str::problemList) {
        RootRequirements::requireTeacherRoots(request, session);
        response[Str::problemList] = JsonFunctions::getIdArray(work->getProblems());
    }
    else {
        throw NotFoundException("Unknown method");
    }
}
