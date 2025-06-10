#include "server.h"
#include "user_resource.h"
#include "group_resource.h"
#include "problem_resource.h"
#include "work_resource.h"
#include "work_result_resource.h"
#include "session.h"

Server::Server(const std::string& applicationPath)
: Wt::WServer(applicationPath) {
    const std::vector<std::string> args = {applicationPath, "--docroot", ".", "--http-address", "127.0.0.1", "--http-port", "8080"};
    setServerConfiguration(applicationPath, args, WTHTTP_CONFIGURATION);
    Session::configureDatabase();
    
    addResource<UserResource>("/api/user");
    //addResource<GroupResource>("/api/group");
    //addResource<ProblemResource>("/api/problem");
    //addResource<WorkResource>("/api/work");
    //addResource<WorkResultResource>("/api/work_result");
}
