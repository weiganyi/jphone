/**********************************************************
* Author: weiganyi
* File: JThread.cpp
* Date: 20110718
* Description: 
*
**********************************************************/

#include "JFrameWork.h"

using namespace JFrameWork;

#include "JSipUa.h"
#include "JLogSrv.h"

namespace JFrameWork{


JThread::JThread(JString* pThdName, JTHRD_TYPE eType)
{
    if (pThdName)
    {
        strThdName = *pThdName;
    }

    m_eType = eType;

	return;
}

JThread::~JThread()
{
    return;
}

JTHRD_TYPE JThread::GetType()
{
    return m_eType;
}

JModuleThread::JModuleThread(JString* pThdName, 
                        JString* pLocalAddr, 
                        JUINT32 uiLocalPort):
                        JThread(pThdName, JTHRD_TYPE_MOD),
                        m_modDataUsed(0)
{
    JUINT32 uiIdx;
    JSOCKADDR_IN lclAddr;

    for (uiIdx=0; uiIdx<JTHREAD_MAX_MODULE; uiIdx++)
    {
        m_modData[uiIdx].eLevel = JMOD_LEVEL_NONE;
        m_modData[uiIdx].pModule = JNULL;
        m_modData[uiIdx].uiRunTime = 0;
    }

    //construct a commengine for communication between the modules
    if (pLocalAddr && uiLocalPort)
    {
        m_pNotifyCommEngine = new JCommEngine(JSOCKET_UDP);
        if (m_pNotifyCommEngine)
        {
            lclAddr.sin_family = AF_INET;
            lclAddr.sin_addr.S_un.S_addr = inet_addr(pLocalAddr->c_str());
            lclAddr.sin_port = htons(uiLocalPort);
            m_pNotifyCommEngine->SetLocalAddr(JS_P_JMAINPROC, pThdName->c_str(), &lclAddr);
        }
    }

	return;
}

JModuleThread::~JModuleThread()
{
    if (m_pNotifyCommEngine)
    {
        delete m_pNotifyCommEngine;
    }

    return;
}

JUINT32 JModuleThread::Init()
{
    JUINT32 uiIdx = 0;

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_THREAD, "JModuleThread::Init");

    #if 1
    //only for RTTI usage test, otherwise I need use virtual function calling here.
    for (uiIdx=0; uiIdx<m_modDataUsed; uiIdx++)
    {
        if (typeid(*(m_modData[uiIdx].pModule)) == typeid(JStaticMemory))
        {
            JStaticMemory* pStaticMemory = dynamic_cast<JStaticMemory*>(m_modData[uiIdx].pModule);
            if (pStaticMemory)
            {
                pStaticMemory->InitFunc();
            }
        }
        else if (typeid(*(m_modData[uiIdx].pModule)) == typeid(JLog))
        {
            JLog* pLog = dynamic_cast<JLog*>(m_modData[uiIdx].pModule);
            if (pLog)
            {
                pLog->InitFunc();
            }
        }
        else if (typeid(*(m_modData[uiIdx].pModule)) == typeid(JTimer))
        {
            JTimer* pTimer = dynamic_cast<JTimer*>(m_modData[uiIdx].pModule);
            if (pTimer)
            {
                pTimer->InitFunc();
            }
        }
        else if (typeid(*(m_modData[uiIdx].pModule)) == typeid(JDaemon))
        {
            JDaemon* pDaemon = dynamic_cast<JDaemon*>(m_modData[uiIdx].pModule);
            if (pDaemon)
            {
                pDaemon->InitFunc();
            }
        }
        else if (typeid(*(m_modData[uiIdx].pModule)) == typeid(JPjSipUa))
        {
            JPjSipUa* pPjSipUa = dynamic_cast<JPjSipUa*>(m_modData[uiIdx].pModule);
            if (pPjSipUa)
            {
                pPjSipUa->InitFunc();
            }
        }
    }
    #else
    for (uiIdx=0; uiIdx<m_modDataUsed; uiIdx++)
    {
        if (m_modData[uiIdx].pModule)
        {
            m_modData[uiIdx].pModule->InitFunc();
        }
    }
    #endif

