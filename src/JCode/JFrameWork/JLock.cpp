/**********************************************************
* Author: weiganyi
* File: JLock.cpp
* Date: 20121102
* Description: 
*
**********************************************************/

#include "JFrameWork.h"


namespace JFrameWork{

JLock::JLock()
{
    m_uiLockNumber = 0;

    InitializeCriticalSection(&m_CriticalSection);
}

JLock::~JLock()
{
    DeleteCriticalSection(&m_CriticalSection);
}

JVOID JLock::Acquire()
{
    EnterCriticalSection(&m_CriticalSection);

    m_uiLockNumber++;

    return;
}

JBOOL JLock::TryAcquire()
{
    JBOOL bRet = JFALSE;

    bRet = static_cast<JBOOL>(TryEnterCriticalSection(&m_CriticalSection));
    if (bRet)
    {
        m_uiLockNumber++;
    }

    return bRet;
}

JVOID JLock::Release()
{
    LeaveCriticalSection(&m_CriticalSection);

    m_uiLockNumber--;

    return;
}

}

