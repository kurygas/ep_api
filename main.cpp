#include "src/server/server.h"

int main(int argc, char** argv) {
    Server server(argc, argv);

    if (server.start()) {
        Wt::WServer::waitForShutdown();
        server.stop();
    }

    return 0;
}