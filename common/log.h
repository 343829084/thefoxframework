#ifndef _TF_LOG_H_
#define _TF_LOG_H_

#include <stdlib.h>
#include <stdio.h>
#include <string>

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
		vfprintf(m_file , fmt, 
	}
	
	void LogNoTimeTag(unsigned int logLevel, const char *fmt, ...)
	{
	
	}
	void SetLogLevel(unsigned int level)
	{
	
	}
	
	unsigned int GetLogLevel();
	
private:
	bool OpenLog(const char *logDir, const char *prefix)
	{
		if (NULL != m_file)
		{
			ClsoeLog();
		}
		
		char filePath[512] = {0};
		int fileLen = sizeof(filepath);
		if (NULL != prifix)
		{
			fileLen -=strlen(prefix);
		}
		fileLen -= 14; 
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
	
	FILE *m_file;
	std::string m_logDir;
	std::string m_prefix;
	time_t m_timeLogBegin;
	time_t m_timeLogEnd;
	unsigned int m_logLevel;
};

#define INIT_TFLOG TFLog g_log;


#endif // _TF_LOG_H_