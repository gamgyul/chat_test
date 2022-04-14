#ifndef SERVER_ACCEPTOR_H
#define SERVER_ACCEPTOR_H
#include "common.h"

#include <boost/asio.hpp>
#include <boost/bind.hpp>

namespace Asio = boost::asio;

namespace server {

class Acceptor{
private:

public: 
    Acceptor();
    
    void StartServer();
    void StartAccept();
};
} //namespace server

#endif // SERVER_ACCEPTOR_H