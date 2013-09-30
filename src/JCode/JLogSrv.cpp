/**********************************************************
* Author: weiganyi
* File: JLogSrv.cpp
* Date: 20121129
* Description: 
*
**********************************************************/

#include "JFrameWork.h"

using namespace JFrameWork;

#include "JLogSrv.h"


JLogSrvCfg::JLogSrvCfg()
{
	return;
}

JLogSrvCfg::~JLogSrvCfg()
{
	return;
}

JUINT32 JLogSrvCfg::GetLength()
{
	JUINT32 uiLen = 0;

	uiLen += m_ListenAddr.GetLength()+sizeof(JUINT32);
	uiLen += m_ListenPort.GetLength()+sizeof(JUINT32);

	return uiLen;
}

JEventBody* JLogSrvCfg::Clone()
{
	JLogSrvCfg* pPjSipUaCfg = JNULL;

	pPjSipUaCfg = new JLogSrvCfg();
	if (pPjSipUaCfg)
	{
		pPjSipUaCfg->SetListenAddr(m_ListenAddr);
		pPjSipUaCfg->SetListenPort(m_ListenPort);
	}

	return pPjSipUaCfg;
}

JUINT32 JLogSrvCfg::Clear()
{
	m_ListenAddr.Clear();
	m_ListenPort.Clear();

	return JSUCCESS;
}

JUINT32 JLogSrvCfg::Serialize(JCHAR* pBuf, JUINT32 uiMaxNum)
{
	JUINT32 uiLen = 0;
	JUINT32* pLen = JNULL;
	JCHAR* pOffset = JNULL;
	JUINT32 tmpMaxNum = 0;

	if (!pBuf || !uiMaxNum)
	{
        JSingleton<JLog>::instance2() << set(JLOG_MOD_LOGSRV, JLOG_ERROR_LEVEL) 
            << "JLogSrvCfg::Serialize input param is NULL\n";
        return JFAILURE;
	}

	pOffset = pBuf;
	tmpMaxNum = uiMaxNum;

	uiLen = m_ListenAddr.GetLength();
	pLen = reinterpret_cast<JUINT32*>(pOffset);
	*pLen = uiLen;
	pOffset += sizeof(JUINT32);
	tmpMaxNum -= sizeof(JUINT32);
	if (uiLen)
	{
		SafeMemcpy(pOffset, m_ListenAddr.c_str(), uiLen, tmpMaxNum);

		pOffset += uiLen;
		tmpMaxNum -= uiLen;
	}

	uiLen = m_ListenPort.GetLength();
	pLen = reinterpret_cast<JUINT32*>(pOffset);
	*pLen = uiLen;
	pOffset += sizeof(JUINT32);
	tmpMaxNum -= sizeof(JUINT32);
	if (uiLen)
	{
		SafeMemcpy(pOffset, m_ListenPort.c_str(), uiLen, tmpMaxNum);

		pOffset += uiLen;
		tmpMaxNum -= uiLen;
	}

	return JSUCCESS;
}

JUINT32 JLogSrvCfg::DeSerialize(JCHAR* pBuf)
{
	JUINT32* pLen = 0;
	JCHAR* pOffset = JNULL;
	JCHAR strBuffer[JMAX_STRING_LEN] = {0};

	if (!pBuf)
	{
        JSingleton<JLog>::instance2() << set(JLOG_MOD_LOGSRV, JLOG_ERROR_LEVEL) 
            << "JLogSrvCfg::DeSerialize input param is NULL\n";
        return JFAILURE;
	}

	pOffset = pBuf;

	pLen = reinterpret_cast<JUINT32*>(pOffset);
	pOffset += sizeof(JUINT32);
	if (*pLen)
	{
		SafeStrncpy(strBuffer, pOffset, *pLen, JMAX_STRING_LEN);
		m_ListenAddr = strBuffer;
		pOffset += *pLen;
	}

	pLen = reinterpret_cast<JUINT32*>(pOffset);
	pOffset += sizeof(JUINT32);
	if (*pLen)
	{
		SafeStrncpy(strBuffer, pOffset, *pLen, JMAX_STRING_LEN);
		m_ListenPort = strBuffer;
		pOffset += *pLen;
	}

	return JSUCCESS;
}

JUINT32 JLogSrvCfg::operator=(JLogSrvCfg& rLogSrvCfg)
{
	m_ListenAddr = rLogSrvCfg.GetListenAddr();
	m_ListenPort = rLogSrvCfg.GetListenPort();

	return JSUCCESS;
}

JString& JLogSrvCfg::GetListenAddr()
{
	return m_ListenAddr;
}

JUINT32 JLogSrvCfg::SetListenAddr(JString& rStr)
{
	m_ListenAddr = rStr;

	return JSUCCESS;
}

JString& JLogSrvCfg::GetListenPort()
{
	return m_ListenPort;
}

JUINT32 JLogSrvCfg::SetListenPort(JString& rStr)
{
	m_ListenPort = rStr;

	return JSUCCESS;
}

JLogSrvNumber::JLogSrvNumber()
{
	return;
}

JLogSrvNumber::~JLogSrvNumber()
{
	return;
}

JUINT32 JLogSrvNumber::GetLength()
{
	JUINT32 uiLen = 0;

	uiLen += m_srvNumber.GetLength()+sizeof(JUINT32);

	return uiLen;
}

JEventBody* JLogSrvNumber::Clone()
{
	JLogSrvNumber* pLogSrvNumber = JNULL;

	pLogSrvNumber = new JLogSrvNumber();
	if (pLogSrvNumber)
	{
		pLogSrvNumber->SetSrvNumber(m_srvNumber);
	}

	return pLogSrvNumber;
}

