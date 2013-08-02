/**********************************************************
* Author: weiganyi
* File: JList_T.cpp
* Date: 20110718
* Description: 
*
**********************************************************/

#ifndef _JLIST_T_CPP_
#define _JLIST_T_CPP_


template<class TYPE> JListItem<TYPE>::JListItem(TYPE* ptrData)
{
    if (!ptrData)
    {
        cout << "JListItem::JListItem input param is NULL" << endl;
        return;
    }

    m_pData = ptrData;

    m_pPrev = JNULL;
    m_pNext = JNULL;

	return;
}

template<class TYPE> JListItem<TYPE>::~JListItem()
{
    if (m_pData)
    {
        JSingleton<JStaticMemory>::instance()->Free(reinterpret_cast<JCHAR*>(m_pData));
    }

	return;
}

template<class TYPE> JUINT32 JListItem<TYPE>::SetPrevItem(JListItem* pItem)
{
    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_LIST, "JListItem::SetPrevItem");

    m_pPrev = pItem;

    return JSUCCESS;
}

template<class TYPE> JUINT32 JListItem<TYPE>::SetNextItem(JListItem* pItem)
{
    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_LIST, "JListItem::SetNextItem");

    m_pNext = pItem;

    return JSUCCESS;
}

template<class TYPE> JListItem<TYPE>* JListItem<TYPE>::GetPrevItem()
{
    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_LIST, "JListItem::GetPrevItem");

    return m_pPrev;
}

template<class TYPE> JListItem<TYPE>* JListItem<TYPE>::GetNextItem()
{
    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_LIST, "JListItem::GetNextItem");

    return m_pNext;
}

template<class TYPE> TYPE* JListItem<TYPE>::GetData()
{
    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_LIST, "JListItem::GetData");

    return m_pData;
}

template<class TYPE> JUINT32 JListItem<TYPE>::SetDataLength(JUINT32 uiLength)
{
    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_LIST, "JListItem::SetDataLength");

    m_Length = uiLength;

    return JSUCCESS;
}

template<class TYPE> JUINT32 JListItem<TYPE>::GetDataLength()
{
    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_LIST, "JListItem::GetDataLength");

    return m_Length;
}

template<class TYPE> JListIterator<TYPE>::JListIterator(JList<TYPE>& rList):m_rList(rList)
{
    m_pCurrListItem = m_rList.GetHeadItem();

    return;
}

template<class TYPE> JListIterator<TYPE>::~JListIterator()
{
    return;
}

template<class TYPE> JVOID JListIterator<TYPE>::First()
{
    m_pCurrListItem = m_rList.GetHeadItem();

    return;
}

template<class TYPE> JVOID JListIterator<TYPE>::Last()
{
    m_pCurrListItem = m_rList.GetTailItem();

    return;
}

template<class TYPE> JVOID JListIterator<TYPE>::Next()
{
    m_pCurrListItem = m_rList.GetNextItem(m_pCurrListItem);

    return;
}

template<class TYPE> JVOID JListIterator<TYPE>::Prev()
{
    m_pCurrListItem = m_rList.GetPrevItem(m_pCurrListItem);

    return;
}

template<class TYPE> JBOOL JListIterator<TYPE>::Done()
{
    if (m_pCurrListItem != JNULL)
    {
        return JTRUE;
    }
    else
    {
        return JFALSE;
    }
}

template<class TYPE> JListItem<TYPE>* JListIterator<TYPE>::Item()
{
    return m_pCurrListItem;
}

template<class TYPE> JList<TYPE>::JList()
{
    m_pHead = JNULL;
    m_pTail = JNULL;

	return;
}

template<class TYPE> JList<TYPE>::JList(JList& rList)
{
    CopyObject(*this, rList);

	return;
}

template<class TYPE> JList<TYPE>::~JList()
{
    RemoveAllItem();

	return;
}

template<class TYPE> JList<TYPE>& JList<TYPE>::operator=(JList& rList)
{
    RemoveAllItem();

    CopyObject(*this, rList);

    return *this;
}

