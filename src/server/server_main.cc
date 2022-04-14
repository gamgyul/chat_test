#include "common.h"

#include <thread>

#include "acceptor.h"
#include "logger.h"

using namespace server;

int main(int argc, char** argv) {

    Asio::io_context acceptor_ios;
    Acceptor();    

    return 0;
}
