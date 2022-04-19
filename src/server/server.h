#ifndef SERVER_ACCEPTOR_H
#define SERVER_ACCEPTOR_H
#include "common.h"

#include <map>
#include <boost/asio.hpp>
#include <boost/bind.hpp>

namespace Asio = boost::asio;
using Tcp = boost::asio::ip::tcp;

namespace server {
class Session;

class Server{
private:
    Asio::io_context server_ioc_;
    Asio::io_context socket_ioc_;
    Tcp::acceptor acceptor_;

    std::map<int, std::shared_ptr<Session>> session_map_;
    int accept_session_id_;

    bool read_thread_start_;
public:
    Server(const Tcp::endpoint& endpoint);

    void Run();
    void ReadThreadRun();
    void StartServer();
    void StartAccept();
};
} //namespace server

#endif // SERVER_ACCEPTOR_H