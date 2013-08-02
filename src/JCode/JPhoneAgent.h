/**********************************************************
* Author: weiganyi
* File: JPhoneAgent.h
* Date: 20120424
* Description: 
*
**********************************************************/

#ifndef _JPHONE_AGENT_H_
#define _JPHONE_AGENT_H_


//JPhoneAgent definication
#define JPHONE_AGENT_SELECT_TIME   500*1000    //millisecond
#define JPHONE_AGENT_SELECT_TIME2  100*1000    //millisecond

#define JPHONE_AGENT_LOCAL_ADDR     JPHONE_LOCAL_ADDR
#define JPHONE_AGENT_LOCAL_PORT     10004+JPRUDUCT_PORT_OFFSET

#define JS_T_JPHONEAGENT    "t_jphoneagent"

class JPhoneAgent: public JObject{
public:
    JPhoneAgent();
    ~JPhoneAgent();

    JUINT32 PressKey(JSIPUA_KEY eKey);
    JUINT32 ClickContact(JPjSipUaClickContact* pPjSipUaClickContact);
    JUINT32 SetDaemonCfg(JDaemonCfg* pDaemonCfg);
    JUINT32 GetDaemonCfg();
    JUINT32 GetDaemonCfgRsp(JDaemonCfg* pDaemonCfg);
    JUINT32 SetSipUaCfg(JPjSipUaCfg* pPjSipUaCfg);
    JUINT32 GetSipUaCfg();
    JUINT32 GetSipUaCfgRsp(JPjSipUaCfg* pPjSipUaCfg);
    JUINT32 SetLogCfg(JLogCfg* pLogCfg);
    JUINT32 GetLogCfg();
    JUINT32 GetLogCfgRsp(JLogCfg* pLogCfg);
    JUINT32 GetStatus();
    JUINT32 GetStatusRsp(JPjSipUaCallStatus* pPjSipUaCallStatus);
    JUINT32 GetContact();
    JUINT32 GetContactRsp(JPjSipUaContactList* pPjSipUaContactList);
    JUINT32 DumpMemory();

private:
    JAgentThread* m_pAgentThread;
};


#endif

