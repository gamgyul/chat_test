#include <iostream>
#include <string>
#include <cstdlib>
#include <cstring>
static const int kHeaderLength = 4;
static const int kBodyLength = 256;

enum class MsgType {
CONNECT_REQUEST = 0,
CONNECT_REPLY = 1,
ENUM_MAX = 2
};


class MsgReaderWriter {
private:
    char data_[kHeaderLength + kBodyLength];

public:
    void set_data(int type, char *data) {
        void *type_ptr = static_cast<void *>(&type);
        memcpy(data_, type_ptr, kHeaderLength); //header 복사
        memcpy(data_ + kHeaderLength, data, kBodyLength); //body 복사
    }

    int ReadHeader() {
        int type;
        memcpy(&type, data_, kHeaderLength);

        return type;
    }


    char * GetBody() {
        return data_ + kHeaderLength;
    }
}
;

class Message {

private:
    char data_[kHeaderLength + kBodyLength];

public:
    void set_data(int type, char *data) {
        void *type_ptr = static_cast<void *>(&type);
        memcpy(data_, type_ptr, kHeaderLength); //header 복사
        memcpy(data_ + kHeaderLength, data, kBodyLength); //body 복사
    }

    int ReadHeader() {
        int type;
        memcpy(&type, data_, kHeaderLength);

        return type;
    }


    char * GetBody() {
        return data_ + kHeaderLength;
    }
};