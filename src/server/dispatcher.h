#ifndef DISPATCHER_H_
#define DISPATCHER_H_

#include "common.h"

#include <condition_variable>

#include "proto/message_protocol.pb.h"

namespace server {

class Server;
class Session;

class Dispatcher{
private:
    std::queue<std::pair<int,MessagePtr>> msg_queue_;
    std::mutex lock_;
    std::condition_variable cv_;

    bool stop_flag_;

    Server* server_;
    std::shared_ptr<Session> cur_sess_;
public:
    Dispatcher(Server* server);

    void Run();
    void Enqueue(int session_id, MessagePtr msg);

    void HandleMsg(int session_id, PacketPtr msg);
    int HandleLoginRequest(int session_id, const ChatProtocol::LoginRequest &msg);
    int HandleShowRoomListRequest(int session_id, const ChatProtocol::ShowRoomListRequest &msg);
    int HandleRoomCreateRequest(int session_id, const ChatProtocol::RoomCreateRequest &msg);
    int HandleSendChatRequest(int session_id, const ChatProtocol::SendChatRequest &msg);
    int HandleRoomJoinRequest(int session_id, const ChatProtocol::RoomJoinRequest &msg);
    int WriteLoginReply(int session_id, ChatProtocol::ErrorType err);
    int WriteShowRoomListReply(ChatProtocol::ErrorType err);
    int WriteRoomCreateReply(int room_id, ChatProtocol::ErrorType err);
    int WriteRoomJoinReply(int room_id, ChatProtocol::ErrorType err);

};

class DispatcherMgr{
private:
    std::vector<std::unique_ptr<Dispatcher>> dispatcher_;

    Server* server_;
public:
    std::vector<std::unique_ptr<Dispatcher>> &dispatcher() {return dispatcher_;}

    void Init(Server* server);
};




} // namespace server


#endif // DISPATCHER_H_
