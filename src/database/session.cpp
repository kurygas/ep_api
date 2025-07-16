#include "session.h"
#include "random_functions.h"

#include <Wt/Dbo/backend/Sqlite3.h>

Session::Session() {
    setConnection(std::make_unique<Wt::Dbo::backend::Sqlite3>("/home/kuryga/projects/ep_api/database.db"));
    mapClass<User>("users");
    mapClass<Group>("groups");
    mapClass<Problem>("problems");
    mapClass<WorkResult>("work_results");
    mapClass<Work>("works");
    mapClass<Point>("points");
    mapClass<Semester>("semesters");
    mapClass<SemesterResult>("semester_results");
}

void Session::configureDatabase() {
    Session session;

    try {
        session.createTables();
    }
    catch (...) {}

    const Wt::Dbo::Transaction tr(session);
    session.checkAdmin("tg_bot");
}

std::string Session::generateToken() {
    std::string token;

    do {
        token = Random::generateRandomString(16);
    }
    while (exist(&Session::getByToken<User>, token));

    return token;
}

void Session::checkAdmin(const std::string& name) {
    if (!exist(&Session::getByTgUsername<User>, "ADMIN:" + name)) {
        add(User::createAdmin("ADMIN:" + name)).modify()->setToken(generateToken());
    }
}

Ptr<Work> Session::getWork(const Ptr<Semester>& semester, const std::string& name) {
    return getPtr(find<Work>()
        .where("semester_id = ?").bind(semester.id())
        .where("name = ?").bind(name));
}

Ptr<WorkResult> Session::getWorkResult(const Ptr<Work>& work, const Ptr<SemesterResult>& semesterResult) {
    return getPtr(find<WorkResult>()
        .where("work_id = ?").bind(work.id())
        .where("semester_result_id = ?").bind(semesterResult.id()));
}

Ptr<Semester> Session::getSemester(const Subject::Type subject, const int semesterNumber, const Ptr<Group>& group) {
    return getPtr(find<Semester>()
        .where("subject = ?").bind(subject)
        .where("semester_number = ?").bind(semesterNumber)
        .where("group_id = ?").bind(group.id())
    );
}

Ptr<SemesterResult> Session::getSemesterResult(const Ptr<Semester>& semester, const Ptr<User>& user) {
    return getPtr(find<SemesterResult>()
        .where("semester_id = ?").bind(semester.id())
        .where("user_id = ?").bind(user.id())
    );
}

Ptr<Point> Session::getPoint(const Ptr<SemesterResult>& semesterResult, const std::string& reason) {
    return getPtr(find<Point>()
        .where("semester_result_id = ?").bind(semesterResult.id())
        .where("reason = ?").bind(reason)
    );
}
