/**********************************************************
* Author: weiganyi
* File: JLog.cpp
* Date: 20110718
* Description: 
*
**********************************************************/

#include "JFrameWork.h"


namespace JFrameWork{

JFmt& set(JLOG_MODULE module, JLOG_LEVEL level)
{
    static JFmt clsFmt;

    clsFmt.m_Module = module;
    clsFmt.m_Level = level;

    return clsFmt;
}

JFmt::JFmt()
{
    m_Module = JLOG_MOD_NONE;
    m_Level = JLOG_INFO_LEVEL;

	return;
}

JFmt::~JFmt()
{
	return;
}

JLogDecorator::JLogDecorator()
{
	return;
}

JLogDecorator::~JLogDecorator()
{
	return;
}

JLogTimeDecorator::JLogTimeDecorator()
{
    SafeMemset(m_outputCatchBuf, 0, JLOG_MAX_BUF_LEN*2);

    m_newLineStart = JTRUE;

	return;
}

JLogTimeDecorator::~JLogTimeDecorator()
{
	return;
}

JUINT32 JLogTimeDecorator::DecoratorBuf(JLogOutput* pLogOutput, JCHAR* pBuf)
{
    JUINT32 uiRet = JFAILURE;
    JSYSTIME stSysTime = {0};
    JCHAR* pOffset = JNULL;
    JUINT32 uiLen = 0;
    JCHAR* pBoundary = JNULL;

    if (!pLogOutput)
    {
        JSingleton<JLog>::instance2() << set(JLOG_MOD_LOG, JLOG_ERROR_LEVEL) 
            << "JLogTimeDecorator::DecoratorBuf input param is NULL\n";
        return JFAILURE;
    }

    GetSysTime(&stSysTime);

    pOffset = pBuf;

    uiLen = SafeStrlen(pOffset);
    while(uiLen)
    {
        if (m_newLineStart)
        {
            SafeMemset(m_outputCatchBuf, 0, JLOG_MAX_BUF_LEN*2);
            SafeSprintf(m_outputCatchBuf, JLOG_MAX_BUF_LEN, JLOG_TIMESTAMP_FORMAT, 
                stSysTime.uiHour, stSysTime.uiMinute, 
                stSysTime.uiSecond, stSysTime.uiMilliseconds);
            m_newLineStart = JFALSE;
        }

        pBoundary = SafeStrstr(pOffset, "\n");
        if (pBoundary)
        {
            uiLen = pBoundary-pOffset;
            SafeMemcat(m_outputCatchBuf, pOffset, uiLen, JLOG_MAX_BUF_LEN);
            m_newLineStart = JTRUE;
        }
        else
        {
            uiLen = SafeStrlen(pOffset);
            SafeMemcat(m_outputCatchBuf, pOffset, uiLen, JLOG_MAX_BUF_LEN);
            return JSUCCESS;
        }

        uiRet = pLogOutput->RawPrint(m_outputCatchBuf);
        if (!uiRet)
        {
            return uiRet;
        }

        pOffset = pBoundary+1;
        uiLen = SafeStrlen(pOffset);
    };

    return JSUCCESS;
}

JLogOutput::JLogOutput()
{
	return;
}

JLogOutput::~JLogOutput()
{
	return;
}

JLogOutputLocal::JLogOutputLocal()
{
    m_pDecorator = new JLogTimeDecorator;

	return;
}

JLogOutputLocal::~JLogOutputLocal()
{
    if (m_pDecorator)
    {
        delete m_pDecorator;
        m_pDecorator = JNULL;
    }

	return;
}

JUINT32 JLogOutputLocal::Print(JCHAR* ptrBuf)
{
    if (m_pDecorator)
    {
        m_pDecorator->DecoratorBuf(this, ptrBuf);
    }
    else
    {
        RawPrint(ptrBuf);
    }

    return JSUCCESS;
}

JUINT32 JLogOutputLocal::RawPrint(JCHAR* ptrBuf)
{
    cout << ptrBuf << endl;

    return JSUCCESS;
}

JCommEngine* JLogOutputLocal::GetCommEngine()
{
    return JNULL;
}

JLogOutputFile::JLogOutputFile(JCHAR* pFileName)
{
    m_pDecorator = new JLogTimeDecorator;

    m_fileHandler = SafeFopen(pFileName, "w+");

	return;
}

JLogOutputFile::~JLogOutputFile()
{
    if (m_fileHandler)
    {
        SafeFclose(m_fileHandler);
    }

    if (m_pDecorator)
    {
        delete m_pDecorator;
        m_pDecorator = JNULL;
    }

	return;
}

JUINT32 JLogOutputFile::Print(JCHAR* ptrBuf)
{
    if (m_pDecorator)
    {
        m_pDecorator->DecoratorBuf(this, ptrBuf);
    }
    else
    {
        RawPrint(ptrBuf);
    }

    return JSUCCESS;
}

JUINT32 JLogOutputFile::RawPrint(JCHAR* ptrBuf)
{
    if (m_fileHandler)
    {
        SafeStrncat(ptrBuf, "\n", 1, JLOG_MAX_BUF_LEN);

        SafeFprintf(m_fileHandler, "%s", ptrBuf);
        SafeFflush(m_fileHandler);
    }

    return JSUCCESS;
}

JCommEngine* JLogOutputFile::GetCommEngine()
{
    return JNULL;
}

JLogOutputRemote::JLogOutputRemote(JString& rLocalAddr,
                                JUINT32 uiLocalPort,
                                JString& rRemoteAddr,
                                JUINT32 uiRemotePort)
{
    JSOCKADDR_IN lclAddr;
    JSOCKADDR_IN rmtAddr;
    JString strLclAddr;

    m_pDecorator = new JLogTimeDecorator;

    m_pCommEngine = JNULL;
    m_pCommConnector = JNULL;

    GetLocalAddr(rRemoteAddr, strLclAddr);
    if (strLclAddr.GetLength()&&
        uiLocalPort && 
        rRemoteAddr.GetLength() &&
        uiRemotePort)
    {
        m_pCommConnector = new JCommConnector;
        if (m_pCommConnector)
        {
            lclAddr.sin_family = AF_INET;
            lclAddr.sin_addr.S_un.S_addr = inet_addr(strLclAddr.c_str());
            lclAddr.sin_port = htons(uiLocalPort);
            m_pCommConnector->SetLocalAddr(&lclAddr);

            rmtAddr.sin_family = AF_INET;
            rmtAddr.sin_addr.S_un.S_addr = inet_addr(rRemoteAddr.c_str());
            rmtAddr.sin_port = htons(uiRemotePort);
            m_pCommConnector->SetRemoteAddr(&rmtAddr);

            m_pCommEngine = m_pCommConnector->Connect();
        }
    }

	return;
}

JLogOutputRemote::~JLogOutputRemote()
{
    if (m_pCommEngine)
    {
        delete m_pCommEngine;
        m_pCommEngine = JNULL;
    }

    if (m_pCommConnector)
    {
        delete m_pCommConnector;
        m_pCommConnector = JNULL;
    }

    if (m_pDecorator)
    {
        delete m_pDecorator;
        m_pDecorator = JNULL;
    }

	return;
}

JUINT32 JLogOutputRemote::Print(JCHAR* ptrBuf)
{
    JUINT32 uiRet = JFAILURE;

    if (m_pDecorator)
    {
        uiRet = m_pDecorator->DecoratorBuf(this, ptrBuf);
    }
    else
    {
        uiRet = RawPrint(ptrBuf);
    }

    return uiRet;
}

JUINT32 JLogOutputRemote::RawPrint(JCHAR* ptrBuf)
{
    JUINT32 uiLen = 0;
    JINT32 iLen = 0;

    if (m_pCommEngine)
    {
        SafeStrncat(ptrBuf, "\n", 1, JLOG_MAX_BUF_LEN);

        uiLen = SafeStrlen(ptrBuf);

        iLen = m_pCommEngine->SendMessage(ptrBuf, uiLen);
        if (iLen == 0)
        {
            return JFAILURE;
        }
    }

    return JSUCCESS;
}

JCommEngine* JLogOutputRemote::GetCommEngine()
{
    return m_pCommEngine;
}

JLogCfg::JLogCfg()
{
	return;
}

JLogCfg::~JLogCfg()
{
	return;
}

JUINT32 JLogCfg::GetLength()
{
	JUINT32 uiLen = 0;

	uiLen += m_strAddress.GetLength()+sizeof(JUINT32);
	uiLen += m_strPort.GetLength()+sizeof(JUINT32);
	uiLen += m_strOutputFile.GetLength()+sizeof(JUINT32);
	uiLen += m_strOutputRemote.GetLength()+sizeof(JUINT32);

	return uiLen;
}

JEventBody* JLogCfg::Clone()
{
	JLogCfg* pLogCfg = JNULL;

	pLogCfg = new JLogCfg();
	if (pLogCfg)
	{
		pLogCfg->SetLogAddress(m_strAddress);
		pLogCfg->SetLogPort(m_strPort);
		pLogCfg->SetOutputFile(m_strOutputFile);
		pLogCfg->SetOutputRemote(m_strOutputRemote);
	}

	return pLogCfg;
}

JUINT32 JLogCfg::Clear()
{
	m_strAddress.Clear();
	m_strPort.Clear();
	m_strOutputFile.Clear();
	m_strOutputRemote.Clear();

	return JSUCCESS;
}

JUINT32 JLogCfg::Serialize(JCHAR* pBuf, JUINT32 uiMaxNum)
{
	JUINT32 uiLen = 0;
	JUINT32* pLen = JNULL;
	JCHAR* pOffset = JNULL;
	JUINT32 tmpMaxNum = 0;

	if (!pBuf || !uiMaxNum)
	{
        JSingleton<JLog>::instance2() << set(JLOG_MOD_LOG, JLOG_ERROR_LEVEL) 
            << "JLogCfg::Serialize input param is NULL\n";
        return JFAILURE;
	}

	pOffset = pBuf;
	tmpMaxNum = uiMaxNum;

	uiLen = m_strAddress.GetLength();
	pLen = reinterpret_cast<JUINT32*>(pOffset);
	*pLen = uiLen;
	pOffset += sizeof(JUINT32);
	tmpMaxNum -= sizeof(JUINT32);
	if (uiLen)
	{
		SafeMemcpy(pOffset, m_strAddress.c_str(), uiLen, tmpMaxNum);

		pOffset += uiLen;
		tmpMaxNum -= uiLen;
	}

	uiLen = m_strPort.GetLength();
	pLen = reinterpret_cast<JUINT32*>(pOffset);
	*pLen = uiLen;
	pOffset += sizeof(JUINT32);
	tmpMaxNum -= sizeof(JUINT32);
	if (uiLen)
	{
		SafeMemcpy(pOffset, m_strPort.c_str(), uiLen, tmpMaxNum);

		pOffset += uiLen;
		tmpMaxNum -= uiLen;
	}

	uiLen = m_strOutputFile.GetLength();
	pLen = reinterpret_cast<JUINT32*>(pOffset);
	*pLen = uiLen;
	pOffset += sizeof(JUINT32);
	tmpMaxNum -= sizeof(JUINT32);
	if (uiLen)
	{
		SafeMemcpy(pOffset, m_strOutputFile.c_str(), uiLen, tmpMaxNum);

		pOffset += uiLen;
		tmpMaxNum -= uiLen;
	}

	uiLen = m_strOutputRemote.GetLength();
	pLen = reinterpret_cast<JUINT32*>(pOffset);
	*pLen = uiLen;
	pOffset += sizeof(JUINT32);
	tmpMaxNum -= sizeof(JUINT32);
	if (uiLen)
	{
		SafeMemcpy(pOffset, m_strOutputRemote.c_str(), uiLen, tmpMaxNum);

		pOffset += uiLen;
		tmpMaxNum -= uiLen;
	}

	return JSUCCESS;
}

JUINT32 JLogCfg::DeSerialize(JCHAR* pBuf)
{
	JUINT32* pLen = 0;
	JCHAR* pOffset = JNULL;
	JCHAR strBuffer[JMAX_STRING_LEN] = {0};

	if (!pBuf)
	{
        JSingleton<JLog>::instance2() << set(JLOG_MOD_LOG, JLOG_ERROR_LEVEL) 
            << "JLogCfg::DeSerialize input param is NULL\n";
        return JFAILURE;
	}

	pOffset = pBuf;

	pLen = reinterpret_cast<JUINT32*>(pOffset);
	pOffset += sizeof(JUINT32);
	if (*pLen)
	{
		SafeStrncpy(strBuffer, pOffset, *pLen, JMAX_STRING_LEN);
		m_strAddress = strBuffer;
		pOffset += *pLen;
	}

	pLen = reinterpret_cast<JUINT32*>(pOffset);
	pOffset += sizeof(JUINT32);
	if (*pLen)
	{
		SafeStrncpy(strBuffer, pOffset, *pLen, JMAX_STRING_LEN);
		m_strPort = strBuffer;
		pOffset += *pLen;
	}

	pLen = reinterpret_cast<JUINT32*>(pOffset);
	pOffset += sizeof(JUINT32);
	if (*pLen)
	{
		SafeStrncpy(strBuffer, pOffset, *pLen, JMAX_STRING_LEN);
		m_strOutputFile = strBuffer;
		pOffset += *pLen;
	}

	pLen = reinterpret_cast<JUINT32*>(pOffset);
	pOffset += sizeof(JUINT32);
	if (*pLen)
	{
		SafeStrncpy(strBuffer, pOffset, *pLen, JMAX_STRING_LEN);
		m_strOutputRemote = strBuffer;
		pOffset += *pLen;
	}

	return JSUCCESS;
}

JUINT32 JLogCfg::operator=(JLogCfg& rLogCfg)
{
	m_strAddress = rLogCfg.GetLogAddress();
	m_strPort = rLogCfg.GetLogPort();
	m_strOutputFile = rLogCfg.GetOutputFile();
	m_strOutputRemote = rLogCfg.GetOutputRemote();

	return JSUCCESS;
}

JString& JLogCfg::GetLogAddress()
{
	return m_strAddress;
}

JUINT32 JLogCfg::SetLogAddress(JString& rStr)
{
	m_strAddress = rStr;

	return JSUCCESS;
}

JString& JLogCfg::GetLogPort()
{
	return m_strPort;
}

JUINT32 JLogCfg::SetLogPort(JString& rStr)
{
	m_strPort = rStr;

	return JSUCCESS;
}

JString& JLogCfg::GetOutputFile()
{
	return m_strOutputFile;
}

JUINT32 JLogCfg::SetOutputFile(JString& rStr)
{
	m_strOutputFile = rStr;

	return JSUCCESS;
}

JString& JLogCfg::GetOutputRemote()
{
	return m_strOutputRemote;
}

JUINT32 JLogCfg::SetOutputRemote(JString& rStr)
{
	m_strOutputRemote = rStr;

	return JSUCCESS;
}

JUINT32 OnCheckConnect(JVOID* pData)
{
    JLog* pLog = reinterpret_cast<JLog*>(pData);
    JEvent* pEvent = JNULL;
    JCommEngine* pComEngine = JNULL;
    JThread* pThread = JNULL;
    JModuleThread* pModuleThread = JNULL;

    if (!pLog)
    {
        return JFAILURE;
    }

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_LOG, "OnCheckConnect");