    //add notify commengine into the group
    if (m_pNotifyCommEngine)
    {
        m_commEngineGroup.AddCommEngine(m_pNotifyCommEngine);
    }

	return JSUCCESS;
}

JUINT32 JModuleThread::Run()
{
    JUINT32 uiRet = JFAILURE;
    JUINT32 uiIdx = 0;
    JListItem<JEvent>* pListItem = JNULL;
    JEvent* pEvent = JNULL;
    JSOCKADDR_IN stAddr;
    JCommEngine* pCommEngine = JNULL;
    JCHAR pBuf[JCOMM_MSG_BUF_LEN] = {0};

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_THREAD, "JModuleThread::Run");

    uiRet = Init();
    if (!uiRet)
    {
        JSingleton<JLog>::instance2() << set(JLOG_MOD_THREAD, JLOG_ERROR_LEVEL) 
            << "JModuleThread::Run Init return failure\n";
        return JFAILURE;
    }

    do{
        //check message
        pCommEngine = m_commEngineGroup.HasMessage(JCOMM_SELECT_INDEFINITE);
        if (pCommEngine && pCommEngine == m_pNotifyCommEngine)  //for thread notify CommEngine
        {
            uiRet = pCommEngine->RecvMessage(pBuf, JCOMM_MSG_BUF_LEN, &stAddr);
            //whether is event notify message
            if (uiRet == 1 && SafeStrcmp(pBuf, "1") ==0)
            {
                while(1)
                {
                    //get module that can run this time
                    uiIdx = GetRunModuleSeq();

                    if (uiIdx<m_modDataUsed)
                    {
                        m_Lock.Acquire();

                        //dequeue the event and process it
                        pListItem = m_modData[uiIdx].hQueue.DeQueue();
                        if (pListItem)
                        {
                            pEvent = pListItem->GetData();
                            if (pEvent)
                            {
                                m_modData[uiIdx].pModule->EventProcFunc(pEvent);
                            }

                            delete pListItem;
                        }

                        m_Lock.Release();
                    }
                    else
                    {
                        break;
                    }

                    //increase runing count for this module
                    IncModuleSeq(uiIdx);
                }
            }
        }
    }while(1);

	return JSUCCESS;
}

JUINT32 JModuleThread::RegisterModule(JMOD_PUB_DATA modPubData)
{
    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_THREAD, "JModuleThread::RegisterModule");

    if (m_modDataUsed >= JTHREAD_MAX_MODULE)
    {
        JSingleton<JLog>::instance2() << set(JLOG_MOD_THREAD, JLOG_ERROR_LEVEL) 
            << "JModuleThread::RegisterModule: registered module already reach to the max number\n";
        return JFAILURE;
    }

    m_modData[m_modDataUsed].strModName = modPubData.strModName;
    m_modData[m_modDataUsed].eLevel = modPubData.eLevel;
    m_modData[m_modDataUsed].pModule = modPubData.pModule;

    m_modData[m_modDataUsed].uiRunTime = 0;

    m_modDataUsed++;

	return JSUCCESS;
}

JUINT32 JModuleThread::EnQueueEvent(JListItem<JEvent>* pItem, JCHAR* pModName)
{
    JQueue<JEvent>* pModuleQueue = JNULL;

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_THREAD, "JModuleThread::EnQueueEvent");

    if (!pItem)
    {
        JSingleton<JLog>::instance2() << set(JLOG_MOD_THREAD, JLOG_ERROR_LEVEL) 
            << "JModuleThread::EnQueueEvent input param is NULL\n";
        return JFAILURE;
    }

    pModuleQueue = GetModuleQueue(pModName);
    if (pModuleQueue)
    {
        m_Lock.Acquire();
        pModuleQueue->EnQueue(pItem);
        m_Lock.Release();
    }
    else
    {
        JSingleton<JLog>::instance2() << set(JLOG_MOD_THREAD, JLOG_ERROR_LEVEL) 
            << "JModuleThread::EnQueueEvent module queue isn't exist\n";
        return JFAILURE;
    }

    return JSUCCESS;
}

