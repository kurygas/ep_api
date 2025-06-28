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

Wt::WString Session::generateToken() {
    Wt::WString token;

    do {
        token = Random::generateRandomString(16);
    }
    while (exist(&Session::getByToken<User>, token));

    return token;
}

void Session::checkAdmin(const Wt::WString& name) {
    if (!exist(&Session::getByTgUsername<User>, "ADMIN:" + name)) {
        add(User::createAdmin("ADMIN:" + name)).modify()->setToken(generateToken());
    }
}

Ptr<Problem> Session::getProblem(Subject::Type subject, const int semester, const int workNumber, const Wt::WString& name) {
    return getPtr(find<Problem>()
            .where("subject = ?").bind(subject)
            .where("semester = ?").bind(semester)
            .where("work_number = ?").bind(workNumber)
            .where("name = ?").bind(name));
}

Ptr<Work> Session::getWork(const Subject::Type subject, const int semester, const int workNumber, 
    const Ptr<Group>& group) {
    return getPtr(find<Work>()
            .where("subject = ?").bind(subject)
            .where("semester = ?").bind(semester)
            .where("work_number = ?").bind(workNumber)
            .where("group_id = ?").bind(group.id()));
}

Ptr<WorkResult> Session::getWorkResult(const Ptr<Work>& work, const Ptr<User>& user) {
    return getPtr(find<WorkResult>()
            .where("work_id = ?").bind(work.id())
            .where("user_id = ?").bind(user.id()));
}
