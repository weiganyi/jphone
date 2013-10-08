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

//JSysTime definition
//the timer level is microsecond
#define JTIMER_MIN_PERIOD   1000
//how much timer can be manage
#define JTIMER_MAX_CB_NUM   10

typedef enum{
    //timer only run one time
    JTIMER_TYPE_ONESHOT,
    //timer can run periodicly
    JTIMER_TYPE_PERIODIC
}JTIMER_TYPE;

//callback function type
typedef JUINT32 (*JTIMER_CALLBACK)(JVOID* pData);

typedef struct{
    //if is used
    JUINT8  isUsed;
    //timer type
    JTIMER_TYPE eType;
    //callback function
    JTIMER_CALLBACK pfFunc;
    //data pointer
    JVOID*  pData;
    //timer length
    JUINT32 uiTime;     //millisecond
    //expire time
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
    //time value
    JUINT32 uiYear; 
    JUINT32 uiMonth; 
    JUINT32 uiDayOfWeek; 
    JUINT32 uiDay; 
    JUINT32 uiHour; 
    JUINT32 uiMinute; 
    JUINT32 uiSecond; 
    JUINT32 uiMilliseconds; 
};


//JTimer definition
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
    //timer handler
    JUINT32 m_handler;

    //timer control block
    JTIMER_CB m_timerCB[JTIMER_MAX_CB_NUM];

    //lock to protect control block access
    JLock m_Lock;

    //agent thread object
    JAgentThread* m_pAgentThread;

    JUINT32 GetFreeTimerCb();
};

}


#endif

