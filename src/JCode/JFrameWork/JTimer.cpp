/**********************************************************
* Author: weiganyi
* File: JTimer.cpp
* Date: 20110718
* Description: 
*
**********************************************************/

#include "JFrameWork.h"


namespace JFrameWork{

void CALLBACK lpTimeProc(UINT uID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2)
{
    JSYSTIME sysTime;
    JTimer* pTimer = reinterpret_cast<JTimer*>(dwUser);
    JEvent* pEvent = JNULL;
    JSysTime* pSysTimeBody = JNULL;
    JCHAR* pSysTimeBuf = JNULL;
    JCommEngine* pComEngine = JNULL;
    JThread* pThread = JNULL;
    JAgentThread* pAgentThread = JNULL;

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_TIMER, "lpTimeProc");

    if (!pTimer)
    {
        JSingleton<JLog>::instance2() << set(JLOG_MOD_TIMER, JLOG_ERROR_LEVEL) 
            << "lpTimeProc input param is NULL\n";
        return;
    }

    pTimer->GetSysTime(&sysTime);

    pEvent = new JEvent(JEVT_TIMER_EXPIRE);
    //only for placement new usage exercise, otherwise I new JSysTime object directly here.
    pSysTimeBuf = JSingleton<JStaticMemory>::instance()->Alloc(sizeof(JSysTime)+1);
    if (pEvent && pSysTimeBuf)
    {
    	pTimer->SafeMemset(pSysTimeBuf, 0, sizeof(JSysTime)+1);
    	pSysTimeBody = new (pSysTimeBuf) JSysTime;

	    pSysTimeBody->SetYear(sysTime.uiYear);
	    pSysTimeBody->SetMonth(sysTime.uiMonth);
	    pSysTimeBody->SetDayOfWeek(sysTime.uiDayOfWeek);
	    pSysTimeBody->SetDay(sysTime.uiDay);
	    pSysTimeBody->SetHour(sysTime.uiHour);
	    pSysTimeBody->SetMinute(sysTime.uiMinute);
	    pSysTimeBody->SetSecond(sysTime.uiSecond);
	    pSysTimeBody->SetMilliseconds(sysTime.uiMilliseconds);

	    pEvent->SetFromProc(JS_P_JMAINPROC);
	    pEvent->SetFromThrd(JS_T_JTIMER);
	    pEvent->SetFromMod(JNULL);
	    pEvent->SetToProc(JS_P_JMAINPROC);
	    pEvent->SetToThrd(JS_T_JMAINTHREAD);
	    pEvent->SetToMod(JS_M_JTIMER);
	    pEvent->SetBody(pSysTimeBody);

        pThread = JSingleton<JThreadManager>::instance()->GetThread(JS_T_JTIMER);
        pAgentThread = dynamic_cast<JAgentThread*>(pThread);
        if (pAgentThread)
        {
    	    pComEngine = pAgentThread->GetNotifyCommEngine();
    	    if (pComEngine)
    	    {
    	        pComEngine->SendEvent(pEvent);
    	    }
	    }
    }
    else
    {
        JSingleton<JLog>::instance2() << set(JLOG_MOD_TIMER, JLOG_ERROR_LEVEL) 
            << "lpTimeProc alloc body failure\n";
    }

    return;
}

JSysTime::JSysTime()
{
	return;
}

JSysTime::~JSysTime()
{
	return;
}

JUINT32 JSysTime::GetLength()
{
	JUINT32 uiLen = 0;

	uiLen += sizeof(JUINT32);
	uiLen += sizeof(JUINT32);
	uiLen += sizeof(JUINT32);
	uiLen += sizeof(JUINT32);
	uiLen += sizeof(JUINT32);
	uiLen += sizeof(JUINT32);
	uiLen += sizeof(JUINT32);
	uiLen += sizeof(JUINT32);

	return uiLen;
}

