/*
* @file tflog.h
* @brief �����־
*
* @ahthor macwe@qq.com
*/

#ifndef _TF_LOG_H_
#define _TF_LOG_H_

namespace thefox
{


#include <direct.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <time.h>
#include "noncopyable.h"

// Efficient Integer to String Conversions, by Matthew Wilson.
const char digits[] = "9876543210123456789";
const char* zero = digits + 9;
ASSERT(sizeof(digits) == 20);

const char digitsHex[] = "0123456789ABCDEF";
ASSERT(sizeof digitsHex == 17);

template<typename T>
inline size_t convert(char buf[], T value)
{
  T i = value;
  char* p = buf;

  do
  {
    int lsd = static_cast<int>(i % 10);
    i /= 10;
    *p++ = zero[lsd];
  } while (i != 0);

  if (value < 0)
  {
    *p++ = '-';
  }
  *p = '\0';
  std::reverse(buf, p);

  return p - buf;
}

inline size_t convertHex(char buf[], uintptr_t value)
{
  uintptr_t i = value;
  char* p = buf;

  do
  {
    int lsd = i % 16;
    i /= 16;
    *p++ = digitsHex[lsd];
  } while (i != 0);

  *p = '\0';
  std::reverse(buf, p);

  return p - buf;
}

// �̶����ȵĻ�����
template<int SIZE>
class TFFixBuffer : noncopyable
{
public:
	TFFixBuffer()
		:m_curPtr(m_data)
	{}
	~TFFixBuffer(){}
	void Append(cosnt char *buf, size_t len)
	{
		if (static_cast<size_t>(Avail()) > len)
		{
			memcpy(m_curPtr, buf, len);
			m_curPtr += len;
		}
	}
	const char GetData() const { return m_data; }
	int Length() const { return static_cast<int>(m_curPtr - m_data); }
	void Add(size_t len) { m_curPtr += len; }
	void reset() { m_curPtr = m_data; }
private:
	char m_data[SIZE];
	char *m_curPtr;
}


// ��־����
class TFLogStream : noncopyable
{
	typedef TFLogStream self;
	typedef TFFixedBuffer Buffer;

public:
	self &operator<<(bool v)
	{
		m_buffer.Append(v ? "1" : "0", 1);
		return *this;
	}
	self& operator<<(short)
	{
		*this << static_cast<int>(v);
		return *this;
	}
	self& operator<<(unsigned short v)
	{
		*this << static_cast<unsigned int>(v);
		return *this;
	}
	self& operator<<(int v)
	{
		FormatInteger(v);
		return *this;
	}
	self& operator<<(unsigned int v)
	{
		FormatInteger(v);
		return *this;
	}
	self& operator<<(long v)
	{
		FormatInteger(v);
		return *this;
	}
	self& operator<<(unsigned long v)
	{
		FormatInteger(v);
		return *this;
	}
	self& operator<<(long long v)
	{
		FormatInteger(v);
		return *this;
	}
	self& operator<<(unsigned long long v)
	{
		FormatInteger(v);
		return *this;
	}
	self& operator<<(const void* p)
	{
		uintptr_t v = reinterpret_cast<unitptr_t>(p);
		if (m_buffer.Avail() >= kMaxNumericSize)
		{
			char *buf = m_buffer.Current();
			buf[0] = '0';
			buf[1] = 'x';
			size_t len  = convertHex(buf + 2, v);
			m_buffer.Add(len + 2);
		}
		return *this;
	}

	self& operator<<(float v)
	{
		*this << static_cast<double>(v);
		return *this;
	}
	self& operator<<(double);
	{
		if (m_buffer.Avail() >= kMaxNumericSize)
		{
			int len = snprintf(m_buffer.Current(), KMaxNumericSize, "%.12g", v);
		}
	}

	self& operator<<(char v)
	{
		buffer_.append(&v, 1);
		return *this;
	}

	self& operator<<(const char* v)
	{
		buffer_.append(v, strlen(v));
		return *this;
	}

	void Append(const char *data, int len) { m_buffer.Append(data, len); }
	const Buffer &GetBuffer() const { return m_buffer; }
	const ResetBuffer() { m_buffer.Reset(); }

private:
	template<typename T>
	void FormatInteger(T v)
	{
	
	}

	Buffer m_buffer;

	static const int kMaxNumericSize = 32;
}


// ��־������
class TFLog
{
public:
	/// @brief ��־�ȼ�
	enum LogLevel
	{
		LOG_TRACE,
		LOG_DEBUG, //< ����
		LOG_INFO, //< ��ʾ
		LOG_WARN,//< ����
		LOG_ERROR //< ����
		LOG_FATAL
		NUM_LOG_LEVELS
	};

	TFLog(const char *logDir, const char *prefix, unsigned int logLevel)
	: m_file(NULL)
	, m_logDir(logDir)
	, m_prefix(prefix)
	, m_timeLogBegin(0)
	, m_timeLogEnd(0)
	, m_logLevel(logLevel)
	{
		ASSERT(logDir);
		ASSERT(prefix);
		
		OpenLog(logDir, prefix);
	}
	~TFLog()
	{
		CloseLog();
	}
	
