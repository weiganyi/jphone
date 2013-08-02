/**********************************************************
* Author: weiganyi
* File: JLogServerAgent.cpp
* Date: 20120504
* Description: 
*
**********************************************************/

#include "JFrameWork.h"

using namespace JFrameWork;

#include "JLogSrv.h"
#include "JLogSrvAgent.h"


JLogServerAgent::JLogServerAgent()
{
    JLog* pLog = JNULL;
    JStaticMemory* pStaticMemory = JNULL;
    JRoute* pRoute = JNULL;
	JTHRD_PUB_DATA thrdPubData;
    JString strThrdName;
    JString strLocalAddr;

    /* create all object */
    pLog = JSingleton<JLog>::instance();
    pStaticMemory = JSingleton<JStaticMemory>::instance();
    pRoute = JSingleton<JRoute>::instance();

    /* set log level */
    pLog->SetLogLevel(JLOG_MOD_ALL, JLOG_DEBUG_LEVEL);
    //ptrLog->SetLogLevel(JLOG_MOD_LOGSERVER_AGENT, JLOG_ENTER_LEVEL);

    /* set log output method */
    pLog->SetLogMethod(JLOG_OUTPUT_LOCAL);

    //create JAgentThread and add into JThreadManager
    strThrdName = JS_T_JLOGSRVAGENT;
    strLocalAddr = JLOGSERVER_AGENT_LOCAL_ADDR;
	m_pAgentThread = new JAgentThread(&strThrdName, &strLocalAddr, JLOGSERVER_AGENT_LOCAL_PORT);

    JThreadManager* pThreadManager = JSingleton<JThreadManager>::instance();
    thrdPubData.strThrdName = JS_T_JLOGSRVAGENT;
    thrdPubData.pThread = m_pAgentThread;
    pThreadManager->RegisterThread(thrdPubData);

	return;
}

JLogServerAgent::~JLogServerAgent()
{
    /* destroy all object */
    if (m_pAgentThread)
    {
        delete m_pAgentThread;
    }

	return;
}

JUINT32 JLogServerAgent::SetLogSrvCfg(JLogSrvCfg* pLogSrvCfg)
{
    JEvent* pEvent = JNULL;
    JLogSrvCfg* pLogSrvCfgBody = JNULL;
    JCommEngine* pComEngine = JNULL;
    JThread* pThread = JNULL;
    JAgentThread* pAgentThread = JNULL;

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_LOGSERVER_AGENT, "JLogServerAgent::SetLogSrvCfg");

    pEvent = new JEvent(JEVT_LOGSRV_SET_CFG);
    pLogSrvCfgBody = new JLogSrvCfg;
	if (pEvent && pLogSrvCfgBody)
	{
		pLogSrvCfgBody->SetListenAddr(pLogSrvCfg->GetListenAddr());
		pLogSrvCfgBody->SetListenPort(pLogSrvCfg->GetListenPort());

	    pEvent->SetFromProc(JS_P_JMAINPROC);
	    pEvent->SetFromThrd(JS_T_JLOGSRVAGENT);
	    pEvent->SetFromMod(JNULL);
	    pEvent->SetToProc(JS_P_JMAINPROC);
	    pEvent->SetToThrd(JS_T_JMAINTHREAD);
	    pEvent->SetToMod(JNULL);
	    pEvent->SetBody(pLogSrvCfgBody);

        pThread = JSingleton<JThreadManager>::instance()->GetThread(JS_T_JLOGSRVAGENT);
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

    return JSUCCESS;
}

JUINT32 JLogServerAgent::GetLogSrvCfg()
{
    JEvent* pEvent = JNULL;
    JCommEngine* pComEngine = JNULL;
    JThread* pThread = JNULL;
    JAgentThread* pAgentThread = JNULL;

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_LOGSERVER_AGENT, "JLogServerAgent::GetLogSrvCfg");

    pEvent = new JEvent(JEVT_LOGSRV_GET_CFG);
    if (pEvent)
    {
	    pEvent->SetFromProc(JS_P_JMAINPROC);
	    pEvent->SetFromThrd(JS_T_JLOGSRVAGENT);
	    pEvent->SetFromMod(JNULL);
	    pEvent->SetToProc(JS_P_JMAINPROC);
	    pEvent->SetToThrd(JS_T_JMAINTHREAD);
	    pEvent->SetToMod(JNULL);
	    pEvent->SetBody(JNULL);

        pThread = JSingleton<JThreadManager>::instance()->GetThread(JS_T_JLOGSRVAGENT);
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

    return JSUCCESS;
}

