/**********************************************************
* Author: weiganyi
* File: JSipUa.h
* Date: 20110718
* Description: 
*
**********************************************************/

#ifndef _JSIPUA_H_
#define _JSIPUA_H_


#include <pjlib.h>
#include <pjlib-util.h>
#include <pjnath.h>
#include <pjsip.h>
#include <pjsip_ua.h>
#include <pjsip_simple.h>
#include <pjsua-lib/pjsua.h>
#include <pjmedia.h>
#include <pjmedia-codec.h>

//debug level for sip stack
#define JPJSIP_CONSOLE_LEVEL    4
//local port for sip stack
#define JPJSIP_LOCAL_PORT       6060

//max contact number
#define JPJSIP_MAX_NUMBER       20

//status for sipua
#define JSIPUA_S_STATUS_IDLE            "idle"
#define JSIPUA_S_STATUS_REGISTERING     "registering"
#define JSIPUA_S_STATUS_REGISTERED      "registered"
#define JSIPUA_S_STATUS_COLLECT         "collect"
#define JSIPUA_S_STATUS_OUTGOING        "outgoing"
#define JSIPUA_S_STATUS_INCOMING        "incoming"
#define JSIPUA_S_STATUS_CONNECTED       "connected"
#define JSIPUA_S_STATUS_HOLDING         "holding"

//dtmf duration for sip stack
#define JSIPUA_ON_DURATION	        100
#define JSIPUA_OFF_DURATION	        100

//interval to flush contact number
#define JSIPUA_FLUSH_INTERVAL       1000*5

//key for contact number
#define JSIPUA_CONTACT_FILE         "jphone_ua_contact"
//key for config
#define JSIPUA_CFG_FILE             "jphone_ua_cfg"

//key for number
#define JSIPUA_NUMBER               "number"
//key for authname
#define JSIPUA_AUTH_NAME            "authname"
//key for password
#define JSIPUA_AUTH_PASSWORD        "authpwd"
//key for proxy address
#define JSIPUA_PROXY_ADDR           "proxyaddr"

//local address
#define JSIPUA_LOCAL_ADDR           JPHONE_LOCAL_ADDR
#define JSIPUA_LOCAL_PORT           10003+JPRODUCT_PORT_OFFSET

//sipus thread name
#define JS_T_JSIPUA             "t_jsipua"

//phone key
typedef enum{
    JSIPUA_KEY_NONE = 0,
    JSIPUA_KEY_0,
    JSIPUA_KEY_1,
    JSIPUA_KEY_2,
    JSIPUA_KEY_3,
    JSIPUA_KEY_4,
    JSIPUA_KEY_5,
    JSIPUA_KEY_6,
    JSIPUA_KEY_7,
    JSIPUA_KEY_8,
    JSIPUA_KEY_9,
    JSIPUA_KEY_STAR,
    JSIPUA_KEY_POUND,
    JSIPUA_KEY_OFFHOOK,
    JSIPUA_KEY_ONHOOK,
    JSIPUA_KEY_MAX
}JSIPUA_KEY;

//status for sipua
typedef enum{
    JSIPUA_STATUS_IDLE,
    JSIPUA_STATUS_REGISTERING,
    JSIPUA_STATUS_REGISTERED,
    JSIPUA_STATUS_COLLECT,
    JSIPUA_STATUS_OUTGOING,
    JSIPUA_STATUS_INCOMING,
    JSIPUA_STATUS_CONNECTED,
    JSIPUA_STATUS_HOLDING
}JSIPUA_STATUS;

//callback function type
typedef enum{
    JSIPUA_CB_TYPE_NONE,
    JSIPUA_CB_TYPE_INCOMING_CALL,
    JSIPUA_CB_TYPE_CALL_MEDIA_STATE,
    JSIPUA_CB_TYPE_CALL_STATE,
    JSIPUA_CB_TYPE_REG_STATE2,
    JSIPUA_CB_TYPE_INCOMING_DTMF,
    JSIPUA_CB_TYPE_MAX
}JSIPUA_CB_TYPE;

//key map
typedef struct{
    JSIPUA_KEY eKey;
    JCHAR  cKey;
}JSIPUA_KEY_MAP;

//status map
typedef struct{
    JSIPUA_STATUS eStatus;
    JCHAR*  pStatus;
}JSIPUA_STATUS_MAP;


class JPjSipUa;

//JPjSipUaCfg definition
class JPjSipUaCfg: public JEventBody{
public:
    JPjSipUaCfg();
    ~JPjSipUaCfg();

    JUINT32 GetLength();

    JEventBody* Clone();
    JUINT32 Clear();

    JUINT32 Serialize(JCHAR* pBuf, JUINT32 uiMaxNum);
    JUINT32 DeSerialize(JCHAR* pBuf);

    //assignment operator
    JUINT32 operator=(JPjSipUaCfg& rPjSipUaCfg);

