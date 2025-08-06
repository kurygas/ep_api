#include "server.h"

int main(int argc, char** argv) {
    Server server(argv[0]);

    if (server.start()) {
        Wt::WServer::waitForShutdown();
        server.stop();
    }

    return 0;
}