template<class TYPE> JUINT32 JList<TYPE>::InsertItem(JListItem<TYPE>* pItem, JListItem<TYPE>* pTarget)
{
    JListItem<TYPE>* pTempItem = JNULL;

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_LIST, "JList::InsertItem");

    if (!pItem)
    {
        JSingleton<JLog>::instance2() << set(JLOG_MOD_LIST, JLOG_ERROR_LEVEL)
            << "JList::InsertItem input param is NULL\n";
        return JFAILURE;
    }

    if (pTarget)    //insert pItem into the back of the pTarget
    {
        if (m_pTail && m_pTail==pTarget)
        {
            pItem->SetPrevItem(pTarget);
            pItem->SetNextItem(JNULL);
            pTarget->SetNextItem(pItem);
            m_pTail = pItem;
        }
        else
        {
            pTempItem = pTarget->GetNextItem();
            pItem->SetPrevItem(pTarget);
            pItem->SetNextItem(pTempItem);
            pTempItem->SetPrevItem(pItem);
            pTarget->SetNextItem(pItem);
        }
    }
    else    //insert pItem into the front of the whole JList
    {
        if (m_pHead)
        {
            pItem->SetNextItem(m_pHead);
            m_pHead->SetPrevItem(pItem);
            pItem->SetPrevItem(JNULL);
            m_pHead = pItem;
        }
        else
        {
            m_pHead = pItem;
            m_pTail = pItem;
            pItem->SetPrevItem(JNULL);
            pItem->SetNextItem(JNULL);
        }
    }

    return JSUCCESS;
}

template<class TYPE> JUINT32 JList<TYPE>::RemoveItem(JListItem<TYPE>* pItem)
{
    JListItem<TYPE>* pPrevItem = JNULL;
    JListItem<TYPE>* pNextItem = JNULL;

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_LIST, "JList::RemoveItem");

    if (!pItem)
    {
        JSingleton<JLog>::instance2() << set(JLOG_MOD_LIST, JLOG_ERROR_LEVEL)
            << "JList::RemoveItem input param is NULL\n";
        return JFAILURE;
    }

    pPrevItem = pItem->GetPrevItem();
    pNextItem = pItem->GetNextItem();

    if (pPrevItem)
    {
        pPrevItem->SetNextItem(pNextItem);
    }
    else
    {
        m_pHead = pNextItem;
        if (pNextItem)
        {
            pNextItem->SetPrevItem(JNULL);
        }
    }

    if (pNextItem)
    {
        pNextItem->SetPrevItem(pPrevItem);
    }
    else
    {
        m_pTail = pPrevItem;
        if (pPrevItem)
        {
            pPrevItem->SetNextItem(JNULL);
        }
    }

    return JSUCCESS;
}

template<class TYPE> JUINT32 JList<TYPE>::RemoveAllItem()
{
    JListItem<TYPE>* pItem = JNULL;
    JListItem<TYPE>* prevItem = JNULL;

    pItem = m_pHead;
    while(pItem)
    {
        prevItem = pItem;
        pItem = prevItem->GetNextItem();
        RemoveItem(prevItem);
        delete prevItem;
    }

	return JSUCCESS;
}

template<class TYPE> JUINT32 JList<TYPE>::RemoveLastItem()
{
    JListItem<TYPE>* pItem = JNULL;

    pItem = GetTailItem();
    RemoveItem(pItem);
    delete pItem;

	return JSUCCESS;
}

template<class TYPE> JListItem<TYPE>* JList<TYPE>::GetPrevItem(JListItem<TYPE>* pTarget)
{
    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_LIST, "JList::GetPrevItem");

    if (!pTarget)
    {
        JSingleton<JLog>::instance2() << set(JLOG_MOD_LIST, JLOG_ERROR_LEVEL)
            << "JList::GetPrevItem input param is NULL\n";
        return JNULL;
    }

    return pTarget->GetPrevItem();
}

template<class TYPE> JListItem<TYPE>* JList<TYPE>::GetNextItem(JListItem<TYPE>* pTarget)
{
    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_LIST, "JList::GetNextItem");

    if (!pTarget)
    {
        JSingleton<JLog>::instance2() << set(JLOG_MOD_LIST, JLOG_ERROR_LEVEL)
            << "JList::GetNextItem input param is NULL\n";
        return JNULL;
    }

    return pTarget->GetNextItem();
}

