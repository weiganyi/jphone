/**********************************************************
* Author: weiganyi
* File: JPhoneThread.cpp
* Date: 20110718
* Description: 
*
**********************************************************/

#include "JFrameWork.h"

using namespace JFrameWork;

#include "JSipUa.h"
#include "JPhoneThread.h"


DWORD JPhoneThreadProc(LPVOID* p)
{
	JUINT32 uiRet = JSUCCESS;
	JMOD_PUB_DATA modPubData;
	JTHRD_PUB_DATA thrdPubData;
    JSER_PERSISTENCE_TYPE eType = JSER_PERSISTENCE_NONE;
    JString strThrdName;
    JString strLocalAddr;

    WORD wVersion;
    WSADATA WSAData;
    wVersion=MAKEWORD(2,0);

    //initialize socket library
    WSAStartup(wVersion,&WSAData);

    //create all object
    JLog* pLog = JSingleton<JLog>::instance();
    JStaticMemory* pStaticMemory = JSingleton<JStaticMemory>::instance();
	JRoute* pRoute = JSingleton<JRoute>::instance();

    JTimer* pTimer = JSingleton<JTimer>::instance();
    JDaemon* pDaemon = JSingleton<JDaemon>::instance();
	
	JPjSipUa* pPjSipUa = JSingleton<JPjSipUa>::instance();

	//construct module thread object
    strThrdName = JS_T_JMAINTHREAD;
    strLocalAddr = JTHREAD_LOCAL_ADDR;
	JModuleThread* pModuleThread = 
	    new JModuleThread(&strThrdName, &strLocalAddr, JTHREAD_LOCAL_PORT);

    //set persistence type
    eType = pDaemon->GetCfgSaveMethod();
    pPjSipUa->SetPersistenceType(eType);
    pLog->SetPersistenceType(eType);

    //register module
    modPubData.strModName = JS_M_JMEMORY;
    modPubData.eLevel = JMOD_LEVEL_1;
    modPubData.pModule = pStaticMemory;
	pModuleThread->RegisterModule(modPubData);
	modPubData.strModName.Clear();

    modPubData.strModName = JS_M_JLOG;
    modPubData.eLevel = JMOD_LEVEL_3;
    modPubData.pModule = pLog;
	pModuleThread->RegisterModule(modPubData);
	modPubData.strModName.Clear();

    modPubData.strModName = JS_M_JTIMER;
    modPubData.eLevel = JMOD_LEVEL_6;
    modPubData.pModule = pTimer;
	pModuleThread->RegisterModule(modPubData);
	modPubData.strModName.Clear();

    modPubData.strModName = JS_M_JDAEMON;
    modPubData.eLevel = JMOD_LEVEL_6;
    modPubData.pModule = pDaemon;
	pModuleThread->RegisterModule(modPubData);
	modPubData.strModName.Clear();

    modPubData.strModName = JS_M_JSIPUA;
    modPubData.eLevel = JMOD_LEVEL_6;
    modPubData.pModule = pPjSipUa;
	pModuleThread->RegisterModule(modPubData);
	modPubData.strModName.Clear();

    //register thread
    JThreadManager* pThreadManager = JSingleton<JThreadManager>::instance();
    thrdPubData.strThrdName = JS_T_JMAINTHREAD;
    thrdPubData.pThread = pModuleThread;
    pThreadManager->RegisterThread(thrdPubData);

    //start threadmanager to run
	uiRet = pThreadManager->Run();
	if (uiRet != JSUCCESS)
	{
	    *pLog << set(JLOG_MOD_PHONE, JLOG_ERROR_LEVEL) << "JPhoneThreadProc return a failure status, exit!";
	}

    //cleanup socket library
    WSACleanup();

	return JSUCCESS;
}