JEventBody* JModuleThread::MakeEventBody(JEVT_TYPE eType, JCHAR* pModName)
{
	JEventBody* pEventBody = JNULL;
	JModule* pModule = JNULL;

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_THREAD, "JModuleThread::MakeEventBody");

	pModule = GetModule(pModName);
	if (pModule)
	{
		pEventBody = pModule->MakeEventBody(static_cast<JUINT32>(eType));
	}

	return pEventBody;
}

//get module from name
JModule* JModuleThread::GetModule(JCHAR* pModName)
{
    JUINT32 uiIdx;

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_THREAD, "JModuleThread::GetModule");

    for (uiIdx=0; uiIdx<m_modDataUsed; uiIdx++)
    {
        if (m_modData[uiIdx].strModName == pModName)
        {
            return m_modData[uiIdx].pModule;
        }
    }

    return JNULL;
}

JCommEngine* JModuleThread::GetNotifyCommEngine()
{
    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_THREAD, "JModuleThread::GetNotifyCommEngine");

    return m_pNotifyCommEngine;
}

JUINT32 JModuleThread::GetRunModuleSeq()
{
    JUINT32 uiIdx = 0;
    JUINT32 uiHighPriMod = m_modDataUsed;
    JUINT32 uiMinRunTime = 0xFFFF;

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_THREAD, "JModuleThread::GetRunModuleSeq");

    //check all modules that have some events, then calculate the smallest 
    //runing time for all modules, use it as highest priority module to run
    for (uiIdx=0; uiIdx<m_modDataUsed; uiIdx++)
    {
        if (!m_modData[uiIdx].hQueue.IsEmpty())
        {
            if (uiMinRunTime > m_modData[uiIdx].uiRunTime)
            {
                uiMinRunTime = m_modData[uiIdx].uiRunTime;
                uiHighPriMod = uiIdx;
            }
        }
    }

    return uiHighPriMod;
}

JUINT32 JModuleThread::IncModuleSeq(const JUINT32 uiModSeq)
{
    JUINT32 uiIdx = 0;
    JUINT32 uiMinRunTime = m_modData[0].uiRunTime;
    JUINT32 uiSwitchValue = 0;
    JUINT32 uiMaxValue = 0xFFFF;
    JUINT32 uiIncValue = 0;

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_THREAD, "JModuleThread::IncModuleSeq");

    if (uiModSeq < m_modDataUsed)
    {
        //increase the running time from the module level, 
        //and process loop case carefully
        uiIncValue = m_modData[uiModSeq].eLevel;
        if ((uiMaxValue-m_modData[uiModSeq].uiRunTime) < uiIncValue)
        {
            for (uiIdx=1; uiIdx<m_modDataUsed; uiIdx++)
            {
                if (uiMinRunTime > m_modData[uiIdx].uiRunTime)
                {
                    uiMinRunTime = m_modData[uiIdx].uiRunTime;
                }
            }

            uiSwitchValue = uiMaxValue-uiMinRunTime;

            for (uiIdx=0; uiIdx<m_modDataUsed; uiIdx++)
            {
                m_modData[uiIdx].uiRunTime += uiSwitchValue;
            }
        }

        m_modData[uiModSeq].uiRunTime += m_modData[uiModSeq].eLevel;
    }

	return JSUCCESS;
}

JQueue<JEvent>* JModuleThread::GetModuleQueue(JCHAR* pModName)
{
    JUINT32 uiIdx;

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_THREAD, "JModuleThread::GetModuleQueue");

    if (!pModName)
    {
        JSingleton<JLog>::instance2() << set(JLOG_MOD_THREAD, JLOG_ERROR_LEVEL) 
            << "JModuleThread::GetModuleQueue input param is NULL\n";
        return JNULL;
    }

    for (uiIdx=0; uiIdx<m_modDataUsed; uiIdx++)
    {
        if (m_modData[uiIdx].strModName == pModName)
        {
            return &m_modData[uiIdx].hQueue;
        }
    }

    return JNULL;
}

