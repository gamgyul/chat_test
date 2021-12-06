#include <iostream>
#include <vector>
#include <ctime>
#include <memory>
#include <boost/asio.hpp>
#include <boost/bind.hpp>

namespace IP = boost::asio::ip;
namespace Asio = boost::asio;

std::string make_string_time() {
    time_t now = time(0);
    return  ctime(&now); 
}

class TCPConnection : public std::enable_shared_from_this<TCPConnection> {
public:
    typedef std::shared_ptr<TCPConnection> pointer;
    static pointer Create(Asio::io_context &io_context) {
        return pointer(new TCPConnection(io_context));
    }

    IP::tcp::socket &socket() {
        return socket_;
    }

    void Start() {
        msg_ = make_string_time();

        Asio::async_write(socket_, Asio::buffer(msg_), boost::bind(&TCPConnection::HandleWrite, shared_from_this(), 
                                                                 Asio::placeholders::error,
                                                                 Asio::placeholders::bytes_transferred));
    }

private:
    TCPConnection(Asio::io_context &io_context) 
        : socket_(io_context) {

    }

    void HandleWrite(const boost::system::error_code &ec, size_t byte_transferred) {

    }

    IP::tcp::socket socket_;
    std::string msg_;
};

class TCPServer {
public:
    TCPServer(Asio::io_context &io_context) 
            : acceptor_(io_context, IP::tcp::endpoint(IP::tcp::v4(), 9999)){
        StartAccept();
    }
private:
    void StartAccept() {
        TCPConnection::pointer new_connection = TCPConnection::Create(acceptor_.get_executor().context());

        acceptor_.async_accept(new_connection->socket(), boost::bind(&TCPServer::HandleAccept, this, new_connection, Asio::placeholders::error));
    }

    void HandleAccept(TCPConnection::pointer new_connection, const boost::system::error_code &ec) {
        if (!ec) {
            new_connection->Start();
        }
        StartAccept();
    }
    IP::tcp::acceptor acceptor_;
};

int main() {
    try {
        Asio::io_context io_context;
        TCPServer server(io_context);
        io_context.run();
    }
    catch(std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}