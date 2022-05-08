#ifndef ROOM_H_
#define ROOM_H_

#include "common.h"

namespace server {

class Session;

class Room {
private: 
    std::string name_;
    int participant_;
    int count_;
    std::set<std::shared_ptr<Session>> session_map_;
    
public:
    Room();
    
    int JoinSession(std::shared_ptr<Session> session);
    int ExitSession(std::shared_ptr<Session> session);
    int BroadCastMessage(std::string &nickname, const std::string &chat);

    /* getter */
    std::string &name() { return name_; }
    int count() {return count_;}
    std::set<std::shared_ptr<Session>> &session_map() { return session_map_; }
};
} //namespace server
#endif // ROOM_H_