JAgentThread::JAgentThread(JString* pThdName, 
                                JString* pLocalAddr, 
                                JUINT32 uiLocalPort):
                                JThread(pThdName, JTHRD_TYPE_AGT)
{
    JSOCKADDR_IN lclAddr;

    if (pLocalAddr && uiLocalPort)
    {
        //construct a notify comm engine to send and recv message
        m_pNotifyCommEngine = new JCommEngine(JSOCKET_UDP);
        if (m_pNotifyCommEngine)
        {
            lclAddr.sin_family = AF_INET;
            lclAddr.sin_addr.S_un.S_addr = inet_addr(pLocalAddr->c_str());
            lclAddr.sin_port = htons(uiLocalPort);
            m_pNotifyCommEngine->SetLocalAddr(JS_P_JMAINPROC, pThdName->c_str(), &lclAddr);

            m_commEngineGroup.AddCommEngine(m_pNotifyCommEngine);
        }
    }

	return;
}

JAgentThread::~JAgentThread()
{
    if (m_pNotifyCommEngine)
    {
        delete m_pNotifyCommEngine;
    }

    return;
}

JUINT32 JAgentThread::Run()
{
    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_THREAD, "JAgentThread::Run");

	return JSUCCESS;
}

JUINT32 JAgentThread::EnQueueEvent(JListItem<JEvent>* pItem, JCHAR* pModName)
{
    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_THREAD, "JAgentThread::EnQueueEvent");

    if (!pItem)
    {
        JSingleton<JLog>::instance2() << set(JLOG_MOD_THREAD, JLOG_ERROR_LEVEL) 
            << "JAgentThread::EnQueueEvent input param is NULL\n";
        return JFAILURE;
    }

    m_Lock.Acquire();
    m_agtThrdData.hQueue.EnQueue(pItem);
    m_Lock.Release();

    return JSUCCESS;
}

JListItem<JEvent>* JAgentThread::DeQueueEvent()
{
    JListItem<JEvent>* pListItem = JNULL;

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_THREAD, "JAgentThread::DeQueueEvent");

    m_Lock.Acquire();
    pListItem = m_agtThrdData.hQueue.DeQueue();
    m_Lock.Release();

    return pListItem;
}

JEventBody* JAgentThread::MakeEventBody(JEVT_TYPE eType, JCHAR* pModName)
{
	JEventBody* pEventBody = JNULL;

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_THREAD, "JAgentThread::MakeEventBody");

	return pEventBody;
}

JCommEngine* JAgentThread::GetNotifyCommEngine()
{
    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_THREAD, "JAgentThread::GetNotifyCommEngine");

    return m_pNotifyCommEngine;
}

JCommEngineGroup& JAgentThread::GetCommEngineGroup()
{
    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_THREAD, "JAgentThread::GetCommEngineGroup");

    return m_commEngineGroup;
}

JQueue<JEvent>& JAgentThread::GetQueue()
{
    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_THREAD, "JAgentThread::GetQueue");

    return m_agtThrdData.hQueue;
}

JThreadManager::JThreadManager()
{
    JUINT32 uiIdx;

    for (uiIdx=0; uiIdx<JTHREAD_MAX_THREAD; uiIdx++)
    {
        m_thrdData[uiIdx].pThread = JNULL;
        m_thrdData[uiIdx].bRunning = JFALSE;
        m_thrdData[uiIdx].pThreadId = JNULL;
    }

    m_thrdDataUsed = 0;

	return;
}

JThreadManager::~JThreadManager()
{
    return;
}

JUINT32 JThreadManager::RegisterThread(JTHRD_PUB_DATA thrdPubData)
{
    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_THREAD, "JThreadManager::RegisterModule");

    if (m_thrdDataUsed >= JTHREAD_MAX_MODULE)
    {
        JSingleton<JLog>::instance2() << set(JLOG_MOD_THREAD, JLOG_ERROR_LEVEL) 
            << "JThreadManager::RegisterModule: registered module already reach to the max number\n";
        return JFAILURE;
    }

    m_thrdData[m_thrdDataUsed].strThrdName = thrdPubData.strThrdName;
    m_thrdData[m_thrdDataUsed].pThread = thrdPubData.pThread;

    m_thrdDataUsed++;

	return JSUCCESS;
}

