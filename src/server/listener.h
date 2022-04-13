#ifndef SERVER_ACCEPTOR_H
#define SERVER_ACCEPTOR_H
#include "common.h"

#include <boost/asio.hpp>
#include <boost/bind.hpp>

namespace Asio = boost::asio;
class Acceptor{
    Acceptor();
    
    void StartServer();
    void StartAccept();
};

#endif // SERVER_ACCEPTOR_H