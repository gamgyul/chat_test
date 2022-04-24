#include "msg_controller.h"

namespace common {

int MsgController::SerializeMessage(PacketPtr msg) {
    write_buf_.fill(0);

    int byte_size = static_cast<int>(msg->ByteSizeLong());
    std::cout << "byte_size : " << byte_size << std::endl;//TODO Erase
    std::memcpy(write_buf_.data(), &byte_size, kMsgHeaderSize);
    msg->SerializeToArray(write_buf_.data() + kMsgHeaderSize, byte_size);
    return byte_size + kMsgHeaderSize;
}


}  // namespace common