JUINT32 JLogServerAgent::GetLogSrvCfgRsp(JLogSrvCfg* pLogSrvCfg)
{
    JUINT32 uiRet = JFAILURE;
    JEvent* pEvent = JNULL;
    JSOCKADDR_IN stAddr;
    JEVT_TYPE pType = JEVT_NONE;
    JLogSrvCfg* pLogSrvCfgBody = JNULL;
    JUINT32 uiInterval = 0;
    JCHAR pBuf[JCOMM_MSG_BUF_LEN] = {0};
    JCommEngine* pCommEngine = JNULL;
    JListItem<JEvent>* pListItem = JNULL;

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_LOGSERVER_AGENT, "JLogServerAgent::GetLogSrvCfgRsp");

	if (!pLogSrvCfg)
	{
		return JFAILURE;
	}
	else
	{
		pLogSrvCfg->Clear();
	}

    while(1)
    {
        if (!uiInterval)
        {
            uiInterval = JLOGSERVER_AGENT_SELECT_TIME;
        }
        else
        {
            uiInterval = JLOGSERVER_AGENT_SELECT_TIME2;
        }

        pCommEngine = m_pAgentThread->GetCommEngineGroup().HasMessage(uiInterval);
        if (pCommEngine && pCommEngine == m_pAgentThread->GetNotifyCommEngine())
        {
            uiRet = pCommEngine->RecvMessage(pBuf, JCOMM_MSG_BUF_LEN, &stAddr);
            if (uiRet == 1 && SafeStrcmp(pBuf, "1") ==0)
            {
                pListItem = m_pAgentThread->DeQueueEvent();
                if (pListItem)
                {
                    pEvent = pListItem->GetData();
                    if (pEvent)
                    {
                        pType = pEvent->GetEventType();
                        pLogSrvCfgBody = dynamic_cast<JLogSrvCfg*>(pEvent->GetBody());
						
                        if (pType == JEVT_LOGSRV_GET_CFG_RSP && pLogSrvCfgBody)
                        {
                        	pLogSrvCfg->SetListenAddr(pLogSrvCfgBody->GetListenAddr());
                        	pLogSrvCfg->SetListenPort(pLogSrvCfgBody->GetListenPort());
                        }
                    }

                    delete pListItem;
                }
            }
        }
        else
        {
            break;
        }
    }

    return JSUCCESS;
}

JUINT32 JLogServerAgent::CleanLog(JLogSrvNumber* pLogSrvNumber)
{
    JEvent* pEvent = JNULL;
    JLogSrvNumber* pLogSrvNumberBody = JNULL;
    JCommEngine* pComEngine = JNULL;
    JThread* pThread = JNULL;
    JAgentThread* pAgentThread = JNULL;

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_LOGSERVER_AGENT, "JLogServerAgent::CleanLog");

    pEvent = new JEvent(JEVT_LOGSRV_CLEAN_LOG);
    pLogSrvNumberBody = new JLogSrvNumber;
	if (pEvent && pLogSrvNumberBody)
	{
		pLogSrvNumberBody->SetSrvNumber(pLogSrvNumber->GetSrvNumber());

	    pEvent->SetFromProc(JS_P_JMAINPROC);
	    pEvent->SetFromThrd(JS_T_JLOGSRVAGENT);
	    pEvent->SetFromMod(JNULL);
	    pEvent->SetToProc(JS_P_JMAINPROC);
	    pEvent->SetToThrd(JS_T_JMAINTHREAD);
	    pEvent->SetToMod(JNULL);
	    pEvent->SetBody(pLogSrvNumberBody);

        pThread = JSingleton<JThreadManager>::instance()->GetThread(JS_T_JLOGSRVAGENT);
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

    return JSUCCESS;
}

JUINT32 JLogServerAgent::GetHasNewMsg(JLogSrvNumber* pLogSrvNumber)
{
    JEvent* pEvent = JNULL;
    JLogSrvNumber* pLogSrvNumberBody = JNULL;
    JCommEngine* pComEngine = JNULL;
    JThread* pThread = JNULL;
    JAgentThread* pAgentThread = JNULL;

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_LOGSERVER_AGENT, "JLogServerAgent::GetHasNewMsg");

    pEvent = new JEvent(JEVT_LOGSRV_GET_HAS_NEW_MSG);
    pLogSrvNumberBody = new JLogSrvNumber;
	if (pEvent && pLogSrvNumberBody)
	{
		pLogSrvNumberBody->SetSrvNumber(pLogSrvNumber->GetSrvNumber());

	    pEvent->SetFromProc(JS_P_JMAINPROC);
	    pEvent->SetFromThrd(JS_T_JLOGSRVAGENT);
	    pEvent->SetFromMod(JNULL);
	    pEvent->SetToProc(JS_P_JMAINPROC);
	    pEvent->SetToThrd(JS_T_JMAINTHREAD);
	    pEvent->SetToMod(JNULL);
	    pEvent->SetBody(pLogSrvNumberBody);

        pThread = JSingleton<JThreadManager>::instance()->GetThread(JS_T_JLOGSRVAGENT);
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

    return JSUCCESS;
}

