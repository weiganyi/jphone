/**********************************************************
* Author: weiganyi
* File: JLogServerAgent.h
* Date: 20120504
* Description: 
*
**********************************************************/

#ifndef _JLOGSERVER_AGENT_H_
#define _JLOGSERVER_AGENT_H_


//JLogServerAgent definication
//the time length for wait response after get request sending
#define JLOGSERVER_AGENT_SELECT_TIME   500*1000    //millisecond
#define JLOGSERVER_AGENT_SELECT_TIME2  100*1000    //millisecond

//the communication socket address in the log server agent 
#define JLOGSERVER_AGENT_LOCAL_ADDR JPHONE_LOCAL_ADDR
#define JLOGSERVER_AGENT_LOCAL_PORT 10003+JPRODUCT_PORT_OFFSET

//thread name of the log server agent
#define JS_T_JLOGSRVAGENT   "t_jlogsrvagent"

class JLogServerAgent: public JObject{
public:
    JLogServerAgent();
    ~JLogServerAgent();

    JUINT32 SetLogSrvCfg(JLogSrvCfg* pLogSrvCfg);
    JUINT32 GetLogSrvCfg();
    JUINT32 GetLogSrvCfgRsp(JLogSrvCfg* pLogSrvCfg);

    JUINT32 CleanLog(JLogSrvNumber* pLogSrvNumber);

    JUINT32 GetHasNewMsg(JLogSrvNumber* pLogSrvNumber);
    JUINT32 GetHasNewMsgRsp(JLogSrvHasNewMsg* pLogSrvHasNewMsg);

    JUINT32 GetLogMsg(JCHAR* currLogSrv, 
                    JCHAR* logMessage, 
                    JUINT32 uiMaxLen, 
                    JUINT32* pOffset);

private:
    //the agent thread object
    JAgentThread* m_pAgentThread;
};


#endif