    pEvent = new JEvent(JEVT_LOG_CHECK_CONNECT);
    if (pEvent)
    {
        pEvent->SetFromProc(JS_P_JMAINPROC);
        pEvent->SetFromThrd(JS_T_JMAINTHREAD);
        pEvent->SetFromMod(JS_M_JLOG);
        pEvent->SetToProc(JS_P_JMAINPROC);
        pEvent->SetToThrd(JS_T_JMAINTHREAD);
        pEvent->SetToMod(JS_M_JLOG);
        pEvent->SetBody(JNULL);

        pThread = JSingleton<JThreadManager>::instance()->GetThread(JS_T_JMAINTHREAD);
        pModuleThread = dynamic_cast<JModuleThread*>(pThread);
        if (pModuleThread)
        {
    	    pComEngine = pModuleThread->GetNotifyCommEngine();
    	    if (pComEngine)
    	    {
    	        pComEngine->SendEvent(pEvent);
    	    }
	    }
    }

	return JSUCCESS;
}

JLog::JLog(JLOG_OUTPUT eMethod):m_outputMethod(eMethod)
{
    JUINT32 idx = 0;

    m_currModule = JLOG_MOD_NONE;
    m_currLevel = JLOG_INFO_LEVEL;

    for (idx=0; idx<JLOG_MAX_MOD_NUM; idx++)
        m_modLvlThres[idx] = JLOG_ERROR_LEVEL;

    SafeMemset(reinterpret_cast<JCHAR*>(m_funcTrace), 0, 
        JLOG_MAX_THREAD_NUM*JLOG_MAX_TRACE*sizeof(JCHAR*));
    SafeMemset(reinterpret_cast<JCHAR*>(m_currOffset), 0, 
        JLOG_MAX_THREAD_NUM*sizeof(JUINT32));
    SafeMemset(reinterpret_cast<JCHAR*>(m_threadID), 0, 
        JLOG_MAX_THREAD_NUM*sizeof(JUINT32));

    m_uiPersistenceType = JSER_PERSISTENCE_NONE;
    m_pSerialization = JNULL;

    m_pOutputLocal = JNULL;
    m_pOutputFile = JNULL;
    m_pOutputRemote = JNULL;

    if ((m_outputMethod & JLOG_OUTPUT_LOCAL) != 0)
    {
        m_pOutputLocal= new JLogOutputLocal();
    }

    if ((m_outputMethod & JLOG_OUTPUT_FILE) != 0)
    {
        m_pOutputFile = new JLogOutputFile(JLOG_FILE_NAME);
    }

    m_timeHandler = JNULL;

	return;
}

