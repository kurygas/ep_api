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
}

void Session::configureDatabase() {
    Session session;

    try {
        session.createTables();
    }
    catch (...) {}

    const Wt::Dbo::Transaction transaction(session);
    
    if (!session.exist(&Session::getByTgId<User>, "admin")) {
        session.add(User::createAdmin());
    }
}

Wt::WString Session::generateToken() {
    Wt::WString token;

    do {
        token = Random::generateRandomString(16);
    }
    while (exist(&Session::getByToken<User>, token));

    return token;
}
