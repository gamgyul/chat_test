
#include "dispatcher.h"

#include <unistd.h> // TODO erase
#include "common/logger.h"
#include "server.h"


using namespace ChatProtocol;

namespace server {

void DispatcherMgr::Init(Server* server) {
    for(int i = 0; i < kDispatcherSize; ++i)
    dispatcher_.push_back(std::move(std::unique_ptr<Dispatcher>(new Dispatcher(server))));
}

Dispatcher::Dispatcher(Server* server) : server_(server) {

}

void Dispatcher::Run() {
    stop_flag_ = true;
    LOG_TEMP << "Dispatcher test" << std::endl;
    while (stop_flag_) {
        std::unique_lock<std::mutex> ul(lock_);

        cv_.wait(ul, [this]() { return !msg_queue_.empty();});
        LOG_TEMP << "Dispatcher Wake UP" << std::endl;
        int session_id = msg_queue_.front().first;
        PacketPtr msg = std::static_pointer_cast<ChatProtocol::Packet>(msg_queue_.front().second);
        msg_queue_.pop();
        
        HandleMsg(session_id, msg);
        
    }
}

void Dispatcher::Enqueue(int session_id, MessagePtr msg) {
    std::unique_lock<std::mutex> ul(lock_);
    msg_queue_.push({session_id, msg});
    ul.unlock();
    cv_.notify_one();
}

void Dispatcher::HandleMsg(int session_id, PacketPtr msg) {
    Packet::PacketType type = msg->type();
    switch(type) {
        case Packet::LOGIN_REQUEST : 
        if(msg->has_login_request()) {
            HandleLoginRequest(session_id, msg->login_request());
        }
        ;
        default : 
        ;
    }
}

int Dispatcher::HandleLoginRequest(int session_id, const ChatProtocol::LoginRequest &msg) {
    LOG_TEMP << "LOGIN_REQEUST" << std::endl;
    std::string nickname = msg.nickname();
    if(server_->nickname_map().find(nickname) != server_->nickname_map().end()) {
        
    }
    else {

    }

    return 0;
}

} // namespace server