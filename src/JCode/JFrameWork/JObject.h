/**********************************************************
* Author: weiganyi
* File: JObject.h
* Date: 20110718
* Description: 
*
**********************************************************/

#ifndef _JOBJECT_H_
#define _JOBJECT_H_


namespace JFrameWork{

//global definition
typedef unsigned int    JUINT32;
typedef int             JINT32;
typedef unsigned short  JUINT16;
typedef short           JINT16;
typedef char            JCHAR;
typedef unsigned char   JUINT8;
typedef char            JINT8;
typedef void            JVOID;
typedef unsigned char   JBOOL;
typedef unsigned int    JSOCKET;
typedef struct fd_set   JFD_SET;
typedef struct timeval  JTIMEVAL;
typedef struct sockaddr_in  JSOCKADDR_IN;
typedef struct sockaddr     JSOCKADDR;


#define JTRUE               1
#define JFALSE              0
#define JSUCCESS            1
#define JFAILURE            0
#define JNULL               0
#define JINVALID            -1

//module socket address in the main thread
#define JPHONE_LOCAL_ADDR           "127.0.0.1"
#define JTHREAD_LOCAL_ADDR          JPHONE_LOCAL_ADDR
#define JTHREAD_LOCAL_PORT          10000+JPRODUCT_PORT_OFFSET
#define JTIMER_LOCAL_ADDR           JPHONE_LOCAL_ADDR
#define JTIMER_LOCAL_PORT           10001+JPRODUCT_PORT_OFFSET
#define JLOG_LOCAL_ADDR             JPHONE_LOCAL_ADDR
#define JLOG_LOCAL_PORT             10002+JPRODUCT_PORT_OFFSET

#define JMAX_STRING_LEN         64
#define JMAX_BUFFER_LEN         256


//class predeclare for compile passing
class JStaticMemory;
class JEvent;
class JSocket;
class JSerialization;
template<class TYPE> class JList;
class JString;
class JEventBody;
class JLogOutput;
class JCommEngine;
class JCommConnector;
class JAgentThread;


//JObject definition
typedef struct _JSYSTIME{
    JUINT32 uiYear; 
    JUINT32 uiMonth; 
    JUINT32 uiDayOfWeek; 
    JUINT32 uiDay; 
    JUINT32 uiHour; 
    JUINT32 uiMinute; 
    JUINT32 uiSecond; 
    JUINT32 uiMilliseconds; 
}JSYSTIME;

#define SYSTIME_LARGER      1
#define SYSTIME_EQUAL       0
#define SYSTIME_SMALLER     -1
#define SYSTIME_INVALID     0xFE

class JObject{
public:
    JObject();
    virtual ~JObject();

protected:
    //safe string operate function
    JUINT32 SafeStrlen(const JCHAR* str);
    JUINT32 SafeStrcmp(JCHAR* dst, const JCHAR* src);
    JCHAR* SafeStrcpy(JCHAR* dst, const JCHAR* src, const JUINT32 maxLen);
    JVOID* SafeStrncpy(JCHAR* dst, const JCHAR* src, const JUINT32 len, const JUINT32 maxLen);
    JVOID* SafeStrncat(JCHAR* dst, const JCHAR* src, const JUINT32 len, const JUINT32 maxLen);
    JVOID* SafeMemset(JCHAR* dst, JCHAR ch, const JUINT32 len);
    JVOID* SafeMemcpy(JCHAR* dst, const JCHAR* src, const JUINT32 len, const JUINT32 maxLen);
    JVOID* SafeMemcat(JCHAR* dst, const JCHAR* src, const JUINT32 len, const JUINT32 maxLen);
    JUINT32 SafeSprintf(JCHAR* dst, JUINT32 maxLen, const JCHAR* fmt, ...);
    JCHAR* SafeStrdup(JCHAR* dst);
    JCHAR* SafeStrstr(const JCHAR* src, const JCHAR* substr);
    JCHAR* SafeStrchr(const JCHAR* src, const JUINT32 ch);

    //file operate function
    FILE* SafeFopen(const JCHAR* name, const JCHAR* param);
    JUINT32 SafeFclose(FILE* fd);
    JUINT32 SafeFprintf(FILE* fd, const JCHAR* fmt, ...);
    JUINT32 SafeFflush(FILE* fd);
    JCHAR* SafeFgets(JCHAR* buf, const JUINT32 len, FILE* fd);

    //system time function
    JUINT32 GetSysTime(JSYSTIME *ptrSysTime);
    JINT32 CmpSysTime(JSYSTIME *srcSysTime, JSYSTIME *dstSysTime);
    JSYSTIME* AddSysTime(JSYSTIME *ptrSysTime, JUINT32 milliseconds);

    JBOOL IsDtmf(JINT32 iDigit);

    //get local mechine address
    JUINT32 GetLocalAddr(JString& rAddr, JString& rIfAddr);
};


//JModule definition
class JModule: public JObject{
public:
    JModule();
    virtual ~JModule();

    virtual JUINT32 InitFunc() = 0;
    virtual JUINT32 EventProcFunc(JEvent* pEvent) = 0;
    virtual JEventBody* MakeEventBody(JUINT32 uiType) = 0;

private:
};


//JException definition
class JException: public JObject{
public:
    JException();
    JException(JCHAR* pStr);
    ~JException();

    JCHAR* GetErrInfo();

private:
    JCHAR* m_pErrInfo;
};


//JEventBody definition
class JEventBody: public JObject{
public:
    JEventBody();
    ~JEventBody();

    virtual JUINT32 GetLength() = 0;

    virtual JEventBody* Clone() = 0;
    virtual JUINT32 Clear() = 0;

    virtual JUINT32 Serialize(JCHAR* pBuf, JUINT32 uiMaxNum) = 0;
    virtual JUINT32 DeSerialize(JCHAR* pBuf) = 0;

private:
};

}


#endif

