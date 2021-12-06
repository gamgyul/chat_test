#include "boost/asio.hpp"
#include <iostream>
#include <string>

namespace Asio = boost::asio;

int main() {
    std::string raw_ip = "127.0.0.1";
    unsigned short port_num = 12000;

    boost::system::error_code ec;

    Asio::ip::address ip_address = Asio::ip::address::from_string(raw_ip, ec);

    Asio::ip::tcp::endpoint ep(ip_address, port_num);
}

int boost_server() {
    unsigned short server_port_num = 15000;

    boost::system::error_code ec;

    Asio::ip::address ip_address = Asio::ip::address_v4::any();

    Asio::ip::tcp::endpoint ep(ip_address, server_port_num);
}