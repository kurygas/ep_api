#include "resource.h"
#include "session.h"
#include "json.h"
#include "request.h"

void Resource::handleRequest(const Wt::Http::Request& request, Wt::Http::Response& response) {
    JsonObject responseContent;

    try {
        Session session;
        const Wt::Dbo::Transaction transaction(session);
        response.setMimeType("application/json");
        processRequest(HttpRequest(request), responseContent, session);
    }
    catch (const std::exception& e) {
        response.setStatus(400);
        responseContent.clear();
        responseContent.putString("error", e.what());
    }

    response.out() << Wt::Json::serialize(responseContent);
}

void Resource::processRequest(const HttpRequest& request, JsonObject& responseContent, Session& session) const {
    const auto method = request.getMethod();

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

void Resource::processGet(const HttpRequest& request, JsonObject& responseContent, Session& session) const {
    throw std::runtime_error("Invalid method");
}

void Resource::processPost(const HttpRequest& request, JsonObject& responseContent, Session& session) const {
    throw std::runtime_error("Invalid method");
}