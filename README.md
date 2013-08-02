Introduce

This project include a application framework, the purpose is to provide some common mechanism for the mutithread application, it will make develop application easily. The framework include some common modules, such as: exception, autoptr, lock, singleton, string, log, memory, timer, socket, route, list, queue, persistence, manager, etc.
This project also include two sample applications, one is a softphone called "jphone", another is a log server. Through these two sample applications, it show the framework can be reused by different applications, and the applications can focus into app special code.
Jphone is a sip softphone, it can start a call to other sip terminal device. it use pjsip stack as sip stack, and use minixml library or mysql to finish data persistence.
The log server can receive log messages from some devices, it support TCP protocol and can listen a series of ports up to ten.
These two sample developed by VC6, and can run on the windows.



Directory

bin                 #absolute running environment
src                 #project source code
  include           #include files of the open source library
  JCode             #main code of the project
    JFrameWork      #source code of the core framework
  JLogServer        #vc6 project of the log server
  JPhone            #vc6 project of the softphone
  lib               #library files of the open source library



Build

jphone
open JPhone.dsw in the JPhone directory by VC6, then build all.

log server
open JLogServer.dsw in the JLogServer directory by VC6, then build all.



Run

There are two .exe file can be run directly in the bin directory.