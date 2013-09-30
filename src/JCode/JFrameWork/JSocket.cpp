/**********************************************************
* Author: weiganyi
* File: JSocket.cpp
* Date: 20110718
* Description: 
*
**********************************************************/

#include "JFrameWork.h"


namespace JFrameWork{

JSocket::JSocket()
{
    m_fd = JNULL;

	return;
}

JSocket::~JSocket()
{
	return;
}

JSOCKET JSocket::GetFd()
{
    return m_fd;
}

JUdpSocket::JUdpSocket()
{
    SafeMemset(reinterpret_cast<JCHAR*>(&m_rmtAddr), 0, sizeof(JSOCKADDR_IN));

	return;
}

JUdpSocket::~JUdpSocket()
{
	return;
}

JSOCKET JUdpSocket::Create(JSOCKET fd)
{
    JINT32 iRet = JFALSE;

    if (!m_fd)
    {
        m_fd = socket(AF_INET, SOCK_DGRAM, 0);
        if (m_fd == INVALID_SOCKET)
        {
            iRet = WSAGetLastError();
            cout << "JUdpSocket::Create socket return error "
                << iRet
                << endl;
            return 0;
        }
        else
        {
            return m_fd;
        }
    }
    else
    {
        m_fd = fd;
        return m_fd;
    }

    return JNULL;
}

JVOID JUdpSocket::Destroy()
{
    if (m_fd != JNULL)
    {
        shutdown(m_fd, 0);
    }

    return;
}

JINT32 JUdpSocket::SetLocalAddr(JSOCKADDR_IN* pAddr)
{
    JINT32 iRet = 0;

    if (!pAddr)
    {
        cout << "JUdpSocket::SetLocalAddr input param is NULL" << endl;
        return JFAILURE;
    }

    iRet = bind(m_fd, (JSOCKADDR *)pAddr, sizeof(JSOCKADDR_IN));

    return iRet;
}

JUINT32 JUdpSocket::SetRemoteAddr(JSOCKADDR_IN* pAddr)
{
    if (!pAddr)
    {
        cout << "JUdpSocket::SetRemoteAddr input param is NULL"<< endl;
        return JFAILURE;
    }

    SafeMemcpy(reinterpret_cast<JCHAR*>(&m_rmtAddr), reinterpret_cast<const JCHAR*>(pAddr), sizeof(JSOCKADDR_IN), sizeof(JSOCKADDR_IN));

    return JSUCCESS;
}

JSOCKADDR_IN* JUdpSocket::GetRemoteAddr()
{
    return &m_rmtAddr;
}

JINT32 JUdpSocket::Select(const JUINT32 uiMilisecond)
{
    JINT32 iRet = 0;
    fd_set readfds;
    UINT32 max_fd = 0;
    JTIMEVAL timeVal;
    JTIMEVAL* pTimeVal = JNULL;

    FD_ZERO(&readfds);
    FD_SET(m_fd, &readfds);
    max_fd = m_fd;

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_SOCKET, "JUdpSocket::Select");

    if (uiMilisecond)
    {
        timeVal.tv_sec = 0;
        timeVal.tv_usec = uiMilisecond;
        pTimeVal = &timeVal;
    }
    else
    {
        pTimeVal = JNULL;
    }

    iRet = select(max_fd+1, &readfds, NULL, NULL, pTimeVal); 
    if (FD_ISSET(max_fd, &readfds))
    {
        return JSUCCESS;
    }

    return JFAILURE;
}

JINT32 JUdpSocket::SendBuf(const JCHAR* ptrBuf, const JUINT32 uiLen)
{
    JINT32 iSendLen = 0;

    if (!ptrBuf)
    {
        cout << "JUdpSocket::SendBuf input param is NULL" << endl;
        return JNULL;
    }

    iSendLen = sendto(m_fd, ptrBuf, uiLen, 0, reinterpret_cast<JSOCKADDR*>(&m_rmtAddr), sizeof(JSOCKADDR_IN));
    if (SOCKET_ERROR == iSendLen)
    {
        cout << "JUdpSocket::SendBuf sendto failure, err code is"
            << WSAGetLastError()
            << endl;
        return JNULL;
    }

    return iSendLen;
}

