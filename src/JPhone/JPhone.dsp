# Microsoft Developer Studio Project File - Name="JPhone" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=JPhone - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "JPhone.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "JPhone.mak" CFG="JPhone - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "JPhone - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "JPhone - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "JPhone - Win32 Release"

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

!ELSEIF  "$(CFG)" == "JPhone - Win32 Debug"

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
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "../JCode" /I "../JCode/JFrameWork" /I "../include/pjproject-1.10" /I "../include/mxml-2.7" /I "../include/mysql" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /D JPRUDUCT_PORT_OFFSET=0 /FR /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ws2_32.lib winmm.lib kernel32.lib ../lib/pjproject-1.10/pjlib-i386-win32-vc6-debug.lib ../lib/pjproject-1.10/pjlib-util-i386-win32-vc6-debug.lib ../lib/pjproject-1.10/libg7221codec-i386-win32-vc6-debug.lib ../lib/pjproject-1.10/libgsmcodec-i386-win32-vc6-debug.lib ../lib/pjproject-1.10/libilbccodec-i386-win32-vc6-debug.lib ../lib/pjproject-1.10/libmilenage-i386-win32-vc6-debug.lib ../lib/pjproject-1.10/libportaudio-i386-win32-vc6-debug.lib ../lib/pjproject-1.10/libresample-i386-win32-vc6-debug.lib ../lib/pjproject-1.10/libspeex-i386-win32-vc6-debug.lib ../lib/pjproject-1.10/libsrtp-i386-win32-vc6-debug.lib ../lib/pjproject-1.10/pjmedia-audiodev-i386-win32-vc6-debug.lib ../lib/pjproject-1.10/pjmedia-codec-i386-win32-vc6-debug.lib ../lib/pjproject-1.10/pjmedia-i386-win32-vc6-debug.lib ../lib/pjproject-1.10/pjnath-i386-win32-vc6-debug.lib ../lib/pjproject-1.10/pjsip-core-i386-win32-vc6-debug.lib ../lib/pjproject-1.10/pjsip-simple-i386-win32-vc6-debug.lib ../lib/pjproject-1.10/pjsip-ua-i386-win32-vc6-debug.lib ../lib/pjproject-1.10/pjsua-lib-i386-win32-vc6-debug.lib ../lib/mysql/libmysql.lib ../lib/mxml-2.7/mxml1.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# SUBTRACT LINK32 /nodefaultlib

!ENDIF 

# Begin Target

# Name "JPhone - Win32 Release"
# Name "JPhone - Win32 Debug"
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

SOURCE=..\JCode\JPhoneAgent.cpp
# End Source File
# Begin Source File

SOURCE=..\JCode\JPhoneThread.cpp
# End Source File
# Begin Source File

SOURCE=..\JCode\JSipUa.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\CfgDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ContactDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\JPhone.cpp
# End Source File
# Begin Source File

SOURCE=.\JPhone.rc
# End Source File
# Begin Source File

SOURCE=.\JPhoneDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\CfgDlg.h
# End Source File
# Begin Source File

SOURCE=.\ContactDlg.h
# End Source File
# Begin Source File

SOURCE=.\JPhone.h
# End Source File
# Begin Source File

SOURCE=.\JPhoneDlg.h
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

SOURCE=.\res\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\res\JPhone.ico
# End Source File
# Begin Source File

SOURCE=.\res\JPhone.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
