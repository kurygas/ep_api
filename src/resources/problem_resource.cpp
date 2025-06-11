#include "problem_resource.h"
#include "root_requirements.h"
#include "subject_type.h"

void ProblemResource::getRequirements(const HttpRequest& request, Session& session) const {
    RootRequirements::requireTeacherRoots(request, session);
}

void ProblemResource::getIdRequirements(const HttpRequest& request, Session& session, const Wt::Dbo::ptr<Problem>& problem) const {
    RootRequirements::requireTeacherRoots(request, session);
}

void ProblemResource::deleteRequirements(const HttpRequest& request, Session& session, const Wt::Dbo::ptr<Problem>& problem) const {
    RootRequirements::requireTeacherRoots(request, session);
}

void ProblemResource::processPatch(const HttpRequest& request, Session& session, const Wt::Dbo::ptr<Problem>& problem) const {
    RootRequirements::requireTeacherRoots(request, session);

    for (const auto& [key, value] : request.body()) {
        if (key == Str::name) {
            if (session.exist(&Session::getByName<Problem>, value)) {
                throw UnprocessableEntityException("Name already exists");
            }

            problem.modify()->setName(value);
        }
        else if (key == Str::statement) {
            problem.modify()->setStatement(value);
        }
        else if (key == Str::subject) {
            problem.modify()->setSubject(JsonFunctions::parse<Subject::Type>(value));
        }
        else if (key == Str::semester) {
            problem.modify()->setSemester(value);
        }
        else if (key == Str::workNumber) {
            problem.modify()->setWorkNumber(value);
        }
    }
}
