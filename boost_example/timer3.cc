#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include <chrono>
#include <functional>
namespace Asio = boost::asio;

void print(const boost::system::error_code &ec, boost::asio::steady_timer *t, int *count) {
    if(*count < 5) {
        std::cout << *count << std::endl;
        ++(*count);
        t->expires_at(t->expiry() + std::chrono::seconds(1));
        t->async_wait(boost::bind(print, Asio::placeholders::error, t, count));
    }
}

int main() {
    
    Asio::io_context io;
    int count = 0;;
    Asio::steady_timer t(io, Asio::chrono::seconds(1));
    
    t.async_wait(boost::bind(print, Asio::placeholders::error, &t, &count));

    io.run();

    return 0;
}