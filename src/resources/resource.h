#pragma once

#include <Wt/WResource.h>
#include <Wt/Dbo/ptr.h>
#include <Wt/Dbo/collection.h>
#include <Wt/Dbo/Transaction.h>
#include <Wt/Http/Request.h>
#include <Wt/Http/Response.h>

#include "resource.h"
#include "session.h"
#include "request.h"
#include "user.h"
#include "str.h"
#include "http_exceptions.h"
#include "utility_functions.h"

template<typename DatabaseType>
class Resource : public Wt::WResource {
public:
    void handleRequest(const Wt::Http::Request& request, Wt::Http::Response& response) override {
        Wt::Json::Object responseContent;

        try {
            Session session;
            Wt::Dbo::Transaction tr(session);
            response.setMimeType("application/json");
            HttpRequest requestContent(request);
            const auto path = Utility::split(request.pathInfo());
            response.setStatus(200);
            Ptr<DatabaseType> ptr = nullptr;
            const auto method = request.method();

            if (path.empty()) {
                if (method == "GET") {
                    getRequirements(requestContent, session);
                    responseContent[Str::list] = JsonFunctions::getIdArray(session.getAll<DatabaseType>());
                }
                else if (method == "POST") {
                    postRequirements(requestContent, session);
                    ptr = session.create<DatabaseType>(requestContent.body());
                    response.setStatus(201);
                }
                else {
                    throw InvalidMethodException("");
                }
            }
            else {
                const auto id = Utility::getId(path);
                ptr = session.load<DatabaseType>(id);
                prepare(session, ptr);

                if (path.size() == 1) {
                    if (method == "GET") {
                        getIdRequirements(requestContent, session, ptr);
                    }
                    else if (method == "PATCH") {
                        processPatch(requestContent, session, ptr);
                    }
                    else if (method == "DELETE") {
                        deleteRequirements(requestContent, session, ptr);
                        responseContent = static_cast<Wt::Json::Object>(*ptr);
                        ptr.remove();
                        ptr = nullptr;
                        response.setStatus(204);
                    }
                    else {
                        throw InvalidMethodException("");
                    }
                }
                else {
                    const auto& pathMethod = Utility::getMethod(path);

                    if (method == "POST") {
                        processPostMethod(requestContent, responseContent, session, ptr, pathMethod);
                    }
                    else if (method == "GET") {
                        processGetMethod(requestContent, responseContent, session, ptr, pathMethod);
                    }
                    else {
                        throw InvalidMethodException("");
                    }

                    ptr = nullptr;
                }
            }

            if (ptr) {
                responseContent = static_cast<Wt::Json::Object>(*ptr);
                tr.commit();
                responseContent[Str::id] = ptr.id();
            }
        }
        catch (const HttpException& e) {
            processException(response, responseContent, e.code(), e.what());
        }
        catch (const std::out_of_range& e) {
            processException(response, responseContent, BadRequestException("").code(), "Couldn't find necessary fields");
        }
        catch (const std::exception& e) {
            processException(response, responseContent, 500, e.what());
        }
        
        response.out() << Wt::Json::serialize(responseContent);
    }

protected:
    virtual void processGetMethod(const HttpRequest& request, Wt::Json::Object& response, Session& session, 
        const Ptr<DatabaseType>& ptr, const std::string& method) const {
        throw InvalidMethodException("");
    }

    virtual void processPostMethod(const HttpRequest& request, Wt::Json::Object& response, Session& session, 
        const Ptr<DatabaseType>& ptr, const std::string& method) const {
        throw InvalidMethodException("");
    }

    virtual void processPatch(const HttpRequest& request, Session& session, const Ptr<DatabaseType>& ptr) const = 0;

    virtual void getRequirements(const HttpRequest& request, Session& session) const {}
    virtual void getIdRequirements(const HttpRequest& request, Session& session, const Ptr<DatabaseType>& ptr) const {}
    virtual void deleteRequirements(const HttpRequest& request, Session& session, const Ptr<DatabaseType>& ptr) const {}
    virtual void postRequirements(const HttpRequest& request, Session& session) const {}

    virtual void prepare(Session& session, const Ptr<DatabaseType>& ptr) const {};

private:
    void processException(Wt::Http::Response& response, Wt::Json::Object& responseContent, int code, const char* error) const {
        response.setStatus(code);
        responseContent.clear();
        
        if (std::strlen(error) > 0) {
            responseContent[Str::error] = error;
        }
    }
};
