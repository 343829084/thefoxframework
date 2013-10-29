/*
* @file tflog.h
* @brief �����־
*
* @ahthor macwe@qq.com
*/

#ifndef _THEFOX_LOGFILE_H_
#define _THEFOX_LOGFILE_H_

#include <direct.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <base/Types.h>
#include <base/noncopyable.h>
#include <base/MutexLock.h>
#include <base/scoped_ptr.h>

namespace thefox
{


class LogFile : noncopyable
{
public:
	LogFile(const string &dir,
            const string &basename,
			size_t rollSize,
			bool threadSafe = true,
			int flushInterval = 3);
	~LogFile();

	void append(const char* logline, int len);
	void flush();

private:
	void append_unlocked(const char* logline, int len);

	static string getLogFileName(const string &dir, const string& basename, time_t* now);
	void rollFile();
    void makePath(string &dir);

	string _dir;
	const string _basename;
	const size_t _rollSize;///< �ļ������дﵽ��ô���ֽڵ����ݺ�д���ļ���
	const int _flushInterval;///< ÿ�����ʱ�佫�ļ��������е�����д���ļ���

	int _count;

	scoped_ptr<MutexLock> _mutex;
	time_t _startOfPeriod;
	time_t _lastRoll;
	time_t _lastFlush;
	class File;
	scoped_ptr<File> _file;

	const static int _kCheckTimeRoll = 1024; ///< ͳ�ƴﵽ��ô���к�д���ļ�
	const static int _kRollPerSeconds = 60*60*24;
};

}

#endif // _THEFOX_LOGFILE_H_