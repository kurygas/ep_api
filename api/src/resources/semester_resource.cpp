#include "semester_resource.h"
#include "root_requirements.h"
#include "semester_result.h"
#include "message_queue.h"

void SemesterResource::processPatch(const HttpRequest& request, Session& session, const Ptr<Semester>& semester) const {
    RootRequirements::requireTeacherRoots(request, session);

    for (const auto& [key, value] : request.body()) {
        if (key == Str::semesterNumber) {
            auto semesterNumber = static_cast<int>(value);

            if (session.exist(&Session::getSemester, semester->getSubject(), semesterNumber, semester->getGroup())) {
                throw UnprocessableEntityException("Already exists");
            }

            semester.modify()->setSemesterNumber(semesterNumber);
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
            auto group = session.load<Group>(value);

            if (session.exist(&Session::getSemester, semester->getSubject(), semester->getSemesterNumber(), group)) {
                throw UnprocessableEntityException("Already exists");
            }

            semester.modify()->setGroup(std::move(group));
        }
        else if (key == Str::cfMaxPoint) {
            semester.modify()->setCfMaxPoint(value);
        }
        else if (key == Str::atcRatio) {
            semester.modify()->setAtcRatio(value);
        }
    }
}

void SemesterResource::getRequirements(const HttpRequest& request, Session& session) const {
    RootRequirements::requireAuth(request);
}

void SemesterResource::postRequirements(const HttpRequest& request, Session& session) const {
    RootRequirements::requireTeacherRoots(request, session);
}

void SemesterResource::getIdRequirements(const HttpRequest& request, Session& session, const Ptr<Semester>& semester) const {
    RootRequirements::requireAuth(request);
}

void SemesterResource::deleteRequirements(const HttpRequest& request, Session& session, const Ptr<Semester>& semester) const {
    RootRequirements::requireTeacherRoots(request, session);
}

Ptr<Semester> SemesterResource::createObject(const Wt::Json::Object& json, Session& session) const {
    const auto semesterNumber = static_cast<int>(json.at(Str::semesterNumber));
    const auto subject = JsonFunctions::parse<Subject::Type>(json.at(Str::subject));
    const auto start = Wt::WDateTime::fromTime_t(json.at(Str::start));
    const auto end = Wt::WDateTime::fromTime_t(json.at(Str::end));
    const auto group = session.load<Group>(json.at(Str::groupId));

    if (session.exist(&Session::getSemester, subject, semesterNumber, group)) {
        throw UnprocessableEntityException("Semester already exists");
    }

    const auto semester = session.add(std::make_unique<Semester>(semesterNumber, subject, start, end, group));

    for (const auto& user : group->getUsers()) {
        const auto semesterResult = session.add(std::make_unique<SemesterResult>(semester, user));

        if (subject == Subject::Type::Algo) {
            session.add(std::make_unique<Point>("cf_point", 0, semesterResult));
            session.add(std::make_unique<Point>("atc_point", 0, semesterResult));
        }
    }

    return semester;
}

void SemesterResource::sendUpdatedInfo(const Ptr<Semester>& semester) const {
    auto message = static_cast<Wt::Json::Object>(*semester);
    message[Str::semesterId] = semester.id();
    MessageQueue::getInstance()->publish("algo_data", message);
}

void SemesterResource::sendDeletedInfo(const Ptr<Semester>& semester) const {
    Wt::Json::Object message;
    message[Str::semesterId] = semester.id();
    message["deleted"] = true;
    MessageQueue::getInstance()->publish("algo_data", message);
}
