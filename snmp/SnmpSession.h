#ifdef _THEFOX_SNMP_SESSION_H_
#define _THEFOX_SNMP_SESSION_H_

#include <base/noncopyable.h>

namespace thefox
{

class SnmpSession : noncopyable
{
public:
	SnmpSession()
		:_sessionPtr(NULL)
	{}
	
	~SnmpSession()
	{}
	bool open()
	{
		snmp_sess_init(&_session);
		_session.version = SNMP_VERSION_1; // �汾
		_session.retries = 0; // ���Դ���
		_session.timeout =  3 * 1000 * 1000; // ��ʱʱ�� ΢���net-snmpĬ��ֵΪ1s��
		
		if (SNMP_VERSION_1 == session.version || SNMP_VERSION_2c == session.version)
		{
			session.community = "public";
			session.community_len = strlen("public");
		}
		else if (SNMP_VERSION_3 == session.version)
		{
			// snmpv3 �û���
			session.securityName = "admin";
			session.securityNameLen = strlen("admin");
			
			...
		}

		if (NULL == (_sessionPtr = snmp_open(&session)))
		{
			
			return false;
		}
		return true;
	}
	
	void close()
	{
		if (NULL == _sessionPtr)
			snmp_close(_sessionPtr);
			_sessionPtr = NULL;
	}
	
	bool getValue()
	{
	}
	
	bool getNext()
	{
	}
	
private:
	struct snmp_session _session;
	struct snmp_session *_sessionPtr;
};

} // nameapace thefox

#endif // _THEFOX_SNMP_SESSION_H_
