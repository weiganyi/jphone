/**********************************************************
* Author: weiganyi
* File: JTimer.h
* Date: 20110718
* Description: 
*
**********************************************************/

#ifndef _JTIMER_H_
#define _JTIMER_H_


namespace JFrameWork{

//JSysTime definication
#define JTIMER_MIN_PERIOD   1000
#define JTIMER_MAX_CB_NUM   10

typedef enum{
    JTIMER_TYPE_ONESHOT,
    JTIMER_TYPE_PERIODIC
}JTIMER_TYPE;

typedef JUINT32 (*JTIMER_CALLBACK)(JVOID* pData);

typedef struct{
    JUINT8  isUsed;
    JTIMER_TYPE eType;
    JTIMER_CALLBACK pfFunc;
    JVOID*  pData;
    JUINT32 uiTime;   //millisecond
    JSYSTIME expire;
}JTIMER_CB;

class JSysTime: public JEventBody{
public:
    JSysTime();
    ~JSysTime();

    JUINT32 GetLength();

    JEventBody* Clone();
    JUINT32 Clear();

    JUINT32 Serialize(JCHAR* pBuf, JUINT32 uiMaxNum);
    JUINT32 DeSerialize(JCHAR* pBuf);

    JUINT32 operator=(JSysTime& rSysTime);
    JUINT32 GetYear();
    JUINT32 SetYear(JUINT32 uiVal);
    JUINT32 GetMonth();
    JUINT32 SetMonth(JUINT32 uiVal);
    JUINT32 GetDayOfWeek();
    JUINT32 SetDayOfWeek(JUINT32 uiVal);
    JUINT32 GetDay();
    JUINT32 SetDay(JUINT32 uiVal);
    JUINT32 GetHour();
    JUINT32 SetHour(JUINT32 uiVal);
    JUINT32 GetMinute();
    JUINT32 SetMinute(JUINT32 uiVal);
    JUINT32 GetSecond();
    JUINT32 SetSecond(JUINT32 uiVal);
    JUINT32 GetMilliseconds();
    JUINT32 SetMilliseconds(JUINT32 uiVal);

private:
    JUINT32 uiYear; 
    JUINT32 uiMonth; 
    JUINT32 uiDayOfWeek; 
    JUINT32 uiDay; 
    JUINT32 uiHour; 
    JUINT32 uiMinute; 
    JUINT32 uiSecond; 
    JUINT32 uiMilliseconds; 
};


//JTimer definication
class JTimer: public JModule{
public:
    JTimer();
    ~JTimer();

    JUINT32 CreateTimer(JTIMER_TYPE eType, 
                        JTIMER_CALLBACK pfFunc, 
                        JVOID* pData, 
                        const JUINT32 uiTime);
    JUINT32 StartTimer(const JUINT32 uiHandler);
    JUINT32 StopTimer(const JUINT32 uiHandler);
    JUINT32 DestoryTimer(const JUINT32 uiHandler);

    JUINT32 InitFunc();
    JUINT32 EventProcFunc(JEvent* pEvent);
    JEventBody* MakeEventBody(JUINT32 uiType);

    JINT32 CheckSysTime(JSYSTIME *srcSysTime, JSYSTIME *dstSysTime);

    friend void CALLBACK lpTimeProc(UINT uID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2);

private:

    JUINT32 m_handler;
    JTIMER_CB m_timerCB[JTIMER_MAX_CB_NUM];

    JLock m_Lock;

    JAgentThread* m_pAgentThread;

    JUINT32 GetFreeTimerCb();
};

}


#endif

