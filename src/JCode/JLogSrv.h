/**********************************************************
* Author: weiganyi
* File: JLogSrv.h
* Date: 20121129
* Description: 
*
**********************************************************/

#ifndef _JLOGSRV_H_
#define _JLOGSRV_H_


#define JLOGSRV_MAX_MSG_SRV     10

//JLogSrvCfg definication
class JLogSrvCfg: public JEventBody{
public:
    JLogSrvCfg();
    ~JLogSrvCfg();

    JUINT32 GetLength();

    JEventBody* Clone();
    JUINT32 Clear();

    JUINT32 Serialize(JCHAR* pBuf, JUINT32 uiMaxNum);
    JUINT32 DeSerialize(JCHAR* pBuf);

    JUINT32 operator=(JLogSrvCfg& rLogSrvCfg);
    JString& GetListenAddr();
    JUINT32 SetListenAddr(JString& rStr);
    JString& GetListenPort();
    JUINT32 SetListenPort(JString& rStr);

private:
    JString m_ListenAddr;
    JString m_ListenPort;
};


//JLogSrvNumber definication
class JLogSrvNumber: public JEventBody{
public:
    JLogSrvNumber();
    ~JLogSrvNumber();

    JUINT32 GetLength();

    JEventBody* Clone();
    JUINT32 Clear();

    JUINT32 Serialize(JCHAR* pBuf, JUINT32 uiMaxNum);
    JUINT32 DeSerialize(JCHAR* pBuf);

    JUINT32 operator=(JLogSrvNumber& rLogSrvNumber);
    JString& GetSrvNumber();
    JUINT32 SetSrvNumber(JString& rStr);

private:
    JString m_srvNumber;
};


//JLogSrvHasNewMsg definication
class JLogSrvHasNewMsg: public JEventBody{
public:
    JLogSrvHasNewMsg();
    ~JLogSrvHasNewMsg();

    JUINT32 GetLength();

    JEventBody* Clone();
    JUINT32 Clear();

    JUINT32 Serialize(JCHAR* pBuf, JUINT32 uiMaxNum);
    JUINT32 DeSerialize(JCHAR* pBuf);

    JUINT32 operator=(JLogSrvHasNewMsg& rLogSrvHasNewMsg);
    JString& GetHasNewMsg();
    JUINT32 SetHasNewMsg(JString& rStr);
    JString& GetRmtAddr(JUINT32 uiIdx);
    JUINT32 SetRmtAddr(JString& rStr, JUINT32 uiIdx);

private:
    JString m_hasNewMsg;
    JString m_rmtAddr[JLOGSRV_MAX_MSG_SRV];
};


//JLogSrv definication
#define JLOGSRV_CFG_FILE             "jlogsrv_cfg"

#define JLOGSRV_LISTEN_ADDR          "listenaddr"
#define JLOGSRV_LISTEN_PORT          "listenport"

#define JLOGSRV_SLEEP_TIME      10
#define JLOGSRV_SELECT_TIME     500*1000

#define JS_T_JLOGMSG_PREFIX   "t_jlogmsg"
#define JS_T_JLOGMSG1         "t_jlogmsg1"
#define JS_T_JLOGMSG2         "t_jlogmsg2"
#define JS_T_JLOGMSG3         "t_jlogmsg3"
#define JS_T_JLOGMSG4         "t_jlogmsg4"
#define JS_T_JLOGMSG5         "t_jlogmsg5"
#define JS_T_JLOGMSG6         "t_jlogmsg6"
#define JS_T_JLOGMSG7         "t_jlogmsg7"
#define JS_T_JLOGMSG8         "t_jlogmsg8"
#define JS_T_JLOGMSG9         "t_jlogmsg9"
#define JS_T_JLOGMSG10        "t_jlogmsg10"

class JLogSrv: public JObject{
public:
    JLogSrv();
    ~JLogSrv();

    JUINT32 InitFunc();
    JUINT32 EventProcFunc(JEvent* pEvent);
    JEventBody* MakeEventBody(JUINT32 uiType);

    JCommEngine* AcceptLogConnect();
    JINT32 RecvLogMsg(JCommEngine* pCommEngine, 
                        JCHAR* pBuf, 
                        JUINT32 uiLen);

private:
    JLogSrvCfg m_cfg;

    JLock m_commLock;
    JLock m_msgLock;

    JCommAcceptor* m_pCommAcceptor;

    JSerialization* m_pSerialization;

    JAgentThread* m_pAgentThread;

    JUINT32 ProcSetCfgEvent(JLogSrvCfg* pLogSrvCfg);
    JUINT32 ProcGetCfgEvent(JEvent* pEvent);
    JUINT32 SetCfgList(JLogSrvCfg* pLogSrvCfg);
    JUINT32 GetCfgList(JLogSrvCfg* pLogSrvCfg);

    JUINT32 ProcCleanEvent(JLogSrvNumber* pLogSrvNumber);
    JUINT32 ProcGetHasNewMsgEvent(JEvent* pEvent, JLogSrvNumber* pLogSrvNumber);
};


//JLogSrvThread definication
class JLogSrvThread: public JThread{
public:
    JLogSrvThread(JString* pThdName = JNULL, 
                        JString* pLocalAddr = JNULL, 
                        JUINT32 uiLocalPort =0);
    ~JLogSrvThread();

    JUINT32 Init();
    JUINT32 Run();
    JUINT32 EnQueueEvent(JListItem<JEvent>* pItem, JCHAR* pModName=JNULL);

    JEventBody* MakeEventBody(JEVT_TYPE eType, JCHAR* pModName=JNULL);
    JCommEngine* GetNotifyCommEngine();

private:
    JQueue<JEvent> m_hQueue;

    JCommEngine* m_pNotifyCommEngine;
    JCommEngineGroup m_commEngineGroup;

    JLock m_Lock;
};


//JLogMsgThread definication
#define JLOGMSG_MAX_MSG  1024

class JLogMsgThread: public JThread{
public:
    JLogMsgThread(JString* pThdName = JNULL);
    ~JLogMsgThread();

    JUINT32 Init();
    JUINT32 Run();
    JUINT32 EnQueueEvent(JListItem<JEvent>* pItem, JCHAR* pModName=JNULL);
    JEventBody* MakeEventBody(JEVT_TYPE eType, JCHAR* pModName=JNULL);

    JUINT32 CleanLog();
    JUINT32 SetHasNewMsg(JBOOL bHasNewMsg);
    JBOOL GetHasNewMsg();
    JSOCKADDR_IN* GetRemoteAddr();
    JUINT32 GetLogMsg(JCHAR* logMessage, 
                    JUINT32 uiMaxLen, 
                    JUINT32* pOffset);

private:
    JCHAR m_logMessage[JLOGMSG_MAX_MSG][JMAX_BUFFER_LEN];
    JUINT32 m_uiLogOffset;

    JCommEngine* m_pCommEngine;

    JBOOL m_bHasNewMsg;

    JLock m_Lock;
};


#endif

