/**********************************************************
* Author: weiganyi
* File: JEvent.cpp
* Date: 20121112
* Description: 
*
**********************************************************/

#include "JFrameWork.h"


namespace JFrameWork{

JEvent::JEvent(const JEVT_TYPE type)
{
    if (type >= JEVT_MAX)
    {
        cout << "JEvent::JEvent type " << type << " is wrong" << endl;
        return;
    }

    m_eType = type;
    m_pBody = JNULL;

	return;
}

JEvent::JEvent(JEvent* pEvent)
{
	Copy(pEvent);

	return;
}

JEvent::~JEvent()
{
    //only to exercise the placement new usage here.
    if (m_eType == JEVT_TIMER_EXPIRE)
    {
        JSingleton<JStaticMemory>::instance()->Free(reinterpret_cast<JCHAR*>(m_pBody));
        m_pBody = JNULL;
    }

	if (m_pBody)
	{
		delete m_pBody;
	}

	return;
}

JUINT32 JEvent::operator=(JEvent& rEvent)
{
	JUINT32 uiRet = 0;

	uiRet = Copy(&rEvent);

	return uiRet;
}

JUINT32 JEvent::SetFromProc(JCHAR* pFromProc)
{
    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_EVENT, "JEvent::SetFromProc");

    if (!pFromProc)
    {
        JSingleton<JLog>::instance2() << set(JLOG_MOD_EVENT, JLOG_ERROR_LEVEL)
            << "JEvent::SetFromProc input param is NULL\n";
        return JFAILURE;
    }

    m_strFromProc = pFromProc;

	return JSUCCESS;
}

JUINT32 JEvent::SetFromThrd(JCHAR* pFromThrd)
{
    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_EVENT, "JEvent::SetFromThrd");

    if (!pFromThrd)
    {
        JSingleton<JLog>::instance2() << set(JLOG_MOD_EVENT, JLOG_ERROR_LEVEL)
            << "JEvent::SetFromThrd input param is NULL\n";
        return JFAILURE;
    }

    m_strFromThrd = pFromThrd;

	return JSUCCESS;
}

JUINT32 JEvent::SetFromMod(JCHAR* pFromMod)
{
    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_EVENT, "JEvent::SetFromMod");

    m_strFromMod = pFromMod;

	return JSUCCESS;
}

JUINT32 JEvent::SetToProc(JCHAR* pToProc)
{
    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_EVENT, "JEvent::SetToProc");

    if (!pToProc)
    {
        JSingleton<JLog>::instance2() << set(JLOG_MOD_EVENT, JLOG_ERROR_LEVEL)
            << "JEvent::SetToProc input param is NULL\n";
        return JFAILURE;
    }

    m_strToProc = pToProc;

	return JSUCCESS;
}

JUINT32 JEvent::SetToThrd(JCHAR* pToThrd)
{
    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_EVENT, "JEvent::SetToThrd");

    if (!pToThrd)
    {
        JSingleton<JLog>::instance2() << set(JLOG_MOD_EVENT, JLOG_ERROR_LEVEL)
            << "JEvent::SetToThrd input param is NULL\n";
        return JFAILURE;
    }

    m_strToThrd = pToThrd;

	return JSUCCESS;
}

JUINT32 JEvent::SetToMod(JCHAR* pToMod)
{
    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_EVENT, "JEvent::SetToMod");

    m_strToMod = pToMod;

	return JSUCCESS;
}

JUINT32 JEvent::SetBody(JEventBody* pBody)
{
    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_EVENT, "JEvent::SetBody");

    m_pBody = pBody;

	return JSUCCESS;
}

JString& JEvent::GetFromProc()
{
    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_EVENT, "JEvent::GetFromProc");

	return m_strFromProc;
}

JString& JEvent::GetFromThrd()
{
    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_EVENT, "JEvent::GetFromThrd");

	return m_strFromThrd;
}

