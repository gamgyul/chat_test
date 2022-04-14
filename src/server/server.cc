
#include "server.h"

#include "logger.h"

namespace server {

Server::Server(const Tcp::endpoint &endpoint)
    :ioc_(), acceptor_(ioc_, endpoint) {

    }

void Server::Run() {

#ifndef _WIN32
    pthread_setname_np(pthread_self(), "Server_thread");
#endif

    LOG_TEMP << "Server Run" << std::endl;

    int a =0;
    while(a ==0) {
        sleep(5);
    }

}

} //namespace server