JUINT32 JLogSrvNumber::Clear()
{
	m_srvNumber.Clear();

	return JSUCCESS;
}

JUINT32 JLogSrvNumber::Serialize(JCHAR* pBuf, JUINT32 uiMaxNum)
{
	JUINT32 uiLen = 0;
	JUINT32* pLen = JNULL;
	JCHAR* pOffset = JNULL;
	JUINT32 tmpMaxNum = 0;

	if (!pBuf || !uiMaxNum)
	{
        JSingleton<JLog>::instance2() << set(JLOG_MOD_LOGSRV, JLOG_ERROR_LEVEL) 
            << "JLogSrvNumber::Serialize input param is NULL\n";
        return JFAILURE;
	}

	pOffset = pBuf;
	tmpMaxNum = uiMaxNum;

	uiLen = m_srvNumber.GetLength();
	pLen = reinterpret_cast<JUINT32*>(pOffset);
	*pLen = uiLen;
	pOffset += sizeof(JUINT32);
	tmpMaxNum -= sizeof(JUINT32);
	if (uiLen)
	{
		SafeMemcpy(pOffset, m_srvNumber.c_str(), uiLen, tmpMaxNum);

		pOffset += uiLen;
		tmpMaxNum -= uiLen;
	}

	return JSUCCESS;
}

JUINT32 JLogSrvNumber::DeSerialize(JCHAR* pBuf)
{
	JUINT32* pLen = 0;
	JCHAR* pOffset = JNULL;
	JCHAR strBuffer[JMAX_STRING_LEN] = {0};

	if (!pBuf)
	{
        JSingleton<JLog>::instance2() << set(JLOG_MOD_LOGSRV, JLOG_ERROR_LEVEL) 
            << "JLogSrvNumber::DeSerialize input param is NULL\n";
        return JFAILURE;
	}

	pOffset = pBuf;

	pLen = reinterpret_cast<JUINT32*>(pOffset);
	pOffset += sizeof(JUINT32);
	if (*pLen)
	{
		SafeStrncpy(strBuffer, pOffset, *pLen, JMAX_STRING_LEN);
		m_srvNumber = strBuffer;
		pOffset += *pLen;
	}

	return JSUCCESS;
}

JUINT32 JLogSrvNumber::operator=(JLogSrvNumber& rLogSrvNumber)
{
	m_srvNumber = rLogSrvNumber.GetSrvNumber();

	return JSUCCESS;
}

JString& JLogSrvNumber::GetSrvNumber()
{
	return m_srvNumber;
}

JUINT32 JLogSrvNumber::SetSrvNumber(JString& rStr)
{
	m_srvNumber = rStr;

	return JSUCCESS;
}

JLogSrvHasNewMsg::JLogSrvHasNewMsg()
{
	return;
}

JLogSrvHasNewMsg::~JLogSrvHasNewMsg()
{
	return;
}

JUINT32 JLogSrvHasNewMsg::GetLength()
{
	JUINT32 uiIdx = 0;
	JUINT32 uiLen = 0;

	uiLen += m_hasNewMsg.GetLength()+sizeof(JUINT32);
	for (uiIdx=0; uiIdx<JLOGSRV_MAX_MSG_SRV; uiIdx++)
	{
	    uiLen += m_rmtAddr[uiIdx].GetLength()+sizeof(JUINT32);
	}

	return uiLen;
}

JEventBody* JLogSrvHasNewMsg::Clone()
{
	JUINT32 uiIdx = 0;
	JLogSrvHasNewMsg* pLogSrvHasNewMsg = JNULL;

	pLogSrvHasNewMsg = new JLogSrvHasNewMsg();
	if (pLogSrvHasNewMsg)
	{
		pLogSrvHasNewMsg->SetHasNewMsg(m_hasNewMsg);
    	for (uiIdx=0; uiIdx<JLOGSRV_MAX_MSG_SRV; uiIdx++)
    	{
		    pLogSrvHasNewMsg->SetRmtAddr(m_rmtAddr[uiIdx], uiIdx);
	    }
	}

	return pLogSrvHasNewMsg;
}

JUINT32 JLogSrvHasNewMsg::Clear()
{
	JUINT32 uiIdx = 0;

	m_hasNewMsg.Clear();
	for (uiIdx=0; uiIdx<JLOGSRV_MAX_MSG_SRV; uiIdx++)
	{
    	m_rmtAddr[uiIdx].Clear();
    }

	return JSUCCESS;
}

JUINT32 JLogSrvHasNewMsg::Serialize(JCHAR* pBuf, JUINT32 uiMaxNum)
{
	JUINT32 uiIdx = 0;
	JUINT32 uiLen = 0;
	JUINT32* pLen = JNULL;
	JCHAR* pOffset = JNULL;
	JUINT32 tmpMaxNum = 0;

	if (!pBuf || !uiMaxNum)
	{
        JSingleton<JLog>::instance2() << set(JLOG_MOD_LOGSRV, JLOG_ERROR_LEVEL) 
            << "JLogSrvHasNewMsg::Serialize input param is NULL\n";
        return JFAILURE;
	}

	pOffset = pBuf;
	tmpMaxNum = uiMaxNum;

	uiLen = m_hasNewMsg.GetLength();
	pLen = reinterpret_cast<JUINT32*>(pOffset);
	*pLen = uiLen;
	pOffset += sizeof(JUINT32);
	tmpMaxNum -= sizeof(JUINT32);
	if (uiLen)
	{
		SafeMemcpy(pOffset, m_hasNewMsg.c_str(), uiLen, tmpMaxNum);

		pOffset += uiLen;
		tmpMaxNum -= uiLen;
	}

	for (uiIdx=0; uiIdx<JLOGSRV_MAX_MSG_SRV; uiIdx++)
	{
    	uiLen = m_rmtAddr[uiIdx].GetLength();
    	pLen = reinterpret_cast<JUINT32*>(pOffset);
    	*pLen = uiLen;
    	pOffset += sizeof(JUINT32);
    	tmpMaxNum -= sizeof(JUINT32);
    	if (uiLen)
    	{
    		SafeMemcpy(pOffset, m_rmtAddr[uiIdx].c_str(), uiLen, tmpMaxNum);

    		pOffset += uiLen;
    		tmpMaxNum -= uiLen;
    	}
	}

	return JSUCCESS;
}

