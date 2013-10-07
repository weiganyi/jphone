/**********************************************************
* Author: weiganyi
* File: JPhoneAgent.cpp
* Date: 20120424
* Description: 
*
**********************************************************/

#include "JFrameWork.h"

using namespace JFrameWork;

#include "JSipUa.h"
#include "JPhoneAgent.h"


JPhoneAgent::JPhoneAgent()
{
    JLog* pLog = JNULL;
    JStaticMemory* pStaticMemory = JNULL;
    JRoute* pRoute = JNULL;
	JTHRD_PUB_DATA thrdPubData;
    JString strThrdName;
    JString strLocalAddr;

    // create all object
    pLog = JSingleton<JLog>::instance();
    pStaticMemory = JSingleton<JStaticMemory>::instance();
    pRoute = JSingleton<JRoute>::instance();

    // set log level
    pLog->SetLogLevel(JLOG_MOD_ALL, JLOG_DEBUG_LEVEL);
    //ptrLog->SetLogLevel(JLOG_MOD_ALL, JLOG_ENTER_LEVEL);

    // set log output method
    pLog->SetLogMethod(JLOG_OUTPUT_LOCAL);

    //create JAgentThread and register into JThreadManager
    strThrdName = JS_T_JPHONEAGENT;
    strLocalAddr = JPHONE_AGENT_LOCAL_ADDR;
	m_pAgentThread = new JAgentThread(&strThrdName, &strLocalAddr, JPHONE_AGENT_LOCAL_PORT);

    JThreadManager* pThreadManager = JSingleton<JThreadManager>::instance();
    thrdPubData.strThrdName = JS_T_JPHONEAGENT;
    thrdPubData.pThread = m_pAgentThread;
    pThreadManager->RegisterThread(thrdPubData);

	return;
}

JPhoneAgent::~JPhoneAgent()
{
    //destroy all object
    if (m_pAgentThread)
    {
        delete m_pAgentThread;
    }

	return;
}

JUINT32 JPhoneAgent::PressKey(JSIPUA_KEY eKey)
{
    JEvent* pEvent = JNULL;
    JPjSipUaKey* pJPjSipUaKeyBody = JNULL;
    JCommEngine* pComEngine = JNULL;
    JThread* pThread = JNULL;
    JAgentThread* pAgentThread = JNULL;

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_PHONE_AGENT, "JPhoneAgent::PressKey");

    pEvent = new JEvent(JEVT_SIPUA_PRESS_KEY);
    pJPjSipUaKeyBody = new JPjSipUaKey;
	if (pEvent && pJPjSipUaKeyBody)
	{
    	JCHAR cKey[2] = {0};
		cKey[0] = JPjSipUa::KeyEnum2Char(eKey);
		JString strKey = cKey;
		//store the press key
		pJPjSipUaKeyBody->SetKey(strKey);

	    pEvent->SetFromProc(JS_P_JMAINPROC);
	    pEvent->SetFromThrd(JS_T_JPHONEAGENT);
	    pEvent->SetFromMod(JNULL);
	    pEvent->SetToProc(JS_P_JMAINPROC);
	    pEvent->SetToThrd(JS_T_JMAINTHREAD);
	    pEvent->SetToMod(JS_M_JSIPUA);
	    pEvent->SetBody(pJPjSipUaKeyBody);

        pThread = JSingleton<JThreadManager>::instance()->GetThread(JS_T_JPHONEAGENT);
        pAgentThread = dynamic_cast<JAgentThread*>(pThread);
        if (pAgentThread)
        {
        	//get the communication engine to send the event
    	    pComEngine = pAgentThread->GetNotifyCommEngine();
    	    if (pComEngine)
    	    {
    	        pComEngine->SendEvent(pEvent);
    	    }
	    }
    }

    return JSUCCESS;
}

