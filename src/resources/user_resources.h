#pragma once

#include "resource.h"

namespace UserResources {
    class RegistrationResource : public Resource {
    protected:
        void processPost(const Wt::Http::Request& request, JsonObject& responseContent, Session& session) const override;
    };

    class AuthResource : public Resource {
    protected:
        void processPost(const Wt::Http::Request& request, JsonObject& responseContent, Session& session) const override;
    };

    class MakeTeacherResource : public AdminResource {
    protected:
        void processPost(const Wt::Http::Request& request, JsonObject& responseContent, Session& session) const override;
    };

    class GetUserInfoResource : public Resource {
    protected:
        void processGet(const Wt::Http::Request& request, JsonObject& responseContent, Session& session) const override;
    };

    class DeleteUserResource : public AdminResource {
    protected:
        void processPost(const Wt::Http::Request& request, JsonObject& responseContent, Session& session) const override;
    };

    class UpdateTgUsernameResource : public TokenResource {
    protected:
        void processPost(const Wt::Http::Request& request, JsonObject& responseContent, Session& session) const override;
    };

    class UpdateFirstNameResource : public TokenResource {
    protected:
        void processPost(const Wt::Http::Request& request, JsonObject& responseContent, Session& session) const override;
    };

    class UpdateSecondNameResource : public TokenResource {
    protected:
        void processPost(const Wt::Http::Request& request, JsonObject& responseContent, Session& session) const override;
    };

    class UpdateEmailResource : public TokenResource {
    protected:
        void processPost(const Wt::Http::Request& request, JsonObject& responseContent, Session& session) const override;
    };

    class GetAllUsersResource : public Resource {
    protected:
        void processGet(const Wt::Http::Request& request, JsonObject& responseContent, Session& session) const override;
    };
}