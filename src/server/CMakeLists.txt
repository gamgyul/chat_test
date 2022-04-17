

include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${Boost_INCLUDE_DIR})

SET (SERVER_SOURCE 
    logger.cc
    server.cc
    session.cc
    server_main.cc
)

add_executable(chat_server
    ${SERVER_SOURCE}
    )

target_link_libraries(chat_server ${Boost_LIBRARIES} pthread)
