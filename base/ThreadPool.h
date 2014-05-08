#ifndef _THEFOX_BASE_THREADPOOL_H_
#define _THEFOX_BASE_THREADPOOL_H_

#include <queue>
#include <base/Types.h>
#include <base/MutexLock.h>
#include <base/Semaphore.h>
#include <base/Thread.h>

namespace thefox
{

class ThreadPool
{
public:
	ThreadPool()
		: _inited(false)
	{}
	~ThreadPool()
	{}
	
	/// @brief ��ʼ���̳߳�
	/// @param[in] threadNum �̸߳���
	bool init(int threadNum = kDefaultThreadNum)
	{
		if (_inited)
			return false;
		_inited = true;
		addWorker(threadNum);
		return _inited;
	}
	
	/// @brief ����߳�����
	void addTask(const ThreadCallback &task)
	{ _tasks.put(task); }
	
	/// @brief �ȴ��̳߳ؽ���
	void join()
	{
		for (ThreadVector::const_iterator it = _threads.begin(); 
				it != _threads.end(); ++it)
			(*it)->join();
			
		_threads.clear();
	}
	
	/// @brief �õ��̸߳���
	size_t size() const { return _threads.size(); }
	
	/// @brief �����̳߳�
	void terminate()
	{
		for (ThreadVector::const_iterator it = _threads.begin(); 
				it != _threads.end(); ++it)
			(*it)->stop();
	}

private:
	THEFOX_DISALLOW_EVIL_CONSTRUCTORS(ThreadPool);
	typedef std::vector<std::shared_ptr<Thread>> ThreadVector;
	
	class TaskQueue
	{
	public:
		TaskQueue() {}
		~TaskQueue() {}
		
		void put(const ThreadCallback &task) 
		{
			MutexLockGuard lock(_mutex);
			_tasks.push(task);
			_sem.signal();
		}
		ThreadCallback get()
		{
			ThreadCallback task;
			_sem.wait(Semaphore::kInfinite);
			MutexLockGuard lock(_mutex);
			task = _tasks.front();
			_tasks.pop();
			return task;
		}
		
	private:
		THEFOX_DISALLOW_EVIL_CONSTRUCTORS(TaskQueue);
		typedef std::queue<ThreadCallback> Tasks;
		Tasks _tasks;
		MutexLock _mutex;
		Semaphore _sem;
	};
	
	// ��ӹ����߳�
	void addWorker(int threadNum)
	{
		for (int i = 0; i < threadNum; ++i)
			_threads.push_back(new Thread(taskRunner, "threadpool.taskrunner"));
	}
	
	// ��������
	void taskRunner()
	{
		while(true) {
			ThreadCallback task = _tasks.get();
			task();
		}
	}
	
	bool _inited;
	ThreadVector _threads;
	TaskQueue _tasks;

	static const int32_t kDefaultThreadNum = 10;
};

} // namespace thefox

#endif // _THEFOX_BASE_THREADPOOL_H_