	/// @brief д����־����ʱ����
	/// @param [in] logLevel ��־����
	/// @param [in] fmt ��־�����ݣ���ʽ��μ�printf����
	void Log(unsigned int logLevel, const char *fmt, ...)
	{
		if (NULL == m_file)
		{
			return;
		}
		if (logLevel < m_logLevel)
		{
			return;
		}
		
		time_t timeNow;
		time(&timeNow);
		
		if (timeNow < m_timeLogBegin || timeNow > m_timeLogEnd)
		{
			CloseLog();
			OpenLog(m_logDir.c_str(), m_prefix.c_str());
			if (NULL == m_file)
			{
				return;
			}
		}
		
		tm *pTm = localtime(&timeNow);
		fprintf(m_file, "%s %02d:%02d:%02d > ", GetLogLevelDescript(logLevel), pTm->tm_hour, pTm->tm_min, pTm->tm_sec);
		va_list ap;
		va_start(ap, fmt);
		vfprintf(m_file , fmt, ap);
		va_end(ap);
		fflush(m_file);
	}
	
	/// @brief ������־����
	/// @param [in] ��־����
	void SetLogLevel(unsigned int level)
	{
		m_logLevel = level;
	}
	
	/// @brief �õ���־����
	unsigned int GetLogLevel() const
	{
		return m_logLevel;
	}

	
private:
	// �õ���־�������������
	const char *GetLogLevelDescript(const unsigned int logLevel) const
	{
		switch (logLevel)
		{
			case LOG_DEBUG: return "DEBUG";
			case LOG_INFO: return "INFO";
			case LOG_WARN: return "WARN";
			case LOG_ERROR: return "ERROE";
			default: return "";
		}
	}
	// ����־�ļ�
	bool OpenLog(const char *logDir, const char *prefix)
	{
		if (NULL != m_file)
		{
			CloseLog();
		}

		char filePath[512] = {0};
		if (MakeFullPath(logDir, filePath, sizeof(filePath)))
		{
			time(&m_timeLogBegin);
			tm *pTm = localtime(&m_timeLogBegin);

			tm tmTemp;
			tmTemp = *pTm;

			pTm->tm_hour = 0;
			pTm->tm_min = 0;
			pTm->tm_sec = 0;
			m_timeLogBegin = mktime(pTm);

			m_timeLogEnd = m_timeLogBegin + 86400;

			if (NULL != prefix && '\0' != *prefix)
			{
				sprintf(&filePath[strlen(filePath)], "%s", prefix);
			}
			sprintf(&filePath[strlen(filePath)], "%04d-%02d-%02d.log",
				pTm->tm_year + 1900, pTm->tm_mon + 1, pTm->tm_mday);
			m_file = fopen(filePath, "a+");
			if (m_file)
			{
				fprintf(m_file, "Log begin at:%02d:%02d:%02d\r\n", tmTemp.tm_hour, tmTemp.tm_min, tmTemp.tm_sec);
			}
		}
		return true;
	}
	// �ر���־�ļ�
	bool CloseLog()
	{
		if (m_file)
		{
			time_t timeNow;
			time(&timeNow);
			tm *pTm = localtime(&timeNow);
			fprintf(m_file, "Log end at:%02d:%02d:%02d\r\n", pTm->tm_hour, pTm->tm_min, pTm->tm_sec);
			
			fclose(m_file);
			m_file = NULL;
		}
		return true;;
	}
	// �õ�������·�������Ҵ���Ŀ¼,ĩβ�����ָ���
	bool MakeFullPath(const char *dir, char *fullPath, size_t fullPathLen) const
	{
		if (fullPathLen < 2)
		{
			return false;
		}

		size_t pathLen = 0;
		char filePath[512] = {0};

		if (NULL == dir)
		{
			_getcwd(filePath, sizeof(filePath));
		}
		else
		{
			bool bAbsolutePath = true;

			// �ж�dir�Ǿ���·���������·��
#ifdef WIN32
			if (NULL == strchr(dir, ':'))
			{
				bAbsolutePath = false;
			}
#else
			if (dir[0] != '/')
			{
				bAbsolutePath = false;
			}
#endif
			_getcwd(filePath, sizeof(filePath));

			if (!bAbsolutePath) // ��������·�������ɾ���·��
			{
				pathLen = strlen(filePath);
				if ('/' != filePath[pathLen] || '\\' != filePath[pathLen])
				{
					strcat(filePath, "/");
					++pathLen;
				}
				strncat(filePath, dir, sizeof(filePath) - pathLen);
			}
			//! ����Ŀ¼
			char *curDir = filePath;
			while ('\0' != *curDir)
			{
				if ('\\' == *curDir || '/' == *curDir)
				{
					*curDir = '\0';
					_mkdir(filePath);
					*curDir = '/';
				}
				++curDir;
			}
			_mkdir(filePath);
		}

		pathLen = strlen(filePath);
		if ('/' != filePath[pathLen])
		{
			strcat(filePath, "/");
			++pathLen;
		}
		if (pathLen < fullPathLen)
		{
			strcpy(fullPath, filePath);
			return true;
		}
		return false;
	}

	FILE *m_file; // ��־�ļ�ָ��
	std::string m_logDir; // ��־�ļ�·���������ļ���
	std::string m_prefix; // ��־�ļ�ǰ׺
	// ������������֤ÿ������һ����־�ļ�
	time_t m_timeLogBegin;// д��־�ļ���ʼʱ���
	time_t m_timeLogEnd;// д��־�ļ�����ʱ���
	unsigned int m_logLevel; // ��־����
};


#define tfInitLogLib(dir, prefix, logLevel) TFLog g_log(dir, prefix, logLevel);

#define tfImportLogLib extern TFLog g_log;

#define tfLog g_log.Log

#define TFLOG_TRACE if (thefox
#define TFLOG_DEBUG
#define TFLOG_INFO
#define TFLOG_WARN
#define TFLOG_ERROR
#define TFLOG_FATAL



}

#endif // _TF_LOG_H_