JINT32 JUdpSocket::RecvBuf(JCHAR* ptrBuf, const JUINT32 uiLen, JSOCKADDR_IN* pFrom, JINT32* iFromLen)
{
    JINT32 iRecvLen = 0;

    if (!ptrBuf || !pFrom || !iFromLen)
    {
        cout << "JUdpSocket::RecvBuf input param is NULL"<< endl;
        return JNULL;
    }

    iRecvLen = recvfrom(m_fd, ptrBuf, uiLen, 0, reinterpret_cast<JSOCKADDR*>(pFrom), iFromLen);
    if (SOCKET_ERROR == iRecvLen)
    {
        cout << "JUdpSocket::RecvBuf recvfrom failure, err code is"
            << WSAGetLastError()
            << endl;
        return JNULL;
    }

    return iRecvLen;
}

JTcpSocket::JTcpSocket()
{
    SafeMemset(reinterpret_cast<JCHAR*>(&m_lclAddr), 0, sizeof(JSOCKADDR_IN));
    SafeMemset(reinterpret_cast<JCHAR*>(&m_rmtAddr), 0, sizeof(JSOCKADDR_IN));

	return;
}

JTcpSocket::~JTcpSocket()
{
	return;
}

JSOCKET JTcpSocket::Create(JSOCKET fd)
{
    m_fd = fd;

    return m_fd;
}

JVOID JTcpSocket::Destroy()
{
    if (m_fd != JNULL)
    {
        shutdown(m_fd, 0);
    }

    return;
}

JINT32 JTcpSocket::SetLocalAddr(JSOCKADDR_IN* pAddr)
{
    SafeMemcpy(reinterpret_cast<JCHAR*>(&m_lclAddr), reinterpret_cast<JCHAR*>(pAddr), sizeof(JSOCKADDR_IN), sizeof(JSOCKADDR_IN));

    return JSUCCESS;
}

JUINT32 JTcpSocket::SetRemoteAddr(JSOCKADDR_IN* pAddr)
{
    SafeMemcpy(reinterpret_cast<JCHAR*>(&m_rmtAddr), reinterpret_cast<JCHAR*>(pAddr), sizeof(JSOCKADDR_IN), sizeof(JSOCKADDR_IN));

    return JSUCCESS;
}

JSOCKADDR_IN* JTcpSocket::GetRemoteAddr()
{
    return &m_rmtAddr;
}

JINT32 JTcpSocket::Select(const JUINT32 uiMilisecond)
{
    JINT32 iRet = 0;
    fd_set readfds;
    UINT32 max_fd = 0;
    JTIMEVAL timeVal;
    JTIMEVAL* pTimeVal = JNULL;

    FD_ZERO(&readfds);
    FD_SET(m_fd, &readfds);
    max_fd = m_fd;

    if (uiMilisecond)
    {
        timeVal.tv_sec = 0;
        timeVal.tv_usec = uiMilisecond;
        pTimeVal = &timeVal;
    }
    else
    {
        pTimeVal = JNULL;
    }

    iRet = select(max_fd+1, &readfds, JNULL, JNULL, pTimeVal); 
    if (FD_ISSET(max_fd, &readfds))
    {
        return JSUCCESS;
    }

    return JFAILURE;
}

JINT32 JTcpSocket::SendBuf(const JCHAR* ptrBuf, const JUINT32 uiLen)
{
    JINT32 iSendLen = 0;

    if (!ptrBuf)
    {
        cout << "JTcpSocket::SendBuf input param is NULL" << endl;
        return JNULL;
    }

    iSendLen = send(m_fd, ptrBuf, uiLen, 0);
    if (SOCKET_ERROR == iSendLen)
    {
        cout << "JTcpSocket::SendBuf sendto failure, err code is"
            << WSAGetLastError()
            << endl;
        return JNULL;
    }

    return iSendLen;
}

JINT32 JTcpSocket::RecvBuf(JCHAR* ptrBuf, const JUINT32 uiLen, JSOCKADDR_IN* pFrom, JINT32* iFromLen)
{
    JINT32 iRecvLen = 0;

    if (!ptrBuf || !pFrom || !iFromLen)
    {
        cout << "JTcpSocket::RecvBuf input param is NULL"<< endl;
        return JNULL;
    }

    iRecvLen = recv(m_fd, ptrBuf, uiLen, 0);
    if (SOCKET_ERROR == iRecvLen)
    {
        cout << "JTcpSocket::RecvBuf recvfrom failure, err code is"
            << WSAGetLastError()
            << endl;
        return JNULL;
    }

    if (*iFromLen >= sizeof(JSOCKADDR_IN))
    {
        SafeMemcpy(reinterpret_cast<JCHAR*>(pFrom), reinterpret_cast<JCHAR*>(&m_rmtAddr), 
            sizeof(JSOCKADDR_IN), sizeof(JSOCKADDR_IN));
    }

    return iRecvLen;
}

