/**********************************************************
* Author: weiganyi
* File: JAutoPtr_T.h
* Date: 20121101
* Description: 
*
**********************************************************/

#ifndef _JAUTOPTR_T_H_
#define _JATUOPTR_T_H_


namespace JFrameWork{

//JAutoPtrBase definication
template<class TYPE>
class JAutoPtrBase: public JObject{
public:
    JAutoPtrBase(TYPE* pObject);
    ~JAutoPtrBase();

    //the operator of object return
    TYPE& operator*();
    //the operator of member access
    TYPE* operator->();

    TYPE* GetObject();

private:
    //the pointer of stored object
    TYPE* m_pObject;
};


//JAutoPtr definication
template<class TYPE>
class JAutoPtr: public JAutoPtrBase<TYPE>{
public:
    JAutoPtr(TYPE* pObject);
    ~JAutoPtr();

    //the assignment operator
    JAutoPtr<TYPE>& operator=(const JAutoPtr<TYPE>& rhs);

private:
    JUINT32 Release();
};


//include the template implement
#include "JAutoPtr_T.cpp"

}


#endif