JLog::~JLog()
{
    if ((m_outputMethod & JLOG_OUTPUT_LOCAL) != 0 && 
        m_pOutputLocal)
    {
        delete m_pOutputLocal;
        m_pOutputLocal = JNULL;
    }

    if ((m_outputMethod & JLOG_OUTPUT_FILE) != 0 && 
        m_pOutputFile)
    {
        delete m_pOutputFile;
        m_pOutputFile = JNULL;
    }

    if ((m_outputMethod & JLOG_OUTPUT_REMOTE) != 0 && 
        m_pOutputRemote)
    {
        delete m_pOutputRemote;
        m_pOutputRemote = JNULL;
    }

    if (m_pSerialization)
    {
        delete m_pSerialization;
    }

    if (m_timeHandler)
    {
        JSingleton<JTimer>::instance()->StopTimer(m_timeHandler);
        JSingleton<JTimer>::instance()->DestoryTimer(m_timeHandler);
        m_timeHandler = JNULL;
    }

	return;
}

JLog& JLog::operator<<(const JFmt& clsFmt)
{
    JAutoPtr<JLock> clsLockAutoPtr(&m_PrintLock);

    m_currModule = clsFmt.m_Module;
    m_currLevel = clsFmt.m_Level;

    return *this;
}

JLog& JLog::operator<<(JCHAR* ptrBuf)
{
    JAutoPtr<JLock> clsLockAutoPtr(&m_PrintLock);

    Print(m_currModule, m_currLevel, ptrBuf);

    return *this;
}

