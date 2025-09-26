#include "point_resource.h"
#include "root_requirements.h"

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
            point.modify()->setReason(static_cast<std::string>(value));
        }
        else if (key == Str::amount) {
            point.modify()->setAmount(value);
        }
        else if (key == Str::semesterResultId) {
            point.modify()->setSemesterResult(session.load<SemesterResult>(value));
        }
    }
}

Ptr<Point> PointResource::createObject(const Wt::Json::Object& json, Session& session) const {
    auto reason = static_cast<std::string>(json.at(Str::reason));
    const auto amount = static_cast<int>(json.at(Str::amount));
    auto semesterResult = session.load<SemesterResult>(json.at(Str::semesterResultId));
    return session.add(std::make_unique<Point>(std::move(reason), amount, std::move(semesterResult)));
}
