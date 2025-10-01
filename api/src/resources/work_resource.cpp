#include "work_resource.h"
#include "root_requirements.h"

void WorkResource::processPatch(const HttpRequest& request, Session& session, const Ptr<Work>& work) const {
    RootRequirements::requireTeacherRoots(request, session);

    for (const auto& [key, value] : request.body()) {
        if (key == Str::start) {
            work.modify()->setStart(Wt::WDateTime::fromTime_t(value));
        }
        else if (key == Str::end) {
            work.modify()->setEnd(Wt::WDateTime::fromTime_t(value));
        }
        else if (key == Str::semesterId) {
            auto semester = session.load<Semester>(value);

            if (session.exist(&Session::getWork, semester, work->getName())) {
                throw UnprocessableEntityException("Already exists");
            }

            work.modify()->setSemester(std::move(semester));
        }
        else if (key == Str::name) {
            auto name = static_cast<std::string>(value);

            if (session.exist(&Session::getWork, work->getSemester(), name)) {
                throw UnprocessableEntityException("Already exists");
            }

            work.modify()->setName(std::move(name));
        }
        else if (key == Str::problemList) {
            work.modify()->setProblems(session.getByArray<Problem>(value));
        }
        else if (key == Str::isExam) {
            work.modify()->setIsExam(value);
        }
    }
}

void WorkResource::postRequirements(const HttpRequest& request, Session& session) const {
    RootRequirements::requireTeacherRoots(request, session);
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

Ptr<Work> WorkResource::createObject(const Wt::Json::Object& json, Session& session) const {
    auto name = static_cast<std::string>(json.at(Str::name));
    const auto start = Wt::WDateTime::fromTime_t(json.at(Str::start));
    const auto end = Wt::WDateTime::fromTime_t(json.at(Str::end));
    auto semester = session.load<Semester>(json.at(Str::semesterId));
    const auto isExam = static_cast<bool>(json.at(Str::isExam));
    
    if (session.exist(&Session::getWork, semester, name)) {
        throw UnprocessableEntityException("Work already exists");
    }

    return session.add(std::make_unique<Work>(std::move(name), start, end, std::move(semester), isExam));
}
