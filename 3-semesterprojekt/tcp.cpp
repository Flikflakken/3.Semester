#include <iostream>
#include <string>
#include "tcp.h"

using namespace std;

tcp::tcp(QObject *parent) : QObject(parent) {

}

bool tcp::connectTcp() {
    mSocket = new QTcpSocket(this);
    mSocket->connectToHost(mIP,mPort);

    if (mSocket->waitForConnected(mTimeOut))
    {
        qDebug() << "Connected!" << Qt::endl;
        mConnectionStatus = true;
        acknowledgeErr();
        return true;
    }else{
        qDebug() << "Connect Error!" << Qt::endl;
        mConnectionStatus = false;
        return false;
    }
}

void tcp::acknowledgeErr()
{
    if (validOperation(1)){
        mReadyStatus = false;
        mSocket->write("FSACK()\n");

        qDebug() << "Acknowledgde success!" << Qt::endl;

        if (readTcpData())
            mReadyStatus = true;
    }
}

bool tcp::readTcpData() {
    mSocket->waitForReadyRead(mTimeOut);
    mReceivedMessage = mSocket->readAll();
    qDebug() << mSocket->readAll();
    qDebug() << mReceivedMessage;

    if(mReceivedMessage.startsWith("ACK"))
    {
        return true;
    }
    else if(mReceivedMessage.startsWith("FIN"))
    {
        return true;
    }
    else if(mReceivedMessage.startsWith("VERBOSE"))
    {
        return true;
    }
    else
    {
        qDebug() << mReceivedMessage;
        return false;
    }
}

void tcp::home() {
    if (validOperation(2)){
        mReadyStatus = false;
        mSocket->write("HOME()\n");
        mSocket->waitForBytesWritten(mTimeOut);
        if (readTcpData())
            mReadyStatus = true;
    }
}

void tcp::move(int width) {
    if (validOperation(2)){
        mReadyStatus = false;
        std::string move = "MOVE(" + std::to_string(width) + ")\n";
        char* c_array;
        c_array = &move[0];
        mSocket->write(c_array);

        mSocket->waitForBytesWritten(mTimeOut);
        if (readTcpData())
            mReadyStatus = true;
    }
}

void tcp::bye() {
    qDebug() << "Disconnecting.." << Qt::endl;
    mSocket->write("BYE()\n");

    if (readTcpData()){
        mSocket->close();
        mConnectionStatus = false;
        qDebug() << "Disconnected." << Qt::endl;
    }else{
        qDebug() << "Error during Disconnect" << Qt::endl;
    }
}

void tcp::verbose(bool status) {
    if (validOperation(1)){
        mReadyStatus = false;
        if(status){
            mSocket->write("VERBOSE=1\n");
        }else{
            mSocket->write("VERBOSE=0\n");
        }
        if (readTcpData())
            mReadyStatus = true;
    }
}

void tcp::grip(float force, float width) {
    if(tcp::validOperation(2)){
        mReadyStatus = false;
        std::string grip = "GRIP(" + std::to_string(force) + "," + std::to_string(width) + ")\n";
        char* c_array;
        c_array = &grip[0];
        qDebug() << "Move command: " << c_array;
        mSocket->write(c_array);
        if(readTcpData()){
            if (readTcpData()){
                mReadyStatus = true;
                mHasGripped = true;
            }

        }
    }
}

void tcp::release(float pullBack) {
    if (validOperation(2)){
        mReadyStatus = false;
        std::string move = "RELEASE(" + std::to_string(pullBack) + ")\n";
        char* c_array;
        c_array = &move[0];
        mSocket->write(c_array);
        mSocket->waitForBytesWritten(mTimeOut);
        if(readTcpData())
            mReadyStatus = true;
    }
}

void tcp::gripperState()
    {
        cout << "inside GripperState" << endl;
        QString state = "";
        if(validOperation(1)) {
            mSocket->write("GRIPSTATE?\n");
            mSocket->waitForBytesWritten(mTimeOut);
            mSocket->waitForReadyRead(mTimeOut);

            state = mSocket->readAll();
            std::cout << "Registered state = " << state.toStdString() << std::endl;
        }

        char stateChar = state.toStdString().at(10);

        if(stateChar == '3') {
            std::cout << "Robot lost it" << std::endl;
            mConfirmedGrip = false;

        } else if(stateChar == '4') {
            std::cout << "Robot is holding" << std::endl;
            mConfirmedGrip = true;
        } else {
            mConfirmedGrip = false;
        }
    }

