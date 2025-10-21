#ifndef UP_TCPCLIENT_H
#define UP_TCPCLIENT_H

#include <QObject>
#include <QtCore>
#include <QtNetwork>

#define UP_TCP_PORT	30966 // TCP端口号


typedef enum {
    UP_CMD_CONNECT, // 连接成功
    UP_CMD_READ, // 读取数据
    UP_CMD_SENT_OK, // 发送完成
    UP_CMD_DIS, //  断开连接
    UP_CMD_ERR, // 连接错误
    UP_CMD_VERIFY,// 验证成功
    UP_CMD_PWDERR, // 账号密码错误
}UP_TCP_CMD;

class UP_TcpClient : public QThread
{
    Q_OBJECT
public:
    explicit UP_TcpClient(QObject *parent = 0);
    ~UP_TcpClient();

    bool newConnect(const QString &host, int port=UP_TCP_PORT); //连接服务器
    bool sentMessage(char *buf,  int len);
    bool sentMessage(QString &str);
    bool sentMessage(QByteArray &data);
    int getData(QByteArray &data);

    bool getSentState(void) {return mListData.isEmpty();}
    bool getConnecttState(void) {return isConnect;}
    void closeConnect(void);
    void clear(void){mRecvData.clear();}

protected:
    int writeMessage(QByteArray &data);

signals:
    void connectSig(int);

public slots:
    void timeoutDone();
    void connectedSlot();
    void disconnectedSlot();
    void readMessageSlot();  //接收数据
    void displayError(QAbstractSocket::SocketError);  //显示错误

private:
    QHostAddress *mServerIP;
    QTcpSocket *mTcpSocket;

    QTimer *timer;
    QList<QByteArray> mListData;
    QByteArray mRecvData;
    bool isConnect;
};

#endif // UP_TCPCLIENT_H
