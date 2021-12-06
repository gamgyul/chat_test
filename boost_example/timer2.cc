#include <iostream>
#include <boost/asio.hpp>
namespace Asio = boost::asio;

void print(const boost::system::error_code &ec) {
    std::cout << "Hello Boost Async!" << std::endl;
}

int main() {
    Asio::io_context io;

    Asio::steady_timer t(io, Asio::chrono::seconds(5));
    t.async_wait(&print);

     io.run();
    std::cout << "tset" <<std::endl;
    return 0;
}
