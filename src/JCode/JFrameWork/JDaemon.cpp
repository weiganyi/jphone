/**********************************************************
* Author: weiganyi
* File: JDaemon.cpp
* Date: 20121108
* Description: 
*
**********************************************************/

#include "JFrameWork.h"


namespace JFrameWork{

JDaemonCfg::JDaemonCfg()
{
	return;
}

JDaemonCfg::~JDaemonCfg()
{
	return;
}

JUINT32 JDaemonCfg::GetLength()
{
	return m_strSaveMethod.GetLength()+sizeof(JUINT32);
}

JEventBody* JDaemonCfg::Clone()
{
	JDaemonCfg* pDaemonCfg = JNULL;

	pDaemonCfg = new JDaemonCfg();
	if (pDaemonCfg)
	{
		pDaemonCfg->SetSaveMethod(m_strSaveMethod);
	}

	return pDaemonCfg;
}

JUINT32 JDaemonCfg::Clear()
{
	m_strSaveMethod.Clear();

	return JSUCCESS;
}

JUINT32 JDaemonCfg::Serialize(JCHAR* pBuf, JUINT32 uiMaxNum)
{
	JUINT32 uiLen = 0;
	JUINT32* pLen = 0;
	JCHAR* pOffset = JNULL;
	JUINT32 tmpMaxNum = 0;

	if (!pBuf || !uiMaxNum)
	{
        JSingleton<JLog>::instance2() << set(JLOG_MOD_DAEMON, JLOG_ERROR_LEVEL) 
            << "JDaemonCfg::Serialize input param is NULL\n";
        return JFAILURE;
	}

	pOffset = pBuf;
	tmpMaxNum = uiMaxNum;

	uiLen = m_strSaveMethod.GetLength();
	pLen = reinterpret_cast<JUINT32*>(pOffset);
	*pLen = uiLen;
	pOffset += sizeof(JUINT32);
	tmpMaxNum -= sizeof(JUINT32);
	if (uiLen)
	{
		SafeMemcpy(pOffset, m_strSaveMethod.c_str(), uiLen, tmpMaxNum);

		pOffset += uiLen;
		tmpMaxNum -= uiLen;
	}

	return JSUCCESS;
}

JUINT32 JDaemonCfg::DeSerialize(JCHAR* pBuf)
{
	JUINT32* pLen = 0;
	JCHAR* pOffset = JNULL;
	JCHAR strBuffer[JMAX_STRING_LEN] = {0};

	if (!pBuf)
	{
        JSingleton<JLog>::instance2() << set(JLOG_MOD_DAEMON, JLOG_ERROR_LEVEL) 
            << "JDaemonCfg::DeSerialize input param is NULL\n";
        return JFAILURE;
	}

	pOffset = pBuf;

	pLen = reinterpret_cast<JUINT32*>(pOffset);
	pOffset += sizeof(JUINT32);
	if (*pLen)
	{
		SafeStrncpy(strBuffer, pOffset, *pLen, JMAX_STRING_LEN);
		m_strSaveMethod = strBuffer;
		pOffset += *pLen;
	}

	return JSUCCESS;
}

JUINT32 JDaemonCfg::operator=(JDaemonCfg& rDaemonCfg)
{
	m_strSaveMethod = rDaemonCfg.GetSaveMethod();

	return JSUCCESS;
}

JString& JDaemonCfg::GetSaveMethod()
{
	return m_strSaveMethod;
}

JUINT32 JDaemonCfg::SetSaveMethod(JString& rStr)
{
	m_strSaveMethod = rStr;

	return JSUCCESS;
}

JDaemon::JDaemon()
{
    m_pSerialization = new JSerialization(JSER_PERSISTENCE_INI);

    if (m_pSerialization)
    {
        GetCfgList(&m_cfg);
    }

	return;
}

JDaemon::~JDaemon()
{
    if (m_pSerialization)
    {
        delete m_pSerialization;
    }

	return;
}

JUINT32 JDaemon::InitFunc()
{
    return JSUCCESS;
}

JSER_PERSISTENCE_TYPE JDaemon::GetCfgSaveMethod()
{
    if (m_cfg.GetSaveMethod() == JDAEMON_SAVE_INI_ENABLED)
    {
        return JSER_PERSISTENCE_INI;
    }
    else if (m_cfg.GetSaveMethod() == JDAEMON_SAVE_SQL_ENABLED)
    {
        return JSER_PERSISTENCE_SQL;
    }
    else if (m_cfg.GetSaveMethod() == JDAEMON_SAVE_XML_ENABLED)
    {
        return JSER_PERSISTENCE_XML;
    }

    return JSER_PERSISTENCE_NONE;
}

JUINT32 JDaemon::EventProcFunc(JEvent* pEvent)
{
    JUINT32 uiRet = JSUCCESS;
    JEVT_TYPE type = JEVT_NONE;
    JDaemonCfg* pDaemonCfg = JNULL;

    JAutoPtr<JLock> clsLockAutoPtr(&m_Lock);

    if (!pEvent)
    {
        JSingleton<JLog>::instance2() << set(JLOG_MOD_DAEMON, JLOG_ERROR_LEVEL) 
            << "JDaemon::EventProcFunc input param is NULL\n";
        return JFAILURE;
    }

    type = pEvent->GetEventType();
    switch(type)
    {
        case JEVT_DAEMON_SET_CFG:
        {
            pDaemonCfg = dynamic_cast<JDaemonCfg*>(pEvent->GetBody());
            if (pDaemonCfg)
            {
                uiRet = ProcSetCfgEvent(pDaemonCfg);
            }
        }
        break;

        case JEVT_DAEMON_GET_CFG:
        {
            uiRet = ProcGetCfgEvent(pEvent);
        }
        break;

        default:
        break;
    };

	return uiRet;
}

JEventBody* JDaemon::MakeEventBody(JUINT32 uiType)
{
	JEventBody* pEventBody = JNULL;

	switch(uiType)
	{
		case JEVT_DAEMON_SET_CFG:
		case JEVT_DAEMON_GET_CFG_RSP:
			pEventBody = new JDaemonCfg;
			break;

		default:
			break;
	};

	return pEventBody;
}

JUINT32 JDaemon::ProcSetCfgEvent(JDaemonCfg* pDaemonCfg)
{
    JBOOL bChanged = JFALSE;

    if (!pDaemonCfg)
    {
        JSingleton<JLog>::instance2() << set(JLOG_MOD_DAEMON, JLOG_ERROR_LEVEL) 
            << "JDaemon::ProcSetCfgEvent pDaemonCfg is NULL\n";
        return JFAILURE;
    }

    if (pDaemonCfg->GetSaveMethod() != m_cfg.GetSaveMethod())
    {
    	m_cfg.SetSaveMethod(pDaemonCfg->GetSaveMethod());
        bChanged = JTRUE;
    }

    if (bChanged)
    {
        SetCfgList(&m_cfg);
    }

	return JSUCCESS;
}

JUINT32 JDaemon::ProcGetCfgEvent(JEvent* pEvent)
{
    JEvent* pNewEvent = JNULL;
    JDaemonCfg* pDaemonCfg = JNULL;
    JCommEngine* pComEngine = JNULL;
    JThread* pThread = JNULL;
    JModuleThread* pModuleThread = JNULL;

    pNewEvent = new JEvent(JEVT_DAEMON_GET_CFG_RSP);
    pDaemonCfg = new JDaemonCfg;
	if (pNewEvent && pDaemonCfg)
	{
	    pDaemonCfg->SetSaveMethod(m_cfg.GetSaveMethod());

	    pNewEvent->SetFromProc(pEvent->GetToProc().c_str());
	    pNewEvent->SetFromThrd(pEvent->GetToThrd().c_str());
	    pNewEvent->SetFromMod(pEvent->GetToMod().c_str());
	    pNewEvent->SetToProc(pEvent->GetFromProc().c_str());
	    pNewEvent->SetToThrd(pEvent->GetFromThrd().c_str());
	    pNewEvent->SetToMod(pEvent->GetFromMod().c_str());
	    pNewEvent->SetBody(pDaemonCfg);

        pThread = JSingleton<JThreadManager>::instance()->GetThread(JS_T_JMAINTHREAD);
        pModuleThread = dynamic_cast<JModuleThread*>(pThread);
        if (pModuleThread)
        {
    	    pComEngine = pModuleThread->GetNotifyCommEngine();
    	    if (pComEngine)
    	    {
    	        pComEngine->SendEvent(pNewEvent);
    	    }
	    }
    }

	return JSUCCESS;
}

JUINT32 JDaemon::SetCfgList(JDaemonCfg* pDaemonCfg)
{
    JUINT32 uiLen = 0;
    JList<JPER_RECORD> clsDstList;
    JListItem<JPER_RECORD>* pDstItem = JNULL;
    JListItem<JPER_RECORD>* prevDstItem = JNULL;
    JPER_RECORD* pDstData = JNULL;

    if (m_pSerialization)
    {
        uiLen = pDaemonCfg->GetSaveMethod().GetLength();
        if (uiLen)
        {
            pDstData = 
                reinterpret_cast<JPER_RECORD*>(JSingleton<JStaticMemory>::instance()->Alloc(sizeof(JPER_RECORD)+1));
            if (pDstData)
            {
	            SafeMemset(reinterpret_cast<JCHAR*>(pDstData), 0, sizeof(JPER_RECORD)+1);
                SafeSprintf(pDstData->strKey, JMAX_STRING_LEN, "%s", JDAEMON_SAVE_METHOD);
                SafeStrcpy(pDstData->strValue, pDaemonCfg->GetSaveMethod().c_str(), uiLen+1);
                pDstItem = new JListItem<JPER_RECORD>(pDstData);
                pDstItem->SetDataLength(sizeof(JPER_RECORD));
                clsDstList.InsertItem(pDstItem, prevDstItem);
                prevDstItem = pDstItem;
            }
            else
            {
                JSingleton<JLog>::instance2() << set(JLOG_MOD_DAEMON, JLOG_ERROR_LEVEL) 
                    << "JDaemon::SetCfgList memory alloc failure\n";
                return JFAILURE;
            }
        }

        m_pSerialization->SetList(clsDstList, JDAEMON_DEFAULT_FILE);
    }

	return JSUCCESS;
}

JUINT32 JDaemon::GetCfgList(JDaemonCfg* pDaemonCfg)
{
    JUINT32 uiLen = 0;
    JUINT32 uiLen2 = 0;
    JList<JPER_RECORD> clsList;
    JListItem<JPER_RECORD>* pItem = JNULL;
    JPER_RECORD* pData = JNULL;

    if (m_pSerialization)
    {
        clsList = m_pSerialization->GetList(JDAEMON_DEFAULT_FILE);

        JListIterator<JPER_RECORD> clsListIter(clsList);
        for (clsListIter.First(); clsListIter.Done(); clsListIter.Next())
        {
            pItem = clsListIter.Item();
            pData = pItem->GetData();

            uiLen = SafeStrlen(pData->strKey);
            uiLen2 = SafeStrlen(pData->strValue);
            if (uiLen && uiLen2)
            {
                if (SafeStrcmp(pData->strKey, JDAEMON_SAVE_METHOD) == 0)
                {
                    pDaemonCfg->GetSaveMethod() = pData->strValue;
                }
            }
        }
    }

	return JSUCCESS;
}

}