JUINT32 JLogSrvHasNewMsg::DeSerialize(JCHAR* pBuf)
{
	JUINT32 uiIdx = 0;
	JUINT32* pLen = 0;
	JCHAR* pOffset = JNULL;
	JCHAR strBuffer[JMAX_STRING_LEN] = {0};

	if (!pBuf)
	{
        JSingleton<JLog>::instance2() << set(JLOG_MOD_LOGSRV, JLOG_ERROR_LEVEL) 
            << "JLogSrvHasNewMsg::DeSerialize input param is NULL\n";
        return JFAILURE;
	}

	pOffset = pBuf;

	pLen = reinterpret_cast<JUINT32*>(pOffset);
	pOffset += sizeof(JUINT32);
	if (*pLen)
	{
		SafeStrncpy(strBuffer, pOffset, *pLen, JMAX_STRING_LEN);
		m_hasNewMsg = strBuffer;
		pOffset += *pLen;
	}

	for (uiIdx=0; uiIdx<JLOGSRV_MAX_MSG_SRV; uiIdx++)
	{
    	pLen = reinterpret_cast<JUINT32*>(pOffset);
    	pOffset += sizeof(JUINT32);
    	if (*pLen)
    	{
    		SafeStrncpy(strBuffer, pOffset, *pLen, JMAX_STRING_LEN);
    		m_rmtAddr[uiIdx] = strBuffer;
    		pOffset += *pLen;
    	}
	}

	return JSUCCESS;
}

JUINT32 JLogSrvHasNewMsg::operator=(JLogSrvHasNewMsg& rLogSrvHasNewMsg)
{
	JUINT32 uiIdx = 0;

	m_hasNewMsg = rLogSrvHasNewMsg.GetHasNewMsg();
	for (uiIdx=0; uiIdx<JLOGSRV_MAX_MSG_SRV; uiIdx++)
	{
    	m_rmtAddr[uiIdx] = rLogSrvHasNewMsg.GetRmtAddr(uiIdx);
	}

	return JSUCCESS;
}

JString& JLogSrvHasNewMsg::GetHasNewMsg()
{
	return m_hasNewMsg;
}

JUINT32 JLogSrvHasNewMsg::SetHasNewMsg(JString& rStr)
{
	m_hasNewMsg = rStr;

	return JSUCCESS;
}

JString& JLogSrvHasNewMsg::GetRmtAddr(JUINT32 uiIdx)
{
	return m_rmtAddr[uiIdx];
}

JUINT32 JLogSrvHasNewMsg::SetRmtAddr(JString& rStr, JUINT32 uiIdx)
{
	m_rmtAddr[uiIdx] = rStr;

	return JSUCCESS;
}

JLogSrv::JLogSrv()
{
    JUINT32 uiIdx = 0;

    m_pSerialization = JNULL;

    m_pCommAcceptor = JNULL;

	return;
}

JLogSrv::~JLogSrv()
{
    JUINT32 uiIdx = 0;

    if (m_pSerialization)
    {
        delete m_pSerialization;
    }

    if (m_pCommAcceptor)
    {
        delete m_pCommAcceptor;
    }

	return;
}

JUINT32 JLogSrv::InitFunc()
{
    JSOCKADDR_IN lclAddr;
    JUINT32 uiListenPort = 0;

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_LOGSRV, "JLogSrv::InitFunc");

    m_pSerialization = new JSerialization(JSER_PERSISTENCE_XML);
    if (m_pSerialization)
    {
        GetCfgList(&m_cfg);
    }

    m_pCommAcceptor = new JCommAcceptor();
    if (m_pCommAcceptor)
    {
        if (m_cfg.GetListenAddr().GetLength() && m_cfg.GetListenPort().GetLength())
        {
            uiListenPort = atoi(m_cfg.GetListenPort().c_str());
            lclAddr.sin_family = AF_INET;
            lclAddr.sin_addr.S_un.S_addr = inet_addr(m_cfg.GetListenAddr().c_str());
            lclAddr.sin_port = htons(uiListenPort);
            m_pCommAcceptor->SetLocalAddr(&lclAddr);

            m_pCommAcceptor->Listen();
        }
    }

    return JSUCCESS;
}

