#include "server.h"
#include "user_resource.h"
#include "group_resource.h"
#include "problem_resource.h"
#include "work_resource.h"
#include "work_result_resource.h"
#include "session.h"
#include "get_solution_resource.h"
#include "point_resource.h"
#include "cf_service.h"
#include "semester_resource.h"
#include "semester_result_resource.h"

#include <fstream>
#include <sstream>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/property_tree/ptree.hpp>

Server::Server(const std::string& applicationPath)
: Wt::WServer(applicationPath) {
    const std::vector<std::string> args = {applicationPath, "--docroot", ".", "--http-address", "127.0.0.1", "--http-port", "8080"};
    setServerConfiguration(applicationPath, args, WTHTTP_CONFIGURATION);
    Session::configureDatabase();
    
    addResource<UserResource>("/api/user");
    addResource<GroupResource>("/api/group");
    addResource<ProblemResource>("/api/problem");
    addResource<WorkResource>("/api/work");
    addResource<WorkResultResource>("/api/work_result");
    addResource<GetSolutionResource>("/api/solution");
    addResource<PointResource>("/api/point");
    addResource<SemesterResource>("/api/semester");
    addResource<SemesterResultResource>("api/semester_result");

    pullConfig();
}

void Server::pullConfig() {
    boost::property_tree::ptree pt;
    boost::property_tree::read_ini("/home/kuryga/projects/ep_api/config.ini", pt);
    Str::botToken = pt.get<std::string>("tokens.bot");
    CfService::key = pt.get<std::string>("cf.key");
    CfService::secret = pt.get<std::string>("cf.secret");
}
