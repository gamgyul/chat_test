#ifndef SERVER_ACCEPTOR_H
#define SERVER_ACCEPTOR_H
#include "common.h"

#include <boost/asio.hpp>
#include <boost/bind.hpp>

namespace Asio = boost::asio;
using Tcp = boost::asio::ip::tcp;
namespace server {

class Server{
private:
    Asio::io_context ioc_;
    Tcp::acceptor acceptor_;
public: 
    Server(const Tcp::endpoint& endpoint);
    
    void Run();
    void StartServer();
    void StartAccept();
};
} //namespace server

#endif // SERVER_ACCEPTOR_H