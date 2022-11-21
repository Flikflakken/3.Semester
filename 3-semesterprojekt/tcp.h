#ifndef tcp_H
#define tcp_H
#include <QObject>
#include <QTcpSocket>
#include <QDebug>
#include <QCoreApplication>

class tcp : public QObject
{
Q_OBJECT;
public:
    explicit tcp(QObject *parent = nullptr);
    ~tcp();

//Funktioner
    bool connectTcp();
    void acknowledgeErr();
    void bye();
    void verbose(bool status);
    bool validOperation(int operation);

    void home();
    void move(int width);
    void grip(float force);
    void release();
    bool readTcpData();
//getter og setter
    int getMTimeOut() const;
    void setMTimeOut(int mTimeOut);
    int getMPort() const;
    void setMPort(int mPort);
    const QString &getMip() const;
    void setMip(const QString &mIp);
    bool getMHasGripped() const;
    void setMHasGripped(bool mHasGripped);

signals:
public slots:

private:
    QTcpSocket *mSocket{};
    int mTimeOut{}, mDelay{}, mPort{};
    QString mIP;
    QString mReceivedMessage = "No message";

    bool mConnectionStatus = false;
    bool mReadyStatus      = false;
    bool mHasGripped       = false;
    bool mConfirmedGrip    = false;
};

#endif // TCP_H