JUINT32 JPhoneAgent::ClickContact(JPjSipUaClickContact* pPjSipUaClickContact)
{
    JEvent* pEvent = JNULL;
    JPjSipUaClickContact* pPjSipUaClickContactBody = JNULL;
    JCommEngine* pComEngine = JNULL;
    JThread* pThread = JNULL;
    JAgentThread* pAgentThread = JNULL;

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_PHONE_AGENT, "JPhoneAgent::ClickContact");

    pEvent = new JEvent(JEVT_SIPUA_CLICK_CONTACT);
    pPjSipUaClickContactBody = new JPjSipUaClickContact;
	if (pEvent && pPjSipUaClickContactBody)
	{
		//store the number selected into the contact list
		pPjSipUaClickContactBody->SetContactNumber(pPjSipUaClickContact->GetContactNumber());

	    pEvent->SetFromProc(JS_P_JMAINPROC);
	    pEvent->SetFromThrd(JS_T_JPHONEAGENT);
	    pEvent->SetFromMod(JNULL);
	    pEvent->SetToProc(JS_P_JMAINPROC);
	    pEvent->SetToThrd(JS_T_JMAINTHREAD);
	    pEvent->SetToMod(JS_M_JSIPUA);
	    pEvent->SetBody(pPjSipUaClickContactBody);

        pThread = JSingleton<JThreadManager>::instance()->GetThread(JS_T_JPHONEAGENT);
        pAgentThread = dynamic_cast<JAgentThread*>(pThread);
        if (pAgentThread)
        {
        	//get the communication engine to send the event
    	    pComEngine = pAgentThread->GetNotifyCommEngine();
    	    if (pComEngine)
    	    {
    	        pComEngine->SendEvent(pEvent);
    	    }
	    }
    }

    return JSUCCESS;
}

JUINT32 JPhoneAgent::SetDaemonCfg(JDaemonCfg* pDaemonCfg)
{
    JEvent* pEvent = JNULL;
    JDaemonCfg* pDaemonCfgBody = JNULL;
    JCommEngine* pComEngine = JNULL;
    JThread* pThread = JNULL;
    JAgentThread* pAgentThread = JNULL;

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_PHONE_AGENT, "JPhoneAgent::SetDaemonCfg");

    pEvent = new JEvent(JEVT_DAEMON_SET_CFG);
    pDaemonCfgBody = new JDaemonCfg;
	if (pEvent && pDaemonCfg)
	{
		//set save methon into the body
		pDaemonCfgBody->SetSaveMethod(pDaemonCfg->GetSaveMethod());

	    pEvent->SetFromProc(JS_P_JMAINPROC);
	    pEvent->SetFromThrd(JS_T_JPHONEAGENT);
	    pEvent->SetFromMod(JNULL);
	    pEvent->SetToProc(JS_P_JMAINPROC);
	    pEvent->SetToThrd(JS_T_JMAINTHREAD);
	    pEvent->SetToMod(JS_M_JDAEMON);
	    pEvent->SetBody(pDaemonCfgBody);

        pThread = JSingleton<JThreadManager>::instance()->GetThread(JS_T_JPHONEAGENT);
        pAgentThread = dynamic_cast<JAgentThread*>(pThread);
        if (pAgentThread)
        {
        	//get the communication engine to send the event
    	    pComEngine = pAgentThread->GetNotifyCommEngine();
    	    if (pComEngine)
    	    {
    	        pComEngine->SendEvent(pEvent);
    	    }
	    }
    }

    return JSUCCESS;
}

JUINT32 JPhoneAgent::GetDaemonCfg()
{
    JEvent* pEvent = JNULL;
    JCommEngine* pComEngine = JNULL;
    JThread* pThread = JNULL;
    JAgentThread* pAgentThread = JNULL;

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_PHONE_AGENT, "JPhoneAgent::GetDaemonCfg");

    pEvent = new JEvent(JEVT_DAEMON_GET_CFG);
    if (pEvent)
    {
	    pEvent->SetFromProc(JS_P_JMAINPROC);
	    pEvent->SetFromThrd(JS_T_JPHONEAGENT);
	    pEvent->SetFromMod(JNULL);
	    pEvent->SetToProc(JS_P_JMAINPROC);
	    pEvent->SetToThrd(JS_T_JMAINTHREAD);
	    pEvent->SetToMod(JS_M_JDAEMON);
	    pEvent->SetBody(JNULL);

        pThread = JSingleton<JThreadManager>::instance()->GetThread(JS_T_JPHONEAGENT);
        pAgentThread = dynamic_cast<JAgentThread*>(pThread);
        if (pAgentThread)
        {
        	//get the communication engine to send the event
    	    pComEngine = pAgentThread->GetNotifyCommEngine();
    	    if (pComEngine)
    	    {
    	        pComEngine->SendEvent(pEvent);
    	    }
	    }
    }

    return JSUCCESS;
}

