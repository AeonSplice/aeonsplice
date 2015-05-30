#ifndef _AEONNETWORK_
#define _AEONNETWORK_

#include <iostream>
#include <winsock2.h>
#include <cstdlib>
#include <cstring>
#include <cstdio>

#include <string>

namespace aeon
{
    class AeonMessage
    {
    public:
        enum { header_length = 4};
        enum { max_body_length = 512};

        AeonMessage() : body_length_(0)
        {}

        const char* data() const
        {
            return data_;
        }

        char* data()
        {
            return data_;
        }

        std::size_t length() const
        {
            return header_length + body_length_;
        }

        const char* body() const
        {
            return data_ + header_length;
        }

        char* body()
        {
            return data_ + header_length;
        }

        std::size_t body_length() const
        {
            return body_length_;
        }

        void body_length(std::size_t new_length)
        {
            body_length_ = new_length;
            if(body_length_ > max_body_length)
                body_length_ = max_body_length;
        }

        bool decode_header()
        {
            char header[header_length + 1] = "";
            // TODO: strncat doesn't exist, what the fuck?
            return false;
        }
    private:
        char data_[header_length + max_body_length];
        std::size_t body_length_;
    };

    class ServerConnection
    {
    public:
        ServerConnection();
        ~ServerConnection();
        bool create(int port);
        void destroy();

        bool running();
        void process();
    private:
        int aPort;
        int aAddr;
    };

    class ClientConnection
    {
    public:
        //virtual ~ClientConnection();
        void connect(std::string addr, int port);
        void disconnect();
        void sendMessage();
        void setCallback();
        void recieveMessages();
    private:
        // TODO: Callback pointer belongs here
    };
}

#endif // _AEONNETWORK_
