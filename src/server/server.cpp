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
    addResource<GetUserInfoResource>("/user/get_info");
    addResource<DeleteUserResource>("/user/delete");
    addResource<UpdateTgUsernameResource>("/user/update/tg_username");
    addResource<UpdateFirstNameResource>("/user/update/first_name");
    addResource<UpdateSecondNameResource>("/user/update/second_name");
    addResource<UpdateEmailResource>("/user/update/email");
    addResource<GetAllUsersResource>("/user/get_all");

    addResource<CreateGroupResource>("/group/create");
}