JUINT32 JLogSrv::EventProcFunc(JEvent* pEvent)
{
    JUINT32 uiRet = JFAILURE;
    JEVT_TYPE type = JEVT_NONE;
    JLogSrvCfg* pLogSrvCfg = JNULL;
    JLogSrvNumber* pLogSrvNumber = JNULL;

    JAutoPtr<JLock> clsLockAutoPtr(&m_commLock);

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_LOGSRV, "JLogSrv::EventProcFunc");

    if (!pEvent)
    {
        JSingleton<JLog>::instance2() << set(JLOG_MOD_LOGSRV, JLOG_ERROR_LEVEL) 
            << "JLogSrv::EventProcFunc input param is NULL\n";
        return JFAILURE;
    }

    type = pEvent->GetEventType();
    switch(type)
    {
        case JEVT_LOGSRV_SET_CFG:
        {
            pLogSrvCfg = dynamic_cast<JLogSrvCfg*>(pEvent->GetBody());
            if (pLogSrvCfg)
            {
                uiRet = ProcSetCfgEvent(pLogSrvCfg);
            }
        }
        break;

        case JEVT_LOGSRV_GET_CFG:
        {
            uiRet = ProcGetCfgEvent(pEvent);
        }
        break;

        case JEVT_LOGSRV_CLEAN_LOG:
        {
            pLogSrvNumber = dynamic_cast<JLogSrvNumber*>(pEvent->GetBody());
            if (pLogSrvNumber)
            {
                uiRet = ProcCleanEvent(pLogSrvNumber);
            }
        }
        break;

        case JEVT_LOGSRV_GET_HAS_NEW_MSG:
        {
            pLogSrvNumber = dynamic_cast<JLogSrvNumber*>(pEvent->GetBody());
            if (pLogSrvNumber)
            {
                uiRet = ProcGetHasNewMsgEvent(pEvent, pLogSrvNumber);
            }
        }
        break;

        default:
        break;
    };

	return uiRet;
}

JEventBody* JLogSrv::MakeEventBody(JUINT32 uiType)
{
	JEventBody* pEventBody = JNULL;

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_LOGSRV, "JLogSrv::MakeEventBody");

	switch(uiType)
	{
		case JEVT_LOGSRV_SET_CFG:
		case JEVT_LOGSRV_GET_CFG:
			pEventBody = new JLogSrvCfg;
			break;

		default:
			break;
	};

	return pEventBody;
}

JCommEngine* JLogSrv::AcceptLogConnect()
{
    JINT32 iRet = 0;
    JCommEngine* pCommEngine = JNULL;

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_LOGSRV, "JLogSrv::AcceptLogConnect");

    if (m_pCommAcceptor)
    {
        while(1)
        {
            m_msgLock.Acquire();

            iRet = m_pCommAcceptor->Select(JCOMM_SELECT_INDEFINITE);
            if (iRet == JSUCCESS)
            {
                pCommEngine = m_pCommAcceptor->Accept();
                if (pCommEngine)
                {
                    m_msgLock.Release();
                	return pCommEngine;
                }
                else
                {
                    Sleep(JLOGSRV_SLEEP_TIME);
                    m_msgLock.Release();
                    continue;
                }
            }

            m_msgLock.Release();
        }
    }

	return JNULL;
}

JINT32 JLogSrv::RecvLogMsg(JCommEngine* pCommEngine, 
                            JCHAR* pBuf, 
                            JUINT32 uiLen)
{
    JUINT32 uiRet = JFAILURE;
    JINT32 iLen = 0;
    JSOCKADDR_IN stAddr;

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_LOGSRV, "JLogSrv::RecvLogMsg");

    if (pCommEngine)
    {
        uiRet = pCommEngine->HasMessage(JLOGSRV_SELECT_TIME);
        if (uiRet)
        {
            SafeMemset(pBuf, 0, uiLen);
            iLen = pCommEngine->RecvMessage(pBuf, uiLen, &stAddr);
            if (iLen == 0)
            {
                JSingleton<JLog>::instance2() << set(JLOG_MOD_LOGSRV, JLOG_ERROR_LEVEL) 
                    << "JLogSrv::RecvLogMsg recv message error "
                    << errno;
            	return JINVALID;
            }
            else
            {
                return iLen;
            }
        }
    }

	return JNULL;
}

JUINT32 JLogSrv::ProcSetCfgEvent(JLogSrvCfg* pLogSrvCfg)
{
    JBOOL bChanged = JFALSE;

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_LOGSRV, "JLogSrv::ProcSetCfgEvent");

    if (!pLogSrvCfg)
    {
        JSingleton<JLog>::instance2() << set(JLOG_MOD_LOGSRV, JLOG_ERROR_LEVEL) 
            << "JLogSrv::ProcSetCfgEvent pAccount is NULL\n";
        return JFAILURE;
    }

    if (pLogSrvCfg->GetListenAddr() != m_cfg.GetListenAddr())
    {
        m_cfg.GetListenAddr() = pLogSrvCfg->GetListenAddr();
        bChanged = JTRUE;
    }

    if (pLogSrvCfg->GetListenPort() != m_cfg.GetListenPort())
    {
        m_cfg.GetListenPort() = pLogSrvCfg->GetListenPort();
        bChanged = JTRUE;
    }

    if (bChanged)
    {
        SetCfgList(&m_cfg);
    }

	return JSUCCESS;
}

