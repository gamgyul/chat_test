#include "room.h"

#include "session.h"
namespace server {
Room::Room() : participant_(0) {

}

void Room::JoinSession(std::shared_ptr<Session> session) {
    session_list_.push_back(session);
}
}  //namespace server