JCommEngine::JCommEngine(JSOCKET_TYPE eType, JSOCKET ofd):m_eType(eType)
{
    JSOCKET fd = JNULL;

    m_pSocket = JNULL;

    if (m_eType == JSOCKET_UDP)
    {
        m_pSocket = new JUdpSocket();
        if (m_pSocket)
        {
            fd  = m_pSocket->Create();
            if (!fd)
            {
                cout << "JCommEngine::JCommEngine create udp socket failure" << endl;
                delete m_pSocket;
                m_pSocket = JNULL;
            }
        }
    }
    else if (m_eType == JSOCKET_TCP)
    {
        m_pSocket = new JTcpSocket();
        if (m_pSocket)
        {
            fd  = m_pSocket->Create(ofd);
            if (!fd)
            {
                cout << "JCommEngine::JCommEngine create tcp socket failure" << endl;
                delete m_pSocket;
                m_pSocket = JNULL;
            }
        }
    }

	return;
}

JCommEngine::~JCommEngine()
{
    if (m_pSocket)
    {
        m_pSocket->Destroy();
        delete m_pSocket;
    }

	return;
}

JUINT32 JCommEngine::SetLocalAddr(JCHAR* pProcName, 
                                JCHAR* pThrdName, 
                                JSOCKADDR_IN* pAddr)
{
    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_SOCKET, "JCommEngine::SetLocalAddr");

    if (!pProcName && !pThrdName && !pAddr)
    {
        JSingleton<JLog>::instance2() << set(JLOG_MOD_SOCKET, JLOG_ERROR_LEVEL) 
            << "JCommEngine::SetLocalAddr input param is NULL\n";
        return JFAILURE;
    }

    JSingleton<JRoute>::instance()->AddRoute(pProcName, pThrdName, pAddr);

    SetLocalAddr(pAddr);

    return JSUCCESS;
}

JUINT32 JCommEngine::SetLocalAddr(JSOCKADDR_IN* pAddr)
{
    if (!pAddr)
    {
        cout << "JCommEngine::SetLocalAddr input param is NULL" << endl;
        return JFAILURE;
    }

    m_pSocket->SetLocalAddr(pAddr);

    return JSUCCESS;
}

JUINT32 JCommEngine::SetRemoteAddr(JCHAR* pProcName, 
                                JCHAR* pThrdName, 
                                JSOCKADDR_IN* pAddr)
{
    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_SOCKET, "JCommEngine::SetRemoteAddr");

    if (!pProcName && !pThrdName && !pAddr)
    {
        JSingleton<JLog>::instance2() << set(JLOG_MOD_SOCKET, JLOG_ERROR_LEVEL) 
            << "JCommEngine::SetRemoteAddr input param is NULL\n";
        return JFAILURE;
    }

    JSingleton<JRoute>::instance()->AddRoute(pProcName, pThrdName, pAddr);

    SetRemoteAddr(pAddr);

    return JSUCCESS;
}

JUINT32 JCommEngine::SetRemoteAddr(JSOCKADDR_IN* pAddr)
{
    if (!pAddr)
    {
        cout << "JCommEngine::SetRemoteAddr input param is NULL" << endl;
        return JFAILURE;
    }

    m_pSocket->SetRemoteAddr(pAddr);

    return JSUCCESS;
}

