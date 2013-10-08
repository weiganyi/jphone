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

//JDaemonCfg definition
//the cfg file name
#define JDAEMON_DEFAULT_FILE        "jdaemon_cfg"

//the cfg save method
#define JDAEMON_SAVE_METHOD         "save_method"

//save in the ini file
#define JDAEMON_SAVE_INI_ENABLED    "1"
//save in the mysql database
#define JDAEMON_SAVE_SQL_ENABLED    "2"
//save in the xml file
#define JDAEMON_SAVE_XML_ENABLED    "3"

class JDaemonCfg: public JEventBody{
public:
    JDaemonCfg();
    ~JDaemonCfg();

    JUINT32 GetLength();

    //clone a new object from the other object
    JEventBody* Clone();
    //clear the saved data
    JUINT32 Clear();

    //serialize to a string buffer, uiMaxNum is the max buffer length 
    JUINT32 Serialize(JCHAR* pBuf, JUINT32 uiMaxNum);
    //deserialize from a string buffer
    JUINT32 DeSerialize(JCHAR* pBuf);

    //the assinment operator
    JUINT32 operator=(JDaemonCfg& rDaemonCfg);

    JString& GetSaveMethod();
    JUINT32 SetSaveMethod(JString& rStr);

private:
    //0 - INI, 1 - SQL, 2 - XML
    JString m_strSaveMethod;
};


//JDaemon definition
//this class run in background and provide some global function for app
class JDaemon: public JModule{
public:
    JDaemon();
    ~JDaemon();

    //init function, invoked by the module theard init
    JUINT32 InitFunc();
    //event process function, invoked by the module thread main event loop
    JUINT32 EventProcFunc(JEvent* pEvent);

    //contruct a event body
    JEventBody* MakeEventBody(JUINT32 uiType);

    JUINT32 ProcSetCfgEvent(JDaemonCfg* pDaemonCfg);
    JUINT32 ProcGetCfgEvent(JEvent* pEvent);

    JSER_PERSISTENCE_TYPE GetCfgSaveMethod();

private:
    //through this object to save class data
    JSerialization* m_pSerialization;

    //lock to protect multhread access
    JLock m_Lock;

    JDaemonCfg m_cfg;

    JUINT32 SetCfgList(JDaemonCfg* pDaemonCfg);
    JUINT32 GetCfgList(JDaemonCfg* pDaemonCfg);
};

}


#endif

