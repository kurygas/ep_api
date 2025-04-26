#include "server.h"
#include "user_resources.h"
#include "group_resources.h"
#include "session.h"

Server::Server(const int argc, char** const argv)
: Wt::WServer(argc, argv, WTHTTP_CONFIGURATION) {
    Session().configureDatabase();
    
    addResource<UserResources::Create>("/user/create");
    addResource<UserResources::Auth>("/user/auth");
    addResource<UserResources::MakeAdmin>("/user/make_admin");
    addResource<UserResources::Get>("/user");
    addResource<UserResources::Delete>("/user/delete");
    addResource<UserResources::SetTgUsername>("/user/set/tg_username");
    addResource<UserResources::SetFirstName>("/user/set/first_name");
    addResource<UserResources::SetSecondName>("/user/set/second_name");
    addResource<UserResources::GetAllId>("/user/all_id");

    addResource<GroupResources::Create>("/group/create");
}