JUINT32 JLogSrv::ProcGetCfgEvent(JEvent* pEvent)
{
    JEvent* pNewEvent = JNULL;
    JLogSrvCfg* pLogSrvCfg = JNULL;
    JCommEngine* pComEngine = JNULL;
    JThread* pThread = JNULL;
    JLogSrvThread* pLogSrvThread = JNULL;

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_LOGSRV, "JLogSrv::ProcGetCfgEvent");

    pNewEvent = new JEvent(JEVT_LOGSRV_GET_CFG_RSP);
    pLogSrvCfg = new JLogSrvCfg;
    if (pNewEvent && pLogSrvCfg)
    {
    	pLogSrvCfg->SetListenAddr(m_cfg.GetListenAddr());
    	pLogSrvCfg->SetListenPort(m_cfg.GetListenPort());

	    pNewEvent->SetFromProc(pEvent->GetToProc().c_str());
	    pNewEvent->SetFromThrd(pEvent->GetToThrd().c_str());
	    pNewEvent->SetFromMod(pEvent->GetToMod().c_str());
	    pNewEvent->SetToProc(pEvent->GetFromProc().c_str());
	    pNewEvent->SetToThrd(pEvent->GetFromThrd().c_str());
	    pNewEvent->SetToMod(pEvent->GetFromMod().c_str());
	    pNewEvent->SetBody(pLogSrvCfg);

        pThread = JSingleton<JThreadManager>::instance()->GetThread(JS_T_JMAINTHREAD);
        pLogSrvThread = dynamic_cast<JLogSrvThread*>(pThread);
        if (pLogSrvThread)
        {
    	    pComEngine = pLogSrvThread->GetNotifyCommEngine();
    	    if (pComEngine)
    	    {
    	        pComEngine->SendEvent(pNewEvent);
    	    }
	    }
    }

	return JSUCCESS;
}

JUINT32 JLogSrv::SetCfgList(JLogSrvCfg* pLogSrvCfg)
{
    JUINT32 uiLen = 0;
    JList<JPER_RECORD> clsDstList;
    JListItem<JPER_RECORD>* pDstItem = JNULL;
    JListItem<JPER_RECORD>* prevDstItem = JNULL;
    JPER_RECORD* pDstData = JNULL;

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_LOGSRV, "JLogSrv::SetCfgList");

    if (m_pSerialization)
    {
        uiLen = pLogSrvCfg->GetListenAddr().GetLength();
        if (uiLen)
        {
            pDstData = 
                reinterpret_cast<JPER_RECORD*>(JSingleton<JStaticMemory>::instance()->Alloc(sizeof(JPER_RECORD)+1));
            if (pDstData)
            {
	            SafeMemset(reinterpret_cast<JCHAR*>(pDstData), 0, sizeof(JPER_RECORD)+1);
                SafeSprintf(pDstData->strKey, JMAX_STRING_LEN, "%s", JLOGSRV_LISTEN_ADDR);
                SafeStrcpy(pDstData->strValue, pLogSrvCfg->GetListenAddr().c_str(), uiLen+1);
                pDstItem = new JListItem<JPER_RECORD>(pDstData);
                pDstItem->SetDataLength(sizeof(JPER_RECORD));
                clsDstList.InsertItem(pDstItem, prevDstItem);
                prevDstItem = pDstItem;
            }
            else
            {
                JSingleton<JLog>::instance2() << set(JLOG_MOD_LOGSRV, JLOG_ERROR_LEVEL) << "JLogSrv::SetCfgList memory alloc failure\n";
                return JFAILURE;
            }
        }

        uiLen = pLogSrvCfg->GetListenPort().GetLength();
        if (uiLen)
        {
            pDstData = 
                reinterpret_cast<JPER_RECORD*>(JSingleton<JStaticMemory>::instance()->Alloc(sizeof(JPER_RECORD)+1));
            if (pDstData)
            {
	            SafeMemset(reinterpret_cast<JCHAR*>(pDstData), 0, sizeof(JPER_RECORD)+1);
                SafeSprintf(pDstData->strKey, JMAX_STRING_LEN, "%s", JLOGSRV_LISTEN_PORT);
                SafeStrcpy(pDstData->strValue, pLogSrvCfg->GetListenPort().c_str(), uiLen+1);
                pDstItem = new JListItem<JPER_RECORD>(pDstData);
                pDstItem->SetDataLength(sizeof(JPER_RECORD));
                clsDstList.InsertItem(pDstItem, prevDstItem);
                prevDstItem = pDstItem;
            }
            else
            {
                JSingleton<JLog>::instance2() << set(JLOG_MOD_LOGSRV, JLOG_ERROR_LEVEL) << "JLogSrv::SetCfgList memory alloc failure\n";
                return JFAILURE;
            }
        }

        m_pSerialization->SetList(clsDstList, JLOGSRV_CFG_FILE);
    }

	return JSUCCESS;
}

JUINT32 JLogSrv::GetCfgList(JLogSrvCfg* pLogSrvCfg)
{
    JUINT32 uiLen = 0;
    JUINT32 uiLen2 = 0;
    JList<JPER_RECORD> clsSrcList;
    JListItem<JPER_RECORD>* pSrcItem = JNULL;
    JPER_RECORD* pSrcData = JNULL;

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_LOGSRV, "JLogSrv::GetCfgList");

    if (m_pSerialization)
    {
        clsSrcList = m_pSerialization->GetList(JLOGSRV_CFG_FILE);

        JListIterator<JPER_RECORD> clsListIter(clsSrcList);
        for (clsListIter.First(); clsListIter.Done(); clsListIter.Next())
        {
            pSrcItem = clsListIter.Item();

            pSrcData = pSrcItem->GetData();

            uiLen = SafeStrlen(pSrcData->strKey);
            uiLen2 = SafeStrlen(pSrcData->strValue);
            if (uiLen && uiLen2)
            {
                if (SafeStrcmp(pSrcData->strKey, JLOGSRV_LISTEN_ADDR) == 0)
                {
                    pLogSrvCfg->GetListenAddr() = pSrcData->strValue;
                }
                else if (SafeStrcmp(pSrcData->strKey, JLOGSRV_LISTEN_PORT) == 0)
                {
                    pLogSrvCfg->GetListenPort() = pSrcData->strValue;
                }
            }
        }
    }

	return JSUCCESS;
}

