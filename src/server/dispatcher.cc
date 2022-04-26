
#include "dispatcher.h"

#include <unistd.h> // TODO erase
#include "common/logger.h"
#include "server.h"
#include "session.h"
#include "room.h"

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
    while (stop_flag_) {
        std::unique_lock<std::mutex> ul(lock_);

        cv_.wait(ul, [this]() { return !msg_queue_.empty();});
        LOG_TEMP << "Dispatcher Wake UP" << std::endl;
        int session_id = msg_queue_.front().first;
        PacketPtr msg = std::static_pointer_cast<Packet>(msg_queue_.front().second);
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
    
    auto it = server_->session_map().find(session_id);
    if(it ==  server_->session_map().end()) {
        LOG_TEMP << " wrong session_id " << std::endl;
        return;
    }
    
    cur_sess_ = it->second;

    Packet::PacketType type = msg->type();
    LOG_TEMP << "Packet Type : " << type << std::endl;
    switch(type) {
        case Packet::LOGIN_REQUEST : 
            if(msg->has_login_request()) {
                HandleLoginRequest(session_id, msg->login_request());
            }
            break;
        case Packet::SHOW_ROOM_LIST_REQUEST :
            if(msg->has_show_room_list_request()) {
                HandleShowRoomListRequest(session_id, msg->show_room_list_request());
            }
            break;
        case Packet::ROOM_CREATE_REQUEST :
            if(msg->has_room_create_request()) {
                HandleRoomCreateRequest(session_id, msg->room_create_request());
            }
            break;
        case Packet::SEND_CHAT_REQUEST : 
            if(msg->has_send_chat_request()) {
                HandleSendChatRequest(session_id, msg->send_chat_request());
            }
            break;
        case Packet::ROOM_JOIN_REQUEST :
            if(msg->has_room_join_request()) {
                HandleRoomJoinRequest(session_id, msg->room_join_request());
            }
            break;
        default : 
            LOG_TEMP << "Invalid Packet Type!!" << std::endl;
            break;
    }
}

int Dispatcher::HandleLoginRequest(int session_id, const LoginRequest &msg) {
    LOG_TEMP << "LOGIN_REQEUST" << std::endl;
    std::string nickname = msg.nickname();
    if(server_->nickname_map().find(nickname) != server_->nickname_map().end()) {
        WriteLoginReply(session_id, ERROR_EXIST);
    }
    else {
        server_->nickname_map().insert(nickname);
        auto it = server_->session_map().find(session_id);
        (it->second)->set_nickname(nickname);
        WriteLoginReply(session_id, ERROR_NONE);
    }

    return 0;
}

int Dispatcher::HandleShowRoomListRequest(int session_id, const ChatProtocol::ShowRoomListRequest &msg) {
    LOG_TEMP << "SHOW_ROOM_LIST_REQUEST" << std::endl;
    WriteShowRoomListReply(ERROR_NONE);

    return 0;
}

int Dispatcher::HandleRoomCreateRequest(int session_id, const ChatProtocol::RoomCreateRequest &msg) {
    LOG_TEMP << "ROOM_CREATE_REQUEST" << std::endl;
    
    int room = server_->CreateRoom(msg.roomname());
    if(room < 0)
        return -1;
    auto session = server_->session_map()[session_id];
    session->set_room(room);
    server_->room_map()[room]->JoinSession(session);
    WriteRoomCreateReply(room, ERROR_NONE);

    return 0;
}

int Dispatcher::HandleSendChatRequest(int session_id, const ChatProtocol::SendChatRequest &msg) {
    LOG_TEMP << "SEND_CHAT_REQUEST" << std::endl;

    auto session = server_->session_map()[session_id];
    auto room = server_->room_map()[session->room()];

    room->BroadCastMessage(session->nickname(), msg.chat());
    return 0;
}

int Dispatcher::HandleRoomJoinRequest(int session_id, const ChatProtocol::RoomJoinRequest &msg) {
    LOG_TEMP <<"ROOM_JOIN_REQUEST" << std::endl;

    auto session = server_->session_map()[session_id];
    int room = msg.room_number();
    if(server_->room_map().find(room) != server_->room_map().end()) {
        session->set_room(room);
        server_->room_map()[room]->JoinSession(session);
        WriteRoomJoinReply(room, ERROR_NONE);
    }
    else {
        WriteRoomJoinReply(room, ERROR_EXIST);
    }
    return 0;
}

int Dispatcher::WriteLoginReply(int session_id, ChatProtocol::ErrorType err) {
    std::shared_ptr<Packet> p = std::make_shared<Packet>();
    p->set_type(Packet::LOGIN_REPLY);
    LoginReply *body =p->mutable_login_reply();

    body->set_err(err);
    if(err == ERROR_NONE) {
        body->set_id(session_id);
    }

    cur_sess_->Write(p);

    return 0;
}

int Dispatcher::WriteShowRoomListReply(ChatProtocol::ErrorType err) {
    std::shared_ptr<Packet> p = std::make_shared<Packet>();
    p->set_type(Packet::SHOW_ROOM_LIST_REPLY);
    ShowRoomListReply *body = p->mutable_show_room_list_reply();

    body->set_err(err);
    for(auto &elem : server_->room_map()) {
        ShowRoomListReply::RoomInfo *room_info = body->add_roomlist();
        room_info->set_number(elem.first);
        room_info->set_name(elem.second->name());
    }

    cur_sess_->Write(p);
    return 0;
}

int Dispatcher::WriteRoomCreateReply(int room_id, ChatProtocol::ErrorType err) {
    std::shared_ptr<Packet> p = std::make_shared<Packet>();
    p->set_type(Packet::ROOM_CREATE_REPLY);
    RoomCreateReply*body =p->mutable_room_create_reply();

    body->set_err(err);
    if(err == ERROR_NONE) {
        body->set_room_number(room_id);
    }

    cur_sess_->Write(p);
    return 0;
}

int Dispatcher::WriteRoomJoinReply(int room_id, ChatProtocol::ErrorType err) {
    std::shared_ptr<Packet> p = std::make_shared<Packet>();
    p->set_type(Packet::ROOM_JOIN_REPLY);
    RoomJoinReply*body =p->mutable_room_join_reply();

    body->set_err(err);
    if(err == ERROR_NONE) {
        body->set_room_number(room_id);
    }
    cur_sess_->Write(p);
    return 0;
}

} // namespace server