JEventBody* JSysTime::Clone()
{
	JSysTime* pSysTime = JNULL;

	pSysTime = new JSysTime();
	if (pSysTime)
	{
		pSysTime->SetYear(uiYear);
		pSysTime->SetMonth(uiMonth);
		pSysTime->SetDayOfWeek(uiDayOfWeek);
		pSysTime->SetDay(uiDay);
		pSysTime->SetHour(uiHour);
		pSysTime->SetMinute(uiMinute);
		pSysTime->SetSecond(uiSecond);
		pSysTime->SetMilliseconds(uiMilliseconds);
	}

	return pSysTime;
}

JUINT32 JSysTime::Clear()
{
	uiYear = 0;
	uiMonth = 0;
	uiDayOfWeek = 0;
	uiDay = 0;
	uiHour = 0;
	uiMinute = 0;
	uiSecond = 0;
	uiMilliseconds = 0;

	return JSUCCESS;
}

JUINT32 JSysTime::Serialize(JCHAR* pBuf, JUINT32 uiMaxNum)
{
	JUINT32 uiLen = 0;
	JUINT32* pLen = JNULL;
	JCHAR* pOffset = JNULL;
	JUINT32 tmpMaxNum = 0;
	JUINT32* pVal = JNULL;

	if (!pBuf || !uiMaxNum)
	{
        JSingleton<JLog>::instance2() << set(JLOG_MOD_TIMER, JLOG_ERROR_LEVEL) 
            << "JSysTime::Serialize input param is NULL\n";
        return JFAILURE;
	}

	pOffset = pBuf;
	tmpMaxNum = uiMaxNum;

	{
		uiLen = sizeof(JUINT32);

		pVal = reinterpret_cast<JUINT32*>(pOffset);
		*pVal = uiYear;

		pOffset += uiLen;
		tmpMaxNum -= uiLen;
	}

	{
		uiLen = sizeof(JUINT32);

		pVal = reinterpret_cast<JUINT32*>(pOffset);
		*pVal = uiMonth;

		pOffset += uiLen;
		tmpMaxNum -= uiLen;
	}

	{
		uiLen = sizeof(JUINT32);

		pVal = reinterpret_cast<JUINT32*>(pOffset);
		*pVal = uiDayOfWeek;

		pOffset += uiLen;
		tmpMaxNum -= uiLen;
	}

	{
		uiLen = sizeof(JUINT32);

		pVal = reinterpret_cast<JUINT32*>(pOffset);
		*pVal = uiDay;

		pOffset += uiLen;
		tmpMaxNum -= uiLen;
	}

	{
		uiLen = sizeof(JUINT32);

		pVal = reinterpret_cast<JUINT32*>(pOffset);
		*pVal = uiHour;

		pOffset += uiLen;
		tmpMaxNum -= uiLen;
	}

	{
		uiLen = sizeof(JUINT32);

		pVal = reinterpret_cast<JUINT32*>(pOffset);
		*pVal = uiMinute;

		pOffset += uiLen;
		tmpMaxNum -= uiLen;
	}

	{
		uiLen = sizeof(JUINT32);

		pVal = reinterpret_cast<JUINT32*>(pOffset);
		*pVal = uiSecond;

		pOffset += uiLen;
		tmpMaxNum -= uiLen;
	}

	{
		uiLen = sizeof(JUINT32);

		pVal = reinterpret_cast<JUINT32*>(pOffset);
		*pVal = uiMilliseconds;

		pOffset += uiLen;
		tmpMaxNum -= uiLen;
	}

	return JSUCCESS;
}

