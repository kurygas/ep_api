#include "server.h"
#include "user_resources.h"
#include "group_resources.h"
#include "session.h"

Server::Server(const int argc, char** const argv)
: Wt::WServer(argc, argv, WTHTTP_CONFIGURATION) {
    Session().configureDatabase();
    
    addResource<RegistrationResource>("user/registration");
    addResource<AuthResource>("/user/auth");
    addResource<MakeTeacherResource>("/user/make_teacher");
    addResource<GetUserResource>("/user/get_user");
    addResource<DeleteUserResource>("/user/delete_user");

    addResource<CreateGroupResource>("/group/create");
}