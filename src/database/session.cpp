#include "session.h"
#include "user.h"
#include "group.h"
#include "problem.h"
#include "work_result.h"
#include "work.h"
#include "random_functions.h"
#include "http_exceptions.h"

#include <Wt/Dbo/backend/Sqlite3.h>

Session::Session() {
    setConnection(std::make_unique<Wt::Dbo::backend::Sqlite3>("database.db"));
    mapClass<User>("users");
    mapClass<Group>("groups");
    mapClass<Problem>("problems");
    mapClass<WorkResult>("work_results");
    mapClass<Work>("works");
}

void Session::configureDatabase() {
    try {
        createTables();
    }
    catch (...) {}

    const Wt::Dbo::Transaction transaction(*this);
    
    if (!exist(&Session::getUserByTgId, "admin")) {
        add(User::createAdmin());
    }
}

Wt::WString Session::generateToken() {
    Wt::WString token;

    do {
        token = Random::generateRandomString(16);
    }
    while (exist(&Session::getUserByToken, token));

    return token;
}

Wt::Dbo::ptr<Problem> Session::addProblem(const Wt::WString& name, const Wt::WString& statement, const Subject::Type subject, const int semester, 
    const int workNumber) {
    checkName<Problem>(name);
    return add(std::make_unique<Problem>(name, statement, subject, semester, workNumber));
}

Wt::Dbo::ptr<User> Session::addUser(const Wt::WString& tgId, const Wt::WString& tgUsername, const Wt::WString& password, 
    const Wt::WString& name, const Wt::WString& surname, const Wt::WString& email) {
    if (exist(&Session::getUserByTgId, tgId)) {
        throw UnprocessableEntityException("User already exists");
    }

    return add(std::make_unique<User>(tgId, tgUsername, password, name, surname, email, generateToken()));
}

Wt::Dbo::ptr<Group> Session::addGroup(const Wt::WString& name) {
    checkName<Group>(name);
    return add(std::make_unique<Group>(name));
}

Wt::Dbo::ptr<Work> Session::addWork(const Wt::WString& name, const Wt::WDateTime& start, const Wt::WDateTime& end, const Subject::Type subject, 
    const int semester, const int workNumber) {
    checkName<Work>(name);
    return add(std::make_unique<Work>(name, start, end, subject, semester, workNumber));
}

Wt::Dbo::ptr<WorkResult> Session::addWorkResult(int userId, int workId) {
    auto user = getById<User>(userId);
    auto work = getById<Work>(workId);
    auto problem = Random::pickRandom(work->getProblemSet());
    return add(std::make_unique<WorkResult>(work, problem, user));
}
