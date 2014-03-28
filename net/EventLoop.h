/*
* @filename EventLoop.h
* @brief �¼�ѭ��
* @author macwe@qq.com
*/

#ifndef _THEFOX_NET_EVENTLOOP_H_
#define _THEFOX_NET_EVENTLOOP_H_

#include <Windows.h>

namespace thefox
{

class IoEvent;

/// @brief �¼�ѭ����
class EventLoop
{
public:
	EventLoop();
	~EventLoop();

	/// @brief ��ʼ��Ϣѭ��(���߳�)
	void start();

	/// @brief �������ȴ�ʱ��ѭ���˳�
	void join();

	/// @brief �˳���Ϣѭ��
	void quit();

	/// @brief ע������ʱ��ѭ����
	/// @param[in] handle �������
	void registerHandle(HANDLE handle);

	/// @brief Ͷ���¼�����Ϣѭ����
	/// @param[in] Ͷ�ݵ��¼�ָ��
	void postEvent(IoEvent *e);

	/// @brief ����һ������ѭ��. ����:���߳�,��Ҫʹ��
	void loop();
private:
	void init();
	int getCpuNum();
	HANDLE _hIocp;
	HANDLE _hQuitEvent;
};

} // namespace thefox

#endif // _THEFOX_NET_EVENTLOOP_H_