JString& JEvent::GetFromMod()
{
    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_EVENT, "JEvent::GetFromMod");

	return m_strFromMod;
}

JString& JEvent::GetToProc()
{
    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_EVENT, "JEvent::GetToProc");

	return m_strToProc;
}

JString& JEvent::GetToThrd()
{
    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_EVENT, "JEvent::GetToThrd");

	return m_strToThrd;
}

JString& JEvent::GetToMod()
{
    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_EVENT, "JEvent::GetToMod(");

	return m_strToMod;
}

JEVT_TYPE JEvent::GetEventType()
{
    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_EVENT, "JEvent::GetEventType");

	return m_eType;
}

JEventBody* JEvent::GetBody()
{
    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_EVENT, "JEvent::GetBody");

	return m_pBody;
}

JCHAR* JEvent::Serialize(JUINT32 *uiLen)
{
    JUINT32 uiAllocLen = 0;
    JUINT32 uiLeaveLen = 0;
    JCHAR* pBuf = JNULL;
    JCHAR* pTmpBuf = JNULL;

    JEVT_TYPE* pEvtType = JNULL;
    JUINT32* pLen = JNULL;
    JUINT32* pBodyLen = JNULL;

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_EVENT, "JEvent::Serialize");

    //calcualte the total event message length
    uiAllocLen += sizeof(JEVT_TYPE);
    //m_strFromProc size
    uiAllocLen += sizeof(JUINT32);
    uiAllocLen += m_strFromProc.GetLength();
    //m_strFromThrd size
    uiAllocLen += sizeof(JUINT32);
    uiAllocLen += m_strFromThrd.GetLength();
    //m_strFromMod size
    uiAllocLen += sizeof(JUINT32);
    uiAllocLen += m_strFromMod.GetLength();
    //m_strToProc size
    uiAllocLen += sizeof(JUINT32);
    uiAllocLen += m_strToProc.GetLength();
    //m_strToThrd size
    uiAllocLen += sizeof(JUINT32);
    uiAllocLen += m_strToThrd.GetLength();
    //m_strToMod size
    uiAllocLen += sizeof(JUINT32);
    uiAllocLen += m_strToMod.GetLength();
    //m_pBody size
    uiAllocLen += sizeof(JUINT32);
    if (m_pBody)
    {
	    uiAllocLen += m_pBody->GetLength();
    }

    pBuf = JSingleton<JStaticMemory>::instance()->Alloc(uiAllocLen+1);
    if (pBuf)
    {
        SafeMemset(pBuf, 0, uiAllocLen+1);

        pTmpBuf = pBuf;
        uiLeaveLen = uiAllocLen+1;

        //store the event type into the buffer
        pEvtType = reinterpret_cast<JEVT_TYPE*>(pTmpBuf);
        *pEvtType = m_eType;
        pTmpBuf += sizeof(JEVT_TYPE);
        uiLeaveLen -= sizeof(JEVT_TYPE);

        //first store the from process length into the buffer
        pLen = reinterpret_cast<JUINT32*>(pTmpBuf);
        *pLen = m_strFromProc.GetLength();
        pTmpBuf += sizeof(JUINT32);
        uiLeaveLen -= sizeof(JUINT32);
        //then store the from process into the buffer
        if (*pLen)
        {
            SafeStrcpy(pTmpBuf, m_strFromProc.c_str(), uiLeaveLen);
            pTmpBuf += *pLen;
            uiLeaveLen -= *pLen;
        }

        //first store the from thread length into the buffer
        pLen = reinterpret_cast<JUINT32*>(pTmpBuf);
        *pLen = m_strFromThrd.GetLength();
        pTmpBuf += sizeof(JUINT32);
        uiLeaveLen -= sizeof(JUINT32);
        //then store the from thread into the buffer
        if (*pLen)
        {
            SafeStrcpy(pTmpBuf, m_strFromThrd.c_str(), uiLeaveLen);
            pTmpBuf += *pLen;
            uiLeaveLen -= *pLen;
        }

        //first store the from module length into the buffer
        pLen = reinterpret_cast<JUINT32*>(pTmpBuf);
        *pLen = m_strFromMod.GetLength();
        pTmpBuf += sizeof(JUINT32);
        uiLeaveLen -= sizeof(JUINT32);
        //then store the from module into the buffer
        if (*pLen)
        {
            SafeStrcpy(pTmpBuf, m_strFromMod.c_str(), uiLeaveLen);
            pTmpBuf += *pLen;
            uiLeaveLen -= *pLen;
        }

        //first store the to process length into the buffer
        pLen = reinterpret_cast<JUINT32*>(pTmpBuf);
        *pLen = m_strToProc.GetLength();
        pTmpBuf += sizeof(JUINT32);
        uiLeaveLen -= sizeof(JUINT32);
        //then store the to process into the buffer
        if (*pLen)
        {
            SafeStrcpy(pTmpBuf, m_strToProc.c_str(), uiLeaveLen);
            pTmpBuf += *pLen;
            uiLeaveLen -= *pLen;
        }

        //first store the to thread length into the buffer
        pLen = reinterpret_cast<JUINT32*>(pTmpBuf);
        *pLen = m_strToThrd.GetLength();
        pTmpBuf += sizeof(JUINT32);
        uiLeaveLen -= sizeof(JUINT32);
        //then store the to thread into the buffer
        if (*pLen)
        {
            SafeStrcpy(pTmpBuf, m_strToThrd.c_str(), uiLeaveLen);
            pTmpBuf += *pLen;
            uiLeaveLen -= *pLen;
        }

        //first store the to module length into the buffer
        pLen = reinterpret_cast<JUINT32*>(pTmpBuf);
        *pLen = m_strToMod.GetLength();
        pTmpBuf += sizeof(JUINT32);
        uiLeaveLen -= sizeof(JUINT32);
        //then store the to module into the buffer
        if (*pLen)
        {
            SafeStrcpy(pTmpBuf, m_strToMod.c_str(), uiLeaveLen);
            pTmpBuf += *pLen;
            uiLeaveLen -= *pLen;
        }

        //first store the body length into the buffer
        pBodyLen = reinterpret_cast<JUINT32*>(pTmpBuf);
        if (m_pBody)
        {
	        *pBodyLen = m_pBody->GetLength();
            pTmpBuf += sizeof(JUINT32);
            uiLeaveLen -= sizeof(JUINT32);

            //then store the body into the buffer
	        if (*pBodyLen)
	        {
	            m_pBody->Serialize(pTmpBuf, uiLeaveLen);
	        }
        }
        else
        {
        	*pBodyLen = 0;
            pTmpBuf += sizeof(JUINT32);
            uiLeaveLen -= sizeof(JUINT32);
        }

        *uiLen = uiAllocLen;

    	return pBuf;
    }

	return JNULL;
}

