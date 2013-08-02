/**********************************************************
* Author: weiganyi
* File: JString.h
* Date: 20121112
* Description: 
*
**********************************************************/

#ifndef _JSTRING_H_
#define _JSTRING_H_


namespace JFrameWork{

//JString definication
class JString: public JObject{
public:
    JString(JCHAR* pStr=0, JStaticMemory* pAllocator=JNULL);
    ~JString();
    JString(JString& rString);

    JUINT32 operator=(JString& rString);
    JUINT32 operator=(JCHAR* pStr);

    JBOOL operator==(JString& rString);
    JBOOL operator==(JCHAR* pStr);

    JBOOL operator!=(JString& rString);
    JBOOL operator!=(JCHAR* pStr);

    JString& operator+=(JString& rString);
    JString& operator+=(JCHAR* pStr);

    JUINT32 Clear();

    JCHAR* c_str();
    JUINT32 GetLength();
    JStaticMemory* GetAllocator();
    JUINT32 SetAllocator(JStaticMemory* pAllocator);

private:
    JStaticMemory* m_pAllocator;
    JUINT32 m_uiLen;
    JCHAR* m_pData;

    JUINT32 Copy(JString& rString);
    JUINT32 Copy(JCHAR* pStr);
    JBOOL NoEqual(JString& rString);
    JBOOL NoEqual(JCHAR* pStr);
};

}


#endif