JLog& JLog::operator<<(const JUINT8 uiValue)
{
    JCHAR strBuf[JLOG_MAX_BUF_LEN] = {0};

    JAutoPtr<JLock> clsLockAutoPtr(&m_PrintLock);

    SafeSprintf(strBuf, JLOG_MAX_BUF_LEN, "%u", uiValue);

    Print(m_currModule, m_currLevel, strBuf);

    return *this;
}

JLog& JLog::operator<<(const JUINT16 uiValue)
{
    JCHAR strBuf[JLOG_MAX_BUF_LEN] = {0};

    JAutoPtr<JLock> clsLockAutoPtr(&m_PrintLock);

    SafeSprintf(strBuf, JLOG_MAX_BUF_LEN, "%u", uiValue);

    Print(m_currModule, m_currLevel, strBuf);

    return *this;
}

JLog& JLog::operator<<(const JUINT32 uiValue)
{
    JCHAR strBuf[JLOG_MAX_BUF_LEN] = {0};

    JAutoPtr<JLock> clsLockAutoPtr(&m_PrintLock);

    SafeSprintf(strBuf, JLOG_MAX_BUF_LEN, "%u", uiValue);

    Print(m_currModule, m_currLevel, strBuf);

    return *this;
}

JLog& JLog::operator<<(const JINT8 iValue)
{
    JCHAR strBuf[JLOG_MAX_BUF_LEN] = {0};

    JAutoPtr<JLock> clsLockAutoPtr(&m_PrintLock);

    SafeSprintf(strBuf, JLOG_MAX_BUF_LEN, "%d", iValue);

    Print(m_currModule, m_currLevel, strBuf);

    return *this;
}

JLog& JLog::operator<<(const JINT16 iValue)
{
    JCHAR strBuf[JLOG_MAX_BUF_LEN] = {0};

    JAutoPtr<JLock> clsLockAutoPtr(&m_PrintLock);

    SafeSprintf(strBuf, JLOG_MAX_BUF_LEN, "%d", iValue);

    Print(m_currModule, m_currLevel, strBuf);

    return *this;
}

JLog& JLog::operator<<(const JINT32 iValue)
{
    JCHAR strBuf[JLOG_MAX_BUF_LEN] = {0};

    JAutoPtr<JLock> clsLockAutoPtr(&m_PrintLock);

    SafeSprintf(strBuf, JLOG_MAX_BUF_LEN, "%d", iValue);

    Print(m_currModule, m_currLevel, strBuf);

    return *this;
}

JUINT32 JLog::SetLogLevel(const JLOG_MODULE uiModule, const JLOG_LEVEL uiValue)
{
    if (uiModule == JLOG_MOD_ALL)
    {
        JUINT32 idx = 0;

        for (idx=0; idx<JLOG_MAX_MOD_NUM; idx++)
            m_modLvlThres[idx] = uiValue;

        return JSUCCESS;
    }

    m_modLvlThres[uiModule-1] = uiValue;

	return JSUCCESS;
}

JUINT32 JLog::SetLogMethod(const JLOG_OUTPUT uiOutput)
{
    JUINT32 uiPort = 0;
    JString strLclAddr;

    if ((uiOutput & JLOG_OUTPUT_LOCAL) != 0)
    {
        m_outputMethod |= JLOG_OUTPUT_LOCAL;

        if (!m_pOutputLocal)
        {
            m_pOutputLocal = new JLogOutputLocal();
        }
    }

    if ((uiOutput & JLOG_OUTPUT_FILE) != 0)
    {
        m_outputMethod |= JLOG_OUTPUT_FILE;

        if (!m_pOutputFile)
        {
            m_pOutputFile = new JLogOutputFile(JLOG_FILE_NAME);
        }
    }

    if ((uiOutput & JLOG_OUTPUT_REMOTE) != 0)
    {
        m_outputMethod |= JLOG_OUTPUT_REMOTE;

        if (!m_pOutputRemote)
        {
            strLclAddr = JLOG_LOCAL_ADDR;
            uiPort = atoi(m_cfg.GetLogPort().c_str());
            m_pOutputRemote = new JLogOutputRemote(strLclAddr, JLOG_LOCAL_PORT, 
                m_cfg.GetLogAddress(), uiPort);
            if (m_pOutputRemote && !m_pOutputRemote->GetCommEngine())
            {
                delete m_pOutputRemote;
                m_pOutputRemote = JNULL;
            }
            StartCheckConnect();
        }
    }

	return JSUCCESS;
}

