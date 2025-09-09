#include "problem_resource.h"
#include "root_requirements.h"
#include "subject_type.h"

void ProblemResource::getRequirements(const HttpRequest& request, Session& session) const {
    RootRequirements::requireTeacherRoots(request, session);
}

void ProblemResource::postRequirements(const HttpRequest& request, Session& session) const {
    RootRequirements::requireTeacherRoots(request, session);
}

void ProblemResource::getIdRequirements(const HttpRequest& request, Session& session, const Ptr<Problem>& problem) const {
    RootRequirements::requireTeacherRoots(request, session);
}

void ProblemResource::deleteRequirements(const HttpRequest& request, Session& session, const Ptr<Problem>& problem) const {
    RootRequirements::requireTeacherRoots(request, session);
}

void ProblemResource::processPatch(const HttpRequest& request, Session& session, const Ptr<Problem>& problem) const {
    RootRequirements::requireTeacherRoots(request, session);

    for (const auto& [key, value] : request.body()) {
        if (key == Str::name) {
            auto name = static_cast<std::string>(value);

            if (session.exist(&Session::getByName<Problem>, name)) {
                throw UnprocessableEntityException("Already exists");
            }

            problem.modify()->setName(name);
        }
        else if (key == Str::statement) {
            problem.modify()->setStatement(static_cast<std::string>(value));
        }
        else if (key == Str::subject) {
            problem.modify()->setSubject(JsonFunctions::parse<Subject::Type>(value));
        }
        else if (key == Str::semesterNumber) {
            problem.modify()->setSemesterNumber(value);
        }
    }
}

Ptr<Problem> ProblemResource::createObject(const Wt::Json::Object& json, Session& session) const {
    auto name = static_cast<std::string>(json.at(Str::name));
    auto statement = static_cast<std::string>(json.at(Str::statement));
    const auto subject = JsonFunctions::parse<Subject::Type>(json.at(Str::subject));
    const auto semesterNumber = static_cast<int>(json.at(Str::semesterNumber));

    if (session.exist(&Session::getByName<Problem>, name)) {
        throw UnprocessableEntityException("Problem already exists");
    }

    return session.add(std::make_unique<Problem>(std::move(name), std::move(statement), subject, semesterNumber));
}
