/**********************************************************
* Author: weiganyi
* File: JLock.h
* Date: 20121102
* Description: 
*
**********************************************************/

#ifndef _JLOCK_H_
#define _JLOCK_H_


namespace JFrameWork{

//JLock definication
class JLock: public JObject{
public:
    JLock();
    ~JLock();

    JVOID Acquire();
    JBOOL TryAcquire();
    JVOID Release();

private:
    CRITICAL_SECTION m_CriticalSection;

    JUINT32 m_uiLockNumber;
};

}


#endif

