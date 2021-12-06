#include <iostream>
#include <boost/asio.hpp>
// #include <boost/bind/bind.hpp>
#include <chrono>
#include <functional>
class Printer{
public:

    Printer(boost::asio::io_context& io)
        : timer_(io, std::chrono::seconds(1)),
          count_(0) {
            timer_.async_wait(std::bind(&Printer::print, this));
        }
    ~Printer() {
        std::cout << "final Count is" << count_ << std::endl;
    }

    void print() {
        std::cout << count_ << std::endl;
        count_++;

        timer_.expires_at(timer_.expiry()+std::chrono::seconds(1));
        timer_.async_wait(std::bind(&Printer::print, this));
    }
private:
    boost::asio::steady_timer timer_;
    int count_;
};

int main() {
    boost::asio::io_context io;
    Printer p(io);
    io.run();

    return 0;
}