#ifndef SERVER_ACCEPTOR_H
#define SERVER_ACCEPTOR_H
#include "common.h"

#include <map>
#include <boost/asio.hpp>
#include <boost/bind.hpp>

#include "dispatcher.h"

namespace Asio = boost::asio;
using Tcp = boost::asio::ip::tcp;

namespace server {
class Session;
class Room;

class Server{
private:
    //asio 관련 변수
    Asio::io_context server_ioc_;
    Asio::io_context socket_ioc_;
    Tcp::acceptor acceptor_;

    //세션 관련 변수
    std::unordered_map<int, std::shared_ptr<Session>> session_map_;
    int accept_session_id_;

    std::set<std::string> nickname_map_;
    std::map<int, std::shared_ptr<Room>> room_map_;
    int room_index_;
    std::vector<std::thread> dispatch_thread_;
    DispatcherMgr dispatch_mgr_;
public:
    Server(const Tcp::endpoint& endpoint);
    ~Server();

    void Run();
    void ReadThreadRun();
    void StartServer();
    void StartAccept();

    int CreateRoom(const std::string &room_name);

    std::set<std::string> &nickname_map() {return nickname_map_;}
    std::unordered_map<int, std::shared_ptr<Session>> &session_map() { return session_map_;};
    std::map<int, std::shared_ptr<Room>> &room_map() { return room_map_; };

    DispatcherMgr &dispatch_mgr() {return dispatch_mgr_;};
};
} //namespace server

#endif // SERVER_ACCEPTOR_H