template<class TYPE> JListItem<TYPE>* JList<TYPE>::GetHeadItem()
{
    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_LIST, "JList::GetHeadItem");

    return m_pHead;
}

template<class TYPE> JListItem<TYPE>* JList<TYPE>::GetTailItem()
{
    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_LIST, "JList::GetTailItem");

    return m_pTail;
}

template<class TYPE> JUINT32 JList<TYPE>::GetItemNum()
{
    JUINT32 uiNum = 0;

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_LIST, "JList::GetItemNum");

    JListIterator<TYPE> clsListIter(*this);
    for (clsListIter.First(); clsListIter.Done(); clsListIter.Next())
    {
        uiNum++;
    }

    return uiNum;
}

template<class TYPE> JUINT32 JList<TYPE>::CopyObject(JList<TYPE>& rDst, JList<TYPE>& rSrc)
{
    TYPE* pDstData = JNULL;
    JListItem<TYPE>* pDstItem = JNULL;
    JUINT32 uiLen = 0;
    TYPE* pSrcData = JNULL;
    JListItem<TYPE>* pSrcItem = JNULL;
    JListItem<TYPE>* prevDstItem = JNULL;

    rDst.m_pHead = JNULL;
    rDst.m_pTail = JNULL;

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_LIST, "JList::CopyObject");

    JListIterator<TYPE> clsListIter(rSrc);
    for (clsListIter.First(); clsListIter.Done(); clsListIter.Next())
    {
        pSrcItem = clsListIter.Item();

        pDstData = JNULL;

        pSrcData = pSrcItem->GetData();
        uiLen = SafeStrlen(reinterpret_cast<JCHAR*>(pSrcData));
        if (uiLen)
        {
            pDstData = reinterpret_cast<TYPE*>(JSingleton<JStaticMemory>::instance()->Alloc(uiLen+1));
            if (pDstData)
            {
	            SafeMemset(reinterpret_cast<JCHAR*>(pDstData), 0, uiLen+1);
                SafeStrcpy(reinterpret_cast<JCHAR*>(pDstData), reinterpret_cast<JCHAR*>(pSrcData), uiLen+1);
            }
            else
            {
                JSingleton<JLog>::instance2() << set(JLOG_MOD_LIST, JLOG_ERROR_LEVEL)
                    << "JList::CopyObject memory alloc failure\n";
                return JFAILURE;
            }
        }

        pDstItem = new JListItem<TYPE>(pDstData);
        rDst.InsertItem(pDstItem, prevDstItem);

        prevDstItem = pDstItem;
    }

    return JSUCCESS;
}

template<class TYPE> JQueue<TYPE>::JQueue()
{
	return;
}

template<class TYPE> JQueue<TYPE>::~JQueue()
{
    m_List.RemoveAllItem();

	return;
}

template<class TYPE> JUINT32 JQueue<TYPE>::EnQueue(JListItem<TYPE>* pItem)
{
    JListItem<TYPE>* pTail = JNULL;

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_LIST, "JQueue::EnQueue");

    if (!pItem)
    {
        JSingleton<JLog>::instance2() << set(JLOG_MOD_LIST, JLOG_ERROR_LEVEL)
            << "JQueue::EnQueue input param is NULL\n";
        return JFAILURE;
    }

    pTail = m_List.GetTailItem();
    m_List.InsertItem(pItem, pTail);

    return JSUCCESS;
}

template<class TYPE> JListItem<TYPE>* JQueue<TYPE>::DeQueue()
{
    JListItem<TYPE>* pHead = JNULL;

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_LIST, "JQueue::DeQueue");

    pHead = m_List.GetHeadItem();
    if (pHead)
    {
        m_List.RemoveItem(pHead);

        return pHead;
    }
    else
    {
        return JNULL;
    }
}

template<class TYPE> JUINT32 JQueue<TYPE>::IsEmpty()
{
    JUINT32 uiNum = 0;

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_LIST, "JQueue::IsEmpty");

    uiNum = m_List.GetItemNum();
    if (uiNum)
    {
        return JFAILURE;
    }
    else
    {
        return JSUCCESS;
    }
}


#endif