JUINT32 JSysTime::DeSerialize(JCHAR* pBuf)
{
	JUINT32* pLen = 0;
	JCHAR* pOffset = JNULL;
	JUINT32* pVal = JNULL;

	if (!pBuf)
	{
        JSingleton<JLog>::instance2() << set(JLOG_MOD_TIMER, JLOG_ERROR_LEVEL) 
            << "JSysTime::DeSerialize input param is NULL\n";
        return JFAILURE;
	}

	pOffset = pBuf;

	pVal = reinterpret_cast<JUINT32*>(pOffset);
	uiYear = *pVal;
	pOffset += sizeof(JUINT32);

	pVal = reinterpret_cast<JUINT32*>(pOffset);
	uiMonth = *pVal;
	pOffset += sizeof(JUINT32);

	pVal = reinterpret_cast<JUINT32*>(pOffset);
	uiDayOfWeek = *pVal;
	pOffset += sizeof(JUINT32);

	pVal = reinterpret_cast<JUINT32*>(pOffset);
	uiDay = *pVal;
	pOffset += sizeof(JUINT32);

	pVal = reinterpret_cast<JUINT32*>(pOffset);
	uiHour = *pVal;
	pOffset += sizeof(JUINT32);

	pVal = reinterpret_cast<JUINT32*>(pOffset);
	uiMinute = *pVal;
	pOffset += sizeof(JUINT32);

	pVal = reinterpret_cast<JUINT32*>(pOffset);
	uiSecond = *pVal;
	pOffset += sizeof(JUINT32);

	pVal = reinterpret_cast<JUINT32*>(pOffset);
	uiMilliseconds = *pVal;
	pOffset += sizeof(JUINT32);

	return JSUCCESS;
}

JUINT32 JSysTime::operator=(JSysTime& rSysTime)
{
	uiYear = rSysTime.GetYear();
	uiMonth = rSysTime.GetMonth();
	uiDayOfWeek = rSysTime.GetDayOfWeek();
	uiDay = rSysTime.GetDay();
	uiHour = rSysTime.GetHour();
	uiMinute = rSysTime.GetMinute();
	uiSecond = rSysTime.GetSecond();
	uiMilliseconds = rSysTime.GetMilliseconds();

	return JSUCCESS;
}

JUINT32 JSysTime::GetYear()
{
	return uiYear;
}

JUINT32 JSysTime::SetYear(JUINT32 uiVal)
{
	uiYear = uiVal;

	return JSUCCESS;
}

JUINT32 JSysTime::GetMonth()
{
	return uiMonth;
}

JUINT32 JSysTime::SetMonth(JUINT32 uiVal)
{
	uiMonth = uiVal;

	return JSUCCESS;
}

JUINT32 JSysTime::GetDayOfWeek()
{
	return uiDayOfWeek;
}

JUINT32 JSysTime::SetDayOfWeek(JUINT32 uiVal)
{
	uiDayOfWeek = uiVal;

	return JSUCCESS;
}

JUINT32 JSysTime::GetDay()
{
	return uiDay;
}

JUINT32 JSysTime::SetDay(JUINT32 uiVal)
{
	uiDay = uiVal;

	return JSUCCESS;
}

JUINT32 JSysTime::GetHour()
{
	return uiHour;
}

JUINT32 JSysTime::SetHour(JUINT32 uiVal)
{
	uiHour = uiVal;

	return JSUCCESS;
}

JUINT32 JSysTime::GetMinute()
{
	return uiMinute;
}

JUINT32 JSysTime::SetMinute(JUINT32 uiVal)
{
	uiMinute = uiVal;

	return JSUCCESS;
}

JUINT32 JSysTime::GetSecond()
{
	return uiSecond;
}

JUINT32 JSysTime::SetSecond(JUINT32 uiVal)
{
	uiSecond = uiVal;

	return JSUCCESS;
}

JUINT32 JSysTime::GetMilliseconds()
{
	return uiMilliseconds;
}

JUINT32 JSysTime::SetMilliseconds(JUINT32 uiVal)
{
	uiMilliseconds = uiVal;

	return JSUCCESS;
}

