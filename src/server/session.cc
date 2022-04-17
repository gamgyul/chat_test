#include "session.h"

namespace server {

Session::Session(Tcp::socket socket, Server* server)
        : socket_(std::move(socket)), server_(server)
    {
        Start();
    }
void Session::Start() {
    
}
}