#include <graphics.h>
#include <windows.h>
#include <iostream>
#include <string>
#include "Main.h"
#include "portget.h"

bool IsPortAvailable(unsigned short port) {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        return false;
    }

    SOCKET testSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (testSocket == INVALID_SOCKET) {
        WSACleanup();
        return false;
    }

    sockaddr_in service;
    service.sin_family = AF_INET;
    service.sin_addr.s_addr = INADDR_ANY;
    service.sin_port = htons(port);

    // 设置SO_REUSEADDR以便快速重用端口
    int reuse = 1;
    setsockopt(testSocket, SOL_SOCKET, SO_REUSEADDR, (const char*)&reuse, sizeof(reuse));

    bool isAvailable = (::bind(testSocket, (SOCKADDR*)&service, sizeof(service)) != SOCKET_ERROR);

    closesocket(testSocket);
    WSACleanup();

    return isAvailable;
}

unsigned short GenerateDynamicPort() {

    int mx = 0;
    unsigned short port;
    bool mach = false;

    while (mx < 100 && !mach)
    {
        randomize();
        mx = random(1000);

        if (IsPortAvailable(mx) && IsPortAvailable(mx + 1))
        {
            mach = true;
        }
    }

    port = mx;

    if (debug) {
        std::cout << "Selected available port: " << port << std::endl;
    }

    return port;
}