/**********************************************************
* Author: weiganyi
* File: JSocket.h
* Date: 20110718
* Description: 
*
**********************************************************/

#ifndef _JSOCKET_H_
#define _JSOCKET_H_


namespace JFrameWork{

//JSocket definition
typedef enum{
    JSOCKET_NONE,
    //udp socket
    JSOCKET_UDP,
    //tcp socket
    JSOCKET_TCP,
    JSOCKET_MAX,
}JSOCKET_TYPE;

class JSocket: public JObject{
public:
    JSocket();
    virtual ~JSocket();

    virtual JSOCKET Create(JSOCKET fd=0) = 0;
    virtual JVOID Destroy() = 0;
    virtual JINT32 SetLocalAddr(JSOCKADDR_IN* pAddr) = 0;
    virtual JUINT32 SetRemoteAddr(JSOCKADDR_IN* pAddr) = 0;
    virtual JSOCKADDR_IN* GetRemoteAddr() = 0;
    virtual JINT32 Select(const JUINT32 uiMilisecond) = 0;
    virtual JINT32 SendBuf(const JCHAR* ptrBuf, const JUINT32 uiLen) = 0;
    virtual JINT32 RecvBuf(JCHAR* ptrBuf, const JUINT32 uiLen, JSOCKADDR_IN* pFrom, JINT32* iFromLen) = 0;

    JSOCKET GetFd();

protected:
    //socket id
    JSOCKET m_fd;
};


//JUdpSocket definition
class JUdpSocket: public JSocket{
public:
    JUdpSocket();
    ~JUdpSocket();

    JSOCKET Create(JSOCKET fd=0);
    JVOID Destroy();
    JINT32 SetLocalAddr(JSOCKADDR_IN* pAddr);
    JUINT32 SetRemoteAddr(JSOCKADDR_IN* pAddr);
    JSOCKADDR_IN* GetRemoteAddr();
    JINT32 Select(const JUINT32 uiMilisecond);
    JINT32 SendBuf(const JCHAR* ptrBuf, const JUINT32 uiLen);
    JINT32 RecvBuf(JCHAR* ptrBuf, const JUINT32 uiLen, JSOCKADDR_IN* pFrom, JINT32* iFromLen);

private:
    //remote address
    JSOCKADDR_IN m_rmtAddr;
};


//JTcpSocket definition
class JTcpSocket: public JSocket{
public:
    JTcpSocket();
    ~JTcpSocket();

    JSOCKET Create(JSOCKET fd=0);
    JVOID Destroy();
    JINT32 SetLocalAddr(JSOCKADDR_IN* pAddr);
    JUINT32 SetRemoteAddr(JSOCKADDR_IN* pAddr);
    JSOCKADDR_IN* GetRemoteAddr();
    JINT32 Select(const JUINT32 uiMilisecond);
    JINT32 SendBuf(const JCHAR* ptrBuf, const JUINT32 uiLen);
    JINT32 RecvBuf(JCHAR* ptrBuf, const JUINT32 uiLen, JSOCKADDR_IN* pFrom, JINT32* iFromLen);

private:
    //local address
    JSOCKADDR_IN m_lclAddr;
    //remote address
    JSOCKADDR_IN m_rmtAddr;
};


//JCommEngine definition
//max length for msg buffer
#define JCOMM_MSG_BUF_LEN       1024
//select time for checking message
#define JCOMM_SELECT_TIME       100*1000    //millisecond
//block to check message
#define JCOMM_SELECT_INDEFINITE 0           

class JCommEngine: public JObject{
public:
    JCommEngine(JSOCKET_TYPE eType=JSOCKET_UDP, JSOCKET ofd=0);
    ~JCommEngine();

    JUINT32 SetLocalAddr(JCHAR* pProcName, 
                        JCHAR* pThrdName, 
                        JSOCKADDR_IN* pAddr);
    JUINT32 SetLocalAddr(JSOCKADDR_IN* pAddr);
    JUINT32 SetRemoteAddr(JCHAR* pProcName, 
                        JCHAR* pThrdName, 
                        JSOCKADDR_IN* pAddr);
    JUINT32 SetRemoteAddr(JSOCKADDR_IN* pAddr);

    JUINT32 SendEvent(JEvent* pEvent);
    JINT32 SendMessage(const JCHAR* pBuf, 
                    const JUINT32 uiTrueLen, 
                    JSOCKADDR_IN* pAddr = JNULL);
    JINT32 RecvMessage(JCHAR* pBuf, JUINT32 uiLen, JSOCKADDR_IN* pAddr);
    JUINT32 RecvEvent(JCHAR* pBuf, JUINT32 uiLen, JEvent** ppEvent, JSOCKADDR_IN* pAddr);

    JINT32 HasMessage(JUINT32 uiMilisecond = JCOMM_SELECT_TIME);
    JSocket* GetSocket();
    JSOCKADDR_IN* GetRemoteAddr();

private:
    //socket type
    JSOCKET_TYPE m_eType;
    //socket handler
    JSocket* m_pSocket;

};


//JCommConnector definition
class JCommConnector: public JObject{
public:
    JCommConnector();
    ~JCommConnector();

    JINT32 SetLocalAddr(JSOCKADDR_IN* pAddr);
    JUINT32 SetRemoteAddr(JSOCKADDR_IN* pAddr);
    JCommEngine* Connect();

private:
    //local address
    JSOCKADDR_IN m_lclAddr;
    //remote address
    JSOCKADDR_IN m_rmtAddr;

    //socket id
    JSOCKET m_fd;

    JSOCKET Create();
    JVOID Destroy();
};


//JCommAcceptor definition
class JCommAcceptor: public JObject{
public:
    JCommAcceptor();
    ~JCommAcceptor();

    JINT32 SetLocalAddr(JSOCKADDR_IN* pAddr);
    JINT32 Select(const JUINT32 uiMilisecond);
    JINT32 Listen();
    JCommEngine* Accept();

private:
    //local address
    JSOCKADDR_IN m_lclAddr;
    //remote address
    JSOCKADDR_IN m_rmtAddr;

    //socket id
    JSOCKET m_fd;

    JSOCKET Create();
    JVOID Destroy();
};


//JCommEngineGroup definition
//max number of the comm engine for one group
#define JCOMM_MAX_COMMENGINE   10

class JCommEngineGroup: public JObject{
public:
    JCommEngineGroup();
    ~JCommEngineGroup();

    JUINT32 AddCommEngine(JCommEngine* pCommEngine);
    JCommEngine* HasMessage(JUINT32 uiMilisecond = JCOMM_SELECT_TIME);

private:
    //commengine handler array
    JCommEngine* m_pCommEngine[JCOMM_MAX_COMMENGINE];
    //how much comm engine already been used
    JUINT32 m_uiUsedCommEngine;
};

}


#endif

