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


//the specialization template

//JAutoPtr specialized by JLock, in order to auto release lock before function exit
template<> inline JAutoPtr<JLock>::JAutoPtr(JLock* pObject);

template<> inline JAutoPtr<JLock>::~JAutoPtr();


//include the template implement
#include "JAutoPtr.cpp"


}


#endif