JUINT32 JLogSrv::ProcCleanEvent(JLogSrvNumber* pLogSrvNumber)
{
    JThread* pThread = JNULL;
    JLogMsgThread* pLogMsgThread = JNULL;
    JUINT32 uiNumber = 0;
    JCHAR strThrdName[JMAX_STRING_LEN] = {0};
    JThreadManager* pThreadManager = JSingleton<JThreadManager>::instance();

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_LOGSRV, "JLogSrv::ProcCleanEvent");

    if (!pLogSrvNumber || !pLogSrvNumber->GetSrvNumber().GetLength())
    {
        JSingleton<JLog>::instance2() << set(JLOG_MOD_LOGSRV, JLOG_ERROR_LEVEL) 
            << "JLogSrv::ProcCleanEvent input param is NULL\n";
        return JFAILURE;
    }

    uiNumber = atoi(pLogSrvNumber->GetSrvNumber().c_str());
    if (uiNumber>=1 && uiNumber<=JLOGSRV_MAX_MSG_SRV)
    {
        SafeStrcpy(strThrdName, JS_T_JLOGMSG_PREFIX, JMAX_STRING_LEN);
        SafeStrncat(strThrdName, pLogSrvNumber->GetSrvNumber().c_str(), 
            pLogSrvNumber->GetSrvNumber().GetLength(), JMAX_STRING_LEN);

        pThread = pThreadManager->GetThread(strThrdName);
        if (pThread)
        {
            pLogMsgThread = dynamic_cast<JLogMsgThread*>(pThread);
            if (pLogMsgThread)
            {
                pLogMsgThread->CleanLog();
            }
        }
    }

	return JSUCCESS;
}

JUINT32 JLogSrv::ProcGetHasNewMsgEvent(JEvent* pEvent, JLogSrvNumber* pLogSrvNumber)
{
    JUINT32 uiIdx = 0;
    JEvent* pNewEvent = JNULL;
    JLogSrvHasNewMsg* pLogSrvHasNewMsg = JNULL;
    JCommEngine* pComEngine = JNULL;
    JThread* pThread = JNULL;
    JLogSrvThread* pLogSrvThread = JNULL;
    JLogMsgThread* pLogMsgThread = JNULL;
    JUINT32 uiNumber = 0;
    JCHAR strThrdName[JMAX_STRING_LEN] = {0};
    JThreadManager* pThreadManager = JSingleton<JThreadManager>::instance();
    JBOOL bHasNewMsg = JFALSE;
    JSOCKADDR_IN* pRmtAddr[JLOGSRV_MAX_MSG_SRV] = {0};
    JCHAR ucHasNewMsg[JMAX_STRING_LEN] = {0};
    JString strHasNewMsg;
    JString strRmtAddr[JLOGSRV_MAX_MSG_SRV];
	JCHAR tmpSeq[JMAX_STRING_LEN] = {0};

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_LOGSRV, "JLogSrv::ProcGetHasNewMsgEvent");

    if (!pEvent || !pLogSrvNumber || !pLogSrvNumber->GetSrvNumber().GetLength())
    {
        JSingleton<JLog>::instance2() << set(JLOG_MOD_LOGSRV, JLOG_ERROR_LEVEL) 
            << "JLogSrv::ProcGetHasNewMsgEvent input param is NULL\n";
        return JFAILURE;
    }

    for (uiIdx=0; uiIdx<JLOGSRV_MAX_MSG_SRV; uiIdx++)
    {
        SafeStrcpy(strThrdName, JS_T_JLOGMSG_PREFIX, JMAX_STRING_LEN);
        SafeMemset(tmpSeq, 0, JMAX_STRING_LEN);
        SafeSprintf(tmpSeq, JMAX_STRING_LEN, "%d", uiIdx+1);
        SafeStrncat(strThrdName, tmpSeq, SafeStrlen(tmpSeq), JMAX_STRING_LEN);
        pThread = pThreadManager->GetThread(strThrdName);
        if (pThread)
        {
            pLogMsgThread = dynamic_cast<JLogMsgThread*>(pThread);
            if (pLogMsgThread)
            {
                pRmtAddr[uiIdx] = pLogMsgThread->GetRemoteAddr();
                if (pRmtAddr[uiIdx])
                {
                    strRmtAddr[uiIdx] = inet_ntoa(pRmtAddr[uiIdx]->sin_addr);
                }
            }
        }
    }

    uiNumber = atoi(pLogSrvNumber->GetSrvNumber().c_str());
    if (uiNumber>=1 && uiNumber<=JLOGSRV_MAX_MSG_SRV)
    {
        SafeStrcpy(strThrdName, JS_T_JLOGMSG_PREFIX, JMAX_STRING_LEN);
        SafeStrncat(strThrdName, pLogSrvNumber->GetSrvNumber().c_str(), 
            pLogSrvNumber->GetSrvNumber().GetLength(), JMAX_STRING_LEN);

        pThread = pThreadManager->GetThread(strThrdName);
        if (pThread)
        {
            pLogMsgThread = dynamic_cast<JLogMsgThread*>(pThread);
            if (pLogMsgThread)
            {
                bHasNewMsg = pLogMsgThread->GetHasNewMsg();
                if (bHasNewMsg)
                {
                    pLogMsgThread->SetHasNewMsg(JFALSE);
                }
                SafeSprintf(ucHasNewMsg, JMAX_STRING_LEN, "%u", bHasNewMsg);
                strHasNewMsg = ucHasNewMsg;

                pNewEvent = new JEvent(JEVT_LOGSRV_GET_HAS_NEW_MSG_RSP);
                pLogSrvHasNewMsg = new JLogSrvHasNewMsg;
                if (pNewEvent && pLogSrvHasNewMsg)
                {
                	pLogSrvHasNewMsg->SetHasNewMsg(strHasNewMsg);
                    for (uiIdx=0; uiIdx<JLOGSRV_MAX_MSG_SRV; uiIdx++)
                    {
                    	pLogSrvHasNewMsg->SetRmtAddr(strRmtAddr[uiIdx], uiIdx);
                	}

            	    pNewEvent->SetFromProc(pEvent->GetToProc().c_str());
            	    pNewEvent->SetFromThrd(pEvent->GetToThrd().c_str());
            	    pNewEvent->SetFromMod(pEvent->GetToMod().c_str());
            	    pNewEvent->SetToProc(pEvent->GetFromProc().c_str());
            	    pNewEvent->SetToThrd(pEvent->GetFromThrd().c_str());
            	    pNewEvent->SetToMod(pEvent->GetFromMod().c_str());
            	    pNewEvent->SetBody(pLogSrvHasNewMsg);

                    pThread = JSingleton<JThreadManager>::instance()->GetThread(JS_T_JMAINTHREAD);
                    pLogSrvThread = dynamic_cast<JLogSrvThread*>(pThread);
                    if (pLogSrvThread)
                    {
                	    pComEngine = pLogSrvThread->GetNotifyCommEngine();
                	    if (pComEngine)
                	    {
                	        pComEngine->SendEvent(pNewEvent);
                	    }
            	    }
                }
            }
        }
    }

	return JSUCCESS;
}

