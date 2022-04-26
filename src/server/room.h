#ifndef ROOM_H_
#define ROOM_H_

#include "common.h"

namespace server {

class Session;

class Room {
private: 
    std::string name_;
    int participant_;
    std::list<std::shared_ptr<Session>> session_list_;
    
public:
    Room();
    
    void JoinSession(std::shared_ptr<Session> session);
    int BroadCastMessage(std::string &nickname, const std::string &chat);

    /* getter */
    std::string &name() { return name_; }
    std::list<std::shared_ptr<Session>> &session_list() { return session_list_; }
};
} //namespace server
#endif // ROOM_H_