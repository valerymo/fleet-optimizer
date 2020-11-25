// Scheduler.h: interface for the CScheduler class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCHEDULER_H__1256A5CA_5AE0_4826_97D8_797CEF786851__INCLUDED_)
#define AFX_SCHEDULER_H__1256A5CA_5AE0_4826_97D8_797CEF786851__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CScheduler  
{
public:
    static CScheduler* Instance();
	//virtual ~CScheduler();
protected:
	CScheduler();
private:
    static CScheduler* _instance;

};

#endif // !defined(AFX_SCHEDULER_H__1256A5CA_5AE0_4826_97D8_797CEF786851__INCLUDED_)
