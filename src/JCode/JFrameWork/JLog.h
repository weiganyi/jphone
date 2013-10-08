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

//JFmt definition
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
    //output log to the console
    JLOG_OUTPUT_LOCAL   = 0x1,
    //output log to the local file
    JLOG_OUTPUT_FILE    = 0x1<<1,
    //output log to the remote log server
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


//JLogDecorator definition
//the max module number
#define JLOG_MAX_MOD_NUM        JLOG_MOD_ALL
//the max number of the log server thread
#define JLOG_MAX_THREAD_NUM     10
//the max buffer length
#define JLOG_MAX_BUF_LEN        512
//the max stack deep of the function call
#define JLOG_MAX_TRACE          100

//the log file name
#define JLOG_FILE_NAME          "jphone_log.txt"
//the timestamp format of the log output
#define JLOG_TIMESTAMP_FORMAT   "T%02d%02d%02d%03d - "

//the cfg file name
#define JLOG_CFG_FILE           "jphone_log_cfg"

//log server address
#define JLOG_ADDRESS            "log_address"
//log server port
#define JLOG_PORT               "log_port"
//whether output to log file
#define JLOG_FILE               "log_file"
//whether output to log server
#define JLOG_REMOTE             "log_remote"

//value for the log file output
#define JLOG_FILE_DISABLE       "0"
#define JLOG_FILE_ENABLE        "1"
//value for the log server output
#define JLOG_REMOTE_DISABLE     "0"
#define JLOG_REMOTE_ENABLE      "1"

//the interval to check tcp connect of the log server
#define JLOG_CHECK_CONNECT_INTERVAL     1000*3

//the select interval in the connect check
#define JLOG_SELECT_TIME                1000

class JLogDecorator: public JObject{
public:
    JLogDecorator();
    ~JLogDecorator();

    //add some decorator into the log output
    virtual JUINT32 DecoratorBuf(JLogOutput* pLogOutput, JCHAR* pBuf) = 0;

private:
};


//JLogTimeDecorator definition
class JLogTimeDecorator: public JLogDecorator{
public:
    JLogTimeDecorator();
    ~JLogTimeDecorator();

    JUINT32 DecoratorBuf(JLogOutput* pLogOutput, JCHAR* pBuf);

private:
    //cache the log, then output log base on line
    JCHAR m_outputCatchBuf[JLOG_MAX_BUF_LEN*2];

    //the flag whether new line start
    JBOOL m_newLineStart;
};


//JLogOutput definition
class JLogOutput: public JObject{
public:
    JLogOutput();
    ~JLogOutput();

    virtual JUINT32 Print(JCHAR* ptrBuf) = 0;
    //the actual print output interface
    virtual JUINT32 RawPrint(JCHAR* ptrBuf) = 0;
    virtual JCommEngine* GetCommEngine() = 0;

protected:
    //point to the decorator
    JLogDecorator* m_pDecorator;
};


//JLogOutputLocal definition
class JLogOutputLocal: public JLogOutput{
public:
    JLogOutputLocal();
    ~JLogOutputLocal();

    JUINT32 Print(JCHAR* ptrBuf);
    JUINT32 RawPrint(JCHAR* ptrBuf);
    JCommEngine* GetCommEngine();

private:
};


//JLogOutputFile definition
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


//JLogOutputRemote definition
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
    //the handler to send log
    JCommEngine* m_pCommEngine;

    //the connector to the log server
    JCommConnector* m_pCommConnector;
};


//JLogCfg definition
class JLogCfg: public JEventBody{
public:
    JLogCfg();
    ~JLogCfg();

    JUINT32 GetLength();

    JEventBody* Clone();
    JUINT32 Clear();

    JUINT32 Serialize(JCHAR* pBuf, JUINT32 uiMaxNum);
    JUINT32 DeSerialize(JCHAR* pBuf);

    //the assinment operator
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

    //1 - disable, 2 - enable
    JString m_strOutputFile;
    //1 - disable, 2 - enable
    JString m_strOutputRemote;
};


//JLog definition
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

    //overload the log output operator for different type
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

    //log trace function enter
    JUINT32 SetFuncEnterTrace(const JLOG_MODULE module, JCHAR* ptrFuncName);
    //log trace while function exit
    JUINT32 SetFuncExitTrace(const JLOG_MODULE module, JCHAR* ptrFuncName);
    JUINT32 GetFuncTrace(JUINT32 uiMaxNum, JCHAR* ucFuncTrace[]);

    //timer expire callback
    friend JUINT32 OnCheckConnect(JVOID* pData);

private:
    //cache output module of the current log
    JLOG_MODULE m_currModule;
    //cache output level of the current log 
    JLOG_LEVEL m_currLevel;

    //if the log level larger than this value, it will output
    JLOG_LEVEL m_modLvlThres[JLOG_MAX_MOD_NUM];

    //the log output method
    JUINT32 m_outputMethod;

    //the log output config
    JLogCfg m_cfg;

    //In order to avoid critical while different thread access log
    //I will store or get log message individually for different thread.
    //store the function name
    JCHAR* m_funcTrace[JLOG_MAX_THREAD_NUM][JLOG_MAX_TRACE];
    //store the thread id
    JUINT32 m_threadID[JLOG_MAX_THREAD_NUM];
    //how much log message had been stored
    JUINT32 m_currOffset[JLOG_MAX_THREAD_NUM];

    //the store type for the log object config
    JUINT32 m_uiPersistenceType;
    //the handler to store config
    JSerialization* m_pSerialization;

    //the lock to protect the log output
    JLock m_PrintLock;
    //the lock to protect the event process
    JLock m_EventLock;
    //the lock to protect the function name trace
    JLock m_TraceLock;

    //point to the object of output console
    JLogOutput* m_pOutputLocal;
    //point to the object of output file
    JLogOutput* m_pOutputFile;
    //point to the object of output log server
    JLogOutput* m_pOutputRemote;

    //the timer to check connect
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


//JLogAutoPtr definition
//I hope function enter will be called while this object construct,
//and function exit will be called while this object destruct(function exit), 
//so I derive from the class autoptrbase
class JLogAutoPtr: public JAutoPtrBase<JLog>{
public:
    JLogAutoPtr(JLog* pObject, const JLOG_MODULE module, JCHAR* ptrFuncName);
    ~JLogAutoPtr();

private:
    //module of the log trace
    JLOG_MODULE m_traceModule;

    JCHAR m_traceFuncName[JMAX_STRING_LEN];
};


JFmt& set(const JLOG_MODULE mod, const JLOG_LEVEL level);

}


#endif

