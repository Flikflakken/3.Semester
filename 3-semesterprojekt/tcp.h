//
// Created by mikkel on 11/8/22.
//

#ifndef INC_3_SEMESTERPROJEKT_TCP_H
#define INC_3_SEMESTERPROJEKT_TCP_H
#include <iostream>
#include "string"
#include "string.h"
#include <sys/types.h>
#include "unistd.h"
#include "sys/socket.h"
#include "netdb.h"
#include "arpa/inet.h"

class tcp {

public:
    tcp();
    bool tcpSocket_Connect();
    void doHome();
    void doMove(int width);
    void acknowledgeErr();
    void sayBye();
    void doGrip(float force, float partWidth);
    void setVerbose(bool status);
    bool sendMessage(std::string input);
    bool recive2Messages();
    bool recive1Messages();



private:
    std::string mIpaddr = "192.168.100.10";
    int mPort = 1000;
    char mBuf[4096];
    int mSock = socket(AF_INET, SOCK_STREAM, 0);

};


#endif //INC_3_SEMESTERPROJEKT_TCP_H
