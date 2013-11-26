#include <net/Eventloop.h>
#include <net/TcpServer.h>
#include <net/Acceptor.h>
#include <net/TcpConnection.h>
#include <net/IoBuffer.h>


using namespace thefox;

Eventloop::Eventloop(TcpServer *server)
	: _server(server)
	, _threadId(::GetCurrentThreadId())
	, _looping(false)
	, _quit(false)
{
}

Eventloop::~Eventloop()
{
}

void Eventloop::loop()
{
	OVERLAPPED *overlapped = NULL;
	ULONG_PTR completionKey = NULL;
	DWORD bytesTransfered = 0;
	
	while (_quit) {
		BOOL retCode = _iocp.getStatus(&bytesTransfered, &completionKey, &overlapped, INFINITE);
		// �յ��˳���־��ֱ���˳�
		if (0 == completionKey)
			break;
		
		if (!retCode) {
			// if (!_errorCallback(conn, ::GetLastError()))
// 			{
// 				break;
// 			}
			continue;
		}
		
		if (retCode && completionKey && overlapped) {
			
			
			
		}
			
			
						
			// �жϿͻ����Ƿ�Ͽ�����
			if ((0 == bytesTransfered) && (RECV == ioBuf->ioType() || WRITE == ioBuf->ioType()) {
				_server->removeConnection(&conn);
				continue;
			} else {
				switch (ioBuf->getIoType()) {
					case IoBuffer::IoType::Accept: {
						Acceptor *acceptor = reinterpret_cast<Acceptor *>(*completionKey);
						acceptor->handleAccept(ioBuffer);
						break;
					}
					case IoBuffer::IoType::Recv: {
						_server->inBuffer.append(ioBuf->_wsaBuf.begin, ioBuf->_wsaBuf.len)
							_server->_messageCallback(conn, conn->_inBuffer);
						break;
					}
					case IoBuffer::IoType::Send:
						_server->_writeCompleteCallback(conn);
						default:
						break;
					
				}
			}
	}
	_looping = false;
}
