Introduce
=====
This project include a application framework, the purpose is to provide some common mechanism for the mutithread application, it will make develop application easily. The framework include some common modules, such as: exception, autoptr, lock, singleton, string, log, static memory, timer, socket, route, list, queue, data persistence, thread manager, etc.<br/>
This project also include two sample applications, one is a softphone called "jphone", another is a log server. Through these two sample applications, it show the framework can be reused by different applications, and the applications can focus into app special code.<br/>
Jphone is a sip softphone, it can start a call to other sip terminal device. it use pjsip stack as sip stack, and use minixml library or mysql to finish data persistence.<br/>
The log server can receive log messages from some devices, it support TCP protocol and can listen a series of ports up to ten.<br/>
These two sample developed by VC6, and can run on the windows.<br/>

Directory
=====
bin - absolute running environment<br/>
src - project source code<br/>
src/include - include files of the open source library<br/>
src/JCode - main code of the project<br/>
src/JCode/JFrameWork - source code of the core framework<br/>
src/JLogServer - vc6 project of the log server<br/>
src/JPhone - vc6 project of the softphone<br/>
src/lib - library files of the open source library<br/>

Build
=====
jphone<br/>
open JPhone.dsw in the src/JPhone directory by VC6, then build all.<br/>

log server<br/>
open JLogServer.dsw in the src/JLogServer directory by VC6, then build all.<br/>

Run
=====
There are two .exe file can be run directly in the bin directory.<br/>
