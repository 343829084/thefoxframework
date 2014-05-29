#ifndef _THEFOX_LOG_LOGMESSAGE_H_
#define _THEFOX_LOG_LOGMESSAGE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <base/common.h>

namespace thefox
{
enum LogLevel {
	LOGLEVEL_TRACE,
	LOGLEVEL_DEBUG,
	LOGLEVEL_INFO,
	LOGLEVEL_WARN,
	LOGLEVEL_ERROR,
	LOGLEVEL_FATAL,
	LOGLEVEL_NUM_LOG_LEVELS
};

namespace detail
{

// ��·���еõ��ļ���
inline const char *getFileNameFromPath(char *pathName)
{
	char *data = pathName;
	char* slash = strrchr(pathName, '/');
	if (0 == slash)
		slash = strrchr(data, '\\');
	if (slash)
		data = slash + 1;

	return data;
}

class LogFinisher;

class LogMessage 
{
public:
	LogMessage(LogLevel level, const char *filename, int line);
	~LogMessage();

	LogMessage &operator<<(const std::string &value);
	LogMessage &operator<<(const char *value);
	LogMessage &operator<<(char value);
	LogMessage &operator<<(int value);
	LogMessage &operator<<(unsigned int value);
	LogMessage &operator<<(long value);
	LogMessage &operator<<(unsigned long value);
	LogMessage &operator<<(long long value);
	LogMessage &operator<<(unsigned long long value);
	LogMessage &operator<<(double value);
	LogMessage &operator<<(const void *p);

private:
	friend class LogFinisher;
	friend class LogTraceFunction;
	void Finish();

	LogLevel _level;
	std::string _message;
};

class LogFinisher
{
public:
	void operator=(LogMessage &other);
};

// ���ٺ����Ŀ�ͷ�ͽ���, ���ں����ĵ�һ��
class LogTraceFunction {
public:
	LogTraceFunction(char *func, char *file, int line);
	~LogTraceFunction();
	
	void operator=(LogMessage &other);
private:
	const char *_func;
	const char *_file;
	int _line;
};

} // namespace detail

// д��־����
typedef std::function<void(const std::string &message)> LogHandler;

void setLogHandler(const LogHandler &newFunc);

void setLogLevel(LogLevel level);
LogLevel logLevel();

#define THEFOX_LOG(LEVEL) if (thefox::logLevel() <= thefox::LOGLEVEL_##LEVEL) \
	thefox::detail::LogFinisher() =                                           \
		thefox::detail::LogMessage(                                           \
			thefox::LOGLEVEL_##LEVEL, thefox::detail::getFileNameFromPath(__FILE__), __LINE__)
#define THEFOX_LOG_IF(LEVEL, CONDITION) \
	!(CONDITION) ? (void)0 : THEFOX_LOG(LEVEL)

#define THEFOX_CHECK(EXPRESSION) \
  THEFOX_LOG_IF(FATAL, !(EXPRESSION)) << "CHECK failed: " #EXPRESSION ": "
#define THEFOX_CHECK_EQ(A, B) THEFOX_CHECK((A) == (B))
#define THEFOX_CHECK_NE(A, B) THEFOX_CHECK((A) != (B))
#define THEFOX_CHECK_LT(A, B) THEFOX_CHECK((A) <  (B))
#define THEFOX_CHECK_LE(A, B) THEFOX_CHECK((A) <= (B))
#define THEFOX_CHECK_GT(A, B) THEFOX_CHECK((A) >  (B))
#define THEFOX_CHECK_GE(A, B) THEFOX_CHECK((A) >= (B))

namespace detail 
{
template<typename T>
T* checkNotNull(const char *name, T* val)
{
	if (val == NULL)
		THEFOX_LOG(FATAL) << name;
	return val;
}

}  // namespace detail

#define THEFOX_CHECK_NOTNULL(A) \
  thefox::detail::checkNotNull("'" #A "' must not be NULL", (A))

#define THEFOX_TRACE_FUNCTION  \
	thefox::detail::LogTraceFunction LogTraceFunction(__FUNCTION__, __FILE__, __LINE__); \
	if (thefox::logLevel() <= thefox::LOGLEVEL_TRACE)	                                 \
		LogTraceFunction = thefox::detail::LogMessage(LOGLEVEL_TRACE,                    \
			thefox::detail::getFileNameFromPath(__FILE__), __LINE__)                     \
			<< __FUNCTION__ << "() begin "
} // namespace thefox

#endif // _THEFOX_LOG_LOGMESSAGE_H_