JUINT32 JLog::UnSetLogMethod(const JLOG_OUTPUT uiOutput)
{
    if ((uiOutput & JLOG_OUTPUT_LOCAL) != 0)
    {
        m_outputMethod &= ~JLOG_OUTPUT_LOCAL;

        if (m_pOutputLocal)
        {
            delete m_pOutputLocal;
            m_pOutputLocal = JNULL;
        }
    }

    if ((uiOutput & JLOG_OUTPUT_FILE) != 0)
    {
        m_outputMethod &= ~JLOG_OUTPUT_FILE;

        if (m_pOutputFile)
        {
            delete m_pOutputFile;
            m_pOutputFile = JNULL;
        }
    }

    if ((uiOutput & JLOG_OUTPUT_REMOTE) != 0)
    {
        m_outputMethod &= ~JLOG_OUTPUT_REMOTE;

        StopCheckConnect();
        if (m_pOutputRemote)
        {
            delete m_pOutputRemote;
            m_pOutputRemote = JNULL;
        }
    }

	return JSUCCESS;
}

JUINT32 JLog::SetFuncEnterTrace(const JLOG_MODULE module, JCHAR* ptrFuncName)
{
    JCHAR strBuf[JLOG_MAX_BUF_LEN] = {0};
    JUINT32 uiLen = 0;
    JUINT32 uiThreadId = 0;
    JINT32 iTmpOffset = 0;
    JINT32 iRet = 0;

	if (!ptrFuncName || !SafeStrlen(ptrFuncName))
	{
		Print(JLOG_MOD_LOG, JLOG_ERROR_LEVEL, "JLog::SetFuncEnterTrace input param is NULL\n");
	}

    JAutoPtr<JLock> clsLockAutoPtr(&m_TraceLock);

    SafeSprintf(strBuf, JLOG_MAX_BUF_LEN, "FUNC %s enter\n", ptrFuncName);

    Print(module, JLOG_ENTER_LEVEL, strBuf);

    iRet = FindThreadId(&uiThreadId);
    if (iRet == JSUCCESS || iRet == JFAILURE)
    {
        iTmpOffset = m_currOffset[uiThreadId];
        uiLen = SafeStrlen(ptrFuncName);
        m_funcTrace[uiThreadId][iTmpOffset] = new char[uiLen+1];
        if (m_funcTrace[uiThreadId][iTmpOffset])
        {
            SafeMemset(m_funcTrace[uiThreadId][iTmpOffset], 0, uiLen+1);
	        SafeStrcpy(m_funcTrace[uiThreadId][iTmpOffset], ptrFuncName, uiLen+1);
            m_currOffset[uiThreadId]++;
        }
        else
        {
            Print(JLOG_MOD_LOG, JLOG_ERROR_LEVEL, "JLog::SetFuncEnterTrace Alloc memory failure\n");
        }
    }

	return JSUCCESS;
}

JUINT32 JLog::SetFuncExitTrace(const JLOG_MODULE module, JCHAR* ptrFuncName)
{
    JCHAR strBuf[JLOG_MAX_BUF_LEN] = {0};
    JINT32 iTmpOffset = 0;
    JUINT32 uiIdx = 0;
    JBOOL bMatch = JFALSE;
    JUINT32 uiThreadId = 0;
    JINT32 iRet = 0;

	if (!ptrFuncName || !SafeStrlen(ptrFuncName))
	{
		Print(JLOG_MOD_LOG, JLOG_ERROR_LEVEL, "JLog::SetFuncExitTrace input param is NULL\n");
	}

    JAutoPtr<JLock> clsLockAutoPtr(&m_TraceLock);

    SafeSprintf(strBuf, JLOG_MAX_BUF_LEN, "FUNC %s exit\n", ptrFuncName);

    Print(module, JLOG_EXIT_LEVEL, strBuf);

    iRet = FindThreadId(&uiThreadId);
    if (iRet == JSUCCESS)
    {
        iTmpOffset = m_currOffset[uiThreadId]-1;
        while (iTmpOffset>=0)
        {
            if (m_funcTrace[uiThreadId][iTmpOffset] && 
                SafeStrcmp(m_funcTrace[uiThreadId][iTmpOffset], ptrFuncName) == 0)
            {
                for (uiIdx=0; uiIdx<(m_currOffset[uiThreadId]-iTmpOffset); uiIdx++)
                {
                    delete [] m_funcTrace[uiThreadId][iTmpOffset+uiIdx];
                    m_funcTrace[uiThreadId][iTmpOffset+uiIdx] = JNULL;
                }

                bMatch = JTRUE;
                break;
            }
            else
            {
                iTmpOffset--;
                SafeSprintf(strBuf, JLOG_MAX_BUF_LEN, "JLog::SetFuncExitTrace %s mismatch\n", ptrFuncName);
                Print(JLOG_MOD_LOG, JLOG_ERROR_LEVEL, strBuf);
            }
        }

        if (bMatch)
            m_currOffset[uiThreadId] = iTmpOffset;
    }

	return JSUCCESS;
}

JUINT32 JLog::GetFuncTrace(JUINT32 uiMaxNum, JCHAR* ucFuncTrace[])
{
    JUINT32 uiTrueNeedFuncTrace = 0;
    JUINT32 uiIdx = 0;
    JUINT32 uiOffset = 0;
    JUINT32 uiLen = 0;
    JUINT32 uiThreadId = 0;
    JINT32 iRet = 0;

    JAutoPtr<JLock> clsLockAutoPtr(&m_TraceLock);

    iRet = FindThreadId(&uiThreadId);
    if (iRet == JSUCCESS)
    {
        uiTrueNeedFuncTrace = uiMaxNum;
        if (m_currOffset[uiThreadId] < uiMaxNum)
            uiTrueNeedFuncTrace = m_currOffset[uiThreadId];

        for (uiOffset=m_currOffset[uiThreadId]-1, uiIdx=0; 
            uiIdx<uiTrueNeedFuncTrace; 
            uiOffset--, uiIdx++)
        {
            if (m_funcTrace[uiThreadId][uiOffset])
            {
                uiLen = SafeStrlen(m_funcTrace[uiThreadId][uiOffset]);
                ucFuncTrace[uiIdx] = new char[uiLen+1];
                if (ucFuncTrace[uiIdx])
                {
		            SafeMemset(ucFuncTrace[uiIdx], 0, uiLen+1);
	                SafeMemcpy(ucFuncTrace[uiIdx], m_funcTrace[uiThreadId][uiOffset], uiLen, uiLen+1);
                }
            }
        }
    }

	return JSUCCESS;
}

