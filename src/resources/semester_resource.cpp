#include "semester_resource.h"
#include "root_requirements.h"

void SemesterResource::processPatch(const HttpRequest& request, Session& session, const Ptr<Semester>& semester) const {
    RootRequirements::requireTeacherRoots(request, session);

    for (const auto& [key, value] : request.body()) {
        if (key == Str::semesterNumber) {
            if (session.exist(&Session::getSemester, semester->getSubject(), value, semester->getGroup())) {
                throw UnprocessableEntityException("Already exists");
            }

            semester.modify()->setSemesterNumber(value);
        }
        else if (key == Str::subject) {
            const auto subject = JsonFunctions::parse<Subject::Type>(value);

            if (session.exist(&Session::getSemester, subject, semester->getSemesterNumber(), semester->getGroup())) {
                throw UnprocessableEntityException("Already exists");
            }

            semester.modify()->setSubject(subject);
        }
        else if (key == Str::start) {
            semester.modify()->setStart(Wt::WDateTime::fromTime_t(value));
        }
        else if (key == Str::end) {
            semester.modify()->setEnd(Wt::WDateTime::fromTime_t(value));
        }
        else if (key == Str::groupId) {
            const auto group = session.load<Group>(value);

            if (session.exist(&Session::getSemester, semester->getSubject(), semester->getSemesterNumber(), group)) {
                throw UnprocessableEntityException("Already exists");
            }

            semester.modify()->setGroup(group);
        }
        else if (key == Str::cfMaxPoint) {
            semester.modify()->setCfMaxPoint(value);
        }
        else if (key == Str::atcMaxPoint) {
            semester.modify()->setAtcMaxPoint(value);
        }
    }
}

void SemesterResource::getRequirements(const HttpRequest& request, Session& session) const {
    RootRequirements::requireAuth(request, session);
}

void SemesterResource::postRequirements(const HttpRequest& request, Session& session) const {
    RootRequirements::requireTeacherRoots(request, session);
}

void SemesterResource::getIdRequirements(const HttpRequest& request, Session& session, const Ptr<Semester>& semester) const {
    RootRequirements::requireAuth(request, session);
}

void SemesterResource::deleteRequirements(const HttpRequest& request, Session& session, const Ptr<Semester>& semester) const {
    RootRequirements::requireTeacherRoots(request, session);
}


