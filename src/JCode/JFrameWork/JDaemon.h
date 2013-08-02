/**********************************************************
* Author: weiganyi
* File: JDaemon.h
* Date: 20121108
* Description: 
*
**********************************************************/

#ifndef _JDEAMON_H_
#define _JDEAMON_H_


namespace JFrameWork{

//JDaemonCfg definication
#define JDAEMON_DEFAULT_FILE        "jdaemon_cfg"

#define JDAEMON_SAVE_METHOD         "save_method"

#define JDAEMON_SAVE_INI_ENABLED    "1"
#define JDAEMON_SAVE_SQL_ENABLED    "2"
#define JDAEMON_SAVE_XML_ENABLED    "3"


class JDaemonCfg: public JEventBody{
public:
    JDaemonCfg();
    ~JDaemonCfg();

    JUINT32 GetLength();

    JEventBody* Clone();
    JUINT32 Clear();

    JUINT32 Serialize(JCHAR* pBuf, JUINT32 uiMaxNum);
    JUINT32 DeSerialize(JCHAR* pBuf);

    JUINT32 operator=(JDaemonCfg& rDaemonCfg);
    JString& GetSaveMethod();
    JUINT32 SetSaveMethod(JString& rStr);

private:
    JString m_strSaveMethod;  //0 - INI, 1 - SQL, 2 - XML
};


//JDaemon definication
class JDaemon: public JModule{
public:
    JDaemon();
    ~JDaemon();

    JUINT32 InitFunc();
    JUINT32 EventProcFunc(JEvent* pEvent);
    JEventBody* MakeEventBody(JUINT32 uiType);

    JUINT32 ProcSetCfgEvent(JDaemonCfg* pDaemonCfg);
    JUINT32 ProcGetCfgEvent(JEvent* pEvent);

    JSER_PERSISTENCE_TYPE GetCfgSaveMethod();

private:
    JSerialization* m_pSerialization;

    JLock m_Lock;

    JDaemonCfg m_cfg;

    JUINT32 SetCfgList(JDaemonCfg* pDaemonCfg);
    JUINT32 GetCfgList(JDaemonCfg* pDaemonCfg);
};

}


#endif

