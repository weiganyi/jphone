# Microsoft Developer Studio Project File - Name="JLogServer" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=JLogServer - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "JLogServer.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "JLogServer.mak" CFG="JLogServer - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "JLogServer - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "JLogServer - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "JLogServer - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "JLogServer - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "../JCode" /I "../JCode/JFrameWork" /I "../include/mxml-2.7" /I "../include/mysql" /I "../include/pjproject-1.10" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "JPURE_CORE" /D JPRUDUCT_PORT_OFFSET=10000 /FR /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ws2_32.lib winmm.lib ../lib/mysql/libmysql.lib ../lib/mxml-2.7/mxml1.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "JLogServer - Win32 Release"
# Name "JLogServer - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "JCode"

# PROP Default_Filter ""
# Begin Group "JFrameWork"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\JCode\JFrameWork\JDaemon.cpp
# End Source File
# Begin Source File

SOURCE=..\JCode\JFrameWork\JEvent.cpp
# End Source File
# Begin Source File

SOURCE=..\JCode\JFrameWork\JLock.cpp
# End Source File
# Begin Source File

SOURCE=..\JCode\JFrameWork\JLog.cpp
# End Source File
# Begin Source File

SOURCE=..\JCode\JFrameWork\JMemory.cpp
# End Source File
# Begin Source File

SOURCE=..\JCode\JFrameWork\JObject.cpp
# End Source File
# Begin Source File

SOURCE=..\JCode\JFrameWork\JRoute.cpp
# End Source File
# Begin Source File

SOURCE=..\JCode\JFrameWork\JSerialization.cpp
# End Source File
# Begin Source File

SOURCE=..\JCode\JFrameWork\JSocket.cpp
# End Source File
# Begin Source File

SOURCE=..\JCode\JFrameWork\JString.cpp
# End Source File
# Begin Source File

SOURCE=..\JCode\JFrameWork\JThread.cpp
# End Source File
# Begin Source File

SOURCE=..\JCode\JFrameWork\JTimer.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=..\JCode\JLogSrv.cpp
# End Source File
# Begin Source File

SOURCE=..\JCode\JLogSrvAgent.cpp
# End Source File
# Begin Source File

SOURCE=..\JCode\JLogSrvThread.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\CfgListen.cpp
# End Source File
# Begin Source File

SOURCE=.\JLogServer.cpp
# End Source File
# Begin Source File

SOURCE=.\JLogServer.rc
# End Source File
# Begin Source File

SOURCE=.\JLogServerDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\JLogServerView.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\CfgListen.h
# End Source File
# Begin Source File

SOURCE=.\JLogServer.h
# End Source File
# Begin Source File

SOURCE=.\JLogServerDoc.h
# End Source File
# Begin Source File

SOURCE=.\JLogServerView.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\JLogServer.ico
# End Source File
# Begin Source File

SOURCE=.\res\JLogServer.rc2
# End Source File
# Begin Source File

SOURCE=.\res\JLogServerDoc.ico
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