JUINT32 JCommEngine::SendEvent(JEvent* pEvent)
{
    JCHAR* pFromProcName = JNULL;
    JCHAR* pFromThrdName = JNULL;
    JCHAR* pToProcName = JNULL;
    JCHAR* pToThrdName = JNULL;
    JCHAR* pToModName = JNULL;
    JUINT32   uiExist = JFAILURE;
    JROUTE_TYPE eType = ROUTE_TYPE_NONE;
    JListItem<JEvent>* pItem = JNULL;
    JUINT32 uiRet = JFAILURE;
    JCHAR* pBuf = JNULL;
    JUINT32 uiBufLen = JNULL;
    JSOCKADDR_IN rmtAddr;

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_SOCKET, "JCommEngine::SendEvent");

    if (!pEvent)
    {
        JSingleton<JLog>::instance2() << set(JLOG_MOD_SOCKET, JLOG_ERROR_LEVEL) 
            << "JCommEngine::SendEvent input param is NULL\n";
        return JFAILURE;
    }

    pFromProcName = pEvent->GetFromProc().c_str();
    pFromThrdName = pEvent->GetFromThrd().c_str();
    pToProcName = pEvent->GetToProc().c_str();
    pToThrdName = pEvent->GetToThrd().c_str();
    pToModName = pEvent->GetToMod().c_str();

    uiExist = JSingleton<JRoute>::instance()->FindRoute(pToProcName, pToThrdName);
    if (uiExist)
    {
        eType = 
            JSingleton<JRoute>::instance()->GetRouteType(pFromProcName, pFromThrdName, pToProcName, pToThrdName);
        if (eType == ROUTE_TYPE_MODULE || eType == ROUTE_TYPE_THREAD)
        {
            pItem = new JListItem<JEvent>(pEvent);

            uiRet = 
                JSingleton<JThreadManager>::instance()->EnQueueEvent(pToThrdName, pToModName, pItem);
            if (uiRet)
            {
                uiRet = 
                    JSingleton<JRoute>::instance()->GetRoute(pToProcName, pToThrdName, &rmtAddr);
                if (uiRet)
                {
                    uiRet = SendMessage("1", 1, &rmtAddr);
                }
            }

            return uiRet;
        }
        else if (eType == ROUTE_TYPE_PROCESS)
        {
            pBuf = pEvent->Serialize(&uiBufLen);
            if (pBuf)
            {
                uiRet = JSingleton<JRoute>::instance()->GetRoute(pToProcName, pToThrdName, &rmtAddr);
                if (uiRet)
                {
                    uiRet = SendMessage(pBuf, uiBufLen, &rmtAddr);
                }

                JSingleton<JStaticMemory>::instance()->Free(pBuf);
                delete pEvent;

                return uiRet;
            }
        }
    }

    return JFAILURE;
}

JINT32 JCommEngine::SendMessage(const JCHAR* pBuf, 
                                const JUINT32 uiTrueLen, 
                                JSOCKADDR_IN* pAddr)
{
    JINT32 iSendLen = 0;

    if (!pBuf || !uiTrueLen)
    {
        cout << "JCommEngine::SendMessage input param is NULL" << endl;
        return JNULL;
    }

    if (pAddr)
    {
        m_pSocket->SetRemoteAddr(pAddr);
    }

    iSendLen = m_pSocket->SendBuf(pBuf, uiTrueLen);

    return iSendLen;
}

JINT32 JCommEngine::RecvMessage(JCHAR* pBuf, JUINT32 uiLen, JSOCKADDR_IN* pAddr)
{
    JINT32 iTrueLen = 0;
    JINT32 uiAddrLen = sizeof(JSOCKADDR_IN);

    if (!pBuf || !pAddr)
    {
        cout << "JCommEngine::RecvMessage input param is NULL" << endl;
        return JNULL;
    }
	
    SafeMemset(pBuf, 0, uiLen);

    iTrueLen = m_pSocket->RecvBuf(pBuf, uiLen, pAddr, &uiAddrLen);

    return iTrueLen;
}

JUINT32 JCommEngine::RecvEvent(JCHAR* pBuf, JUINT32 uiLen, JEvent** ppEvent, JSOCKADDR_IN* pAddr)
{
    JEvent* pNewEvent = JNULL;
    JEVT_TYPE eType = JEVT_NONE;

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_SOCKET, "JCommEngine::RecvEvent");

    if (!pBuf || !pAddr)
    {
        JSingleton<JLog>::instance2() << set(JLOG_MOD_SOCKET, JLOG_ERROR_LEVEL) 
            << "JCommEngine::RecvEvent input param is NULL\n";
        return JFAILURE;
    }

    pNewEvent = new JEvent(JEVT_NONE);
    if (pNewEvent)
    {
        pNewEvent->DeSerialize(pBuf);
        eType = pNewEvent->GetEventType();

        if (eType > JEVT_NONE && eType < JEVT_MAX)
        {
            *ppEvent = pNewEvent;

            return JSUCCESS;
        }
        else
        {
            delete pNewEvent;
        }
    }

    return JFAILURE;
}

JINT32 JCommEngine::HasMessage(JUINT32 uiMilisecond)
{
    JINT32 iRet = 0;

    iRet = m_pSocket->Select(uiMilisecond);

    return iRet;
}

JSocket* JCommEngine::GetSocket()
{
    return m_pSocket;
}

JSOCKADDR_IN* JCommEngine::GetRemoteAddr()
{
    return m_pSocket->GetRemoteAddr();
}

