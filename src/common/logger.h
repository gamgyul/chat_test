#ifndef LOGGER_H_
#define LOGGER_H_

#include "common.h"

#define LOG_TEMP GetLogger().WriteLog(__FILE__, __LINE__)

namespace server {

class Log {

private:
    std::ostream &log_stream_;
    
public:
    Log();
    ~Log();
    std::ostream &WriteLog(std::string_view filename, int line);
};

} //namespace server

server::Log& GetLogger();

#endif // LOGGER_H_