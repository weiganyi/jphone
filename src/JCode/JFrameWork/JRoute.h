/**********************************************************
* Author: weiganyi
* File: JRoute.h
* Date: 20110718
* Description: 
*
**********************************************************/

#ifndef _JROUTE_H_
#define _JROUTE_H_


namespace JFrameWork{

//JHash definication
class JHash: public JObject{
public:
    JHash();
    ~JHash();

    JUINT32 CalcHashValue(JCHAR* pName);

private:
};


//JHashData definication
#define JS_M_JMEMORY        "m_jmemory"
#define JS_M_JTIMER         "m_jtimer"
#define JS_M_JSIPUA         "m_jsipua"
#define JS_M_JLOG           "m_jlog"
#define JS_M_JDAEMON        "m_jdaemon"
#define JS_T_JMAINTHREAD    "t_jmainthread"
#define JS_T_JTIMER         "t_jtimer"
#define JS_P_JMAINPROC      "p_jmainproc"

#define JROUTE_MAX_HASH_TABLE   10

typedef enum{
    ROUTE_TYPE_NONE,
    ROUTE_TYPE_MODULE,
    ROUTE_TYPE_THREAD,
    ROUTE_TYPE_PROCESS
}JROUTE_TYPE;

class JHashData: public JObject{
public:
    JHashData();
    ~JHashData();

    JString& GetProcName();
    JUINT32 SetProcName(JString& rStr);
    JString& GetThrdName();
    JUINT32 SetThrdName(JString& rStr);
    JSOCKADDR_IN* GetAddr();
    JUINT32 SetAddr(JSOCKADDR_IN* pAddr);

private:
    JString m_strProcName;
    JString m_strThrdName;
    JSOCKADDR_IN m_stAddr;
};


//JRoute definication
//class JRoute only works for the communication between with the JCommEngine of the processes 
//and threads, and the other JCommEngines that works for the other purpuses willn't find route 
//from the class JRoute.
class JRoute: public JObject{
public:
    JRoute();
    ~JRoute();

    JUINT32 AddRoute(JCHAR* pProcName, JCHAR* pThrdName, JSOCKADDR_IN* pAddr);
    JUINT32 DelRoute(JCHAR* pProcName, JCHAR* pThrdName);
    JROUTE_TYPE GetRouteType(JCHAR* pFromProcName, 
                            JCHAR* pFromThrdName,
                            JCHAR* pToProcName, 
                            JCHAR* pToThrdName);
    JUINT32 FindRoute(JCHAR* pProcName, JCHAR* pThrdName);
    JUINT32 GetRoute(JCHAR* pProcName, JCHAR* pThrdName, JSOCKADDR_IN* pAddr);

private:
    JList<JHashData> m_pHashTable[JROUTE_MAX_HASH_TABLE];

    JLock m_Lock;

    JUINT32 privAddRoute(JCHAR* pProcName, 
                        JCHAR* pThrdName, 
                        JSOCKADDR_IN* pAddr);
};

}


#endif

