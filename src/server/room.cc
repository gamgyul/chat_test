#include "room.h"

#include "session.h"

#include "proto/message_protocol.pb.h"
#include "common/logger.h"
namespace server {
Room::Room() : participant_(0) {

}
void Room::JoinSession(std::shared_ptr<Session> session) {
    session_list_.push_back(session);
}

int Room::BroadCastMessage(std::string &nickname, const std::string &chat) {
    LOG_TEMP << "Room::BroadCast " << std::endl;
    std::shared_ptr<ChatProtocol::Packet> p = std::make_shared<ChatProtocol::Packet>();
    p->set_type(ChatProtocol::Packet::NOTI_RECV_CHAT);
    ChatProtocol::NotiRecvChat *body =p->mutable_noti_recv_chat();

    body->set_nickname(nickname);
    body->set_chat(chat);

    for(auto &session : session_list_) {
        session->Write(p);
    }

    return 0;
}
}  //namespace server