JCommConnector::JCommConnector()
{
    SafeMemset(reinterpret_cast<JCHAR*>(&m_lclAddr), 0, sizeof(JSOCKADDR_IN));
    SafeMemset(reinterpret_cast<JCHAR*>(&m_rmtAddr), 0, sizeof(JSOCKADDR_IN));

    m_fd = JNULL;

    Create();

	return;
}

JCommConnector::~JCommConnector()
{
    Destroy();

	return;
}

JSOCKET JCommConnector::Create()
{
    JINT32 iRet = JFALSE;
    JCHAR bReUse = 1;

    m_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (m_fd == INVALID_SOCKET)
    {
        iRet = WSAGetLastError();
        cout << "JCommConnector::Create socket return error "
            << iRet
            << endl;
        return 0;
    }
    else
    {
        iRet = setsockopt(m_fd, SOL_SOCKET, SO_REUSEADDR, &bReUse, sizeof(bReUse));
        return m_fd;
    }

    return JNULL;
}

JVOID JCommConnector::Destroy()
{
    if (m_fd != JNULL)
    {
        shutdown(m_fd, 0);
    }

    return;
}

JINT32 JCommConnector::SetLocalAddr(JSOCKADDR_IN* pAddr)
{
    JINT32 iRet = 0;

    if (!pAddr)
    {
        cout << "JCommConnector::SetLocalAddr input param is NULL" << endl;
        return JFAILURE;
    }

    SafeMemcpy(reinterpret_cast<JCHAR*>(&m_lclAddr), reinterpret_cast<const JCHAR*>(pAddr), sizeof(JSOCKADDR_IN), sizeof(JSOCKADDR_IN));

    iRet = bind(m_fd, (JSOCKADDR *)pAddr, sizeof(JSOCKADDR_IN));

    return iRet;
}

JUINT32 JCommConnector::SetRemoteAddr(JSOCKADDR_IN* pAddr)
{
    if (!pAddr)
    {
        cout << "JCommConnector::SetRemoteAddr input param is NULL"<< endl;
        return JFAILURE;
    }

    SafeMemcpy(reinterpret_cast<JCHAR*>(&m_rmtAddr), reinterpret_cast<const JCHAR*>(pAddr), sizeof(JSOCKADDR_IN), sizeof(JSOCKADDR_IN));

    return JSUCCESS;
}

JCommEngine* JCommConnector::Connect()
{
    JINT32 iRet = 0;
    JCommEngine* pCommEngine = JNULL;

    iRet = connect(m_fd, reinterpret_cast<JSOCKADDR*>(&m_rmtAddr), sizeof(JSOCKADDR_IN));
    if (iRet != SOCKET_ERROR)
    {
        pCommEngine = new JCommEngine(JSOCKET_TCP, m_fd);
        if (pCommEngine)
        {
            pCommEngine->SetLocalAddr(&m_lclAddr);
            pCommEngine->SetRemoteAddr(&m_rmtAddr);

            return pCommEngine;
        }
    }

    return JNULL;
}

JCommAcceptor::JCommAcceptor()
{
    SafeMemset(reinterpret_cast<JCHAR*>(&m_lclAddr), 0, sizeof(JSOCKADDR_IN));
    SafeMemset(reinterpret_cast<JCHAR*>(&m_rmtAddr), 0, sizeof(JSOCKADDR_IN));

    m_fd = JNULL;

    Create();

	return;
}

JCommAcceptor::~JCommAcceptor()
{
    Destroy();

	return;
}

JSOCKET JCommAcceptor::Create()
{
    JINT32 iRet = JFALSE;

    m_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (m_fd == INVALID_SOCKET)
    {
        iRet = WSAGetLastError();
        cout << "JCommAcceptor::Create socket return error "
            << iRet
            << endl;
        return 0;
    }
    else
    {
        return m_fd;
    }

    return JNULL;
}

JVOID JCommAcceptor::Destroy()
{
    if (m_fd != JNULL)
    {
        shutdown(m_fd, 0);
    }

    return;
}

JINT32 JCommAcceptor::SetLocalAddr(JSOCKADDR_IN* pAddr)
{
    JINT32 iRet = 0;

    if (!pAddr)
    {
        cout << "JCommAcceptor::SetLocalAddr input param is NULL" << endl;
        return JFAILURE;
    }

    SafeMemcpy(reinterpret_cast<JCHAR*>(&m_lclAddr), reinterpret_cast<const JCHAR*>(pAddr), sizeof(JSOCKADDR_IN), sizeof(JSOCKADDR_IN));

    iRet = bind(m_fd, (JSOCKADDR *)pAddr, sizeof(JSOCKADDR_IN));

    return iRet;
}

