#include "work_resource.h"
#include "root_requirements.h"

void WorkResource::processPatch(const HttpRequest& request, Session& session, const Wt::Dbo::ptr<Work>& work) const {
    RootRequirements::requireTeacherRoots(request, session);

    for (const auto& [key, value] : request.body()) {
        if (key == Str::name) {
            if (session.exist(&Session::getByName<Work>, value)) {
                throw UnprocessableEntityException("Name already exists");
            }

            work.modify()->setName(value);
        }
        else if (key == Str::start) {
            work.modify()->setStart(Wt::WDateTime::fromString(value));
        }
        else if (key == Str::end) {
            work.modify()->setEnd(Wt::WDateTime::fromString(value));
        }
        else if (key == Str::subject) {
            work.modify()->setSubject(JsonFunctions::parse<Subject::Type>(value));
        }
        else if (key == Str::semester) {
            work.modify()->setSemester(value);
        }
        else if (key == Str::workNumber) {
            work.modify()->setWorkNumber(value);
        }
        else if (key == Str::groupId) {
            work.modify()->setGroup(session.getById<Group>(value));
        }
        else if (key == Str::problemList) {
            work.modify()->setProblems(session.getByArray<Problem>(value));
        }
    }
}

void WorkResource::getRequirements(const HttpRequest& request, Session& session) const {
    RootRequirements::requireAuth(request, session);
}

void WorkResource::getIdRequirements(const HttpRequest& request, Session& session, const Wt::Dbo::ptr<Work>& work) const {
    RootRequirements::requireAuth(request, session);
}

void WorkResource::deleteRequirements(const HttpRequest& request, Session& session, const Wt::Dbo::ptr<Work>& work) const {
    RootRequirements::requireTeacherRoots(request, session);
}

void WorkResource::processGetMethod(const HttpRequest& request, Wt::Json::Object& response, Session& session, 
    const Wt::Dbo::ptr<Work>& work, const std::string& method) const {
    if (method == Str::problemList) {
        RootRequirements::requireTeacherRoots(request, session);
        response[Str::problemList] = JsonFunctions::getIdArray(work->getProblems());
    }
    else {
        throw NotFoundException("unknown method");
    }
}
