/**********************************************************
* Author: weiganyi
* File: JAutoPtr.h
* Date: 20121101
* Description: 
*
**********************************************************/

#ifndef _JAUTOPTR_H_
#define _JAUTOPTR_H_


namespace JFrameWork{


//definition of specialization template 
template<> inline JAutoPtr<JLock>::JAutoPtr(JLock* pObject);

template<> inline JAutoPtr<JLock>::~JAutoPtr();


//include the specialization template implement
#include "JAutoPtr.cpp"


}


#endif

