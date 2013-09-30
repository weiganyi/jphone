/**********************************************************
* Author: weiganyi
* File: JList.cpp
* Date: 20120426
* Description: 
*
**********************************************************/

//implement of specialization template
template<> inline JListItem<JHEAP_ALLOC_INFO>::~JListItem()
{
}

template<> inline JListItem<JEvent>::~JListItem()
{
    if (m_pData)
    {
        delete m_pData;
    }
}

template<> inline JListItem<JHashData>::~JListItem()
{
    if (m_pData)
    {
        delete m_pData;
    }
}

template<> inline JUINT32 JList<JPER_RECORD>::CopyObject(JList<JPER_RECORD>& rDst, 
                                                        JList<JPER_RECORD>& rSrc)

{
    JPER_RECORD* pDstData = JNULL;
    JListItem<JPER_RECORD>* pDstItem = JNULL;
    JUINT32 uiLen = 0;
    JPER_RECORD* pSrcData = JNULL;
    JListItem<JPER_RECORD>* pSrcItem = JNULL;
    JListItem<JPER_RECORD>* prevDstItem = JNULL;

    rDst.m_pHead = JNULL;
    rDst.m_pTail = JNULL;

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_LIST, "JList::CopyObject");

    JListIterator<JPER_RECORD> clsListIter(rSrc);
    for (clsListIter.First(); clsListIter.Done(); clsListIter.Next())
    {
        pSrcItem = clsListIter.Item();

        pDstData = JNULL;

        pSrcData = pSrcItem->GetData();
        uiLen = pSrcItem->GetDataLength();
        if (uiLen)
        {
            //alloc memory for the dst data
            pDstData = 
                reinterpret_cast<JPER_RECORD*>(JSingleton<JStaticMemory>::instance()->Alloc(uiLen+1));
            if (pDstData)
            {
	            SafeMemset(reinterpret_cast<JCHAR*>(pDstData), 0, uiLen+1);
                SafeMemcpy(reinterpret_cast<JCHAR*>(pDstData), 
                    reinterpret_cast<JCHAR*>(pSrcData), uiLen, uiLen+1);
            }
            else
            {
                JSingleton<JLog>::instance2() << set(JLOG_MOD_LIST, JLOG_ERROR_LEVEL)
                    << "JList::CopyObject memory alloc failure\n";
                return JFAILURE;
            }
        }

        //construct the dst item
        pDstItem = new JListItem<JPER_RECORD>(pDstData);
        pDstItem->SetDataLength(uiLen);
        rDst.InsertItem(pDstItem, prevDstItem);

        prevDstItem = pDstItem;
    }

    return JSUCCESS;
}