JINT32 JCommAcceptor::Select(const JUINT32 uiMilisecond)
{
    JINT32 iRet = 0;
    fd_set readfds;
    JUINT32 max_fd = 0;
    JTIMEVAL timeVal;
    JTIMEVAL* pTimeVal = JNULL;

    FD_ZERO(&readfds);
    FD_SET(m_fd, &readfds);
    max_fd = m_fd;

    if (uiMilisecond)
    {
        timeVal.tv_sec = 0;
        timeVal.tv_usec = uiMilisecond;
        pTimeVal = &timeVal;
    }
    else
    {
        pTimeVal = JNULL;
    }

    iRet = select(max_fd+1, &readfds, JNULL, JNULL, pTimeVal); 
    if (FD_ISSET(max_fd, &readfds))
    {
        return JSUCCESS;
    }

    return JFAILURE;
}

JINT32 JCommAcceptor::Listen()
{
    JINT32 iRet = 0;
	
    iRet = listen(m_fd, 10);

    return iRet;
}

JCommEngine* JCommAcceptor::Accept()
{
    JSOCKET fd;
    JCommEngine* pCommEngine = JNULL;
    JINT32 iAddrLen = sizeof(JSOCKADDR_IN);

    fd = accept(m_fd, reinterpret_cast<JSOCKADDR*>(&m_rmtAddr), &iAddrLen);
    if (fd)
    {
        pCommEngine = new JCommEngine(JSOCKET_TCP, fd);
        if (pCommEngine)
        {
            pCommEngine->SetLocalAddr(&m_lclAddr);
            pCommEngine->SetRemoteAddr(&m_rmtAddr);

            return pCommEngine;
        }
    }

    return JNULL;
}

JCommEngineGroup::JCommEngineGroup()
{
    SafeMemset(reinterpret_cast<JCHAR*>(m_pCommEngine), 0, 
        JCOMM_MAX_COMMENGINE*sizeof(JCommEngine*));
    m_uiUsedCommEngine = 0;

	return;
}

JCommEngineGroup::~JCommEngineGroup()
{
	return;
}

JUINT32 JCommEngineGroup::AddCommEngine(JCommEngine* pCommEngine)
{
    JUINT32 uiIdx = 0;

    if (!pCommEngine)
    {
        JSingleton<JLog>::instance2() << set(JLOG_MOD_SOCKET, JLOG_ERROR_LEVEL) 
            << "JCommEngineGroup::AddCommEngine input param is NULL\n";
        return JFAILURE;
    }

    m_pCommEngine[m_uiUsedCommEngine] = pCommEngine;
    m_uiUsedCommEngine++;

    return JSUCCESS;
}

JCommEngine* JCommEngineGroup::HasMessage(JUINT32 uiMilisecond)
{
    JTIMEVAL timeVal;
    JTIMEVAL* pTimeVal = JNULL;
    JINT32 iRet = 0;
    fd_set readfds;
    JUINT32 max_fd = 0;
    JUINT32 uiIdx = 0;
    JSOCKET fd;

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_SOCKET, "JCommEngineGroup::HasMessage");

    if (uiMilisecond)
    {
        timeVal.tv_sec = 0;
        timeVal.tv_usec = uiMilisecond;
        pTimeVal = &timeVal;
    }
    else
    {
        pTimeVal = JNULL;
    }

    FD_ZERO(&readfds);
    for (uiIdx=0; uiIdx<m_uiUsedCommEngine; uiIdx++)
    {
        if (m_pCommEngine[uiIdx])
        {
            fd = m_pCommEngine[uiIdx]->GetSocket()->GetFd();
            FD_SET(fd, &readfds);
            max_fd = fd;
        }
    }

    iRet = select(max_fd+1, &readfds, NULL, NULL, pTimeVal); 
    for (uiIdx=0; uiIdx<m_uiUsedCommEngine; uiIdx++)
    {
        if (m_pCommEngine[uiIdx])
        {
            fd = m_pCommEngine[uiIdx]->GetSocket()->GetFd();
            if (FD_ISSET(fd, &readfds))
            {
                return m_pCommEngine[uiIdx];
            }
        }
    }

    return JNULL;
}

}

