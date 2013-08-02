/**********************************************************
* Author: weiganyi
* File: JSingleton_T.h
* Date: 20121105
* Description: 
*
**********************************************************/

#ifndef _JSINGLETON_T_H_
#define _JSINGLETON_T_H_


namespace JFrameWork{

//JSingleton definication
template<class TYPE>
class JSingleton: public JObject{
public:
    JSingleton();
    ~JSingleton();

    static TYPE* instance();
    static TYPE& instance2();
    static TYPE* try_instance();

private:
    static TYPE* m_pObject;
};


//include the template implement
#include "JSingleton_T.cpp"

}


#endif