JUINT32 JThreadManager::EnQueueEvent(JCHAR* pThrdName, JCHAR* pModName, JListItem<JEvent>* pItem)
{
    JUINT32 uiRet = JFAILURE;
    JThread* pThread = JNULL;

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_THREAD, "JThreadManager::EnQueueEvent");

    if (!pThrdName && !pModName && !pItem)
    {
        JSingleton<JLog>::instance2() << set(JLOG_MOD_THREAD, JLOG_ERROR_LEVEL) 
            << "JThreadManager::EnQueueEvent input param is NULL\n";
        return JFAILURE;
    }

    pThread = GetThread(pThrdName);
    if (pThread)
    {
        uiRet = pThread->EnQueueEvent(pItem, pModName);
    }

    return uiRet;
}

JUINT32 JThreadManager::Run()
{
    JUINT32 uiIdx = 0;
    JUINT32 uiRet = 0;

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_THREAD, "JThreadManager::Run");

    //start thread to run all thread
    for (uiIdx=0; uiIdx<m_thrdDataUsed; uiIdx++)
    {
        if (m_thrdData[uiIdx].pThread && 
            !m_thrdData[uiIdx].bRunning &&
            (m_thrdData[uiIdx].pThread->GetType() == JTHRD_TYPE_MOD ||
            m_thrdData[uiIdx].pThread->GetType() == JTHRD_TYPE_SRV ||
            m_thrdData[uiIdx].pThread->GetType() == JTHRD_TYPE_MSG))
        {
            m_thrdData[uiIdx].pThreadId = CreateThread(JNULL, JNULL, 
                (LPTHREAD_START_ROUTINE)JThreadProc, m_thrdData[uiIdx].pThread, 0, 
                (LPDWORD)JNULL);
            if (!m_thrdData[uiIdx].pThreadId)
            {
                JSingleton<JLog>::instance2() << set(JLOG_MOD_THREAD, JLOG_ERROR_LEVEL) 
                    << "JThreadManager::Run CreateThread return failure\n";
                return JFAILURE;
            }

            m_thrdData[uiIdx].bRunning = JTRUE;
        }
    }

	return JSUCCESS;
}

JEventBody* JThreadManager::MakeEventBody(JCHAR* pThrdName, JCHAR* pModName, JEVT_TYPE eType)
{
    JThread* pThread = JNULL;
	JEventBody* pEventBody = JNULL;

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_THREAD, "JThreadManager::MakeEventBody");

    pThread = GetThread(pThrdName);
    if (pThread)
    {
        pEventBody = pThread->MakeEventBody(eType, pModName);
    }

	return pEventBody;
}

//get thread handler from name
JThread* JThreadManager::GetThread(JCHAR* pThrdName)
{
    JUINT32 uiIdx;

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_THREAD, "JThreadManager::GetThread");

    for (uiIdx=0; uiIdx<m_thrdDataUsed; uiIdx++)
    {
        if (m_thrdData[uiIdx].strThrdName == pThrdName)
        {
            return m_thrdData[uiIdx].pThread;
        }
    }

    return JNULL;
}

DWORD JThreadProc(JThread* pThread)
{
    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_THREAD, "JThreadProc");

    if (!pThread)
    {
        cout << "JThreadProc input param is NULL" << endl;
        return JFAILURE;
    }

    pThread->Run();
    return JSUCCESS;
}

//get module from thread and module name
JModule* GetRegisterModule(JCHAR* pThrdName, JCHAR* pModName)
{
    JThread* pThread = JNULL;
    JModuleThread* pModuleThread = JNULL;
    JModule* pModule = JNULL;

    JThreadManager* pThreadManager = JSingleton<JThreadManager>::instance();

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_THREAD, "GetRegisterModule");

    if (!pThrdName || !pModName)
    {
        return JNULL;
    }

    pThread = pThreadManager->GetThread(pThrdName);
    if (pThread && pThread->GetType() == JTHRD_TYPE_MOD)
    {
        pModuleThread = dynamic_cast<JModuleThread*>(pThread);
        if (pModuleThread)
        {
            pModule = pModuleThread->GetModule(pModName);
        }
    }

    return pModule;
}

}