JLogSrvThread::JLogSrvThread(JString* pThdName, 
                            JString* pLocalAddr, 
                            JUINT32 uiLocalPort):
                            JThread(pThdName, JTHRD_TYPE_SRV)
{
    JSOCKADDR_IN lclAddr;
    JLogSrv* pLogSrv = JSingleton<JLogSrv>::instance();

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

    pLogSrv->InitFunc();

	return;
}

JLogSrvThread::~JLogSrvThread()
{
    if (m_pNotifyCommEngine)
    {
        delete m_pNotifyCommEngine;
    }

    return;
}

JUINT32 JLogSrvThread::Init()
{
    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_THREAD, "JLogSrvThread::Init");

    if (m_pNotifyCommEngine)
    {
        m_commEngineGroup.AddCommEngine(m_pNotifyCommEngine);
    }

	return JSUCCESS;
}

JUINT32 JLogSrvThread::Run()
{
    JUINT32 uiRet = JFAILURE;
    JListItem<JEvent>* pListItem = JNULL;
    JEvent* pEvent = JNULL;
    JSOCKADDR_IN stAddr;
    JCommEngine* pCommEngine = JNULL;
    JCHAR pBuf[JCOMM_MSG_BUF_LEN] = {0};
    JLogSrv* pLogSrv = JSingleton<JLogSrv>::instance();

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_THREAD, "JLogSrvThread::Run");

    uiRet = Init();
    if (!uiRet)
    {
        JSingleton<JLog>::instance2() << set(JLOG_MOD_THREAD, JLOG_ERROR_LEVEL) 
            << "JLogSrvThread::Run Init return failure\n";
        return JFAILURE;
    }

    do{
        pCommEngine = m_commEngineGroup.HasMessage(JCOMM_SELECT_INDEFINITE);
        //for thread notify CommEngine
        if (pCommEngine && pCommEngine == m_pNotifyCommEngine)
        {
            uiRet = pCommEngine->RecvMessage(pBuf, JCOMM_MSG_BUF_LEN, &stAddr);
            if (uiRet == 1 && SafeStrcmp(pBuf, "1") ==0)
            {
                while(1)
                {
                    m_Lock.Acquire();

                    pListItem = m_hQueue.DeQueue();
                    if (pListItem)
                    {
                        pEvent = pListItem->GetData();
                        if (pEvent)
                        {
                            pLogSrv->EventProcFunc(pEvent);
                        }

                        delete pListItem;
                        m_Lock.Release();
                    }
                    else
                    {
                        m_Lock.Release();
                        break;
                    }
                }
            }
            //JEvent recv, and route also must be added here.
            else
            {
            }
        }
        //other thread JCommEngine
        else
        {
        }
    }while(1);

	return JSUCCESS;
}

JUINT32 JLogSrvThread::EnQueueEvent(JListItem<JEvent>* pItem, JCHAR* pModName)
{
    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_THREAD, "JLogSrvThread::EnQueueEvent");

    if (!pItem)
    {
        JSingleton<JLog>::instance2() << set(JLOG_MOD_THREAD, JLOG_ERROR_LEVEL) 
            << "JLogSrvThread::EnQueueEvent input param is NULL\n";
        return JFAILURE;
    }

    m_Lock.Acquire();
    m_hQueue.EnQueue(pItem);
    m_Lock.Release();

    return JSUCCESS;
}

JEventBody* JLogSrvThread::MakeEventBody(JEVT_TYPE eType, JCHAR* pModName)
{
	JEventBody* pEventBody = JNULL;
    JLogSrv* pLogSrv = JSingleton<JLogSrv>::instance();

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_THREAD, "JLogSrvThread::MakeEventBody");

    pEventBody = pLogSrv->MakeEventBody(static_cast<JUINT32>(eType));

	return pEventBody;
}

JCommEngine* JLogSrvThread::GetNotifyCommEngine()
{
    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_THREAD, "JLogSrvThread::GetNotifyCommEngine");

    return m_pNotifyCommEngine;
}