JUINT32 JPhoneAgent::GetDaemonCfgRsp(JDaemonCfg* pDaemonCfg)
{
    JUINT32 uiRet = JFAILURE;
    JEvent* pEvent = JNULL;
    JSOCKADDR_IN stAddr;
    JEVT_TYPE pType = JEVT_NONE;
    JDaemonCfg* pDaemonCfgBody = JNULL;
    JUINT32 uiInterval = 0;
    JCHAR pBuf[JCOMM_MSG_BUF_LEN] = {0};
    JCommEngine* pCommEngine = JNULL;
    JListItem<JEvent>* pListItem = JNULL;

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_PHONE_AGENT, "JPhoneAgent::GetDaemonCfgRsp");

	if (!pDaemonCfg)
	{
		return JFAILURE;
	}
	else
	{
		pDaemonCfg->Clear();
	}

    while(1)
    {
    	//first select time is 500ms, the second is 100ms, because normally the response
    	//be received in the 500ms after request be sent, so it will make it more likely to 
    	//receive response at the first select loop.
        if (!uiInterval)
        {
            uiInterval = JPHONE_AGENT_SELECT_TIME;
        }
        else
        {
            uiInterval = JPHONE_AGENT_SELECT_TIME2;
        }

		//if response event be received, first a notify message "1" be received
		//the get event for the thread queue, because it's through queue read/write
		//while communication happens between two threads of one process.
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
                        pDaemonCfgBody = dynamic_cast<JDaemonCfg*>(pEvent->GetBody());

						//check the response and save the data
                        if (pType == JEVT_DAEMON_GET_CFG_RSP && pDaemonCfgBody)
                        {
                        	pDaemonCfg->SetSaveMethod(pDaemonCfgBody->GetSaveMethod());
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

JUINT32 JPhoneAgent::SetSipUaCfg(JPjSipUaCfg* pPjSipUaCfg)
{
    JEvent* pEvent = JNULL;
    JPjSipUaCfg* pPjSipUaCfgBody = JNULL;
    JCommEngine* pComEngine = JNULL;
    JThread* pThread = JNULL;
    JAgentThread* pAgentThread = JNULL;

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_PHONE_AGENT, "JPhoneAgent::SetSipUaCfg");

    pEvent = new JEvent(JEVT_SIPUA_SET_CFG);
    pPjSipUaCfgBody = new JPjSipUaCfg;
	if (pEvent && pPjSipUaCfgBody)
	{
		//set account info into the body
		pPjSipUaCfgBody->SetNumber(pPjSipUaCfg->GetNumber());
		pPjSipUaCfgBody->SetAuthName(pPjSipUaCfg->GetAuthName());
		pPjSipUaCfgBody->SetAuthPasswd(pPjSipUaCfg->GetAuthPasswd());
		pPjSipUaCfgBody->SetProxyAddr(pPjSipUaCfg->GetProxyAddr());

	    pEvent->SetFromProc(JS_P_JMAINPROC);
	    pEvent->SetFromThrd(JS_T_JPHONEAGENT);
	    pEvent->SetFromMod(JNULL);
	    pEvent->SetToProc(JS_P_JMAINPROC);
	    pEvent->SetToThrd(JS_T_JMAINTHREAD);
	    pEvent->SetToMod(JS_M_JSIPUA);
	    pEvent->SetBody(pPjSipUaCfgBody);

        pThread = JSingleton<JThreadManager>::instance()->GetThread(JS_T_JPHONEAGENT);
        pAgentThread = dynamic_cast<JAgentThread*>(pThread);
        if (pAgentThread)
        {
        	//get the communication engine to send the event
    	    pComEngine = pAgentThread->GetNotifyCommEngine();
    	    if (pComEngine)
    	    {
    	        pComEngine->SendEvent(pEvent);
    	    }
	    }
    }

    return JSUCCESS;
}

JUINT32 JPhoneAgent::GetSipUaCfg()
{
    JEvent* pEvent = JNULL;
    JCommEngine* pComEngine = JNULL;
    JThread* pThread = JNULL;
    JAgentThread* pAgentThread = JNULL;

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_PHONE_AGENT, "JPhoneAgent::GetSipUaCfg");

    pEvent = new JEvent(JEVT_SIPUA_GET_CFG);
    if (pEvent)
    {
	    pEvent->SetFromProc(JS_P_JMAINPROC);
	    pEvent->SetFromThrd(JS_T_JPHONEAGENT);
	    pEvent->SetFromMod(JNULL);
	    pEvent->SetToProc(JS_P_JMAINPROC);
	    pEvent->SetToThrd(JS_T_JMAINTHREAD);
	    pEvent->SetToMod(JS_M_JSIPUA);
	    pEvent->SetBody(JNULL);

        pThread = JSingleton<JThreadManager>::instance()->GetThread(JS_T_JPHONEAGENT);
        pAgentThread = dynamic_cast<JAgentThread*>(pThread);
        if (pAgentThread)
        {
        	//get the communication engine to send the event
    	    pComEngine = pAgentThread->GetNotifyCommEngine();
    	    if (pComEngine)
    	    {
    	        pComEngine->SendEvent(pEvent);
    	    }
	    }
    }

    return JSUCCESS;
}