JUINT32 JLogServerAgent::GetHasNewMsgRsp(JLogSrvHasNewMsg* pLogSrvHasNewMsg)
{
    JUINT32 uiIdx = 0;
    JUINT32 uiRet = JFAILURE;
    JEvent* pEvent = JNULL;
    JSOCKADDR_IN stAddr;
    JEVT_TYPE pType = JEVT_NONE;
    JLogSrvHasNewMsg* pLogSrvHasNewMsgBody = JNULL;
    JUINT32 uiInterval = 0;
    JCHAR pBuf[JCOMM_MSG_BUF_LEN] = {0};
    JCommEngine* pCommEngine = JNULL;
    JListItem<JEvent>* pListItem = JNULL;

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_LOGSERVER_AGENT, "JLogServerAgent::GetHasNewMsgRsp");

	if (!pLogSrvHasNewMsg)
	{
		return JFAILURE;
	}
	else
	{
		pLogSrvHasNewMsg->Clear();
	}

    while(1)
    {
        if (!uiInterval)
        {
            uiInterval = JLOGSERVER_AGENT_SELECT_TIME;
        }
        else
        {
            uiInterval = JLOGSERVER_AGENT_SELECT_TIME2;
        }

        pCommEngine = m_pAgentThread->GetCommEngineGroup().HasMessage(uiInterval);
        if (pCommEngine && pCommEngine == m_pAgentThread->GetNotifyCommEngine())
        {
            uiRet = pCommEngine->RecvMessage(pBuf, JCOMM_MSG_BUF_LEN, &stAddr);
            if (uiRet == 1 && SafeStrcmp(pBuf, "1") ==0)
            {
                pListItem = m_pAgentThread->DeQueueEvent();
                if (pListItem)
                {
                    pEvent = pListItem->GetData();
                    if (pEvent)
                    {
                        pType = pEvent->GetEventType();
                        pLogSrvHasNewMsgBody = dynamic_cast<JLogSrvHasNewMsg*>(pEvent->GetBody());

                        if (pType == JEVT_LOGSRV_GET_HAS_NEW_MSG_RSP && pLogSrvHasNewMsgBody)
                        {
                        	pLogSrvHasNewMsg->SetHasNewMsg(pLogSrvHasNewMsgBody->GetHasNewMsg());
                            for (uiIdx=0; uiIdx<JLOGSRV_MAX_MSG_SRV; uiIdx++)
                            {
                            	pLogSrvHasNewMsg->SetRmtAddr(
                            	    pLogSrvHasNewMsgBody->GetRmtAddr(uiIdx), uiIdx);
                        	}
                        }
                    }

                    delete pListItem;
                }
            }
        }
        else
        {
            break;
        }
    }

    return JSUCCESS;
}

JUINT32 JLogServerAgent::GetLogMsg(JCHAR* currLogSrv, 
                                    JCHAR* logMessage, 
                                    JUINT32 uiMaxLen, 
                                    JUINT32* pOffset)
{
    JCHAR strThrdName[JMAX_STRING_LEN] = {0};
    JThreadManager* pThreadManager = JSingleton<JThreadManager>::instance();
    JThread* pThread = JNULL;
    JLogMsgThread* pLogMsgThread = JNULL;
    JUINT32 uiLen = 0;

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_LOGSERVER_AGENT, "JLogServerAgent::GetLogMsg");

    SafeStrcpy(strThrdName, JS_T_JLOGMSG_PREFIX, JMAX_STRING_LEN);
    uiLen = SafeStrlen(currLogSrv);
    SafeStrncat(strThrdName, currLogSrv, uiLen, JMAX_STRING_LEN);

    pThread = pThreadManager->GetThread(strThrdName);
    if (pThread)
    {
        pLogMsgThread = dynamic_cast<JLogMsgThread*>(pThread);
        if (pLogMsgThread)
        {
            pLogMsgThread->GetLogMsg(logMessage, uiMaxLen, pOffset);
        }
    }

    return JSUCCESS;
}

