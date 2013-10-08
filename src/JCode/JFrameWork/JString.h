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

//JString definition
class JString: public JObject{
public:
    JString(JCHAR* pStr=0, JStaticMemory* pAllocator=JNULL);
    ~JString();
    JString(JString& rString);

    //assinment operator
    JUINT32 operator=(JString& rString);
    JUINT32 operator=(JCHAR* pStr);

    //equal operator
    JBOOL operator==(JString& rString);
    JBOOL operator==(JCHAR* pStr);

    //unequal operator
    JBOOL operator!=(JString& rString);
    JBOOL operator!=(JCHAR* pStr);

    //add operator
    JString& operator+=(JString& rString);
    JString& operator+=(JCHAR* pStr);

    JUINT32 Clear();

    JCHAR* c_str();

    JUINT32 GetLength();
    JStaticMemory* GetAllocator();
    JUINT32 SetAllocator(JStaticMemory* pAllocator);

private:
    //static memory handler
    JStaticMemory* m_pAllocator;

    //data and length
    JCHAR* m_pData;
    JUINT32 m_uiLen;

    JUINT32 Copy(JString& rString);
    JUINT32 Copy(JCHAR* pStr);
    JBOOL NoEqual(JString& rString);
    JBOOL NoEqual(JCHAR* pStr);
};

}


#endif

