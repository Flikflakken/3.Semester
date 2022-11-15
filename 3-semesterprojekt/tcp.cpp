#include "tcp.h"

using namespace std;

tcp::tcp() {

}

bool tcp::tcpSocket_Connect() {
    if (mSock == -1)
    {
        cout << "Can't create a socket" << endl;
        return false;
    }
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(mPort);
    inet_pton(AF_INET, mIpaddr.c_str(), &hint.sin_addr);
    //	Connect to the server on the socket
    int connectRes = connect(mSock, (sockaddr*)&hint, sizeof(hint));
    if (connectRes == -1)
    {
        cout << "Can't connect to server!" << endl;
        return false;
    }
    //acknowledgeErr();
    return true;
}

bool tcp::sendMessage(std::string input) {
    char homestr[] = {'h','o','m','e','(',')','\0'};
    int sendRes = send(mSock, input.c_str(), input.size(),MSG_NOSIGNAL);
    //int sendRes = send(mSock, input.c_str(), input.size(),MSG_NOSIGNAL);
    if (sendRes == -1)
    {
        cout << "Could not send to server! Whoops!\r\n";;
        return false;
    }
    return true;
}

bool tcp::recive2Messages() {
    memset(&mBuf, 0, 100);
    int bytesReceived = recv(mSock, &mBuf, 100, 0);
    if (bytesReceived == -1){
        cout << "There was an error getting response from server\r\n";
        return false;
    }else{
        //		Display response
        cout << "SERVER> " << string(mBuf, bytesReceived) << "\n";
    }
    memset(&mBuf, 0, 100);
    int bytesReceived1 = recv(mSock, &mBuf, 100, 0);
    if (bytesReceived1 == -1){
        cout << "There was an error getting response from server\r\n";
    }else{
        //		Display response
        cout << "SERVER> " << string(mBuf, bytesReceived1) << "\n";
    }
    return true;
}

bool tcp::recive1Messages() {
    memset(&mBuf, 0, 4096);
    ssize_t bytesReceived = recv(mSock, mBuf, 4069, 0);
    if (bytesReceived == -1){
        cout << "There was an error getting response from server\r\n";
    }else{
        //		Display response
        cout << "SERVER> " << string(mBuf, bytesReceived) << "\n";
    }
    return true;
}

void tcp::doHome() {
    string userInput = "HOME()\n";
    sendMessage(userInput);
    recive2Messages();
}

void tcp::doMove(int width) {
    string move = "MOVE(" + std::to_string(width) + ")\n";
    char* c_array;
    c_array =&move[0];
    cout << c_array << endl;
    sendMessage(c_array);
    //recive2Messages();
}

void tcp::acknowledgeErr() {
    string FSACK = "FSACK()\n";
    sendMessage(FSACK);
    recive1Messages();
}

void tcp::sayBye() {
    string userInput = "BYE()\n";
    sendMessage(userInput);
    recive1Messages();
    close(mSock);
}

void tcp::doGrip(float force, float partWidth) {
    string userInput;
    userInput = "GRIP(" + std::to_string(force) + "," + std::to_string(partWidth) + ")\n";
    sendMessage(userInput);
    recive2Messages();
}

void tcp::setVerbose(bool status) { // Status 1 og 0
    string userInput;
    userInput = "VERBOSE=(" + std::to_string(status) + ")\n";
    sendMessage(userInput);
    recive2Messages();
}