JTimer::JTimer()
{
	JUINT32 uiSize = sizeof(JTIMER_CB);
    MMRESULT ret;
	JTHRD_PUB_DATA thrdPubData;
    JString strThrdName;
    JString strLocalAddr;

    //create JAgentThread and add into JThreadManager
    strThrdName = JS_T_JTIMER;
    strLocalAddr = JTIMER_LOCAL_ADDR;
	m_pAgentThread = new JAgentThread(&strThrdName, &strLocalAddr, JTIMER_LOCAL_PORT);

    JThreadManager* pThreadManager = JSingleton<JThreadManager>::instance();
    thrdPubData.strThrdName = JS_T_JTIMER;
    thrdPubData.pThread = m_pAgentThread;
    pThreadManager->RegisterThread(thrdPubData);

    //create timer resource
    ret = timeBeginPeriod(JTIMER_MIN_PERIOD);
    if (ret == TIMERR_NOERROR)
    {
        m_handler = timeSetEvent(JTIMER_MIN_PERIOD, 0, lpTimeProc, reinterpret_cast<DWORD>(this), TIME_PERIODIC);
        if (!m_handler)
        {
            cout << "JTimer::JTimer create timer failure" << endl;
            timeEndPeriod(JTIMER_MIN_PERIOD);
        }
    }

    SafeMemset(reinterpret_cast<JCHAR*>(m_timerCB), 0, uiSize*JTIMER_MAX_CB_NUM);

	return;
}

JTimer::~JTimer()
{
    timeKillEvent(m_handler);

    timeEndPeriod(JTIMER_MIN_PERIOD);

    if (m_pAgentThread)
    {
        delete m_pAgentThread;
    }

	return;
}

JUINT32 JTimer::CreateTimer(JTIMER_TYPE eType, 
                        JTIMER_CALLBACK pfFunc, 
                        JVOID* pData, 
                        const JUINT32 uiTime)
{
    JUINT32 uiHandler = 0;

    JAutoPtr<JLock> clsLockAutoPtr(&m_Lock);

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_TIMER, "JTimer::CreateTimer");

    uiHandler = GetFreeTimerCb();
    if (uiHandler)
    {
        m_timerCB[uiHandler].isUsed = JTRUE;
        m_timerCB[uiHandler].eType = eType;
        m_timerCB[uiHandler].pfFunc = pfFunc;
        m_timerCB[uiHandler].pData = pData;
        m_timerCB[uiHandler].uiTime = uiTime;

        return uiHandler;
    }
    else
    {
        return JNULL;
    }
}

JUINT32 JTimer::StartTimer(const JUINT32 uiHandler)
{
    JSYSTIME sysTime;

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_TIMER, "JTimer::StartTimer");

    GetSysTime(&sysTime);
    AddSysTime(&sysTime, m_timerCB[uiHandler].uiTime);
    SafeMemcpy(reinterpret_cast<JCHAR*>(&m_timerCB[uiHandler].expire), 
        reinterpret_cast<JCHAR*>(&sysTime), 
        sizeof(JSYSTIME), sizeof(JSYSTIME));

    return JSUCCESS;
}

JUINT32 JTimer::StopTimer(const JUINT32 uiHandler)
{
    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_TIMER, "JTimer::StopTimer");

    SafeMemset(reinterpret_cast<JCHAR*>(&m_timerCB[uiHandler].expire), 0, sizeof(JSYSTIME));

    return JSUCCESS;
}

JUINT32 JTimer::DestoryTimer(const JUINT32 uiHandler)
{
    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_TIMER, "JTimer::DestoryTimer");

    SafeMemset(reinterpret_cast<JCHAR*>(&m_timerCB[uiHandler]), 0, sizeof(JTIMER_CB));

    return JSUCCESS;
}

JUINT32 JTimer::InitFunc()
{
    return JSUCCESS;
}

