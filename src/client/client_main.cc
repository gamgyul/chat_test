#include "common.h"

#include <boost/asio.hpp>

#include "common/msg_controller.h"
#include "common/logger.h"
#include "proto/message_protocol.pb.h"

using namespace boost::asio;

class ChatClient {
private:
    ip::tcp::socket socket_;
    common::MsgController msg_;
    std::thread *writer;

    enum State {
        CONNECTED,
        LOGIN,
        ROOM
    };
    
public:
    ChatClient(io_context &io_context, ip::tcp::resolver::results_type endpoints)
            : socket_(io_context) {
        Connect(endpoints);
    }

void Connect(const ip::tcp::resolver::results_type endpoints) {
    boost::asio::async_connect(socket_, endpoints,
        [this](boost::system::error_code ec, ip::tcp::endpoint)
        {
          if (!ec)
          {
            std::cout << "connected" << std::endl;

            writer = new std::thread(&ChatClient::Write, this);
            ReadHeader();
          }
          else {
              std::cout << "connection fail : " << ec.message() << std::endl;
          }
        });
    }

void ReadHeader() {
    //초기화
    msg_.read_buf().fill(0);
    // async 함수 수행시 Session 객체가 살아있는것을 보장.
    async_read(socket_, boost::asio::buffer(&msg_.header(), 4),
                [this](boost::system::error_code ec, std::size_t length) {
        LOG_TEMP << "header length :" << length << ", " << msg_.header() << std::endl;

        if(!ec) {
            ReadBody(msg_.header());
        }
        else {
            LOG_TEMP << "header read error :" << ec.message() << std::endl;
        }
    });
}

void ReadBody(int length) {
    
    async_read(socket_, boost::asio::buffer(msg_.read_buf(), length),
                [this](boost::system::error_code ec, std::size_t test) {
        if(!ec) {
            LOG_TEMP << "body :" << std::string(msg_.read_buf().data()) << "length :" <<test << std::endl;
            ReadHeader();
        }
        else {
            LOG_TEMP << "Body read error :" << ec.message() << std::endl;
        }
    });
}

void Write() {
    // Write 테스트 중
    
    std::shared_ptr<ChatProtocol::Packet> p = std::make_shared<ChatProtocol::Packet>();
    char nickname[17] = {0, };
    LOG_TEMP << "Write test" << std::endl;

    std::cout << "Login  nickname : ";
    std::cin.getline(nickname, 17);

    p->set_type(ChatProtocol::Packet::LOGIN_REQUEST);
    ChatProtocol::LoginRequest *body =p->mutable_login_request();

    body->set_nickname(nickname);

    int len = msg_.SerializeMessage(p);

    async_write(socket_, boost::asio::buffer(msg_.write_buf(), len), 
                [](boost::system::error_code ec, std::size_t) {});
}
};

int main(int argc, char** argv) {
    if (argc != 3)
    {
      std::cerr << "Usage: chat_client <host> <port>\n";
      return 1;
    }

    GOOGLE_PROTOBUF_VERIFY_VERSION;

    io_context io_context;

    ip::tcp::resolver resolver(io_context);
    auto endpoints = resolver.resolve(argv[1], argv[2]);
    ChatClient chat_client(io_context, endpoints);
   
    io_context.run();
    return 0;
}