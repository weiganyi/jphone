/**********************************************************
* Author: weiganyi
* File: JEvent.h
* Date: 20121112
* Description: 
*
**********************************************************/

#ifndef _JEVENT_H_
#define _JEVENT_H_


namespace JFrameWork{

//JEvent definition
//these events are used to communicate between module or thread
typedef enum{
	JEVT_NONE,
	//check memory overwrite
	JEVT_MEMORY_CHECK,
	//dump alloc info of the memory
	JEVT_MEMORY_DUMP_MEMORY,
	//set cfg of the log module
	JEVT_LOG_SET_CFG,
	//get cfg of the log module
	JEVT_LOG_GET_CFG,
	//get cfg response of the log module
	JEVT_LOG_GET_CFG_RSP,
	//check tcp connect break to log server
	JEVT_LOG_CHECK_CONNECT,
	//timer expire
	JEVT_TIMER_EXPIRE,
	//set cfg of the deamon module
	JEVT_DAEMON_SET_CFG,
	//get cfg of the deamon module
	JEVT_DAEMON_GET_CFG,
	//get cfg response of the deamon module
	JEVT_DAEMON_GET_CFG_RSP,
	//set cfg of the sipua module
	JEVT_SIPUA_SET_CFG,
	//get cfg of the sipua module
	JEVT_SIPUA_GET_CFG,
	//get cfg response of the sipua module
	JEVT_SIPUA_GET_CFG_RSP,
	//press key of the phone
	JEVT_SIPUA_PRESS_KEY,
	//callee number get through click the contact list
	JEVT_SIPUA_CLICK_CONTACT,
	//get call status of the sipua module
	JEVT_SIPUA_GET_CALL_STATUS,
	//get call status response of the sipua module
	JEVT_SIPUA_GET_CALL_STATUS_RSP,
	//get  of the sipua module
	JEVT_SIPUA_GET_CONTACT_LIST,
	//get call history response of the sipua module
	JEVT_SIPUA_GET_CONTACT_LIST_RSP,
	//save call history in period
	JEVT_SIPUA_FLUAH_CONTACT_LIST,
	//pjsip stack callback function
	JEVT_SIPUA_CALLBACK,
	//set cfg on the log server
	JEVT_LOGSRV_SET_CFG,
	//get cfg on the log server
	JEVT_LOGSRV_GET_CFG,
	//get cfg response on the log server
	JEVT_LOGSRV_GET_CFG_RSP,
	//clean log saved on the log server
	JEVT_LOGSRV_CLEAN_LOG,
	//check whether new msg on the log server
	JEVT_LOGSRV_GET_HAS_NEW_MSG,
	//check new msg response on the log server
	JEVT_LOGSRV_GET_HAS_NEW_MSG_RSP,
	JEVT_MAX
}JEVT_TYPE;

class JEvent: public JObject{
public:
    JEvent(JEVT_TYPE type);
    //the copy construct function
    JEvent(JEvent* pEvent);
    ~JEvent();

    //the assignmen operator
    JUINT32 operator=(JEvent& rEvent);

    //the function group to set event address
    JUINT32 SetFromProc(JCHAR* pFromProc);
    JUINT32 SetFromThrd(JCHAR* pFromThrd);
    JUINT32 SetFromMod(JCHAR* pFromMod);
    JUINT32 SetToProc(JCHAR* pToProc);
    JUINT32 SetToThrd(JCHAR* pToThrd);
    JUINT32 SetToMod(JCHAR* pToMod);
    JUINT32 SetBody(JEventBody* pBody);

    //the function group to get event address
    JString& GetFromProc();
    JString& GetFromThrd();
    JString& GetFromMod();
    JString& GetToProc();
    JString& GetToThrd();
    JString& GetToMod();
    JEVT_TYPE GetEventType();
    JEventBody* GetBody();

    JCHAR* Serialize(JUINT32 *uiLen);
    JUINT32 DeSerialize(JCHAR* pBuf);

private:
    //the source process
    JString m_strFromProc;
    //the source thread
    JString m_strFromThrd;
    //the source module
    JString m_strFromMod;

    //the destination process
    JString m_strToProc;
    //the destination thread
    JString m_strToThrd;
    //the destination module
    JString m_strToMod;

    JEVT_TYPE m_eType;
    JEventBody* m_pBody;

    JUINT32 Copy(JEvent* pEvent);
};

}


#endif

