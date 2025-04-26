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

    class GetUserInfoResource : public Resource {
    protected:
        void processGet(const HttpRequest& request, Wt::Json::Object& response, Session& session) const override;
    };

    class DeleteUserResource : public AdminResource {
    protected:
        void processPost(const HttpRequest& request, Wt::Json::Object& response, Session& session) const override;
    };

    class UpdateTgUsernameResource : public TokenResource {
    protected:
        void processPost(const HttpRequest& request, Wt::Json::Object& response, Session& session) const override;
    };

    class UpdateFirstNameResource : public TokenResource {
    protected:
        void processPost(const HttpRequest& request, Wt::Json::Object& response, Session& session) const override;
    };

    class UpdateSecondNameResource : public TokenResource {
    protected:
        void processPost(const HttpRequest& request, Wt::Json::Object& response, Session& session) const override;
    };

    class GetAllUsersResource : public Resource {
    protected:
        void processGet(const HttpRequest& request, Wt::Json::Object& response, Session& session) const override;
    };
}