#include "session.h"
#include "user.h"
#include "group.h"
#include "problem.h"
#include "work_result.h"
#include "work.h"
#include "random_functions.h"

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

Wt::Dbo::ptr<Problem> Session::addProblem(const Wt::WString& name, const Wt::WString& statement, const SubjectType subject, const int semester, 
    const int workNumber) {
    if (exist(&Session::getProblemByName, name)) {
        throw std::runtime_error("Problem name already exists");
    }

    return add(std::make_unique<Problem>(name, statement, subject, semester, workNumber));
}

Wt::Dbo::ptr<User> Session::getUserByTgId(const Wt::WString& tgId) {
    return getPtr(find<User>().where("tg_id = ?").bind(tgId));
}

Wt::Dbo::ptr<User> Session::getUserByToken(const Wt::WString& token) {
    return getPtr(find<User>().where("token = ?").bind(token));
}

Wt::Dbo::ptr<Group> Session::getGroupByName(const Wt::WString& groupName) {
    return getPtr(find<Group>().where("group_name = ?").bind(groupName));
}

Wt::Dbo::ptr<Group> Session::getGroupById(const int groupId) {
    return getPtr(find<Group>().where("id = ?").bind(groupId));
}

Wt::Dbo::ptr<Problem> Session::getProblemByName(const Wt::WString& problemName) {
    return getPtr(find<Problem>().where("name = ?").bind(problemName));
}

Wt::Dbo::ptr<Work> Session::getWorkByName(const Wt::WString& workName) {
    return getPtr(find<Work>().where("name = ?").bind(workName));
}

Wt::Dbo::ptr<Work> Session::getWorkByTimeSegment(const Wt::WDateTime& start, const Wt::WDateTime& end) {
    if (start > end) {
        throw std::runtime_error("Invalid time segment");
    }

    const auto works = getAll<Work>();
    auto it = std::find_if(works.begin(), works.end(), [&](const Wt::Dbo::ptr<Work>& x){
        return x->getStart() <= end && x->getEnd() >= start; 
    });

    if (it == works.end()) {
        throw std::runtime_error("Work in this segment does not exist");
    }

    return *it;
}

Wt::Dbo::ptr<Work> Session::getCurrentWork(const Wt::Dbo::ptr<Group>& group) {
    const auto currentTime = Wt::WDateTime::currentDateTime();
    return getWorkByTimeSegment(currentTime, currentTime);
}

Wt::Dbo::ptr<User> Session::addUser(const Wt::WString& tgId, const Wt::WString& tgUsername, const Wt::WString& password, const Wt::WString& firstName, 
    const Wt::WString& secondName, const Wt::WString& email) {
    if (exist(&Session::getUserByTgId, tgId)) {
        throw std::runtime_error("User already exists");
    }

    return add(std::make_unique<User>(tgId, tgUsername, password, firstName, secondName, email, generateToken()));
}

Wt::Dbo::ptr<Group> Session::addGroup(const Wt::WString& groupName) {
    if (exist(&Session::getGroupByName, groupName)) {
        throw std::runtime_error("Group already exists");
    }

    return add(std::make_unique<Group>(groupName));
}

Wt::Dbo::ptr<Work> Session::addWork(const Wt::WString& name, const Wt::WDateTime& start, const Wt::WDateTime& end, const SubjectType subject, 
    const int semester, const int workNumber) {
    if (exist(&Session::getWorkByName, name) || exist(&Session::getWorkByTimeSegment, start, end)) {
        throw std::runtime_error("Invalid work");
    }

    return add(std::make_unique<Work>(name, start, end, subject, semester, workNumber));
}

Wt::Dbo::ptr<WorkResult> Session::addWorkResult(const Wt::WString& token) {
    auto user = getUserByToken(token);
    auto work = getCurrentWork(user->getGroup());
    auto problem = Random::pickRandom(work->getProblemSet());
    return add(std::make_unique<WorkResult>(work, problem, user));
}
