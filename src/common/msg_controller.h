#ifndef MSG_CONTROLLER_H_
#define MSG_CONTROLLER_H_

#include "common.h"

#include "proto/message_protocol.pb.h"

namespace common {

class MsgController {
private:
    int header_;
    std::array<char, kMaxBufferSize> read_buf_;
    std::array<char, kMaxBufferSize> write_buf_;

public:
    // ChatProtocol::Pakcet::PacketType ReadHeader();
    MessagePtr ReadMsg(PacketPtr msg);

    int SerializeMessage(PacketPtr msg);

    int &header() {return header_;}
    std::array<char, kMaxBufferSize> &read_buf() { return read_buf_; }
    std::array<char, kMaxBufferSize> &write_buf() { return write_buf_; }

};

} // namespace common

#endif // MSG_CONTROLLER_H_