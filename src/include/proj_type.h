#ifndef PROJ_TYPE_H_
#define PROJ_TYPE_H_

#include "proto/message_protocol.pb.h"

using MessageBase = google::protobuf::MessageLite;
using MessagePtr = std::shared_ptr<MessageBase> ;

static const int kMaxBufferSize = 65536;
static const int kMsgHeaderSize = 4;

#endif // PROJ_TYPE_H_