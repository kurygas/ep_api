#pragma once

#include <Wt/WResource.h>
#include <Wt/Http/Request.h>
#include <Wt/Http/Response.h>

class GetSolutionResource : public Wt::WResource {
public:
    void handleRequest(const Wt::Http::Request& request, Wt::Http::Response& response) override;
    void processException(Wt::Http::Response& response, const std::exception& e, int code) const;
};
