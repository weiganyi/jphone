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
    //if cann't get the lock, still return false
    JBOOL TryAcquire();
    JVOID Release();

private:
    //critical object
    CRITICAL_SECTION m_CriticalSection;

    //the number that the lock had beed got
    JUINT32 m_uiLockNumber;
};

}


#endif