bool tcp::connectTcpMatLab(){
    mSocket = new QTcpSocket(this);
    mSocket->connectToHost(mIP,mPort);

    if (mSocket->waitForConnected(mTimeOut))
    {
        qDebug() << "Connected!" << Qt::endl;
        return true;
    }else{
        qDebug() << "Connect Error!" << Qt::endl;
        return false;
    }
}

std::string tcp::sendDataMatLab(double input, double input2) {
    qDebug() << "Sending Data" << Qt::endl;
    //std::string send = "" + std::to_string(input) + "";
    //char* c_array;
    //c_array = &send[0];
    //std::string send2 = "" + std::to_string(input2) + "";
    //char* c_array2;
    //c_array2 = &send2[0];
    //mSocket->write(c_array);
    //mSocket->write(c_array2);
    //double d = 1.234;
    char c[sizeof(input)];
    qDebug() << "size of " << sizeof(input) << "this is data1" << Qt::endl;
    memcpy(c,&input,sizeof(input));

    char d[sizeof(input2)];
    qDebug() << "size of " << sizeof(input2) << "this is data2" << Qt::endl;
    memcpy(d,&input2,sizeof(input2));
    mSocket->write(c);
    mSocket->waitForBytesWritten(mTimeOut);
    qDebug() << "data 1 send!" << Qt::endl;
    mSocket->write(d);
    mSocket->waitForBytesWritten(mTimeOut);
    qDebug() << "data 2 send!" << Qt::endl;
    mSocket->waitForReadyRead(mTimeOut);
    //qDebug() << readTcpData() << Qt::endl;
    string output;
    output = readTcpDataMatLab();
    mSocket->close();
    return output;
}

std::string tcp::readTcpDataMatLab() {
    mSocket->waitForReadyRead(mTimeOut);
    QString readOutput;
    readOutput = mSocket->readAll();
    return readOutput.toStdString();
}

//Getter og Setter------------------------------------------

int tcp::getMPort() const {
    return mPort;
}

void tcp::setMPort(int mPort) {
    tcp::mPort = mPort;
}

const QString &tcp::getMip() const {
    return mIP;
}

void tcp::setMip(const QString &mIp) {
    mIP = mIp;
}

int tcp::getMTimeOut() const {
    return mTimeOut;
}

void tcp::setMTimeOut(int mTimeOut) {
    tcp::mTimeOut = mTimeOut;
}

bool tcp::getMHasGripped() const {
    return mHasGripped;
}

void tcp::setMHasGripped(bool mHasGripped) {
    tcp::mHasGripped = mHasGripped;
}

bool tcp::validOperation(int operation){
    switch(operation){
        case 1:{
            if(mConnectionStatus){
                return true;
                break;
            }
            else if(!mConnectionStatus){
                qDebug() << "No connection.";
                return false;
                break;
            }
            else{
                qDebug() << "Unspecified error while attempting operation.";
                return false;
                break;
            }
        }
        case 2:{
            if(mConnectionStatus && mReadyStatus){
                return true;
                break;
            }
            else if(!mConnectionStatus){
                qDebug() << "No connection.";
                return false;
                break;
            }
            else if(!mReadyStatus){
                qDebug() << "Gripper not ready - Case 2!";
                return false;
                break;
            }
            else{
                qDebug() << "Unspecified error while attempting operation.";
                return false;
                break;
            }
        }
        case 3:{
            if(mConnectionStatus && mReadyStatus && mHasGripped){
                return true;
                break;
            }
            else if(!mConnectionStatus){
                qDebug() << "No connection.";
                return false;
                break;
            }
            else if(!mReadyStatus){
                qDebug() << "Gripper not ready - Case 3!";
                return false;
                break;
            }
            else if(!mHasGripped){
                qDebug() << "Nothing Gripped Yet!";
                return false;
                break;
            }
            else{
                qDebug() << "Unspecified error while attempting operation.";
                return false;
                break;
            }
        }
        default:{
            qDebug() << "Statement fell through all cases.. Recheck that?";
            return false;
        }
    }
}

tcp::~tcp() = default;



