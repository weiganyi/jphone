/**********************************************************
* Author: weiganyi
* File: JLog.h
* Date: 20110718
* Description: 
*
**********************************************************/

#ifndef _JLOG_H_
#define _JLOG_H_


namespace JFrameWork{

//JFmt definication
typedef enum{
    JLOG_MOD_NONE,
    JLOG_MOD_LOG,
    JLOG_MOD_PHONE,
    JLOG_MOD_OBJECT,
    JLOG_MOD_MEMORY,
    JLOG_MOD_TIMER,
    JLOG_MOD_SOCKET,
    JLOG_MOD_EVENT,
    JLOG_MOD_LIST,
    JLOG_MOD_ROUTE,
    JLOG_MOD_THREAD,
    JLOG_MOD_SERIALIZATION,
    JLOG_MOD_DAEMON,
    JLOG_MOD_SIPUA,
    JLOG_MOD_LOGSRV,
    JLOG_MOD_PHONE_AGENT,
    JLOG_MOD_LOGSERVER_AGENT,
    JLOG_MOD_ALL,
    JLOG_MOD_MAX
}JLOG_MODULE;

typedef enum{
    JLOG_INFO_LEVEL,
    JLOG_ENTER_LEVEL,
    JLOG_EXIT_LEVEL,
    JLOG_DEBUG_LEVEL,
    JLOG_ERROR_LEVEL,
    JLOG_EMER_LEVEL
}JLOG_LEVEL;

typedef enum{
    JLOG_OUTPUT_NONE    = 0x0,
    JLOG_OUTPUT_LOCAL   = 0x1,
    JLOG_OUTPUT_FILE    = 0x1<<1,
    JLOG_OUTPUT_REMOTE  = 0x1<<2
}JLOG_OUTPUT;


class JFmt: public JObject{
public:
    JFmt();
    ~JFmt();

    friend JFmt& set(const JLOG_MODULE module, const JLOG_LEVEL level);
    friend class JLog;

private:
    JLOG_MODULE m_Module;
    JLOG_LEVEL m_Level;
};


//JLogDecorator definication
#define JLOG_MAX_MOD_NUM        JLOG_MOD_ALL
#define JLOG_MAX_THREAD_NUM     10
#define JLOG_MAX_BUF_LEN        512
#define JLOG_MAX_TRACE          100

#define JLOG_FILE_NAME          "jphone_log.txt"
#define JLOG_TIMESTAMP_FORMAT   "T%02d%02d%02d%03d - "

#define JLOG_CFG_FILE           "jphone_log_cfg"

#define JLOG_ADDRESS            "log_address"
#define JLOG_PORT               "log_port"
#define JLOG_FILE               "log_file"
#define JLOG_REMOTE             "log_remote"

#define JLOG_FILE_DISABLE       "0"
#define JLOG_FILE_ENABLE        "1"
#define JLOG_REMOTE_DISABLE     "0"
#define JLOG_REMOTE_ENABLE      "1"

#define JLOG_CHECK_CONNECT_INTERVAL     1000*3

#define JLOG_SELECT_TIME                1000

class JLogDecorator: public JObject{
public:
    JLogDecorator();
    ~JLogDecorator();

    virtual JUINT32 DecoratorBuf(JLogOutput* pLogOutput, JCHAR* pBuf) = 0;

private:
};


//JLogTimeDecorator definication
class JLogTimeDecorator: public JLogDecorator{
public:
    JLogTimeDecorator();
    ~JLogTimeDecorator();

    JUINT32 DecoratorBuf(JLogOutput* pLogOutput, JCHAR* pBuf);

private:
    JCHAR m_outputCatchBuf[JLOG_MAX_BUF_LEN*2];

    JBOOL m_newLineStart;
};


//JLogOutput definication
class JLogOutput: public JObject{
public:
    JLogOutput();
    ~JLogOutput();

    virtual JUINT32 Print(JCHAR* ptrBuf) = 0;
    virtual JUINT32 RawPrint(JCHAR* ptrBuf) = 0;
    virtual JCommEngine* GetCommEngine() = 0;

protected:
    JLogDecorator* m_pDecorator;
};


//JLogOutputLocal definication
class JLogOutputLocal: public JLogOutput{
public:
    JLogOutputLocal();
    ~JLogOutputLocal();

    JUINT32 Print(JCHAR* ptrBuf);
    JUINT32 RawPrint(JCHAR* ptrBuf);
    JCommEngine* GetCommEngine();

private:
};


//JLogOutputFile definication
class JLogOutputFile: public JLogOutput{
public:
    JLogOutputFile(JCHAR* pFileName);
    ~JLogOutputFile();

    JUINT32 Print(JCHAR* ptrBuf);
    JUINT32 RawPrint(JCHAR* ptrBuf);
    JCommEngine* GetCommEngine();

private:
    FILE* m_fileHandler;
};


//JLogOutputRemote definication
class JLogOutputRemote: public JLogOutput{
public:
    JLogOutputRemote(JString& rLocalAddr,
                    JUINT32 uiLocalPort,
                    JString& rRemoteAddr,
                    JUINT32 uiRemotePort);
    ~JLogOutputRemote();

