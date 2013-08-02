/**********************************************************
* Author: weiganyi
* File: JThread.h
* Date: 20110718
* Description: 
*
**********************************************************/

#ifndef _JTHREAD_H_
#define _JTHREAD_H_

namespace JFrameWork{

//JThread definication
typedef enum{
    JTHRD_TYPE_NONE,
	JTHRD_TYPE_MOD,
	JTHRD_TYPE_SRV,
	JTHRD_TYPE_MSG,
	JTHRD_TYPE_AGT,
	JTHRD_TYPE_MAX
}JTHRD_TYPE;

class JThread: public JObject{
public:
    JThread(JString* pThdName, JTHRD_TYPE eType);
    ~JThread();

    virtual JUINT32 EnQueueEvent(JListItem<JEvent>* pItem, JCHAR* pModName=JNULL) = 0;
    virtual JEventBody* MakeEventBody(JEVT_TYPE eType, JCHAR* pModName=JNULL) = 0;
    virtual JUINT32 Run() = 0;

    JTHRD_TYPE GetType();

protected:
    JString strThdName;

    JTHRD_TYPE m_eType;
};


//JModuleThread definication
//smaller level has higher priority
typedef enum{
    JMOD_LEVEL_NONE,
	JMOD_LEVEL_1,
	JMOD_LEVEL_2,
	JMOD_LEVEL_3,
	JMOD_LEVEL_4,
	JMOD_LEVEL_5,
	JMOD_LEVEL_6,
	JMOD_LEVEL_7,
	JMOD_LEVEL_MAX
}JMOD_PRI_LEVEL;

typedef struct _JMOD_PUB_DATA{
    JString strModName;
    JMOD_PRI_LEVEL eLevel;
    JModule* pModule;
}JMOD_PUB_DATA;

typedef struct _JMOD_PRI_DATA{
    JString strModName;
    JMOD_PRI_LEVEL eLevel;
    JModule* pModule;

    JUINT32 uiRunTime;
    JQueue<JEvent> hQueue;
}JMOD_PRI_DATA;

#define JTHREAD_MAX_MODULE  10

class JModuleThread: public JThread{
public:
    JModuleThread(JString* pThdName = JNULL, 
                JString* pLocalAddr = JNULL, 
                JUINT32 uiLocalPort =0);
    ~JModuleThread();

    JUINT32 Init();
    JUINT32 Run();
    JUINT32 RegisterModule(JMOD_PUB_DATA modPubData);
    JUINT32 EnQueueEvent(JListItem<JEvent>* pItem, JCHAR* pModName=JNULL);

    JEventBody* MakeEventBody(JEVT_TYPE eType, JCHAR* pModName=JNULL);

    JModule* GetModule(JCHAR* pModName);
    JCommEngine* GetNotifyCommEngine();

    friend DWORD JThreadProc(JThread* pThread);

private:
    JMOD_PRI_DATA m_modData[JTHREAD_MAX_MODULE];
    JUINT32 m_modDataUsed;

    JCommEngine* m_pNotifyCommEngine;

    JCommEngineGroup m_commEngineGroup;

    JLock m_Lock;

    JUINT32 GetRunModuleSeq();
    JUINT32 IncModuleSeq(const JUINT32 uiModSeq);
    JQueue<JEvent>* GetModuleQueue(JCHAR* pModName);
};


//JAgentThread definication
typedef struct _JEXT_THRD_PRI_DATA{
    JQueue<JEvent> hQueue;
}JAGT_THRD_PRI_DATA;

class JAgentThread: public JThread{
public:
    JAgentThread(JString* pThdName = JNULL, 
                    JString* pLocalAddr = JNULL, 
                    JUINT32 uiLocalPort =0);
    ~JAgentThread();

    JUINT32 Run();

    JUINT32 EnQueueEvent(JListItem<JEvent>* pItem, JCHAR* pModName=JNULL);
    JListItem<JEvent>* DeQueueEvent();

    JEventBody* MakeEventBody(JEVT_TYPE eType, JCHAR* pModName=JNULL);

    JCommEngine* GetNotifyCommEngine();
    JCommEngineGroup& GetCommEngineGroup();
    JQueue<JEvent>& GetQueue();

private:
    JAGT_THRD_PRI_DATA m_agtThrdData;

    JCommEngine* m_pNotifyCommEngine;
    JCommEngineGroup m_commEngineGroup;

    JLock m_Lock;
};


//JThreadManager definication
typedef struct _JTHRD_PUB_DATA{
    JString strThrdName;
    JThread* pThread;
}JTHRD_PUB_DATA;

typedef struct _JTHRD_PRI_DATA{
    JString strThrdName;
    JThread* pThread;

    JBOOL bRunning;
	HANDLE pThreadId;
}JTHRD_PRI_DATA;

#define JTHREAD_MAX_THREAD  20

class JThreadManager: public JObject{
public:
    JThreadManager();
    ~JThreadManager();

    JUINT32 RegisterThread(JTHRD_PUB_DATA thdPubData);
    JUINT32 EnQueueEvent(JCHAR* pThrdName, JCHAR* pModName, JListItem<JEvent>* pItem);
    JUINT32 Run();

    JEventBody* MakeEventBody(JCHAR* pThrdName, JCHAR* pModName, JEVT_TYPE eType);

    JThread* GetThread(JCHAR* pThrdName);

    friend DWORD JThreadProc(JThread* pThread);
    friend JModule* GetRegisterModule(JCHAR* pThrdName, JCHAR* pModName);

private:
    JTHRD_PRI_DATA m_thrdData[JTHREAD_MAX_THREAD];
    JUINT32 m_thrdDataUsed;
};


extern DWORD JThreadProc(JThread* pThread);
extern JModule* GetRegisterModule(JCHAR* pThrdName, JCHAR* pModName);

}


#endif