JLogMsgThread::JLogMsgThread(JString* pThdName):JThread(pThdName, JTHRD_TYPE_MSG)
{
    SafeMemset(reinterpret_cast<JCHAR*>(m_logMessage), 0, JLOGMSG_MAX_MSG*JMAX_BUFFER_LEN);
    m_uiLogOffset = 0;

    m_pCommEngine = JNULL;

    m_bHasNewMsg = JFALSE;

	return;
}

JLogMsgThread::~JLogMsgThread()
{
    if (m_pCommEngine)
    {
        delete m_pCommEngine;
        m_pCommEngine = JNULL;
    }

    return;
}

JUINT32 JLogMsgThread::Init()
{
	return JSUCCESS;
}

JUINT32 JLogMsgThread::Run()
{
    JUINT32 uiRet = JFAILURE;
    JINT32 iRet = JFAILURE;
    JCHAR pBuf[JMAX_BUFFER_LEN] = {0};
    JLogSrv* pLogSrv = JSingleton<JLogSrv>::instance();
    JCHAR* pOffset = JNULL;
    JCHAR* pNewLine = JNULL;
    JUINT32 uiLen = 0;
    JUINT32 uiLen2 = 0;

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_THREAD, "JLogMsgThread::Run");

    uiRet = Init();
    if (!uiRet)
    {
        JSingleton<JLog>::instance2() << set(JLOG_MOD_THREAD, JLOG_ERROR_LEVEL) 
            << "JLogMsgThread::Run Init return failure\n";
        return JFAILURE;
    }

    do{
        m_pCommEngine = pLogSrv->AcceptLogConnect();
        if (m_pCommEngine)
        {
            while(1)
            {
                m_Lock.Acquire();

                iRet = pLogSrv->RecvLogMsg(m_pCommEngine, pBuf, JMAX_BUFFER_LEN);
                if (iRet == JINVALID)
                {
                    delete m_pCommEngine;
                    m_pCommEngine = JNULL;

                    m_Lock.Release();
                    break;
                }
                else if (iRet > 0)
                {
                    uiLen = SafeStrlen(pBuf);
                    pOffset = pBuf;

                    uiLen2 = SafeStrlen(pOffset);
                    while(uiLen2)
                    {
                        pNewLine = SafeStrchr(pOffset, '\n');
                        if (pNewLine)
                        {
                            SafeStrncat(m_logMessage[m_uiLogOffset], pOffset, pNewLine-pOffset, 
                                JMAX_BUFFER_LEN);
                            pOffset = pNewLine+1;

                            m_uiLogOffset++;
                            if (m_uiLogOffset == JLOGMSG_MAX_MSG)
                            {
                                m_uiLogOffset = 0;
                            }
                        }
                        else
                        {
                            SafeStrncat(m_logMessage[m_uiLogOffset], pOffset, uiLen2, JMAX_BUFFER_LEN);
                            pOffset += uiLen2;
                        }

                        uiLen2 = SafeStrlen(pOffset);
                    }

                    m_bHasNewMsg = JTRUE;
                }

                m_Lock.Release();
            };
        }
    }while(1);

	return JSUCCESS;
}

JUINT32 JLogMsgThread::EnQueueEvent(JListItem<JEvent>* pItem, JCHAR* pModName)
{
    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_THREAD, "JLogMsgThread::EnQueueEvent");

    return JSUCCESS;
}

JEventBody* JLogMsgThread::MakeEventBody(JEVT_TYPE eType, JCHAR* pModName)
{
	JEventBody* pEventBody = JNULL;

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_THREAD, "JLogMsgThread::MakeEventBody");

	return pEventBody;
}

JUINT32 JLogMsgThread::CleanLog()
{
    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_THREAD, "JLogMsgThread::CleanLog");

    m_Lock.Acquire();
    SafeMemset(reinterpret_cast<JCHAR*>(m_logMessage), 0, JLOGMSG_MAX_MSG*JMAX_BUFFER_LEN);
    m_uiLogOffset = 0;
    m_Lock.Release();

	return JSUCCESS;
}

JUINT32 JLogMsgThread::SetHasNewMsg(JBOOL bHasNewMsg)
{
    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_THREAD, "JLogMsgThread::SetHasNewMsg");

    m_Lock.Acquire();
    m_bHasNewMsg = bHasNewMsg;
    m_Lock.Release();

	return JSUCCESS;
}

JBOOL JLogMsgThread::GetHasNewMsg()
{
    JBOOL bHasNewMsg;

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_THREAD, "JLogMsgThread::GetHasNewMsg");

    m_Lock.Acquire();
    bHasNewMsg = m_bHasNewMsg;
    m_Lock.Release();

	return bHasNewMsg;
}

JSOCKADDR_IN* JLogMsgThread::GetRemoteAddr()
{
    JSOCKADDR_IN* pRmtAddr = JNULL;

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_THREAD, "JLogMsgThread::GetRemoteAddr");

    m_Lock.Acquire();
    if (m_pCommEngine)
    {
        pRmtAddr = m_pCommEngine->GetRemoteAddr();;
    }
    m_Lock.Release();

	return pRmtAddr;
}

JUINT32 JLogMsgThread::GetLogMsg(JCHAR* logMessage, 
                                JUINT32 uiMaxLen, 
                                JUINT32* pOffset)
{
    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_THREAD, "JLogMsgThread::GetLogMsg");

    m_Lock.Acquire();
    SafeMemcpy(logMessage, reinterpret_cast<JCHAR*>(m_logMessage), JLOGMSG_MAX_MSG*JMAX_BUFFER_LEN, uiMaxLen);
    *pOffset = m_uiLogOffset;
    m_Lock.Release();

	return JSUCCESS;
}

