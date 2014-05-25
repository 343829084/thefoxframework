#include <net/tcp_connection.h>
#include <log/logging.h>
#include <net/event_loop.h>

using namespace thefox;

void thefox::defaultMessageCallback(TcpConnection *conn,
                                Buffer* buf,
                                Timestamp recvTime)
{
	buf->retrieveAll();
}

TcpConnection::TcpConnection(EventLoop *loop, SOCKET sockfd,
							int id, const InetAddress &peerAddr)
	: _loop(loop)
    , _id(id)
    , _peerAddr(peerAddr)
    , _readBytes(0)
    , _writeBytes(0)
	, _arg(NULL)
	, _state(kConnecting)
{
	_socket.setFd(sockfd);
	_event.init(this);
}

TcpConnection::~TcpConnection()
{
}

void TcpConnection::send(const std::string &data)
{
	this->send(data.c_str(), data.length());
}

void TcpConnection::send(const char *data, size_t len)
{
	_writeBuffer.append(data, len);

	MutexGuard lock(_mutex);
	if (!_event.write) {
		_event.write = true;
		_loop->updateWrite(&_event);
	}
}

bool TcpConnection::shutdown()
{
	return _socket.shutdownWrite();
}

void TcpConnection::setTcpNoDelay(bool on)
{
	_socket.setTcpNoDelay(on);
}

void TcpConnection::forceClose()
{
	if (kConnected == _state || kDisconnecting == _state) {
		setState(kDisconnecting);
		// Ͷ�ݹر��¼�
		_event.close = true;
		_loop->postClose(&_event);
	}
}

void TcpConnection::connectEstablished()
{
	THEFOX_LOG(DEBUG) << "TcpConnection::connectEstablished()";

	assert(kConnecting == _state );
	setState(kConnected);

	if (_connectionCallback)
		_connectionCallback(this);

	_loop->addEvent(&_event);

	// Ͷ�ݶ��¼�
	_event.read = true;
	_loop->updateRead(&_event);
}

void TcpConnection::connectDestroyed()
{
	if (kConnected == _state || kDisconnecting == _state) {
		setState(kDisconnected);
		THEFOX_LOG(DEBUG) << "connection closed, addr:" << _peerAddr.toIpPort();

		_loop->delConnection(this);
		_socket.close();

		if (_connectionCallback)
			_connectionCallback(this);
	}

	if (0 == _event.refCount()) {
		if (_removeConnectionCallback)
			_removeConnectionCallback(this); // �������һ��
		return;
	}
}
