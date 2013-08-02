/**********************************************************
* Author: weiganyi
* File: JList.h
* Date: 20120426
* Description: 
*
**********************************************************/

#ifndef _JLIST_H_
#define _JLIST_H_


namespace JFrameWork{


//definition of specialization template 
template<> inline JListItem<JHEAP_ALLOC_INFO>::~JListItem();

template<> inline JListItem<JEvent>::~JListItem();

template<> inline JListItem<JHashData>::~JListItem();

template<> inline JUINT32 JList<JPER_RECORD>::CopyObject(JList<JPER_RECORD>& rDst, 
                                                JList<JPER_RECORD>& rSrc);


//include the specialization template implement
#include "JList.cpp"


}


#endif

