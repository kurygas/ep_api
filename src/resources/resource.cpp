#include "resource.h"
#include "session.h"
#include "json.h"
#include "user.h"

JsonObject Resource::getBodyContent(const Wt::Http::Request& request) {
    JsonObject bodyContent;

    try {
        Wt::Json::parse(std::string(static_cast<std::istreambuf_iterator<char>>(request.in()), {}), bodyContent);
    }
    catch (...) {}

    return bodyContent;
}

void Resource::handleRequest(const Wt::Http::Request& request, Wt::Http::Response& response) {
    JsonObject responseContent;

    try {
        Session session;
        const Wt::Dbo::Transaction transaction(session);
        processRequest(request.method(), getBodyContent(request), responseContent, session);
    }
    catch (const std::exception& e) {
        response.setStatus(400);
        responseContent.clear();
        responseContent.put("error", e.what());
    }

    response.out() << Wt::Json::serialize(responseContent);
}

void Resource::processRequest(const std::string& method, const JsonObject& requestContent, JsonObject& responseContent, Session& session) const {
    if (method == "GET") {
        processGet(requestContent, responseContent, session);
    }
    else if (method == "POST") {
        processPost(requestContent, responseContent, session);
    }
}

void Resource::processGet(const JsonObject& requestContent, JsonObject& responseContent, Session& session) const {
    throw std::runtime_error("Invalid method");
}

void Resource::processPost(const JsonObject& requestContent, JsonObject& responseContent, Session& session) const {
    throw std::runtime_error("Invalid method");
}

void TokenResource::processRequest(const std::string& method, const JsonObject& requestContent, JsonObject& responseContent, 
    Session& session) const {
    checkToken(requestContent, session);
    Resource::processRequest(method, requestContent, responseContent, session);
}

void TokenResource::checkToken(const JsonObject& requestContent, Session& session) const {
    if (session.getUser(requestContent).modify()->getToken() != requestContent.getString("token")) {
        throw std::runtime_error("Invalid token");
    }
}

void AdminResource::processRequest(const std::string& method, const JsonObject& requestContent, JsonObject& responseContent, 
    Session& session) const {
    checkToken(requestContent, session); 

    if (session.getUser(requestContent)->getUserType() == User::UserType::Student) {
        throw std::runtime_error("No permission");
    }

    Resource::processRequest(method, requestContent, responseContent, session);
}