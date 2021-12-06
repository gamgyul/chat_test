#include <iostream>
#include <vector>
#include <boost/asio.hpp>
#include <ctime>

namespace IP = boost::asio::ip;
namespace Asio = boost::asio;

std::string make_string_time() {
    time_t now = time(0);
    return  ctime(&now); 
}

int main () {
    try{
        Asio::io_context io_context;
        IP::tcp::acceptor acceptor(io_context, IP::tcp::endpoint(IP::tcp::v4(),9990));
        
        for(;;) {
        IP::tcp::socket sock(io_context);
        acceptor.accept(sock);

        std::string msg = make_string_time();

        boost::system::error_code ec;
        Asio::write(sock, Asio::buffer(msg), ec);
        }
    }
    catch(std::exception &e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
