#include "resource.h"
#include "session.h"
#include "json.h"
#include "user.h"

JsonObject Resource::getBodyContent(const Wt::Http::Request& request) {
    JsonObject bodyContent;

    try {
        Wt::Json::parse(std::string(static_cast<std::istreambuf_iterator<char>>(request.in()), {}), bodyContent);
    }
    catch (...) {
        throw std::runtime_error("Invalid payload");
    }

    return bodyContent;
}

std::string Resource::getToken(const Wt::Http::Request& request) {
    const auto token = request.headerValue("Authorization");

    if (token.substr(0, 7) != "Bearer ") {
        throw std::runtime_error("Invalid token");
    }

    return token.substr(7);
}

const std::string& Resource::getParameter(const Wt::Http::Request& request, const std::string& key) {
    auto value = request.getParameter(key);

    if (!value) {
        throw std::runtime_error("Does not contain parameter " + key);
    }

    return *value;
}

void Resource::handleRequest(const Wt::Http::Request& request, Wt::Http::Response& response) {
    JsonObject responseContent;

    try {
        Session session;
        const Wt::Dbo::Transaction transaction(session);
        response.setMimeType("application/json");
        processRequest(request, responseContent, session);
    }
    catch (const std::exception& e) {
        response.setStatus(400);
        responseContent.clear();
        responseContent.putString("error", e.what());
        std::cerr << "here" << std::endl;
    }

    response.out() << Wt::Json::serialize(responseContent);
}

void Resource::processRequest(const Wt::Http::Request& request, JsonObject& responseContent, Session& session) const {
    const auto method = request.method();

    if (method == "GET") {
        processGet(request, responseContent, session);
    }
    else if (method == "POST") {
        processPost(request, responseContent, session);
    }
    else {
        throw std::runtime_error("Invalid method");
    }
}

void Resource::processGet(const Wt::Http::Request& request, JsonObject& responseContent, Session& session) const {
    throw std::runtime_error("Invalid method");
}

void Resource::processPost(const Wt::Http::Request& request, JsonObject& responseContent, Session& session) const {
    throw std::runtime_error("Invalid method");
}

void TokenResource::processRequest(const Wt::Http::Request& request, JsonObject& responseContent, Session& session) const {
    if (!session.tokenExists(getToken(request))) {
        throw std::runtime_error("Token does not exist");
    }

    Resource::processRequest(request, responseContent, session);
}

void AdminResource::processRequest(const Wt::Http::Request& request, JsonObject& responseContent, Session& session) const {
    if (session.getUserByToken(getToken(request))->getUserType() == User::UserType::Student) {
        throw std::runtime_error("No permission");
    }

    Resource::processRequest(request, responseContent, session);
}