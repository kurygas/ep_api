#pragma once

#include "resource.h"

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

class GetUserResource : public TokenResource {
protected:
    void processGet(const Wt::Http::Request& request, JsonObject& responseContent, Session& session) const override;
};

class DeleteUserResource : public AdminResource {
protected:
    void processPost(const Wt::Http::Request& request, JsonObject& responseContent, Session& session) const override;
};