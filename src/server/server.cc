
#include "server.h"

namespace server {

Server::Server(const Tcp::endpoint &endpoint)
    :ioc_(), acceptor_(ioc_, endpoint) {

    }

void Server::Run() {

#ifndef _WIN32
    pthread_setname_np(pthread_self(), "Server Run thread");
#endif

    

}

} //namespace server