    JUINT32 Print(JCHAR* ptrBuf);
    JUINT32 RawPrint(JCHAR* ptrBuf);
    JCommEngine* GetCommEngine();

private:
    JCommEngine* m_pCommEngine;
    JCommConnector* m_pCommConnector;
};


//JLogCfg definication
class JLogCfg: public JEventBody{
public:
    JLogCfg();
    ~JLogCfg();

    JUINT32 GetLength();

    JEventBody* Clone();
    JUINT32 Clear();

    JUINT32 Serialize(JCHAR* pBuf, JUINT32 uiMaxNum);
    JUINT32 DeSerialize(JCHAR* pBuf);

    JUINT32 operator=(JLogCfg& rLogCfg);
    JString& GetLogAddress();
    JUINT32 SetLogAddress(JString& rStr);
    JString& GetLogPort();
    JUINT32 SetLogPort(JString& rStr);
    JString& GetOutputFile();
    JUINT32 SetOutputFile(JString& rStr);
    JString& GetOutputRemote();
    JUINT32 SetOutputRemote(JString& rStr);

private:
    JString m_strAddress;
    JString m_strPort;
    JString m_strOutputFile;   //1 - disable, 2 - enable
    JString m_strOutputRemote; //1 - disable, 2 - enable
};


//JLog definication
class JLog: public JModule{
public:
    JLog(JLOG_OUTPUT eMethod=JLOG_OUTPUT_NONE);
    ~JLog();

    JUINT32 InitFunc();
    JUINT32 EventProcFunc(JEvent* pEvent);
    JEventBody* MakeEventBody(JUINT32 uiType);
    JUINT32 SetPersistenceType(JUINT32 uiType);

    JUINT32 ProcSetCfgEvent(JLogCfg* pLogCfg);
    JUINT32 ProcGetCfgEvent(JEvent* pEvent);

    JLog& operator<<(const JFmt& clsFmt);
    JLog& operator<<(JCHAR* ptrBuf);
    JLog& operator<<(const JUINT8 uiValue);
    JLog& operator<<(const JUINT16 uiValue);
    JLog& operator<<(const JUINT32 uiValue);
    JLog& operator<<(const JINT8 iValue);
    JLog& operator<<(const JINT16 iValue);
    JLog& operator<<(const JINT32 iValue);

    JUINT32 SetLogLevel(const JLOG_MODULE uiModule, const JLOG_LEVEL uiValue);
    JUINT32 SetLogMethod(const JLOG_OUTPUT uiOutput);
    JUINT32 UnSetLogMethod(const JLOG_OUTPUT uiOutput);

    JUINT32 SetFuncEnterTrace(const JLOG_MODULE module, JCHAR* ptrFuncName);
    JUINT32 SetFuncExitTrace(const JLOG_MODULE module, JCHAR* ptrFuncName);
    JUINT32 GetFuncTrace(JUINT32 uiMaxNum, JCHAR* ucFuncTrace[]);

    friend JUINT32 OnCheckConnect(JVOID* pData);

private:
    JLOG_MODULE m_currModule;
    JLOG_LEVEL m_currLevel;
    JLOG_LEVEL m_modLvlThres[JLOG_MAX_MOD_NUM];
    JUINT32 m_outputMethod;
    JLogCfg m_cfg;

    JCHAR* m_funcTrace[JLOG_MAX_THREAD_NUM][JLOG_MAX_TRACE];
    JUINT32 m_threadID[JLOG_MAX_THREAD_NUM];
    JUINT32 m_currOffset[JLOG_MAX_THREAD_NUM];

    JUINT32 m_uiPersistenceType;
    JSerialization* m_pSerialization;

    JLock m_PrintLock;
    JLock m_EventLock;
    JLock m_TraceLock;

    JLogOutput* m_pOutputLocal;
    JLogOutput* m_pOutputFile;
    JLogOutput* m_pOutputRemote;

    JUINT32 m_timeHandler;

    JUINT32 SetCfgList(JLogCfg* pLogCfg);
    JUINT32 GetCfgList(JLogCfg* pLogCfg);

    JUINT32 DoCheckConnect();

    JUINT32 Print(const JLOG_MODULE mod, const JLOG_LEVEL level, JCHAR* ptrBuf);

    JUINT32 GetCurrentThreadId();
    JINT32 FindThreadId(JUINT32* uiThreadId);

    JUINT32 StartCheckConnect();
    JUINT32 StopCheckConnect();
};


//JLogAutoPtr definication
class JLogAutoPtr: public JAutoPtrBase<JLog>{
public:
    JLogAutoPtr(JLog* pObject, const JLOG_MODULE module, JCHAR* ptrFuncName);
    ~JLogAutoPtr();

private:
    JLOG_MODULE m_traceModule;
    JCHAR m_traceFuncName[JMAX_STRING_LEN];
};


JFmt& set(const JLOG_MODULE mod, const JLOG_LEVEL level);

}


#endif

