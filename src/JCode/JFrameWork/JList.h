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


//the specialization template

//JListItem specialized by JHEAP_ALLOC_INFO, to do none in the destruct function
template<> inline JListItem<JHEAP_ALLOC_INFO>::~JListItem();

//JListItem specialized by JEvent, to delete event in the destruct function
template<> inline JListItem<JEvent>::~JListItem();

//JListItem specialized by JHashData, to delete hashdata in the destruct function
template<> inline JListItem<JHashData>::~JListItem();

//JList specialized by JPER_RECORD, have a special DataLength access
template<> inline JUINT32 JList<JPER_RECORD>::CopyObject(JList<JPER_RECORD>& rDst, 
                                                JList<JPER_RECORD>& rSrc);


//include the specialization template implement
#include "JList.cpp"


}


#endif