    JString& GetNumber();
    JUINT32 SetNumber(JString& rStr);
    JString& GetAuthName();
    JUINT32 SetAuthName(JString& rStr);
    JString& GetAuthPasswd();
    JUINT32 SetAuthPasswd(JString& rStr);
    JString& GetProxyAddr();
    JUINT32 SetProxyAddr(JString& rStr);

private:
    //sip number
    JString m_Number;
    //sip auth number
    JString m_AuthName;
    //sip password
    JString m_AuthPasswd;
    //sip proxy address
    JString m_ProxyAddr;
};


//JPjSipUaKey definition
class JPjSipUaKey: public JEventBody{
public:
    JPjSipUaKey();
    ~JPjSipUaKey();

    JUINT32 GetLength();

    JEventBody* Clone();
    JUINT32 Clear();

    JUINT32 Serialize(JCHAR* pBuf, JUINT32 uiMaxNum);
    JUINT32 DeSerialize(JCHAR* pBuf);

    //assignment operator
    JUINT32 operator=(JPjSipUaKey& rPjSipUaKey);

    JString& GetKey();
    JUINT32 SetKey(JString& rStr);

private:
    //presss key
    JString m_Key;
};


//JPjSipUaClickContact definition
class JPjSipUaClickContact: public JEventBody{
public:
    JPjSipUaClickContact();
    ~JPjSipUaClickContact();

    JUINT32 GetLength();

    JEventBody* Clone();
    JUINT32 Clear();

    JUINT32 Serialize(JCHAR* pBuf, JUINT32 uiMaxNum);
    JUINT32 DeSerialize(JCHAR* pBuf);

    //assignment operator
    JUINT32 operator=(JPjSipUaClickContact& rPjSipUaClickContact);

    JString& GetContactNumber();
    JUINT32 SetContactNumber(JString& rStr);

private:
    //selected contact number
    JString m_ContactNumber;
};


//JPjSipUaCallStatus definition
class JPjSipUaCallStatus: public JEventBody{
public:
    JPjSipUaCallStatus();
    ~JPjSipUaCallStatus();

    JUINT32 GetLength();

    JEventBody* Clone();
    JUINT32 Clear();

    JUINT32 Serialize(JCHAR* pBuf, JUINT32 uiMaxNum);
    JUINT32 DeSerialize(JCHAR* pBuf);

    //assignment operator
    JUINT32 operator=(JPjSipUaCallStatus& rPjSipUaCallStatus);

    JString& GetNumber();
    JUINT32 SetNumber(JString& rStr);
    JString& GetStatus();
    JUINT32 SetStatus(JString& rStr);

private:
    //call number
    JString m_Number;
    //number status
    JString m_Status;
};


//JPjSipUaContactList definition
class JPjSipUaContactList: public JEventBody{
public:
    JPjSipUaContactList();
    ~JPjSipUaContactList();

    JUINT32 GetLength();

    JEventBody* Clone();
    JUINT32 Clear();

    JUINT32 Serialize(JCHAR* pBuf, JUINT32 uiMaxNum);
    JUINT32 DeSerialize(JCHAR* pBuf);

    //assignment operator
    JUINT32 operator=(JPjSipUaContactList& rPjSipUaContactList);

    JString& GetContactNumber(JUINT32 uiIdx);
    JUINT32 SetContactNumber(JUINT32 uiIdx, JString& rStr);

private:
    //contact number list
    JString m_ContactNumber[JPJSIP_MAX_NUMBER];
};


//JPjSipCallback definition
class JPjSipCallback: public JEventBody{
public:
    JPjSipCallback();
    ~JPjSipCallback();

    JUINT32 GetLength();

    JEventBody* Clone();
    JUINT32 Clear();

    JUINT32 Serialize(JCHAR* pBuf, JUINT32 uiMaxNum);
    JUINT32 DeSerialize(JCHAR* pBuf);

    //assignment operator
    JUINT32 operator=(JPjSipCallback& rPjSipCallback);

    JSIPUA_CB_TYPE GetCbType();
    JUINT32 SetCbType(JSIPUA_CB_TYPE type);
    JUINT32 GetCode();
    JUINT32 SetCode(JUINT32 code);
    pjsip_inv_state GetState();
    JUINT32 SetState(pjsip_inv_state eState);
    pjsua_acc_id GetAccId();
    JUINT32 SetAccId(pjsua_acc_id eId);
    pjsua_call_id GetCallId();
    JUINT32 SetCallId(pjsua_call_id eId);
    JString& GetRemoteNumber();
    JUINT32 SetRemoteNumber(JString& rStr);

private:
    //type for callback function
    JSIPUA_CB_TYPE eType;
    //result code for callback function
    JUINT32 uiCode;
    //state for callback function
    pjsip_inv_state	state;
    //acc id for callback function
    pjsua_acc_id acc_id;
    //call id for callback function
    pjsua_call_id call_id;
    //caller number for callback function
    JString pRemoteNumber;
};