JUINT32 JPhoneAgent::GetSipUaCfgRsp(JPjSipUaCfg* pPjSipUaCfg)
{
    JUINT32 uiRet = JFAILURE;
    JEvent* pEvent = JNULL;
    JSOCKADDR_IN stAddr;
    JEVT_TYPE pType = JEVT_NONE;
    JPjSipUaCfg* pPjSipUaCfgBody = JNULL;
    JUINT32 uiInterval = 0;
    JCHAR pBuf[JCOMM_MSG_BUF_LEN] = {0};
    JCommEngine* pCommEngine = JNULL;
    JListItem<JEvent>* pListItem = JNULL;

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_PHONE_AGENT, "JPhoneAgent::GetSipUaCfgRsp");

	if (!pPjSipUaCfg)
	{
		return JFAILURE;
	}
	else
	{
		pPjSipUaCfg->Clear();
	}

    while(1)
    {
        if (!uiInterval)
        {
            uiInterval = JPHONE_AGENT_SELECT_TIME;
        }
        else
        {
            uiInterval = JPHONE_AGENT_SELECT_TIME2;
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
                        pPjSipUaCfgBody = dynamic_cast<JPjSipUaCfg*>(pEvent->GetBody());

						//check the response and save the data
                        if (pType == JEVT_SIPUA_GET_CFG_RSP && pPjSipUaCfgBody)
                        {
                        	pPjSipUaCfg->SetNumber(pPjSipUaCfgBody->GetNumber());
                        	pPjSipUaCfg->SetAuthName(pPjSipUaCfgBody->GetAuthName());
                        	pPjSipUaCfg->SetAuthPasswd(pPjSipUaCfgBody->GetAuthPasswd());
                        	pPjSipUaCfg->SetProxyAddr(pPjSipUaCfgBody->GetProxyAddr());
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

JUINT32 JPhoneAgent::SetLogCfg(JLogCfg* pLogCfg)
{
    JEvent* pEvent = JNULL;
    JLogCfg* pLogCfgBody = JNULL;
    JCommEngine* pComEngine = JNULL;
    JThread* pThread = JNULL;
    JAgentThread* pAgentThread = JNULL;

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_PHONE_AGENT, "JPhoneAgent::SetLogCfg");

    pEvent = new JEvent(JEVT_LOG_SET_CFG);
    pLogCfgBody = new JLogCfg;
	if (pEvent && pLogCfgBody)
	{
		//set log config into the body
		pLogCfgBody->SetLogAddress(pLogCfg->GetLogAddress());
		pLogCfgBody->SetLogPort(pLogCfg->GetLogPort());
		pLogCfgBody->SetOutputFile(pLogCfg->GetOutputFile());
		pLogCfgBody->SetOutputRemote(pLogCfg->GetOutputRemote());

	    pEvent->SetFromProc(JS_P_JMAINPROC);
	    pEvent->SetFromThrd(JS_T_JPHONEAGENT);
	    pEvent->SetFromMod(JNULL);
	    pEvent->SetToProc(JS_P_JMAINPROC);
	    pEvent->SetToThrd(JS_T_JMAINTHREAD);
	    pEvent->SetToMod(JS_M_JLOG);
	    pEvent->SetBody(pLogCfgBody);

        pThread = JSingleton<JThreadManager>::instance()->GetThread(JS_T_JPHONEAGENT);
        pAgentThread = dynamic_cast<JAgentThread*>(pThread);
        if (pAgentThread)
        {
        	//get the communication engine to send the event
    	    pComEngine = pAgentThread->GetNotifyCommEngine();
    	    if (pComEngine)
    	    {
    	        pComEngine->SendEvent(pEvent);
    	    }
	    }
    }

    return JSUCCESS;
}

JUINT32 JPhoneAgent::GetLogCfg()
{
    JEvent* pEvent = JNULL;
    JCommEngine* pComEngine = JNULL;
    JThread* pThread = JNULL;
    JAgentThread* pAgentThread = JNULL;

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_PHONE_AGENT, "JPhoneAgent::GetLogCfg");

    pEvent = new JEvent(JEVT_LOG_GET_CFG);
    if (pEvent)
    {
	    pEvent->SetFromProc(JS_P_JMAINPROC);
	    pEvent->SetFromThrd(JS_T_JPHONEAGENT);
	    pEvent->SetFromMod(JNULL);
	    pEvent->SetToProc(JS_P_JMAINPROC);
	    pEvent->SetToThrd(JS_T_JMAINTHREAD);
	    pEvent->SetToMod(JS_M_JLOG);
	    pEvent->SetBody(JNULL);

        pThread = JSingleton<JThreadManager>::instance()->GetThread(JS_T_JPHONEAGENT);
        pAgentThread = dynamic_cast<JAgentThread*>(pThread);
        if (pAgentThread)
        {
        	//get the communication engine to send the event
    	    pComEngine = pAgentThread->GetNotifyCommEngine();
    	    if (pComEngine)
    	    {
    	        pComEngine->SendEvent(pEvent);
    	    }
	    }
    }

    return JSUCCESS;
}

JUINT32 JPhoneAgent::GetLogCfgRsp(JLogCfg* pLogCfg)
{
    JUINT32 uiRet = JFAILURE;
    JEvent* pEvent = JNULL;
    JSOCKADDR_IN stAddr;
    JEVT_TYPE pType = JEVT_NONE;
    JLogCfg* pLogCfgBody = JNULL;
    JUINT32 uiInterval = 0;
    JCHAR pBuf[JCOMM_MSG_BUF_LEN] = {0};
    JCommEngine* pCommEngine = JNULL;
    JListItem<JEvent>* pListItem = JNULL;

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_PHONE_AGENT, "JPhoneAgent::GetLogCfgRsp");

	if (!pLogCfg)
	{
		return JFAILURE;
	}
	else
	{
		pLogCfg->Clear();
	}

    while(1)
    {
        if (!uiInterval)
        {
            uiInterval = JPHONE_AGENT_SELECT_TIME;
        }
        else
        {
            uiInterval = JPHONE_AGENT_SELECT_TIME2;
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
                        pLogCfgBody = dynamic_cast<JLogCfg*>(pEvent->GetBody());

						//check the response and save the data
                        if (pType == JEVT_LOG_GET_CFG_RSP && pLogCfgBody)
                        {
                        	pLogCfg->SetLogAddress(pLogCfgBody->GetLogAddress());
                        	pLogCfg->SetLogPort(pLogCfgBody->GetLogPort());
                        	pLogCfg->SetOutputFile(pLogCfgBody->GetOutputFile());
                        	pLogCfg->SetOutputRemote(pLogCfgBody->GetOutputRemote());
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

JUINT32 JPhoneAgent::GetStatus()
{
    JEvent* pEvent = JNULL;
    JCommEngine* pComEngine = JNULL;
    JThread* pThread = JNULL;
    JAgentThread* pAgentThread = JNULL;

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_PHONE_AGENT, "JPhoneAgent::GetStatus");

    pEvent = new JEvent(JEVT_SIPUA_GET_CALL_STATUS);
    if (pEvent)
    {
	    pEvent->SetFromProc(JS_P_JMAINPROC);
	    pEvent->SetFromThrd(JS_T_JPHONEAGENT);
	    pEvent->SetFromMod(JNULL);
	    pEvent->SetToProc(JS_P_JMAINPROC);
	    pEvent->SetToThrd(JS_T_JMAINTHREAD);
	    pEvent->SetToMod(JS_M_JSIPUA);
	    pEvent->SetBody(JNULL);

        pThread = JSingleton<JThreadManager>::instance()->GetThread(JS_T_JPHONEAGENT);
        pAgentThread = dynamic_cast<JAgentThread*>(pThread);
        if (pAgentThread)
        {
        	//get the communication engine to send the event
    	    pComEngine = pAgentThread->GetNotifyCommEngine();
    	    if (pComEngine)
    	    {
    	        pComEngine->SendEvent(pEvent);
    	    }
	    }
    }

    return JSUCCESS;
}

JUINT32 JPhoneAgent::GetStatusRsp(JPjSipUaCallStatus* pPjSipUaCallStatus)
{
    JUINT32 uiRet = JFAILURE;
    JEvent* pEvent = JNULL;
    JSOCKADDR_IN stAddr;
    JEVT_TYPE pType = JEVT_NONE;
    JPjSipUaCallStatus* pPjSipUaCallStatusBody = JNULL;
    JUINT32 uiInterval = 0;
    JCHAR pBuf[JCOMM_MSG_BUF_LEN] = {0};
    JCommEngine* pCommEngine = JNULL;
    JListItem<JEvent>* pListItem = JNULL;

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_PHONE_AGENT, "JPhoneAgent::GetStatusRsp");

	if (!pPjSipUaCallStatus)
	{
		return JFAILURE;
	}
	else
	{
		pPjSipUaCallStatus->Clear();
	}

    while(1)
    {
        if (!uiInterval)
        {
            uiInterval = JPHONE_AGENT_SELECT_TIME;
        }
        else
        {
            uiInterval = JPHONE_AGENT_SELECT_TIME2;
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
                        pPjSipUaCallStatusBody = dynamic_cast<JPjSipUaCallStatus*>(pEvent->GetBody());

						//check the response and save the data
                        if (pType == JEVT_SIPUA_GET_CALL_STATUS_RSP && pPjSipUaCallStatusBody)
                        {
                        	pPjSipUaCallStatus->SetNumber(pPjSipUaCallStatusBody->GetNumber());
                        	pPjSipUaCallStatus->SetStatus(pPjSipUaCallStatusBody->GetStatus());
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

JUINT32 JPhoneAgent::GetContact()
{
    JEvent* pEvent = JNULL;
    JCommEngine* pComEngine = JNULL;
    JThread* pThread = JNULL;
    JAgentThread* pAgentThread = JNULL;

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_PHONE_AGENT, "JPhoneAgent::GetContact");

    pEvent = new JEvent(JEVT_SIPUA_GET_CONTACT_LIST);
    if (pEvent)
    {
	    pEvent->SetFromProc(JS_P_JMAINPROC);
	    pEvent->SetFromThrd(JS_T_JPHONEAGENT);
	    pEvent->SetFromMod(JNULL);
	    pEvent->SetToProc(JS_P_JMAINPROC);
	    pEvent->SetToThrd(JS_T_JMAINTHREAD);
	    pEvent->SetToMod(JS_M_JSIPUA);
	    pEvent->SetBody(JNULL);

        pThread = JSingleton<JThreadManager>::instance()->GetThread(JS_T_JPHONEAGENT);
        pAgentThread = dynamic_cast<JAgentThread*>(pThread);
        if (pAgentThread)
        {
        	//get the communication engine to send the event
    	    pComEngine = pAgentThread->GetNotifyCommEngine();
    	    if (pComEngine)
    	    {
    	        pComEngine->SendEvent(pEvent);
    	    }
	    }
    }

    return JSUCCESS;
}

JUINT32 JPhoneAgent::GetContactRsp(JPjSipUaContactList* pPjSipUaContactList)
{
    JUINT32 uiRet = JFAILURE;
    JEvent* pEvent = JNULL;
    JSOCKADDR_IN stAddr;
    JEVT_TYPE pType = JEVT_NONE;
    JPjSipUaContactList* pPjSipUaContactListBody = JNULL;
    JUINT32 uiInterval = 0;
    JUINT32 uiIdx = 0;
    JCHAR pBuf[JCOMM_MSG_BUF_LEN] = {0};
    JCommEngine* pCommEngine = JNULL;
    JListItem<JEvent>* pListItem = JNULL;

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_PHONE_AGENT, "JPhoneAgent::GetContactRsp");

	if (!pPjSipUaContactList)
	{
		return JFAILURE;
	}
	else
	{
		pPjSipUaContactList->Clear();
	}

    while(1)
    {
        if (!uiInterval)
        {
            uiInterval = JPHONE_AGENT_SELECT_TIME;
        }
        else
        {
            uiInterval = JPHONE_AGENT_SELECT_TIME2;
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
                        pPjSipUaContactListBody = dynamic_cast<JPjSipUaContactList*>(pEvent->GetBody());

						//check the response and save the data
                        if (pType == JEVT_SIPUA_GET_CONTACT_LIST_RSP && pPjSipUaContactListBody)
                        {
        					for (uiIdx=0; uiIdx<JPJSIP_MAX_NUMBER; uiIdx++)
        					{
        						if(!pPjSipUaContactListBody->GetContactNumber(uiIdx).GetLength())
        						{
        							break;
        						}

        	                	pPjSipUaContactList->SetContactNumber(uiIdx, 
        	                		pPjSipUaContactListBody->GetContactNumber(uiIdx));
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

JUINT32 JPhoneAgent::DumpMemory()
{
    JEvent* pEvent = JNULL;
    JCommEngine* pComEngine = JNULL;
    JThread* pThread = JNULL;
    JAgentThread* pAgentThread = JNULL;

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_PHONE_AGENT, "JPhoneAgent::DumpMemory");

    pEvent = new JEvent(JEVT_MEMORY_DUMP_MEMORY);
    if (pEvent)
    {
	    pEvent->SetFromProc(JS_P_JMAINPROC);
	    pEvent->SetFromThrd(JS_T_JPHONEAGENT);
	    pEvent->SetFromMod(JNULL);
	    pEvent->SetToProc(JS_P_JMAINPROC);
	    pEvent->SetToThrd(JS_T_JMAINTHREAD);
	    pEvent->SetToMod(JS_M_JMEMORY);
	    pEvent->SetBody(JNULL);

        pThread = JSingleton<JThreadManager>::instance()->GetThread(JS_T_JPHONEAGENT);
        pAgentThread = dynamic_cast<JAgentThread*>(pThread);
        if (pAgentThread)
        {
        	//get the communication engine to send the event
    	    pComEngine = pAgentThread->GetNotifyCommEngine();
    	    if (pComEngine)
    	    {
    	        pComEngine->SendEvent(pEvent);
    	    }
	    }
    }

    return JSUCCESS;
}

