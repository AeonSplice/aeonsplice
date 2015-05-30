#include "aeonnetwork.hpp"
#include "aeonutil.hpp"
#include "aeonlog.hpp"

#include <iostream>
#include <windows.h>
#include <winsock2.h>

#include <string>

using namespace std;

namespace aeon
{
    ServerConnection::ServerConnection()
    {
        return;
    }
    ServerConnection::~ServerConnection()
    {
        return;
    }
    bool ServerConnection::create(int port)
    {
        log("Starting network.", AEON_INFO);
        WSAData wsaData;
        if (WSAStartup(MAKEWORD(1,1), &wsaData) != 0)
        {
            log("Failed WSAStartup.", AEON_ERROR);
            return false;
        }
        else
        {
            log("Successfully started server.", AEON_INFO);
            return true;
        }
    }
    void ServerConnection::destroy()
    {
        WSACleanup();
    }

    bool ServerConnection::running()
    {
        return false;
    }
    void ServerConnection::process()
    {
        // TODO: Process network packets
    }
}
