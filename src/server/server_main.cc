#include "common.h"

#include <thread>

#include "server.h"
#include "logger.h"

using namespace server;

int main(int argc, char** argv) {

    int listener_port = atoi(argv[1]);
    LOG_TEMP << "listener_port : " << listener_port << std::endl;
    Server chat_server(Tcp::endpoint(Tcp::v4(), listener_port));

    std::thread server_thread(&Server::Run, &chat_server);

    return 0;
}
