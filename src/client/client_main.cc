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

    int session_id_;

    enum State {
        WAIT,
        CONNECTED,
        LOGIN,
        ROOM,
        ERROR
    };
    State state_;
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
            state_ = CONNECTED;
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
    msg_.header() = 0;
    // async 함수 수행시 Session 객체가 살아있는것을 보장.
    async_read(socket_, boost::asio::buffer(&msg_.header(), kMsgHeaderSize),
                [this](boost::system::error_code ec, std::size_t length) {
        // LOG_TEMP << "header length :" << length << ", " << msg_.header() << std::endl;

        if(!ec && msg_.header() != 0) {
            ReadBody(msg_.header());
        }
        else {
            LOG_TEMP << "header read error :" << ec.message() << std::endl;
        }
    });
}

void ReadBody(int length) {
    
    async_read(socket_, boost::asio::buffer(msg_.read_buf(), length),
                [this](boost::system::error_code ec, std::size_t size) {
        if(!ec) {
            // LOG_TEMP << "body :" << std::string(msg_.read_buf().data()) << "length :" <<size << std::endl;
            PacketPtr p = std::make_shared<ChatProtocol::Packet>();
            p->ParseFromArray(msg_.read_buf().data(), size);
            HandleMsg(p);
            ReadHeader();
        }
        else {
            LOG_TEMP << "Body read error :" << ec.message() << std::endl;
        }
    });
}

void HandleMsg(PacketPtr msg) {
    ChatProtocol::Packet::PacketType type = msg->type();
    switch(type) {
        case ChatProtocol::Packet::LOGIN_REPLY : 
            if(msg->has_login_reply()) {
                HandleLoginReply(msg->login_reply());
            }
            break;
        case ChatProtocol::Packet::SHOW_ROOM_LIST_REPLY :
            HandleShowRoomListReply(msg->show_room_list_reply());
            break;
        case ChatProtocol::Packet::ROOM_CREATE_REPLY :
            HandleRoomCreateReply(msg->room_create_reply());
            break;
        case ChatProtocol::Packet::ROOM_JOIN_REPLY : 
            HandleRoomJoinReply(msg->room_join_reply());
            break;
        case ChatProtocol::Packet::NOTI_RECV_CHAT :
            HandleNotiRecvChat(msg->noti_recv_chat());
            break;
        default : 
            std::cout << " invalid type :" << type << std::endl; 
        ;
    }
}
void HandleLoginReply(const ChatProtocol::LoginReply &msg) {
    if(msg.err() == ChatProtocol::ErrorType::ERROR_NONE) {
        std::cout << "Login Success" <<std::endl;
        session_id_ = msg.id();
        DoShowRoomListRequest();
    }
    else {
        std::cout << "Login Failed. error type :" <<static_cast<int>(msg.err()) <<std::endl;
        state_ = ERROR;
    }
}

void HandleShowRoomListReply(const ChatProtocol::ShowRoomListReply &msg) {
    if(msg.err() == ChatProtocol::ErrorType::ERROR_NONE) {
        std::cout << "############ room list ############" <<std::endl;
        if(msg.roomlist_size() == 0) {
            std::cout << "no room exist " << std::endl;
        }
        else {
            for(int i = 0; i < msg.roomlist_size(); ++i) {
                auto &room_info = msg.roomlist(i);
                std::cout << room_info.number() << " : " << room_info.name() << std::endl;
            }
        }
        std::cout << "###################################" <<std::endl;
        state_ = LOGIN;
    }
    else {
        state_ = ERROR;
    }
}

void HandleRoomCreateReply(const ChatProtocol::RoomCreateReply &msg) {
    if(msg.err() == ChatProtocol::ErrorType::ERROR_NONE) {
        std::cout << "############ Joined Room " << msg.room_number() << "############" <<std::endl;
        state_ = ROOM;
    }
    else {
        state_ = LOGIN;
    }
}

void HandleRoomJoinReply(const ChatProtocol::RoomJoinReply &msg) {
    if(msg.err() == ChatProtocol::ErrorType::ERROR_NONE) {
    std::cout << "############ Joined Room " << msg.room_number() << "############" <<std::endl;
        state_ = ROOM;
    }
    else {
        std::cout << "Room Join Failed !" << std::endl;
        state_ = LOGIN;
    }
}

void HandleNotiRecvChat(const ChatProtocol::NotiRecvChat &msg) {
    std::cout << msg.nickname() <<" : " << msg.chat() << std::endl;
}

