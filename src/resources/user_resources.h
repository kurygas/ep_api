#pragma once

#include "resource.h"

namespace UserResources {
    class Create : public Resource {
    protected:
        void processPost(const HttpRequest& request, Wt::Json::Object& response, Session& session) const override;
    };

    class Auth : public Resource {
    protected:
        void processPost(const HttpRequest& request, Wt::Json::Object& response, Session& session) const override;
    };

    class MakeAdmin : public AdminResource {
    protected:
        void processPost(const HttpRequest& request, Wt::Json::Object& response, Session& session) const override;
    };

    class Get : public TokenResource {
    protected:
        void processGet(const HttpRequest& request, Wt::Json::Object& response, Session& session) const override;
    };

    class Delete : public AdminResource {
    protected:
        void processPost(const HttpRequest& request, Wt::Json::Object& response, Session& session) const override;
    };

    class SetTgUsername : public TokenResource {
    protected:
        void processPost(const HttpRequest& request, Wt::Json::Object& response, Session& session) const override;
    };

    class SetFirstName : public TokenResource {
    protected:
        void processPost(const HttpRequest& request, Wt::Json::Object& response, Session& session) const override;
    };

    class SetSecondName : public TokenResource {
    protected:
        void processPost(const HttpRequest& request, Wt::Json::Object& response, Session& session) const override;
    };

    class GetAllId : public TokenResource {
    protected:
        void processGet(const HttpRequest& request, Wt::Json::Object& response, Session& session) const override;
    };
}
