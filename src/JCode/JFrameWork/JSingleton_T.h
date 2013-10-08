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

//JSingleton definition
template<class TYPE>
class JSingleton: public JObject{
public:
    JSingleton();
    ~JSingleton();

    //return pointer
    static TYPE* instance();

    //return reference
    static TYPE& instance2();

    //only return object but not create new one
    static TYPE* try_instance();

private:
    //the object pointer
    static TYPE* m_pObject;
};


//include the template implement
#include "JSingleton_T.cpp"

}


#endif

