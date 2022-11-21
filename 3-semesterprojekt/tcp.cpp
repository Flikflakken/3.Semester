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

void tcp::grip(float force) {
    if(tcp::validOperation(2)){
        mReadyStatus = false;
        std::string grip = "GRIP(" + std::to_string(force) + ")\n";
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

void tcp::release() {
    if (validOperation(2)){
        mReadyStatus = false;
        mSocket->write("RELEASE()\n");
        mSocket->waitForBytesWritten(mTimeOut);
        if(readTcpData())
            mReadyStatus = true;
    }
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



