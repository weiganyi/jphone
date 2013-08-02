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

//JEvent definication
typedef enum{
	JEVT_NONE,
		JEVT_MEMORY_CHECK,
		JEVT_MEMORY_DUMP_MEMORY,
		JEVT_LOG_SET_CFG,
		JEVT_LOG_GET_CFG,
		JEVT_LOG_GET_CFG_RSP,
		JEVT_LOG_CHECK_CONNECT,
		JEVT_TIMER_EXPIRE,
		JEVT_DAEMON_SET_CFG,
		JEVT_DAEMON_GET_CFG,
		JEVT_DAEMON_GET_CFG_RSP,
		JEVT_SIPUA_SET_CFG,
		JEVT_SIPUA_GET_CFG,
		JEVT_SIPUA_GET_CFG_RSP,
		JEVT_SIPUA_PRESS_KEY,
		JEVT_SIPUA_CLICK_CONTACT,
		JEVT_SIPUA_GET_CALL_STATUS,
		JEVT_SIPUA_GET_CALL_STATUS_RSP,
		JEVT_SIPUA_GET_CONTACT_LIST,
		JEVT_SIPUA_GET_CONTACT_LIST_RSP,
		JEVT_SIPUA_FLUAH_CONTACT_LIST,
		JEVT_SIPUA_CALLBACK,
		JEVT_LOGSRV_SET_CFG,
		JEVT_LOGSRV_GET_CFG,
		JEVT_LOGSRV_GET_CFG_RSP,
		JEVT_LOGSRV_CLEAN_LOG,
		JEVT_LOGSRV_GET_HAS_NEW_MSG,
		JEVT_LOGSRV_GET_HAS_NEW_MSG_RSP,
		JEVT_MAX
}JEVT_TYPE;

class JEvent: public JObject{
public:
    JEvent(JEVT_TYPE type);
    JEvent(JEvent* pEvent);
    ~JEvent();

    JUINT32 operator=(JEvent& rEvent);

    JUINT32 SetFromProc(JCHAR* pFromProc);
    JUINT32 SetFromThrd(JCHAR* pFromThrd);
    JUINT32 SetFromMod(JCHAR* pFromMod);
    JUINT32 SetToProc(JCHAR* pToProc);
    JUINT32 SetToThrd(JCHAR* pToThrd);
    JUINT32 SetToMod(JCHAR* pToMod);
    JUINT32 SetBody(JEventBody* pBody);

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
    JString m_strFromProc;
    JString m_strFromThrd;
    JString m_strFromMod;

    JString m_strToProc;
    JString m_strToThrd;
    JString m_strToMod;

    JEVT_TYPE m_eType;
    JEventBody* m_pBody;

    JUINT32 Copy(JEvent* pEvent);
};

}


#endif

