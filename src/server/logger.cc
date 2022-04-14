#include "logger.h"

#include <iomanip>

namespace server {

// temp로 cout사용
Log::Log() : log_stream_(std::cout){

}
Log::~Log() {

}
std::ostream &Log::WriteLog(std::string_view filename, int line) {

    this->log_stream_ << filename << ":" << line << "   ";

    return this->log_stream_;
}
} // namespace server

server::Log& GetLogger() {
    static server::Log logger;
    return logger;
}