JUINT32 JTimer::EventProcFunc(JEvent* pEvent)
{
    JEVT_TYPE type = JEVT_NONE;
    JSysTime* pJSysTimeBody = JNULL;
    JUINT32 uiIdx = 0;

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_TIMER, "JTimer::EventProcFunc");

    if (!pEvent)
    {
        JSingleton<JLog>::instance2() << set(JLOG_MOD_TIMER, JLOG_ERROR_LEVEL) 
            << "JTimer::EventProcFunc input param is NULL\n";
        return JFAILURE;
    }

    type = pEvent->GetEventType();
    switch(type)
    {
        case JEVT_TIMER_EXPIRE:
        {
            pJSysTimeBody = dynamic_cast<JSysTime*>(pEvent->GetBody());
            if (pJSysTimeBody)
            {
                JSYSTIME sysTime;

                sysTime.uiYear = pJSysTimeBody->GetYear();
                sysTime.uiMonth = pJSysTimeBody->GetMonth();
                sysTime.uiDayOfWeek = pJSysTimeBody->GetDayOfWeek();
                sysTime.uiDay = pJSysTimeBody->GetDay();
                sysTime.uiHour = pJSysTimeBody->GetHour();
                sysTime.uiMinute = pJSysTimeBody->GetMinute();
                sysTime.uiSecond = pJSysTimeBody->GetSecond();
                sysTime.uiMilliseconds = pJSysTimeBody->GetMilliseconds();

                for (uiIdx=0; uiIdx<JTIMER_MAX_CB_NUM; uiIdx++)
                {
                    if (m_timerCB[uiIdx].isUsed && 
                        CmpSysTime(&m_timerCB[uiIdx].expire, &sysTime) == SYSTIME_SMALLER && 
                        CheckSysTime(&m_timerCB[uiIdx].expire, &sysTime) == SYSTIME_SMALLER)
                    {
                        if (m_timerCB[uiIdx].eType == JTIMER_TYPE_PERIODIC)
                        {
                            AddSysTime(&m_timerCB[uiIdx].expire, m_timerCB[uiIdx].uiTime);
                        }

                        m_timerCB[uiIdx].pfFunc(m_timerCB[uiIdx].pData);
                    }
                }
            }
        }
        break;

        default:
        break;
    };

	return JSUCCESS;
}

JEventBody* JTimer::MakeEventBody(JUINT32 uiType)
{
	JEventBody* pEventBody = JNULL;

	switch(uiType)
	{
		case JEVT_TIMER_EXPIRE:
			pEventBody = new JSysTime;
			break;

		default:
			break;
	};

	return pEventBody;
}

JUINT32 JTimer::GetFreeTimerCb()
{
    JUINT32 uiIdx = 0;

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_TIMER, "JTimer::GetFreeTimerCb");

    for (uiIdx=1; uiIdx<JTIMER_MAX_CB_NUM; uiIdx++)
    {
        if (m_timerCB[uiIdx].isUsed == JFALSE)
        {
            break;
        }
    }

    if (uiIdx == JTIMER_MAX_CB_NUM)
    {
        return JNULL;
    }
    else
    {
    	return uiIdx;
	}
}

JINT32 JTimer::CheckSysTime(JSYSTIME *srcSysTime, JSYSTIME *dstSysTime)
{
    JINT32 result = SYSTIME_SMALLER;
    JINT32 iHour = 0;
    JINT32 iMinute = 0;
    JINT32 iSecond = 0;
    JINT32 iMilliseconds = 0;

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_TIMER, "JTimer::CheckSysTime");

    iHour = dstSysTime->uiHour - srcSysTime->uiHour;
    iMinute = (dstSysTime->uiMinute - srcSysTime->uiMinute) + iHour*60;
    iSecond = (dstSysTime->uiSecond - srcSysTime->uiSecond) + iMinute*60;
    iMilliseconds = (dstSysTime->uiMilliseconds - srcSysTime->uiMilliseconds) + iSecond*1000;
    if (iMilliseconds > JTIMER_MIN_PERIOD*10)
    {
        result = SYSTIME_INVALID;
    }

	return result;
}

}

