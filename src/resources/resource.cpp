#include "resource.h"
#include "session.h"
#include "request.h"
#include "user.h"

void Resource::handleRequest(const Wt::Http::Request& request, Wt::Http::Response& response) {
    Wt::Json::Object responseContent;

    try {
        Session session;
        const Wt::Dbo::Transaction transaction(session);
        response.setMimeType("application/json");
        processRequest(HttpRequest(request), responseContent, session);
    }
    catch (const std::exception& e) {
        response.setStatus(400);
        responseContent.clear();
        responseContent["error"] = e.what();
    }

    response.out() << Wt::Json::serialize(responseContent);
}

void Resource::processRequest(const HttpRequest& request, Wt::Json::Object& response, Session& session) const {
    const auto method = request.getMethod();

    if (method == "GET") {
        processGet(request, response, session);
    }
    else if (method == "POST") {
        processPost(request, response, session);
    }
    else {
        throw std::runtime_error("Invalid method");
    }
}

void Resource::processGet(const HttpRequest& request, Wt::Json::Object& response, Session& session) const {
    throw std::runtime_error("Invalid method");
}

void Resource::processPost(const HttpRequest& request, Wt::Json::Object& response, Session& session) const {
    throw std::runtime_error("Invalid method");
}

void TokenResource::processRequest(const HttpRequest& request, Wt::Json::Object& response, Session& session) const {
    if (!session.tokenExists(request.getToken())) {
        throw std::runtime_error("Token does not exist");
    }

    Resource::processRequest(request, response, session);
}

void AdminResource::processRequest(const HttpRequest& request, Wt::Json::Object& response, Session& session) const {
    if (session.getUserByToken(request.getToken())->getUserType() == User::UserType::Student) {
        throw std::runtime_error("No permission");
    }

    Resource::processRequest(request, response, session);
}