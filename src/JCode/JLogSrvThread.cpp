/**********************************************************
* Author: weiganyi
* File: JLogServerThread.cpp
* Date: 20121128
* Description: 
*
**********************************************************/

#include "JFrameWork.h"

using namespace JFrameWork;

#include "JLogSrv.h"
#include "JLogSrvThread.h"


DWORD JLogSrvThreadProc(LPVOID* p)
{
	JUINT32 uiRet = JSUCCESS;
	JMOD_PUB_DATA modPubData;
	JTHRD_PUB_DATA thrdPubData;
    JString strThrdName;
    JString strLocalAddr;
    JLogMsgThread* pLogMsgThread;
    JThreadManager* pThreadManager = JSingleton<JThreadManager>::instance();

    WORD wVersion;
    WSADATA WSAData;
    wVersion=MAKEWORD(2,0);

    //initialize socket library
    WSAStartup(wVersion,&WSAData);

    //create all object
    JLog* pLog = JSingleton<JLog>::instance();
    JStaticMemory* pStaticMemory = JSingleton<JStaticMemory>::instance();
	JRoute* pRoute = JSingleton<JRoute>::instance();

	JLogSrv* pLogSrv = JSingleton<JLogSrv>::instance();

    //create JLogSrvThread thread object
    strThrdName = JS_T_JMAINTHREAD;
    strLocalAddr = JTHREAD_LOCAL_ADDR;
	JLogSrvThread* pLogSrvThread = 
	    new JLogSrvThread(&strThrdName, &strLocalAddr, JTHREAD_LOCAL_PORT);

    thrdPubData.strThrdName = JS_T_JMAINTHREAD;
    thrdPubData.pThread = pLogSrvThread;
    pThreadManager->RegisterThread(thrdPubData);

    //create JLogMsgThread thread1 object
    strThrdName = JS_T_JLOGMSG1;
	pLogMsgThread = new JLogMsgThread(&strThrdName);

    thrdPubData.strThrdName = JS_T_JLOGMSG1;
    thrdPubData.pThread = pLogMsgThread;
    pThreadManager->RegisterThread(thrdPubData);

    //create JLogMsgThread thread2 object
    strThrdName = JS_T_JLOGMSG2;
	pLogMsgThread = new JLogMsgThread(&strThrdName);

    thrdPubData.strThrdName = JS_T_JLOGMSG2;
    thrdPubData.pThread = pLogMsgThread;
    pThreadManager->RegisterThread(thrdPubData);

    //create JLogMsgThread thread3 object
    strThrdName = JS_T_JLOGMSG3;
	pLogMsgThread = new JLogMsgThread(&strThrdName);

    thrdPubData.strThrdName = JS_T_JLOGMSG3;
    thrdPubData.pThread = pLogMsgThread;
    pThreadManager->RegisterThread(thrdPubData);

    //create JLogMsgThread thread4 object
    strThrdName = JS_T_JLOGMSG4;
	pLogMsgThread = new JLogMsgThread(&strThrdName);

    thrdPubData.strThrdName = JS_T_JLOGMSG4;
    thrdPubData.pThread = pLogMsgThread;
    pThreadManager->RegisterThread(thrdPubData);

    //create JLogMsgThread thread5 object
    strThrdName = JS_T_JLOGMSG5;
	pLogMsgThread = new JLogMsgThread(&strThrdName);

    thrdPubData.strThrdName = JS_T_JLOGMSG5;
    thrdPubData.pThread = pLogMsgThread;
    pThreadManager->RegisterThread(thrdPubData);

    //create JLogMsgThread thread6 object
    strThrdName = JS_T_JLOGMSG6;
	pLogMsgThread = new JLogMsgThread(&strThrdName);

    thrdPubData.strThrdName = JS_T_JLOGMSG6;
    thrdPubData.pThread = pLogMsgThread;
    pThreadManager->RegisterThread(thrdPubData);

    //create JLogMsgThread thread7 object
    strThrdName = JS_T_JLOGMSG7;
	pLogMsgThread = new JLogMsgThread(&strThrdName);

    thrdPubData.strThrdName = JS_T_JLOGMSG7;
    thrdPubData.pThread = pLogMsgThread;
    pThreadManager->RegisterThread(thrdPubData);

    //create JLogMsgThread thread8 object
    strThrdName = JS_T_JLOGMSG8;
	pLogMsgThread = new JLogMsgThread(&strThrdName);

    thrdPubData.strThrdName = JS_T_JLOGMSG8;
    thrdPubData.pThread = pLogMsgThread;
    pThreadManager->RegisterThread(thrdPubData);

    //create JLogMsgThread thread9 object
    strThrdName = JS_T_JLOGMSG9;
	pLogMsgThread = new JLogMsgThread(&strThrdName);

    thrdPubData.strThrdName = JS_T_JLOGMSG9;
    thrdPubData.pThread = pLogMsgThread;
    pThreadManager->RegisterThread(thrdPubData);

    //create JLogMsgThread thread10 object
    strThrdName = JS_T_JLOGMSG10;
	pLogMsgThread = new JLogMsgThread(&strThrdName);

    thrdPubData.strThrdName = JS_T_JLOGMSG10;
    thrdPubData.pThread = pLogMsgThread;
    pThreadManager->RegisterThread(thrdPubData);

    //start threadmanager to run
	uiRet = pThreadManager->Run();
	if (uiRet != JSUCCESS)
	{
	    *pLog << set(JLOG_MOD_LOGSRV, JLOG_ERROR_LEVEL) << "JLogServerThreadProc return a failure status, exit!";
	}

    //cleanup socket library
    WSACleanup();

	return JSUCCESS;
}

