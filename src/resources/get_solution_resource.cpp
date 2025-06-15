#include "get_solution_resource.h"
#include "utility_functions.h"
#include "str.h"
#include "http_exceptions.h"
#include "session.h"
#include "json.h"

#include <fstream>

void GetSolutionResource::handleRequest(const Wt::Http::Request& request, Wt::Http::Response& response) {
    try {
        const auto path = Utility::split(request.pathInfo().substr(1));

        if (request.method() == "GET" && path.size() == 1) {
            Session session;
            const Wt::Dbo::Transaction tr(session);
            const auto workResult = session.getById<WorkResult>(Utility::getId(path));
            std::ifstream file(workResult->getSolutionPath(), std::ios::binary);
            response.out() << file.rdbuf();
        }
        else {
            throw NotFoundException("");
        }
    }
    catch (const HttpException& e) {
        response.setStatus(e.code());
        Wt::Json::Object responseContent;
        responseContent[Str::error] = e.what();
        response.out() << Wt::Json::serialize(responseContent);
    }
    catch (const std::exception& e) {
        response.setStatus(500);
        Wt::Json::Object responseContent;
        responseContent[Str::error] = e.what();
        response.out() << Wt::Json::serialize(responseContent);
    }
}