JUINT32 JLog::SetPersistenceType(JUINT32 uiType)
{
    if (uiType != JSER_PERSISTENCE_SQL && 
        uiType != JSER_PERSISTENCE_XML)
    {
        Print(JLOG_MOD_LOG, JLOG_ERROR_LEVEL, "JLog::SetPersistenceType input param is wrong\n");
        return JFAILURE;
    }

    m_uiPersistenceType = uiType;

    return JSUCCESS;
}

JUINT32 JLog::InitFunc()
{
    m_pSerialization = new JSerialization(static_cast<JSER_PERSISTENCE_TYPE>(m_uiPersistenceType));

    if (m_pSerialization)
    {
        GetCfgList(&m_cfg);

        if (m_cfg.GetOutputFile() == JLOG_FILE_DISABLE)
        {
            UnSetLogMethod(JLOG_OUTPUT_FILE);
        }
        else if (m_cfg.GetOutputFile() == JLOG_FILE_ENABLE)
        {
            SetLogMethod(JLOG_OUTPUT_FILE);
        }

        if (m_cfg.GetOutputRemote() == JLOG_REMOTE_DISABLE)
        {
            UnSetLogMethod(JLOG_OUTPUT_REMOTE);
        }
        else if (m_cfg.GetOutputRemote() == JLOG_REMOTE_ENABLE)
        {
            SetLogMethod(JLOG_OUTPUT_REMOTE);
        }
    }

    return JSUCCESS;
}

JUINT32 JLog::EventProcFunc(JEvent* pEvent)
{
    JUINT32 uiRet = JFAILURE;
    JEVT_TYPE type = JEVT_NONE;
    JLogCfg* pLogCfg = JNULL;

    JAutoPtr<JLock> clsLockAutoPtr(&m_EventLock);

    if (!pEvent)
    {
        Print(JLOG_MOD_LOG, JLOG_ERROR_LEVEL, "JLog::EventProcFunc input param is NULL\n");
        return JFAILURE;
    }

    type = pEvent->GetEventType();
    switch(type)
    {
        case JEVT_LOG_SET_CFG:
        {
            pLogCfg = dynamic_cast<JLogCfg*>(pEvent->GetBody());
            if (pLogCfg)
            {
                uiRet = ProcSetCfgEvent(pLogCfg);
            }
        }
        break;

        case JEVT_LOG_GET_CFG:
        {
            uiRet = ProcGetCfgEvent(pEvent);
        }
        break;

        case JEVT_LOG_CHECK_CONNECT:
        {
            DoCheckConnect();
        }
        break;

        default:
        break;
    };

	return JSUCCESS;
}

JEventBody* JLog::MakeEventBody(JUINT32 uiType)
{
	JEventBody* pEventBody = JNULL;

	switch(uiType)
	{
		case JEVT_LOG_SET_CFG:
		case JEVT_LOG_GET_CFG_RSP:
			pEventBody = new JLogCfg;
			break;

		default:
			break;
	};

	return pEventBody;
}

JUINT32 JLog::ProcSetCfgEvent(JLogCfg* pLogCfg)
{
    JBOOL bChanged = JFALSE;

    if (!pLogCfg)
    {
        Print(JLOG_MOD_LOG, JLOG_ERROR_LEVEL, "JLog::ProcSetCfgEvent pLogCfg is NULL\n");
        return JFAILURE;
    }

    if (pLogCfg->GetLogAddress() != m_cfg.GetLogAddress())
    {
        m_cfg.SetLogAddress(pLogCfg->GetLogAddress());
        bChanged = JTRUE;
    }

    if (pLogCfg->GetLogPort() != m_cfg.GetLogPort())
    {
        m_cfg.SetLogPort(pLogCfg->GetLogPort());
        bChanged = JTRUE;
    }

    if (pLogCfg->GetOutputFile() != m_cfg.GetOutputFile())
    {
        m_cfg.SetOutputFile(pLogCfg->GetOutputFile());
        if (m_cfg.GetOutputFile() == JLOG_FILE_DISABLE)
        {
            UnSetLogMethod(JLOG_OUTPUT_FILE);
        }
        else if (m_cfg.GetOutputFile() == JLOG_FILE_ENABLE)
        {
            SetLogMethod(JLOG_OUTPUT_FILE);
        }
        bChanged = JTRUE;
    }

    if (pLogCfg->GetOutputRemote() != m_cfg.GetOutputRemote())
    {
        m_cfg.SetOutputRemote(pLogCfg->GetOutputRemote());
        if (m_cfg.GetOutputRemote() == JLOG_REMOTE_DISABLE)
        {
            UnSetLogMethod(JLOG_OUTPUT_REMOTE);
        }
        else if (m_cfg.GetOutputRemote() == JLOG_REMOTE_ENABLE)
        {
            SetLogMethod(JLOG_OUTPUT_REMOTE);
        }
        bChanged = JTRUE;
    }

    if (bChanged)
    {
        SetCfgList(&m_cfg);
    }

	return JSUCCESS;
}