//JPjSip definition
class JPjSip: public JObject{
public:
    JPjSip();
    ~JPjSip();

    JUINT32 SipUaCreate();
    JUINT32 SipUaDestory();

    JUINT32 Register(JString& rNumber,
                    JString& rAuthName,
                    JString& rAuthPasswd,
                    JString& rProxyAddr);
    JUINT32 UnRegister();

    JUINT32 StartCall(JString& rNumber, JString& rProxyAddr);
    JUINT32 DialDtmf(JCHAR cKey);
    JUINT32 Answer(JUINT32 uiCode);
    JUINT32 Hold();
    JUINT32 UnHold();
    JUINT32 HangUp();

    JUINT32 SetCodecPriority();
    JUINT32 GetRemoteNumber(JCHAR* remote_info, JCHAR* number);

    JVOID ProcOnIncomingCall(JPjSipCallback* pPjSipCallback, JPjSipUa* pPjSipUa);
    JVOID ProcOnCallMediaState(JPjSipCallback* pPjSipCallback, JPjSipUa* pPjSipUa);
    JVOID ProcOnCallState(JPjSipCallback* pPjSipCallback, JPjSipUa* pPjSipUa);
    JVOID ProcOnRegState2(JPjSipCallback* pPjSipCallback, JPjSipUa* pPjSipUa);
    JVOID ProcOnIncomingDtmf(JPjSipCallback* pPjSipCallback, JPjSipUa* pPjSipUa);

private:
    //acc id
    pjsua_acc_id m_accId;
    //call id
    pjsua_call_id m_callId;
};


//JPjSipUa definition
class JPjSipUa: public JModule{
public:
    JPjSipUa();
    ~JPjSipUa();

    JUINT32 SetPersistenceType(JSER_PERSISTENCE_TYPE eType);

    JUINT32 InitFunc();
    JUINT32 EventProcFunc(JEvent* pEvent);
    JEventBody* MakeEventBody(JUINT32 uiType);

    static JCHAR KeyEnum2Char(JSIPUA_KEY eKey);
    static JSIPUA_KEY KeyChar2Enum(JCHAR* pKey);

    JUINT32 StateChange(JSIPUA_STATUS eStatus);
    JUINT32 StoreContactNumber();
    JUINT32 SetCurrNumber(JString& rNumber);

    JSIPUA_STATUS GetStatus();
    JPjSip* GetPjSip();

    friend JUINT32 OnFlushContactList(JVOID* pData);
    friend JVOID OnIncomingCall(pjsua_acc_id acc_id, 
                                pjsua_call_id call_id,
        			            pjsip_rx_data *rdata);
    friend JVOID OnCallMediaState(pjsua_call_id call_id);
    friend JVOID OnCallState(pjsua_call_id call_id, pjsip_event *e);
    friend JVOID OnRegState2(pjsua_acc_id acc_id, pjsua_reg_info *info);
    friend JVOID OnIncomingDtmf(pjsua_call_id call_id, JINT32 dtmf);

private:
    //local sipua config
    JPjSipUaCfg m_cfg;

    //current sipua status
    JSIPUA_STATUS m_status;
    //current pressed number
    JString m_strCurrNumber;

    //current contact number
    JList<JCHAR>  m_contactList;
    JUINT32 m_contactListNumber;

    //timer handler
    JUINT32 m_timeHandler;

    //lock to protect callback function access
    JLock m_Lock;

    //persistence type
    JSER_PERSISTENCE_TYPE m_ePersistenceType;
    //persistence handler
    JSerialization* m_pSerialization;

    //pjsip object pointer
    JPjSip* m_pPjSip;

    //agent thread object pointer
    JAgentThread* m_pAgentThread;

    JUINT32 ProcOnFlushContactList();
    JUINT32 ProcCallBackEvent(JPjSipCallback* pPjSipCallback);

    JUINT32 ProcSetCfgEvent(JPjSipUaCfg* pPjSipUaCfg);
    JUINT32 ProcGetCfgEvent(JEvent* pEvent);
    JUINT32 ProcPressKeyEvent(JPjSipUaKey* pPjSipUaKey);
    JUINT32 ProcClickContactEvent(JPjSipUaClickContact* pPjSipUaClickContact);
    JUINT32 ProcGetCallStatusEvent(JEvent* pEvent);
    JUINT32 ProcGetContactListEvent(JEvent* pEvent);

    JCHAR* StatusEnum2Char(JSIPUA_STATUS eStatus);

    JUINT32 SetContactList(JList<JCHAR>& rSrcList);
    JUINT32 GetContactList(JList<JCHAR>& rDstList);
    JUINT32 SetCfgList(JPjSipUaCfg* pPjSipUaCfg);
    JUINT32 GetCfgList(JPjSipUaCfg* pPjSipUaCfg);
};

extern JSIPUA_KEY_MAP gKeyMap[];


#endif

