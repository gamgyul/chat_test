#include "session.h"

#include "logger.h"

namespace server {

Session::Session(Tcp::socket socket, Server* server, int session_id)
        : socket_(std::move(socket)), server_(server), session_id_(session_id)
    {
    }
void Session::Start() {
    LOG_TEMP << "Session Start " << std::endl;
    ReadHeader();
}

void Session::ReadHeader() {
    //초기화
    buf_.fill(0);
    // async 함수 수행시 Session 객체가 살아있는것을 보장.
    auto self(shared_from_this());
    Asio::async_read(socket_, Asio::buffer(buf_, 4),
                [self](boost::system::error_code ec, std::size_t length) {
        LOG_TEMP << "header length :" << length << ", " <<atoi(self->buf_.data()) << std::endl;

        if(!ec) {
            self->ReadBody(atoi(self->buf_.data()));
        }
        else {
            LOG_TEMP << "header read error :" << ec.message() << std::endl;
        }
    });
}

void Session::ReadBody(int length) {

    auto self(shared_from_this());
    Asio::async_read(socket_, Asio::buffer(buf_, length),
                [self](boost::system::error_code ec, std::size_t test) {
        if(!ec) {
            LOG_TEMP << "body :" << std::string(self->buf_.data()) << "length :" <<test << std::endl;
            self->ReadHeader();
        }
        else {
            LOG_TEMP << "Body read error :" << ec.message() << std::endl;
        }
    });
}


} // namespace server