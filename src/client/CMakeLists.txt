

include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${Boost_INCLUDE_DIR})

SET (CLIENT_SOURCE 
    client_main.cc
)

add_executable(chat_client
    ${CLIENT_SOURCE}
    )

target_link_libraries(chat_client
                      common ${Boost_LIBRARIES} pthread ${Protobuf_LIBRARIES})