JUINT32 JEvent::DeSerialize(JCHAR* pBuf)
{
    JUINT32 uiBufLen = 0;
    JCHAR* pTmpBuf = JNULL;

    JEVT_TYPE* pEvtType = JNULL;
    JUINT32* pLen = JNULL;
    JCHAR strName[JMAX_BUFFER_LEN] = {0};
    JUINT32* pBodyLen = JNULL;

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_EVENT, "JEvent::DeSerialize");

    uiBufLen = SafeStrlen(pBuf);
    if (uiBufLen)
    {
        pTmpBuf = pBuf;

        //get the event type from the buffer
        pEvtType = reinterpret_cast<JEVT_TYPE*>(pTmpBuf);
        m_eType = *pEvtType;
        pTmpBuf += sizeof(JEVT_TYPE);

        //first get the from process length from buffer
        pLen = reinterpret_cast<JUINT32*>(pTmpBuf);
        pTmpBuf += sizeof(JUINT32);
        //then get the from process from buffer
        if (*pLen)
        {
            SafeStrncpy(strName, pTmpBuf, *pLen, JMAX_BUFFER_LEN);
        	m_strFromProc = strName;
            pTmpBuf += *pLen;
        }

        //first get the from thread length from buffer
        pLen = reinterpret_cast<JUINT32*>(pTmpBuf);
        pTmpBuf += sizeof(JUINT32);
        //then get the from thread from buffer
        if (*pLen)
        {
            SafeStrncpy(strName, pTmpBuf, *pLen, JMAX_BUFFER_LEN);
        	m_strFromThrd = strName;
            pTmpBuf += *pLen;
        }

        //first get the from module length from buffer
        pLen = reinterpret_cast<JUINT32*>(pTmpBuf);
        pTmpBuf += sizeof(JUINT32);
        //then get the from module from buffer
        if (*pLen)
        {
            SafeStrncpy(strName, pTmpBuf, *pLen, JMAX_BUFFER_LEN);
        	m_strFromMod = strName;
            pTmpBuf += *pLen;
        }

        //first get the to process length from buffer
        pLen = reinterpret_cast<JUINT32*>(pTmpBuf);
        pTmpBuf += sizeof(JUINT32);
        //then get the to process from buffer
        if (*pLen)
        {
            SafeStrncpy(strName, pTmpBuf, *pLen, JMAX_BUFFER_LEN);
        	m_strToProc = strName;
            pTmpBuf += *pLen;
        }

        //first get the to thread length from buffer
        pLen = reinterpret_cast<JUINT32*>(pTmpBuf);
        pTmpBuf += sizeof(JUINT32);
        //then get the to thread from buffer
        if (*pLen)
        {
            SafeStrncpy(strName, pTmpBuf, *pLen, JMAX_BUFFER_LEN);
        	m_strToThrd = strName;
            pTmpBuf += *pLen;
        }

        //first get the to module length from buffer
        pLen = reinterpret_cast<JUINT32*>(pTmpBuf);
        pTmpBuf += sizeof(JUINT32);
        //then get the to module from buffer
        if (*pLen)
        {
            SafeStrncpy(strName, pTmpBuf, *pLen, JMAX_BUFFER_LEN);
        	m_strToMod = strName;
            pTmpBuf += *pLen;
        }

        //first get the body length from buffer
        pBodyLen = reinterpret_cast<JUINT32*>(pTmpBuf);
        pTmpBuf += sizeof(JUINT32);
        //then get the body from buffer
        if (*pBodyLen)
        {
    		if (m_pBody)
    		{
    			delete m_pBody;
    			m_pBody = JNULL;
    		}

            //construct a event body
			m_pBody = JSingleton<JThreadManager>::instance()->MakeEventBody(m_strToThrd.c_str(), 
			    m_strToMod.c_str(), m_eType);
			if (m_pBody)
			{
			    //deserialize body from buffer
    			m_pBody->DeSerialize(pTmpBuf);
			}
        }

    	return JSUCCESS;
    }

	return JFAILURE;
}

JUINT32 JEvent::Copy(JEvent* pEvent)
{
    JUINT32 uiLen = 0;

    if (!pEvent)
    {
        cout << "JEvent::Copy input param is NULL" << endl;
        return JFAILURE;
    }

    m_eType = pEvent->m_eType;
    m_strFromProc = pEvent->m_strFromProc;
    m_strFromThrd = pEvent->m_strFromThrd;
    m_strFromMod = pEvent->m_strFromMod;
    m_strToProc = pEvent->m_strToProc;
    m_strToThrd = pEvent->m_strToThrd;
    m_strToMod = pEvent->m_strToMod;
    m_pBody = pEvent->m_pBody->Clone();

	return JSUCCESS;
}

}

