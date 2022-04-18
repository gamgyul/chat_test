#include "common.h"

#include <thread>

#include "server.h"
#include "logger.h"

using namespace server;

int main(int argc, char** argv) {

    if(argc < 2) {   
        std::cout << "usage ./chat_server [port]" << std::endl;
        std::cout << "Port Not Specified" << std::endl;
        return 0;
    }

    int listener_port = atoi(argv[1]);
    LOG_TEMP << "listener_port : " << listener_port << std::endl;
    Server chat_server(Tcp::endpoint(Tcp::v4(), listener_port));

    std::thread server_thread(&Server::Run, &chat_server);

    server_thread.join();

    return 0;
}
