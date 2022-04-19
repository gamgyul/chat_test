#ifndef SESSION_H_
#define SESSION_H_

#include "common.h"
#include <memory>

#include <boost/asio.hpp>

#include "common/msg_controller.h"

namespace Asio = boost::asio;
using Tcp = boost::asio::ip::tcp;

namespace server {

class Server;

class Session : public std::enable_shared_from_this<Session> {
private:
    int session_id_;
    Server *server_;

    Tcp::socket socket_;
    common::MsgController msg_;
    
public:
    Session(Tcp::socket socket, Server* server, int session_id);

    void Start();
    void ReadHeader();
    void ReadBody(int bodylength);
};

} // namespace server

#endif // SESSION_H_