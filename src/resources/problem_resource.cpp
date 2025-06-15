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
            if (session.exist(&Session::getProblem, problem->getSubject(), problem->getSemester(), problem->getWorkNumber(), value)) {
                throw UnprocessableEntityException("Already exists");
            }

            problem.modify()->setName(value);
        }
        else if (key == Str::statement) {
            problem.modify()->setStatement(value);
        }
        else if (key == Str::subject) {
            const auto subject = JsonFunctions::parse<Subject::Type>(value);

            if (session.exist(&Session::getProblem, subject, problem->getSemester(), problem->getWorkNumber(), problem->getName())) {
                throw UnprocessableEntityException("Already exists");
            }

            problem.modify()->setSubject(subject);
        }
        else if (key == Str::semester) {
            if (session.exist(&Session::getProblem, problem->getSubject(), value, problem->getWorkNumber(), problem->getName())) {
                throw UnprocessableEntityException("Already exists");
            }

            problem.modify()->setSemester(value);
        }
        else if (key == Str::workNumber) {
            if (session.exist(&Session::getProblem, problem->getSubject(), problem->getSemester(), value, problem->getName())) {
                throw UnprocessableEntityException("Already exists");
            }

            problem.modify()->setWorkNumber(value);
        }
    }
}
