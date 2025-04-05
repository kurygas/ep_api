#include "server.h"
#include "user_resources.h"
#include "group_resources.h"
#include "session.h"

Server::Server(const int argc, char** const argv)
: Wt::WServer(argc, argv, WTHTTP_CONFIGURATION) {
    Session().configureDatabase();
    
    addResource<UserResources::RegistrationResource>("/user/registration");
    addResource<UserResources::AuthResource>("/user/auth");
    addResource<UserResources::MakeTeacherResource>("/user/make_teacher");
    addResource<UserResources::GetUserInfoResource>("/user/get_info");
    addResource<UserResources::DeleteUserResource>("/user/delete");
    addResource<UserResources::UpdateTgUsernameResource>("/user/update/tg_username");
    addResource<UserResources::UpdateFirstNameResource>("/user/update/first_name");
    addResource<UserResources::UpdateSecondNameResource>("/user/update/second_name");
    addResource<UserResources::UpdateEmailResource>("/user/update/email");
    addResource<UserResources::GetAllUsersResource>("/user/get_all");

    addResource<GroupResources::CreateGroupResource>("/group/create");
    addResource<GroupResources::AddUserResource>("/group/add_user");
}