void Write() {
    // Write 테스트 중
    while(true) {
        switch (state_) {
            case CONNECTED :
            DoLoginRequest();
            break;
            case WAIT :
            std::cin.clear();
            usleep(1000);
            break;
            case LOGIN :
            DoLoginStep();
            break;
            case ROOM :
            DoChat();
            break;
            case ERROR :
            exit(1);
            break;
            default :
            break;
        }
    }

}

void DoLoginRequest() {
    std::shared_ptr<ChatProtocol::Packet> p = std::make_shared<ChatProtocol::Packet>();
    char nickname[17] = {0, };

    std::cout << "Login  nickname : ";
    std::cin.getline(nickname, 17);

    p->set_type(ChatProtocol::Packet::LOGIN_REQUEST);
    ChatProtocol::LoginRequest *body =p->mutable_login_request();

    body->set_nickname(nickname);

    int len = msg_.SerializeMessage(p);

    boost::asio::write(socket_, boost::asio::buffer(msg_.write_buf(), len));
    state_ = WAIT;
}

void DoShowRoomListRequest() {
    std::cout << "showroomListRequest" << std::endl;
    std::shared_ptr<ChatProtocol::Packet> p = std::make_shared<ChatProtocol::Packet>();

    p->set_type(ChatProtocol::Packet::SHOW_ROOM_LIST_REQUEST);
    p->mutable_show_room_list_request();
    int len = msg_.SerializeMessage(p);

    boost::asio::write(socket_, boost::asio::buffer(msg_.write_buf(), len));

    state_ = WAIT;

}

void DoLoginStep() {
    std::cout << " 1. create room" << std::endl;
    std::cout << " 2. join room" << std::endl;
    bool flag = false;
    int selected = 0;
    while(!flag) {
        std::cout << "select(use number) : " ;

        std::cin >> selected;
        if(selected == 1 || selected == 2) {
            flag = true;
            std::cin.ignore(10, '\n');
        }
        else {
            std::cout << "wrong number" << std::endl;
            std::cin.clear();
            std::cin.ignore(10, '\n');
        }
    }
    switch(selected) {
        case 1 :
        DoRoomCreateRequest();
        break;
        case 2 :
        DoRoomJoinRequest();
        break;
        default :
        break;
    }
}
void DoRoomCreateRequest() {
    std::cout << "Create Room" << std::endl;
    std::shared_ptr<ChatProtocol::Packet> p = std::make_shared<ChatProtocol::Packet>();

    char roomName[17] = {0, };

    std::cout << "Room Name : ";
    
    std::cin.getline(roomName, 17);
    // std::cin.ignore(10, '\n');

    p->set_type(ChatProtocol::Packet::ROOM_CREATE_REQUEST);
    ChatProtocol::RoomCreateRequest *body =p->mutable_room_create_request();
    body->set_roomname(roomName);
    int len = msg_.SerializeMessage(p);

    boost::asio::write(socket_, boost::asio::buffer(msg_.write_buf(), len));

    state_ = WAIT;

}

void DoRoomJoinRequest() {
    std::cout << "Join Room" << std::endl;
    std::shared_ptr<ChatProtocol::Packet> p = std::make_shared<ChatProtocol::Packet>();

    std::cout << "Room Number : ";
    int selected = -1;
    bool flag;
    while(!flag) {
        std::cout << "select(use number) : " ;

        std::cin >> selected;

        if(std::cin.fail()) {
            std::cout << "wrong number" << std::endl;
            std::cin.ignore(10, '\n');
            std::cin.clear();
        }
        else {
            flag = true;
            std::cin.ignore(10, '\n');
        };
    }
    p->set_type(ChatProtocol::Packet::ROOM_JOIN_REQUEST);
    ChatProtocol::RoomJoinRequest *body =p->mutable_room_join_request();
    body->set_room_number(selected);
    int len = msg_.SerializeMessage(p);    
    boost::asio::write(socket_, boost::asio::buffer(msg_.write_buf(), len));

    state_ = WAIT;
}

void DoChat() {
    std::shared_ptr<ChatProtocol::Packet> p = std::make_shared<ChatProtocol::Packet>();
    char chatmsg[256] = {0, };
    // std::cout << "Chat : ";
    std::cin.getline(chatmsg, 256);

    p->set_type(ChatProtocol::Packet::SEND_CHAT_REQUEST);
    ChatProtocol::SendChatRequest *body =p->mutable_send_chat_request();

    body->set_chat(chatmsg);

    int len = msg_.SerializeMessage(p);

    boost::asio::write(socket_, boost::asio::buffer(msg_.write_buf(), len));
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