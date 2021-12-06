#include <iostream>
#include <boost/asio.hpp>
#include <vector>
namespace IP = boost::asio::ip;
namespace Asio = boost::asio;

int main(int argc, char** argv) {
    try {
        if(argc != 2) {
            std::cerr << "Usage: client for localhostserver <port>" << std::endl;
            return 1;
        }
        int port_num = atoi(argv[1]);
        std::cout <<"port num: " << port_num << std::endl;
        Asio::io_context io_context;
        IP::tcp::endpoint endpoint(IP::address::from_string("127.0.0.1"), port_num);
        IP::tcp::socket sock(io_context);
        sock.connect(endpoint);

        for(;;) {
            std::vector<char> buf(128);
            boost::system::error_code error;

            size_t len = sock.read_some(Asio::buffer(buf), error);
            if(error == Asio::error::eof) {
                break;
            }
            else if(error)
                throw error;

            std::cout.write(buf.data(), len);
;        }
    }
    catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}