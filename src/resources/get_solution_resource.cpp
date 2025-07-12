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
            const auto workResult = session.load<WorkResult>(Utility::getId(path));
            std::ifstream file(Utility::getFilepath(workResult.id()), std::ios::binary);

            if (!file.is_open()) {
                throw NotFoundException("");
            }

            response.out() << file.rdbuf();
        }
        else {
            throw NotFoundException("");
        }
    }
    catch (const HttpException& e) {
        processException(response, e, e.code());
    }
    catch (const std::exception& e) {
        processException(response, e, 500);
    }
}

void GetSolutionResource::processException(Wt::Http::Response& response, const std::exception& e, const int code) const {
    response.setStatus(code);
    Wt::Json::Object responseContent;
    responseContent[Str::error] = e.what();
    response.out() << Wt::Json::serialize(responseContent);
}
