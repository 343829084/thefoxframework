#include <net/tcp_server.h>
#include <base/types.h>

#ifdef WIN32
#include <net/iocp.h>
#else
#include <net/epoll.h>
#endif

using namespace thefox;

TcpServer::TcpServer(EventLoop *eventloop, const std::string &nameArg)
{
#ifdef WIN32
    _model = new Iocp(eventloop, nameArg);
#else
    _model = new Epoll(eventloop, nameArg);
#endif
}

TcpServer::~TcpServer()
{
    delete _model;
}

bool TcpServer::start(const InetAddress &listenAddr)
{
    return _model->start(listenAddr);
}

bool TcpServer::started()
{
    return _model->started();
}

void TcpServer::send(const TcpConnectionPtr &conn, const char *data, size_t len)
{
    _model->send(conn, data, len);
}
void TcpServer::send(const TcpConnectionPtr &conn, const std::string &data)
{
    _model->send(conn, data.c_str(), data.length());
}

void TcpServer::removeConnection(TcpConnectionPtr conn)
{
    _model->removeConnection(conn);
}

void TcpServer::setConnectionCallback(const ConnectionCallback &cb)
{
    _model->setConnectionCallback(cb);
}
void TcpServer::setCloseCallback(const CloseCallback &cb)
{
    _model->setCloseCallback(cb);
}

void TcpServer::setMessageCallback(const MessageCallback &cb)
{
    _model->setMessageCallback(cb);
}

void TcpServer::setWriteCompleteCallback(const WriteCompleteCallback &cb)
{
    _model->setWriteCompleteCallback(cb);
}