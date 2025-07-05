#include "point_resource.h"
#include "root_requirements.h"

void PointResource::getRequirements(const HttpRequest& request, Session& session) const {
    RootRequirements::requireAuth(request, session);
}

void PointResource::postRequirements(const HttpRequest& request, Session& session) const {
    RootRequirements::requireTeacherRoots(request, session);
}

void PointResource::getIdRequirements(const HttpRequest& request, Session& session, const Ptr<Point>& point) const {
    RootRequirements::requireAuthId(request, session, point->getSemesterResult()->getUser());
}

void PointResource::deleteRequirements(const HttpRequest& request, Session& session, const Ptr<Point>& point) const {
    RootRequirements::requireTeacherRoots(request, session);
}

void PointResource::processPatch(const HttpRequest& request, Session& session, const Ptr<Point>& point) const {
    RootRequirements::requireTeacherRoots(request, session);

    for (const auto& [key, value] : request.body()) {
        if (key == Str::reason) {
            point.modify()->setReason(value);
        }
        else if (key == Str::amount) {
            point.modify()->setAmount(value);
        }
        else if (key == Str::semesterResultId) {
            point.modify()->setSemesterResult(session.getById<SemesterResult>(value));
        }
    }
}
