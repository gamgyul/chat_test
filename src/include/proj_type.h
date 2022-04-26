#ifndef PROJ_TYPE_H_
#define PROJ_TYPE_H_

// #include "proto/message_protocol.pb.h"
namespace google {
namespace protobuf {
    class MessageLite;
}
}
class MessageLite;
namespace ChatProtocol{
class Packet;
}

using MessageBase = google::protobuf::MessageLite;
using MessagePtr = std::shared_ptr<MessageBase>;
using PacketPtr = std::shared_ptr<ChatProtocol::Packet>;

// 메세지 관련 const
// header 4byte에 messagelength를 적는다.
static const int kMaxBufferSize = 65536;
static const int kMsgHeaderSize = 4;

//
static const int kDispatcherSize = 3;
#endif // PROJ_TYPE_H_