#ifndef _TF_LOG_H_
#define _TF_LOG_H_

#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <time.h>

class TFLog
{
public:

	enum LogLevel
	{
		Debug,///< ָ��ϸ������Ϣ�¼��Ե���Ӧ�ó����Ƿǳ��а�����
		info,///< ���� ��Ϣ�ڴ����ȼ�����ͻ��ǿ��Ӧ�ó�������й���
		warning,///< ���������Ǳ�ڴ��������
		error,///< ָ����Ȼ���������¼�������Ȼ��Ӱ��ϵͳ�ļ�������
		fatal///< ָ��ÿ�����صĴ����¼����ᵼ��Ӧ�ó�����˳�
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
		
		if (timeNow < m_timeLogBegin || timeNow < m_timeLogEnd)
		{
			ClostLog();
			OpenLog(m_logDir.c_str(), m_prefix.c_str());
			if (NULL == m_file)
			{
				return;
			}
		}
		
		tm *pTm = localtime(&timeNow);
		fprintf(m_file, "%02d:%02d:%02d > ", pTm->tm_hour, pTm->tm_min, pTm->tm_sec);
		va_list ap;
		va_start(ap, fmt)
		vfprintf(m_file , fmt, ap);
		va_end(ap);
		fflush(m_file);
	}
	
	void LogNoTimeTag(unsigned int logLevel, const char *fmt, ...)
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
		
		if (timeNow < m_timeLogBegin || timeNow < m_timeLogEnd)
		{
			ClostLog();
			OpenLog(m_logDir.c_str(), m_prefix.c_str());
			if (NULL == m_file)
			{
				return;
			}
		}
		va_list ap;
		va_start(ap, fmt)
		vfprintf(m_file , fmt, ap);
		va_end(ap);
		fflush(m_file);
	}
	void SetLogLevel(unsigned int level)
	{
		m_logLevel = level
	}
	
	unsigned int GetLogLevel() const
	{
		return m_logLevel;
	}

	
private:
	bool OpenLog(const char *logDir, const char *prefix)
	{
		if (NULL != m_file)
		{
			ClsoeLog();
		}
		
		


	}
	
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
	/// @brief 
	bool MakeFullPath(const char *dir, char *fullPath, int fullPathLen)
	{
		if (fullPathLen < 2)
		{
			return false;
		}

		int pathLen = 0;
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
			char *curDir = filePath
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

	FILE *m_file;
	std::string m_logDir;
	std::string m_prefix;
	time_t m_timeLogBegin;
	time_t m_timeLogEnd;
	unsigned int m_logLevel;
};


#define tfInitLogLib(dir, prefix, logLevel) TFLog g_log(dir, prefix, logLevel);

#define tfImportLogLib extern TFLog g_log;

#define tfLog g_Log.Log

#endif // _TF_LOG_H_