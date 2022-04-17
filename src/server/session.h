#ifndef SESSION_H_
#define SESSION_H_

#include "common.h"
#include <memory>

#include <boost/asio.hpp>

namespace Asio = boost::asio;
using Tcp = boost::asio::ip::tcp;

namespace server {

class Server;

class Session : public std::enable_shared_from_this<Session> {
private:
    int a;
    Server *server_;

    Tcp::socket socket_;
public:
    Session(Tcp::socket socket, Server* server);

    void Start();
};

} // namespace server

#endif // SESSION_H_