JUINT32 JLog::ProcGetCfgEvent(JEvent* pEvent)
{
    JEvent* pNewEvent = JNULL;
    JLogCfg* pLogCfg = JNULL;
    JCommEngine* pComEngine = JNULL;
    JThread* pThread = JNULL;
    JModuleThread* pModuleThread = JNULL;

    pNewEvent = new JEvent(JEVT_LOG_GET_CFG_RSP);
    pLogCfg = new JLogCfg;
    if (pNewEvent && pLogCfg)
    {
        pLogCfg->SetLogAddress(m_cfg.GetLogAddress());
        pLogCfg->SetLogPort(m_cfg.GetLogPort());
        pLogCfg->SetOutputFile(m_cfg.GetOutputFile());
        pLogCfg->SetOutputRemote(m_cfg.GetOutputRemote());

	    pNewEvent->SetFromProc(pEvent->GetToProc().c_str());
	    pNewEvent->SetFromThrd(pEvent->GetToThrd().c_str());
	    pNewEvent->SetFromMod(pEvent->GetToMod().c_str());
	    pNewEvent->SetToProc(pEvent->GetFromProc().c_str());
	    pNewEvent->SetToThrd(pEvent->GetFromThrd().c_str());
	    pNewEvent->SetToMod(pEvent->GetFromMod().c_str());
	    pNewEvent->SetBody(pLogCfg);

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

JUINT32 JLog::SetCfgList(JLogCfg* pLogCfg)
{
    JUINT32 uiLen = 0;
    JList<JPER_RECORD> clsDstList;
    JListItem<JPER_RECORD>* pDstItem = JNULL;
    JListItem<JPER_RECORD>* prevDstItem = JNULL;
    JPER_RECORD* pDstData = JNULL;

    if (m_pSerialization)
    {
        uiLen = pLogCfg->GetLogAddress().GetLength();
        if (uiLen)
        {
            pDstData = 
                reinterpret_cast<JPER_RECORD*>(JSingleton<JStaticMemory>::instance()->Alloc(sizeof(JPER_RECORD)+1));
            if (pDstData)
            {
	            SafeMemset(reinterpret_cast<JCHAR*>(pDstData), 0, sizeof(JPER_RECORD)+1);
                SafeSprintf(pDstData->strKey, JMAX_STRING_LEN, "%s", JLOG_ADDRESS);
                SafeStrcpy(pDstData->strValue, pLogCfg->GetLogAddress().c_str(), uiLen+1);
                pDstItem = new JListItem<JPER_RECORD>(pDstData);
                pDstItem->SetDataLength(sizeof(JPER_RECORD));
                clsDstList.InsertItem(pDstItem, prevDstItem);
                prevDstItem = pDstItem;
            }
            else
            {
                Print(JLOG_MOD_LOG, JLOG_ERROR_LEVEL, "JLog::SetCfgList memory alloc failure\n");
                return JFAILURE;
            }
        }

        uiLen = pLogCfg->GetLogPort().GetLength();
        if (uiLen)
        {
            pDstData = 
                reinterpret_cast<JPER_RECORD*>(JSingleton<JStaticMemory>::instance()->Alloc(sizeof(JPER_RECORD)+1));
            if (pDstData)
            {
	            SafeMemset(reinterpret_cast<JCHAR*>(pDstData), 0, sizeof(JPER_RECORD)+1);
                SafeSprintf(pDstData->strKey, JMAX_STRING_LEN, "%s", JLOG_PORT);
                SafeStrcpy(pDstData->strValue, pLogCfg->GetLogPort().c_str(), uiLen+1);
                pDstItem = new JListItem<JPER_RECORD>(pDstData);
                pDstItem->SetDataLength(sizeof(JPER_RECORD));
                clsDstList.InsertItem(pDstItem, prevDstItem);
                prevDstItem = pDstItem;
            }
            else
            {
                Print(JLOG_MOD_LOG, JLOG_ERROR_LEVEL, "JLog::SetCfgList memory alloc failure\n");
                return JFAILURE;
            }
        }

        uiLen = pLogCfg->GetOutputFile().GetLength();
        if (uiLen)
        {
            pDstData = 
                reinterpret_cast<JPER_RECORD*>(JSingleton<JStaticMemory>::instance()->Alloc(sizeof(JPER_RECORD)+1));
            if (pDstData)
            {
	            SafeMemset(reinterpret_cast<JCHAR*>(pDstData), 0, sizeof(JPER_RECORD)+1);
                SafeSprintf(pDstData->strKey, JMAX_STRING_LEN, "%s", JLOG_FILE);
                SafeStrcpy(pDstData->strValue, pLogCfg->GetOutputFile().c_str(), uiLen+1);
                pDstItem = new JListItem<JPER_RECORD>(pDstData);
                pDstItem->SetDataLength(sizeof(JPER_RECORD));
                clsDstList.InsertItem(pDstItem, prevDstItem);
                prevDstItem = pDstItem;
            }
            else
            {
                Print(JLOG_MOD_LOG, JLOG_ERROR_LEVEL, "JLog::SetCfgList memory alloc failure\n");
                return JFAILURE;
            }
        }

        uiLen = pLogCfg->GetOutputRemote().GetLength();
        if (uiLen)
        {
            pDstData = 
                reinterpret_cast<JPER_RECORD*>(JSingleton<JStaticMemory>::instance()->Alloc(sizeof(JPER_RECORD)+1));
            if (pDstData)
            {
	            SafeMemset(reinterpret_cast<JCHAR*>(pDstData), 0, sizeof(JPER_RECORD)+1);
                SafeSprintf(pDstData->strKey, JMAX_STRING_LEN, "%s", JLOG_REMOTE);
                SafeStrcpy(pDstData->strValue, pLogCfg->GetOutputRemote().c_str(), uiLen+1);
                pDstItem = new JListItem<JPER_RECORD>(pDstData);
                pDstItem->SetDataLength(sizeof(JPER_RECORD));
                clsDstList.InsertItem(pDstItem, prevDstItem);
                prevDstItem = pDstItem;
            }
            else
            {
                Print(JLOG_MOD_LOG, JLOG_ERROR_LEVEL, "JLog::SetCfgList memory alloc failure\n");
                return JFAILURE;
            }
        }

        m_pSerialization->SetList(clsDstList, JLOG_CFG_FILE);
    }

	return JSUCCESS;
}

JUINT32 JLog::GetCfgList(JLogCfg* pLogCfg)
{
    JUINT32 uiLen = 0;
    JUINT32 uiLen2 = 0;
    JList<JPER_RECORD> clsSrcList;
    JListItem<JPER_RECORD>* pSrcItem = JNULL;
    JPER_RECORD* pSrcData = JNULL;

    if (m_pSerialization)
    {
        clsSrcList = m_pSerialization->GetList(JLOG_CFG_FILE);

        JListIterator<JPER_RECORD> clsListIter(clsSrcList);
        for (clsListIter.First(); clsListIter.Done(); clsListIter.Next())
        {
            pSrcItem = clsListIter.Item();
            pSrcData = pSrcItem->GetData();

            uiLen = SafeStrlen(pSrcData->strKey);
            uiLen2 = SafeStrlen(pSrcData->strValue);
            if (uiLen && uiLen2)
            {
                if (SafeStrcmp(pSrcData->strKey, JLOG_ADDRESS) == 0)
                {
                    pLogCfg->GetLogAddress() = pSrcData->strValue;
                }
                else if (SafeStrcmp(pSrcData->strKey, JLOG_PORT) == 0)
                {
                    pLogCfg->GetLogPort() = pSrcData->strValue;
                }
                else if (SafeStrcmp(pSrcData->strKey, JLOG_FILE) == 0)
                {
                    pLogCfg->GetOutputFile() = pSrcData->strValue;
                }
                else if (SafeStrcmp(pSrcData->strKey, JLOG_REMOTE) == 0)
                {
                    pLogCfg->GetOutputRemote() = pSrcData->strValue;
                }
            }
        }
    }

	return JSUCCESS;
}

JUINT32 JLog::DoCheckConnect()
{
    JUINT32 uiRet = JFAILURE;
    JINT32 iLen = 0;
    JUINT32 uiPort = 0;
    JString strLclAddr;
    JCommEngine* pCommEngine = JNULL;
    JCHAR strBuf[JMAX_STRING_LEN] = {0};
    JSOCKADDR_IN stAddr;

    if (!m_pOutputRemote)
    {
        strLclAddr = JLOG_LOCAL_ADDR;
        uiPort = atoi(m_cfg.GetLogPort().c_str());
        m_pOutputRemote = new JLogOutputRemote(strLclAddr, JLOG_LOCAL_PORT, 
            m_cfg.GetLogAddress(), uiPort);
        if (m_pOutputRemote && !m_pOutputRemote->GetCommEngine())
        {
            delete m_pOutputRemote;
            m_pOutputRemote = JNULL;
        }
    }
    else
    {
        pCommEngine = m_pOutputRemote->GetCommEngine();
        if (pCommEngine)
        {
            uiRet = pCommEngine->HasMessage(JLOG_SELECT_TIME);
            if (uiRet)
            {
                SafeMemset(strBuf, 0, JMAX_STRING_LEN);
                iLen = pCommEngine->RecvMessage(strBuf, JMAX_STRING_LEN, &stAddr);
                if (iLen == 0)
                {
                    JSingleton<JLog>::instance2() << set(JLOG_MOD_LOG, JLOG_ERROR_LEVEL) 
                        << "JLog::DoCheckConnect connect break "
                        << errno;
                    delete m_pOutputRemote;
                    m_pOutputRemote = JNULL;
                }
            }
        }
    }

	return JSUCCESS;
}

JUINT32 JLog::Print(const JLOG_MODULE mod, const JLOG_LEVEL level, JCHAR* ptrBuf)
{
    JUINT32 uiRet = JFAILURE;

    if (!ptrBuf)
    {
        cout << "JLog::Print input param is NULL" << endl;
        return JFAILURE;
    }

    if (m_modLvlThres[mod-1] > level)
    {
        return JSUCCESS;
    }

    if ((m_outputMethod & JLOG_OUTPUT_LOCAL) != 0 && m_pOutputLocal)
    {
        m_pOutputLocal->Print(ptrBuf);
    }

    if ((m_outputMethod & JLOG_OUTPUT_FILE) != 0 && m_pOutputFile)
    {
        m_pOutputFile->Print(ptrBuf);
    }

    if ((m_outputMethod & JLOG_OUTPUT_REMOTE) != 0 && m_pOutputRemote)
    {
        uiRet = m_pOutputRemote->Print(ptrBuf);
    }

    return JSUCCESS;
}

JUINT32 JLog::GetCurrentThreadId()
{
    return ::GetCurrentThreadId();
}

JINT32 JLog::FindThreadId(JUINT32* uiThreadId)
{
    JUINT32 uiIdx = 0;
    JUINT32 uiThread = 0;

    uiThread = GetCurrentThreadId();
    for (uiIdx=0; uiIdx<JLOG_MAX_THREAD_NUM; uiIdx++)
    {
        if (m_threadID[uiIdx])
        {
            if (m_threadID[uiIdx] == uiThread)
            {
                *uiThreadId = uiIdx;
                return JSUCCESS;
            }
        }
        else
        {
            m_threadID[uiIdx] = uiThread;
            *uiThreadId = uiIdx;
            return JFAILURE;
        }
    }

    *uiThreadId = JLOG_MAX_THREAD_NUM;
    return JINVALID;
}

JUINT32 JLog::StartCheckConnect()
{
    if (!m_timeHandler)
    {
        m_timeHandler = JSingleton<JTimer>::instance()->CreateTimer(JTIMER_TYPE_PERIODIC, 
                                            OnCheckConnect, 
                                            this, 
                                            JLOG_CHECK_CONNECT_INTERVAL);
        JSingleton<JTimer>::instance()->StartTimer(m_timeHandler);
    }

    return JSUCCESS;
}

JUINT32 JLog::StopCheckConnect()
{
    if (m_timeHandler)
    {
        JSingleton<JTimer>::instance()->StopTimer(m_timeHandler);
        JSingleton<JTimer>::instance()->DestoryTimer(m_timeHandler);
        m_timeHandler = JNULL;
    }

    return JSUCCESS;
}

JLogAutoPtr::JLogAutoPtr(JLog* pObject, 
                        const JLOG_MODULE module, 
                        JCHAR* ptrFuncName):
                        JAutoPtrBase<JLog>(pObject)
{
    m_traceModule = module;

	if (!ptrFuncName || !SafeStrlen(ptrFuncName))
	{
		return;
	}

	SafeStrcpy(m_traceFuncName, ptrFuncName, JMAX_STRING_LEN);

    if (this->GetObject())
    {
        this->GetObject()->SetFuncEnterTrace(m_traceModule, m_traceFuncName);
    }

    return;
}

JLogAutoPtr::~JLogAutoPtr()
{
    if (this->GetObject())
    {
        this->GetObject()->SetFuncExitTrace(m_traceModule, m_traceFuncName);
    }

    return;
}

}

