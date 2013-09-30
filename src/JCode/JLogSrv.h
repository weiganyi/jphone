/**********************************************************
* Author: weiganyi
* File: JLogSrv.h
* Date: 20121129
* Description: 
*
**********************************************************/

#ifndef _JLOGSRV_H_
#define _JLOGSRV_H_


//the max log server number
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

    //assginment operator
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

    //assginment operator
    JUINT32 operator=(JLogSrvNumber& rLogSrvNumber);

    JString& GetSrvNumber();
    JUINT32 SetSrvNumber(JString& rStr);

private:
    //the log server sequence
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

    //assginment operator
    JUINT32 operator=(JLogSrvHasNewMsg& rLogSrvHasNewMsg);

    JString& GetHasNewMsg();
    JUINT32 SetHasNewMsg(JString& rStr);
    JString& GetRmtAddr(JUINT32 uiIdx);
    JUINT32 SetRmtAddr(JString& rStr, JUINT32 uiIdx);

private:
    //whether has new msg
    JString m_hasNewMsg;

    //the address where log message sent
    JString m_rmtAddr[JLOGSRV_MAX_MSG_SRV];
};


//JLogSrv definication
//cfg file name of the log server
#define JLOGSRV_CFG_FILE             "jlogsrv_cfg"

//listen address of the log server
#define JLOGSRV_LISTEN_ADDR          "listenaddr"
//listen port of the log server
#define JLOGSRV_LISTEN_PORT          "listenport"

//sleep time after log server accept fail while log client want to connect
#define JLOGSRV_SLEEP_TIME      10
//select time while recv msg
#define JLOGSRV_SELECT_TIME     500*1000

//log server thread name
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

//this is log server object, the log server thread and the log message thread 
//will call this object to do something
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
    //store log server config
    JLogSrvCfg m_cfg;

    //lock for event process
    JLock m_commLock;
    //lock for accept log connect
    JLock m_msgLock;

    //accept object for log connect
    JCommAcceptor* m_pCommAcceptor;

    //config save handler
    JSerialization* m_pSerialization;

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
    //the event queue for this thread
    JQueue<JEvent> m_hQueue;

    //event notify communication engine
    JCommEngine* m_pNotifyCommEngine;

    //listen communication engine group, now there is only include notify communication engine,
    //so may be it's no need in here
    JCommEngineGroup m_commEngineGroup;

    //lock to protect queue access
    JLock m_Lock;
};


//JLogMsgThread definication
//the max log message number
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
    //store log message
    JCHAR m_logMessage[JLOGMSG_MAX_MSG][JMAX_BUFFER_LEN];
    //current log message sequence
    JUINT32 m_uiLogOffset;

    //the communication engine to recv log message
    JCommEngine* m_pCommEngine;

    //flag whether has new message
    JBOOL m_bHasNewMsg;

    //lock to protect JLogSrv access
    JLock m_Lock;
};


#endif

