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

    std::string nickname_;
    int room_;
public:
    Session(Tcp::socket socket, Server* server, int session_id);

    void Start();
    void ReadHeader();
    void ReadBody(int bodylength);

    void Write(PacketPtr msg);

    
    /* setter */
    void set_nickname(std::string &nickname) {nickname_ = nickname;}
    void set_room(int room) { room_ = room; }
    
    /* getter */
    common::MsgController &msg() { return msg_; };
    std::string &nickname() { return nickname_; }
};

} // namespace server

#endif // SESSION_H_