#include "resource.h"
#include "session.h"
#include "request.h"
#include "user.h"
#include "str.h"
#include "http_exceptions.h"

void Resource::handleRequest(const Wt::Http::Request& request, Wt::Http::Response& response) {
    Wt::Json::Object responseContent;

    try {
        Session session;
        Wt::Dbo::Transaction tr(session);
        response.setMimeType("application/json");
        const auto method = request.method();
        HttpRequest requestContent(request);
        const auto path = split(request.pathInfo().substr(1));
        response.setStatus(200);

        if (path.empty()) {
            if (method == "GET") {
                processGet(requestContent, responseContent, session);
            }
            else if (method == "POST") {
                processPost(requestContent, responseContent, session, tr);
                response.setStatus(201);
            }
            else {
                throw InvalidMethodException("");
            }
        }
        else if (path.size() == 1) {
            const auto id = getId(path); 

            if (method == "GET") {
                processGetId(requestContent, responseContent, session, id);
            }
            else if (method == "PATCH") {
                processPatch(requestContent, responseContent, session, id);
            }
            else if (method == "DELETE") {
                processDelete(requestContent, responseContent, session, id);
                response.setStatus(204);
            }
            else {
                throw InvalidMethodException("");
            }
        }
        else {
            if (method == "POST") {
                processPostMethod(requestContent, responseContent, session, getId(path), getMethod(path));
            }
            else {
                throw InvalidMethodException("");
            }
        }
    }
    catch (const HttpException& e) {
        response.setStatus(e.code());
        responseContent.clear();
        responseContent[Str::error] = e.what();
    }
    catch (const std::exception& e) {
        response.setStatus(500);
        responseContent.clear();
        responseContent[Str::error] = e.what();
    }

    response.out() << Wt::Json::serialize(responseContent);
}

void Resource::processGet(const HttpRequest& request, Wt::Json::Object& response, Session& session) const {
    throw InvalidMethodException("");
}

void Resource::processPost(const HttpRequest& request, Wt::Json::Object& response, Session& session, Wt::Dbo::Transaction& tr) const {
    throw InvalidMethodException("");
}

void Resource::processPatch(const HttpRequest& request, Wt::Json::Object& response, Session& session, const int id) const {
    throw InvalidMethodException("");
}

void Resource::processDelete(const HttpRequest& request, Wt::Json::Object& response, Session& session, const int id) const {
    throw InvalidMethodException("");
}

Wt::Dbo::ptr<User> Resource::checkAuth(const HttpRequest& request, Session& session) {
    if (!session.exist(&Session::getByToken<User>, request.token())) {
        throw AuthException("");
    }

    return session.getByToken<User>(request.token());
}

Wt::Dbo::ptr<User> Resource::checkTeacher(const HttpRequest& request, Session& session) {
    const auto caller = checkAuth(request, session);

    if (caller->getUserType() == UserType::Student) {
        throw ForbiddenException("Not teacher");
    }

    return caller;
}

Wt::Dbo::ptr<User> Resource::checkAdmin(const HttpRequest& request, Session& session) {
    const auto caller = checkAuth(request, session);

    if (caller->getUserType() != UserType::Admin) {
        throw ForbiddenException("Not admin");
    }

    return caller;
}

Wt::Dbo::ptr<User> Resource::checkAuthId(const HttpRequest& request, Session& session, const int id) {
    const auto caller = checkAuth(request, session);

    if (caller.id() != id && caller->getUserType() != UserType::Admin) {
        throw ForbiddenException("Don't have access to this user");
    }

    return caller;
}

void Resource::processGetId(const HttpRequest& request, Wt::Json::Object& response, Session& session, const int id) const {
    throw InvalidMethodException("");
}

void Resource::processPostMethod(const HttpRequest& request, Wt::Json::Object& response, Session& session, const int id, 
    const std::string& method) const {
    throw InvalidMethodException("");
}

std::vector<std::string> Resource::split(const std::string& str) {
    std::string buf;
    std::vector<std::string> result;

    for (const auto& letter : str) {
        if (letter == '/') {
            result.emplace_back(buf);
            buf.clear();
        }
        else {
            buf.push_back(letter);
        }
    }

    if (!buf.empty()) {
        result.emplace_back(buf);
    }

    return result;
}

int Resource::getId(const std::vector<std::string>& path) {
    return std::stoi(path[0]);
}

const std::string& Resource::getMethod(const std::vector<std::string>& path) {
    return path[2];
}
