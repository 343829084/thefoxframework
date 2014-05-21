#ifndef _THEFOX_NET_TCP_HANDLER_H_
#define _THEFOX_NET_TCP_HANDLER_H_

#include <net/callbacks.h>

namespace thefox
{

class TcpHandler
{
public:
	friend class TcpConnection;

	TcpHandler();
	virtual ~TcpHandler();

	/// @brief ��������״̬�ı�ص�����
    void setConnectionCallback(const ConnectionCallback &cb)
    { _connectionCallback = cb; }
	 /// @brief �������ӹرջص�����
    void setCloseCallback(const CloseCallback &cb)
    { _closeCallback = cb; }
	/// @brief �����յ����ݵĻص�����
    void setMessageCallback(const MessageCallback &cb)
    { _messageCallback = cb; }
	/// @brief ���û����������ݷ�����ɺ�Ļص�����
    void setWriteCompleteCallback(const WriteCompleteCallback &cb)
    { _writeCompleteCallback = cb; }
protected:
	ConnectionCallback _connectionCallback;
    CloseCallback _closeCallback;
    MessageCallback _messageCallback;
    WriteCompleteCallback _writeCompleteCallback;
};

} // namespace thefox

#endif // _THEFOX_NET_TCP_HANDLER_H_