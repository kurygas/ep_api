#pragma once

#include <Wt/WServer.h>

class Server : public Wt::WServer {
public:
    Server(const std::string& applicationPath);

    template<typename Resource>
    void addResource(const std::string& path) {
        Wt::WServer::addResource(std::make_shared<Resource>(), path);
    }

private:
    void pullConfig();
};
