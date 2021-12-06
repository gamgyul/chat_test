#include <iostream>
#include <boost/asio.hpp>
namespace Asio = boost::asio;
int main() {
    Asio::io_context io;

    Asio::steady_timer t(io, std::chrono::seconds(5));
    t.wait();

    std::cout << "Hello Boost!" << std::endl;

    return 0;
}