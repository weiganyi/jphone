/**********************************************************
* Author: weiganyi
* File: JFrameWork.h
* Date: 20120423
* Description: 
*
**********************************************************/

#ifndef _JFRAMEWORK_H_
#define _JFRAMEWORK_H_


//resolve issue: VC6 π”√TryEnterCriticalSection ±±‡“Î¥ÌŒÛ°∞error C2065: 'TryEnterCriticalSection' : 
//undeclared identifier°±
#define  _WIN32_WINNT   0x0400

//header file of the system lib
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <windows.h>
#include <winbase.h>
#include <iostream.h>
#include <mmsystem.h>
#include <winsock.h>
#include <typeinfo.h> 
#include <new.h>

//header file of the project class
#include "JObject.h"
#include "JString.h"
#include "JLock.h"
#include "JAutoPtr_T.h"
#include "JLog.h"
#include "JSingleton_T.h"
//the following class will use the class JLog
#include "JList_T.h"
#include "JSocket.h"
#include "JTimer.h"
#include "JMemory.h"
#include "JEvent.h"
#include "JRoute.h"
#include "JThread.h"
#include "JSerialization.h"
#include "JDaemon.h"
#include "JAutoPtr.h"
#include "JList.h"


#endif

