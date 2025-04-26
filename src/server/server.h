#pragma once

#include <Wt/WServer.h>

class Server : public Wt::WServer {
public:
    Server(int argc, char** argv);

    template<typename Resource>
    void addResource(const std::string& path) {
        Wt::WServer::addResource(std::make_shared<Resource>(), path);
    }
};
