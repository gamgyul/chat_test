INCLUDE_DIRECTORIES(${Protobuf_INCLUDE_DIR})
Protobuf_GENERATE_CPP(PROTO_SRC PROTO_HEADER ../proto/message_protocol.proto)
SET_SOURCE_FILES_PROPERTIES(${PROTO_SRC} ${PROTO_HEADER} PROPERTIES GENERATED TRUE)

include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/include)

SET (COMMON_SOURCE 
    logger.cc
    msg_controller.cc
)

# protobuf로 gen 된 cc로 라이브러리 생성
ADD_LIBRARY(common ${COMMON_SOURCE} ${PROTO_HEADER} ${PROTO_SRC})

