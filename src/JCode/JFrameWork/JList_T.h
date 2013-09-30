/**********************************************************
* Author: weiganyi
* File: JList_T.h
* Date: 20110718
* Description: 
*
**********************************************************/

#ifndef _JLIST_T_H_
#define _JLIST_T_H_


namespace JFrameWork{

//JListItem definication
template<class TYPE> class JListItem: public JObject{
public:
    JListItem(TYPE* ptrData);
    ~JListItem();

    JUINT32 SetPrevItem(JListItem* pItem);
    JUINT32 SetNextItem(JListItem* pItem);
    JListItem* GetPrevItem();
    JListItem* GetNextItem();
    TYPE* GetData();
    JUINT32 SetDataLength(JUINT32 uiLength);
    JUINT32 GetDataLength();

private:
    //the stored data
    TYPE* m_pData;
    JUINT32 m_Length;

    JListItem* m_pPrev;
    JListItem* m_pNext;
};


//JListIterator definication
template<class TYPE> class JListIterator: public JObject{
public:
    JListIterator(JList<TYPE>& rList);
    ~JListIterator();

    JVOID First();
    JVOID Last();
    JVOID Next();
    JVOID Prev();
    JBOOL Done();
    JListItem<TYPE>* Item();

private:
    //the iterator list
    JList<TYPE>& m_rList;

    //current location of the list 
    JListItem<TYPE>* m_pCurrListItem;
};


//JList definication
template<class TYPE> class JList: public JObject{
public:
    JList();
    //copy construct function
    JList(JList& rLog);
    ~JList();

    //assignment operator
    JList& operator=(JList& rList);

    JUINT32 InsertItem(JListItem<TYPE>* pItem, JListItem<TYPE>* pTarget);
    JUINT32 RemoveItem(JListItem<TYPE>* pItem);

    JUINT32 RemoveAllItem();
    JUINT32 RemoveTailItem();

    JListItem<TYPE>* GetPrevItem(JListItem<TYPE>* pTarget);
    JListItem<TYPE>* GetNextItem(JListItem<TYPE>* pTarget);
    JListItem<TYPE>* GetHeadItem();
    JListItem<TYPE>* GetTailItem();

    JUINT32 GetItemNum();

private:
    //point to the head of the list
    JListItem<TYPE>* m_pHead;
    //point to the tail of the list
    JListItem<TYPE>* m_pTail;

    JUINT32 CopyObject(JList& rDst, JList& rSrc);
};


//JQueue definication
template<class TYPE> class JQueue: public JObject{
public:
    JQueue();
    ~JQueue();

    JUINT32 EnQueue(JListItem<TYPE>* pItem);
    JListItem<TYPE>* DeQueue();

    JUINT32 IsEmpty();

private:
    //the stored list for queue
    JList<TYPE> m_List;
};


//include the template implement
#include "JList_T.cpp"


}


#endif

