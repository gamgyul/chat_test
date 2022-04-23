
#include "server.h"

#include <boost/asio.hpp>
#include <boost/bind.hpp>

#include "session.h"
#include "common/logger.h"


namespace server {

Server::Server(const Tcp::endpoint &endpoint)
    :server_ioc_(), acceptor_(server_ioc_, endpoint),
    accept_session_id_(0) {

    dispatch_mgr_.Init(this);

    for(int i = 0; i < kDispatcherSize; ++i) {
        dispatch_thread_.push_back(std::thread(&Dispatcher::Run ,dispatch_mgr_.dispatcher()[i].get())); 
    }
}

Server::~Server() {

}

void Server::Run() {

#ifndef _WIN32
    pthread_setname_np(pthread_self(), "Server_thread");
#endif

    LOG_TEMP << "Server Run" << std::endl;

    StartAccept();

    std::thread socket_thread(&Server::ReadThreadRun,this);
    socket_thread.detach();
    server_ioc_.run();
    
}

void Server::ReadThreadRun() {
    #ifndef _WIN32
    pthread_setname_np(pthread_self(), "Read_thread");
#endif
    while(true) {
        socket_ioc_.run();
        socket_ioc_.restart();
        sleep(1);

    }
}

void Server::StartAccept() {
    acceptor_.async_accept(socket_ioc_,
        [this](boost::system::error_code ec, Tcp::socket socket) {
            if(!ec) {
                while(session_map_.find(accept_session_id_) != session_map_.end())
                    accept_session_id_++;
                std::shared_ptr<Session> new_session = std::make_shared<Session>(std::move(socket), this, accept_session_id_);
                auto ret = session_map_.insert({accept_session_id_, new_session});

                if(!ret.second)
                    LOG_TEMP << "session map error" << std::endl;
                else
                    LOG_TEMP << "Session " << accept_session_id_ << " accepted" << std::endl;
                new_session->Start();
            }
            else {
                LOG_TEMP << "accept error : " << ec.message() << std::endl;
            }
            StartAccept();
        }
    );
}

} //namespace server