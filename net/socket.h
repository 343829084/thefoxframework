#ifndef _THEFOX_NET_SOCKET_H_
#define _THEFOX_NET_SOCKET_H_

#ifdef WIN32
#include <Winsock2.h>
#define socklen_t int

class SocketLibrary
{
public:
	/// @breaf socket���ʼ��
    static void startup()
	{
        WSADATA wsd;
        ::WSAStartup(MAKEWORD(2, 2), &wsd);
    }

	/// @breaf socket��ע��
    static void cleanup() { ::WSACleanup(); }
};

#else
#include <sys/types.h>
#include <sys/socket.h>
#define SOCKET int
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#endif

#include <base/types.h>

namespace thefox
{

class InetAddress;

class Socket
{
public:
	explicit Socket(SOCKET sockfd);
	~Socket();

	static InetAddress getLocalAddr(SOCKET sockfd);
	static SOCKET create();
	static bool connect(SOCKET sockfd, const InetAddress &serverAddr);
	static bool close(SOCKET sockfd);
	
	bool bind(const InetAddress &listenAddr);
	bool listen();
	SOCKET accept(InetAddress *peerAddr);
	
	bool shutdownWrite();
	// ȡ��nagle�㷨
	void setTcpNoDelay(bool on);
	void setKeepAlive(bool on);

	SOCKET fd() const { return _sockfd; }

private:
	SOCKET _sockfd;
};

} // namespace thefox

#endif // _THEFOX_NET_SOCKET_H_