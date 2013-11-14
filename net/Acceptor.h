#ifndef _THEFOX_NET_ACCEPTOR_H_
#define _THEFOX_NET_ACCEPTOR_H_

namespace thefox
{

class Acceptor : noncopyable
{
public:
	OVERLAPPED _overlapped;
	
private:
	LPFN_ACCEPTEX                m_lpfnAcceptEx;                // AcceptEx �� GetAcceptExSockaddrs �ĺ���ָ�룬���ڵ�����������չ����
	LPFN_GETACCEPTEXSOCKADDRS    m_lpfnGetAcceptExSockAddrs; 
	SOCKET _sock;
};

}

#endif // _THEFOX_NET_ACCEPTOR_H_