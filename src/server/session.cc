#include "session.h"

#include "common/logger.h"
#include "server.h"
#include "dispatcher.h"

namespace server {

Session::Session(Tcp::socket socket, Server* server, int session_id)
        : socket_(std::move(socket)), server_(server), session_id_(session_id),
          room_(-1)
    {
    }
void Session::Start() {
    LOG_TEMP << "Session Start " << std::endl;
    ReadHeader();
}

void Session::ReadHeader() {
    //초기화
    msg_.read_buf().fill(0);
    // async 함수 수행시 Session 객체가 살아있는것을 보장.
    auto self(shared_from_this());
    Asio::async_read(socket_, Asio::buffer(&msg_.header(), 4),
                [self](boost::system::error_code ec, std::size_t length) {
        LOG_TEMP << "header length :" << length << ", " 
                 << self->msg_.header() << std::endl;

        if(!ec) {
            self->ReadBody(self->msg_.header());
        }
        else {
            LOG_TEMP << "header read error :" << ec.message() << std::endl;
        }
    });
}

void Session::ReadBody(int length) {

    auto self(shared_from_this());
    Asio::async_read(socket_, Asio::buffer(msg_.read_buf(), length),
                [self](boost::system::error_code ec, std::size_t size) {
        if(!ec) {
            LOG_TEMP << "body : length :" <<size << std::endl;
            PacketPtr p = std::make_shared<ChatProtocol::Packet>();
            p->ParseFromArray(self->msg_.read_buf().data(), size);
            if(self->room_ == -1) {
                self->server_->dispatch_mgr().dispatcher()[0].get()->
                            Enqueue(self->session_id_, std::static_pointer_cast<MessageBase>(p));
            }
            self->ReadHeader();
        }
        else {
            LOG_TEMP << "Body read error :" << ec.message() << std::endl;
        }
    });
}

void Session::Write(PacketPtr msg) {
    int len = msg_.SerializeMessage(msg);
    LOG_TEMP << "Write() Session : " << session_id_ << ", len : " << len << std::endl;
    boost::asio::write(socket_, boost::asio::buffer(msg_.write_buf(), len));
}

} // namespace server