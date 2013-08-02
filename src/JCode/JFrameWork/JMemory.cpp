/**********************************************************
* Author: weiganyi
* File: JMemory.cpp
* Date: 20110718
* Description: 
*
**********************************************************/

#include "JFrameWork.h"


namespace JFrameWork{

JMemory::JMemory()
{
	return;
}

JMemory::~JMemory()
{
	return;
}

//trunk memory section
JTRUNK_MEMORY_CFG gTrunkMemoryCfg[] = {
 {JTMEM_BLK_SIZE1, JTMEM_BLK_NUM1},
 {JTMEM_BLK_SIZE2, JTMEM_BLK_NUM2},
 {JTMEM_BLK_SIZE3, JTMEM_BLK_NUM3},
 {JTMEM_BLK_SIZE4, JTMEM_BLK_NUM4}
 };

JTrunkMemory::JTrunkMemory(JMEMORY_SIZE eSizeType):m_eSizeType(eSizeType)
{
    JUINT32 idx = 0;;

    m_blkTypeNum = 0;
    for (idx=0; idx<JTMEM_MAX_BLK_TYPE_NUM; idx++)
    {
        m_blkStartAddr[idx] = 0;
        m_blkSize[idx] = 0;
        m_blkTotalSize[idx] = 0;
        m_blkNum[idx] = 0;

        m_blkMaxAllocNum[idx] = 0;
        m_blkFreeNum[idx] = 0;
    }

    m_pTrunkMemory = JNULL;

	return;
}

JTrunkMemory::~JTrunkMemory()
{
    if (m_pTrunkMemory)
        delete [] m_pTrunkMemory;

	return;
}

JUINT32 JTrunkMemory::Init()
{
    JUINT32 idx = 0;
    JUINT32 idx2 = 0;
    JUINT32 uiTotalSize = 0;
    JCHAR* pStartPos = JNULL;

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_MEMORY, "JTrunkMemory::Init");

    m_blkTypeNum = sizeof(gTrunkMemoryCfg)/ sizeof(JTRUNK_MEMORY_CFG);

    if (m_blkTypeNum >= JTMEM_MAX_BLK_TYPE_NUM)
    {
        JSingleton<JLog>::instance2() << set(JLOG_MOD_MEMORY, JLOG_ERROR_LEVEL)
            << "JTrunkMemory::Init Block type number is too much\n";
        return JFAILURE;
    }

    for (idx=0; idx<m_blkTypeNum; idx++)
    {
        m_blkSize[idx] = gTrunkMemoryCfg[idx].uiSize;
        m_blkTotalSize[idx] = gTrunkMemoryCfg[idx].uiSize+sizeof(JTRUNK_HDR)+sizeof(JTRUNK_TAL);
        m_blkNum[idx] = gTrunkMemoryCfg[idx].uiNum/m_eSizeType;
    }

    for (idx=0; idx<m_blkTypeNum; idx++)
    {
        uiTotalSize += (m_blkTotalSize[idx] * m_blkNum[idx]);
    }

    if (uiTotalSize)
        m_pTrunkMemory = new char[uiTotalSize+1];

    if (!m_pTrunkMemory)
    {
        JSingleton<JLog>::instance2() << set(JLOG_MOD_MEMORY, JLOG_ERROR_LEVEL)
            << "JTrunkMemory::Malloc memory failure\n";
        return JFAILURE;
    }

    SafeMemset(m_pTrunkMemory, 0, uiTotalSize+1);

    m_blkStartAddr[0] = m_pTrunkMemory;
    for (idx=1; idx<m_blkTypeNum; idx++)
    {
        m_blkStartAddr[idx] = m_blkStartAddr[idx-1] + (m_blkTotalSize[idx-1] * m_blkNum[idx-1]);
    }

    for (idx=0; idx<m_blkTypeNum; idx++)
    {
        for (idx2=0; idx2<m_blkNum[idx]; idx2++)
        {
            pStartPos = m_blkStartAddr[idx] + (m_blkTotalSize[idx] * idx2);
            (reinterpret_cast<JTRUNK_HDR*>(pStartPos))->uiAdjust = 0xA55A;
            (reinterpret_cast<JTRUNK_HDR*>(pStartPos))->ucMemoryType = JMEM_TYPE_TRUNK;
            pStartPos += (sizeof(JTRUNK_HDR) + m_blkSize[idx]);
            (reinterpret_cast<JTRUNK_TAL*>(pStartPos))->uiAdjust = 0xA55A;
        }
    }

    for (idx=0; idx<m_blkTypeNum; idx++)
    {
        m_blkFreeNum[idx] = m_blkNum[idx];
    }

    return JSUCCESS;
}

JCHAR* JTrunkMemory::Alloc(const JUINT32 uiLen)
{
    JUINT32 idx = 0;
    JUINT32 idx2 = 0;
    JTRUNK_HDR* pBlkHdr = JNULL;
	JCHAR* pAddr = JNULL;
    JUINT32 uiHdrPadSize = sizeof(JTRUNK_HDR);
    JTRUNK_TAL* pBlkTal = JNULL;

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_MEMORY, "JTrunkMemory::Alloc");

    if (!uiLen)
    {
        JSingleton<JLog>::instance2() << set(JLOG_MOD_MEMORY, JLOG_ERROR_LEVEL)
            << "JTrunkMemory::Alloc input param is NULL\n";
        return JNULL;
    }

    for (idx=0; idx<m_blkTypeNum; idx++)
    {
        if (uiLen < m_blkSize[idx])
            break;
    }

    if (idx<m_blkTypeNum)
    {
        if (m_blkFreeNum[idx])
        {
            for (idx2=0; idx2<m_blkNum[idx]; idx2++)
            {
                pBlkHdr = reinterpret_cast<JTRUNK_HDR*>(m_blkStartAddr[idx] + idx2*m_blkTotalSize[idx]);
                if (!pBlkHdr->ubIsUsed)
                {
                    pBlkHdr->ubIsUsed = JTRUE;
                    pBlkHdr->usAllocSize = uiLen;

                    JSingleton<JLog>::instance()->GetFuncTrace(JTMEM_CALL_TRACE_NUM, pBlkHdr->uiCallTrace);

                    m_blkFreeNum[idx]--;

                    if ((m_blkNum[idx]-m_blkFreeNum[idx]) > m_blkMaxAllocNum[idx])
                        m_blkMaxAllocNum[idx] = m_blkNum[idx]-m_blkFreeNum[idx];

                    pAddr = reinterpret_cast<JCHAR*>(reinterpret_cast<JUINT32>(pBlkHdr)+uiHdrPadSize);
                    pBlkTal = reinterpret_cast<JTRUNK_TAL*>(pAddr+m_blkSize[idx]);
                    return pAddr;
                }
            }
        }
    }

    return JNULL;
}

JVOID JTrunkMemory::Free(JCHAR* ptrAddr)
{
    JUINT32 idx = 0;
    JUINT32 idx2 = 0;
    JUINT32 idx3 = 0;
    JTRUNK_HDR* pBlkHdr = JNULL;
    JTRUNK_HDR* pStartPos = JNULL;

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_MEMORY, "JTrunkMemory::Free");

    if (!ptrAddr)
    {
        JSingleton<JLog>::instance2() << set(JLOG_MOD_MEMORY, JLOG_ERROR_LEVEL)
            << "JTrunkMemory::Free input param is NULL\n";
        return;
    }

    pBlkHdr = reinterpret_cast<JTRUNK_HDR*>(ptrAddr-sizeof(JTRUNK_HDR));
    for (idx=0; idx<m_blkTypeNum; idx++)
    {
        if (pBlkHdr->usAllocSize < m_blkSize[idx])
            break;
    }

    if (idx>=m_blkTypeNum)
    {
        JSingleton<JLog>::instance2() << set(JLOG_MOD_MEMORY, JLOG_ERROR_LEVEL) 
            << "JTrunkMemory::Free this memory is exceed the max memory length, ptrAddr is:" 
            << ptrAddr 
            << "\n";
        return;
    }

    for (idx2=0; idx2<m_blkNum[idx]; idx2++)
    {
        pStartPos = reinterpret_cast<JTRUNK_HDR*>(m_blkStartAddr[idx] + (m_blkTotalSize[idx] * idx2));
        if (pStartPos == pBlkHdr)
        {
            if (!pBlkHdr->ubIsUsed)
            {
                JSingleton<JLog>::instance2() << set(JLOG_MOD_MEMORY, JLOG_ERROR_LEVEL)
                    << "JTrunkMemory::Free this memory has already free\n";
                return;
            }
            else if (pBlkHdr->ucMemoryType != JMEM_TYPE_TRUNK)
            {
                JSingleton<JLog>::instance2() << set(JLOG_MOD_MEMORY, JLOG_ERROR_LEVEL)
                    << "JTrunkMemory::Free this memory has been overwrite\n";
                return;
            }

            pBlkHdr->ubIsUsed = JFALSE;
            pBlkHdr->usAllocSize = 0;

            for (idx3=0; idx3<JTMEM_CALL_TRACE_NUM; idx3++)
            {
                if (pBlkHdr->uiCallTrace[idx3])
                {
                    delete [] pBlkHdr->uiCallTrace[idx3];
                    pBlkHdr->uiCallTrace[idx3] = 0;
                }
            }

            m_blkFreeNum[idx]++;
            return;
        }
    }

    JSingleton<JLog>::instance2() << set(JLOG_MOD_MEMORY, JLOG_ERROR_LEVEL)
        << "JTrunkMemory::Free this memory isn't found, ptrAddr is:\n" 
        << ptrAddr;

	return;
}

JUINT32 JTrunkMemory::DumpMemory()
{
    JUINT32 uiIdx = 0;
    JUINT32 uiIdx2 = 0;
    JUINT32 uiIdx3 = 0;
    JTRUNK_HDR* pBlkHdr = JNULL;
    JUINT32 uiHdrPadSize = sizeof(JTRUNK_HDR);
    JCHAR* pAddr = JNULL;
    JUINT32 uiAddr = 0;

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_MEMORY, "JTrunkMemory::DumpMemory");

    JSingleton<JLog>::instance2() << set(JLOG_MOD_MEMORY, JLOG_DEBUG_LEVEL) 
        << "JTrunkMemory::DumpMemory nofree memory dump\n"
        << "==========================================\n";

    for (uiIdx=0; uiIdx<m_blkTypeNum; uiIdx++)
    {
        JSingleton<JLog>::instance2() << set(JLOG_MOD_MEMORY, JLOG_DEBUG_LEVEL) 
            << "=====size "
            << m_blkSize[uiIdx]
            << " dump=====\n";

        for (uiIdx2=0; uiIdx2<m_blkNum[uiIdx]; uiIdx2++)
        {
            pBlkHdr = reinterpret_cast<JTRUNK_HDR*>(m_blkStartAddr[uiIdx] + uiIdx2*m_blkTotalSize[uiIdx]);
            if (pBlkHdr->ubIsUsed)
            {
                pAddr = reinterpret_cast<JCHAR*>(reinterpret_cast<JUINT32>(pBlkHdr)+uiHdrPadSize);
                uiAddr = reinterpret_cast<JUINT32>(pAddr);
                JSingleton<JLog>::instance2() << set(JLOG_MOD_MEMORY, JLOG_DEBUG_LEVEL) 
                    << "addr "
                    << uiAddr
                    << " size "
                    << pBlkHdr->usAllocSize
                    << " calltrace:\n";

                for (uiIdx3=0; uiIdx3<JTMEM_CALL_TRACE_NUM; uiIdx3++)
                {
                    if (pBlkHdr->uiCallTrace[uiIdx3] && SafeStrlen(pBlkHdr->uiCallTrace[uiIdx3]))
                    {
                        JSingleton<JLog>::instance2() << set(JLOG_MOD_MEMORY, JLOG_DEBUG_LEVEL) 
                            << pBlkHdr->uiCallTrace[uiIdx3]
                            << "\n";
                    }
                    else
                    {
                        break;
                    }
                }

                JSingleton<JLog>::instance2() << set(JLOG_MOD_MEMORY, JLOG_DEBUG_LEVEL) 
                    << "\n";
            }
        }
    }

    return JSUCCESS;
}

JUINT32 JTrunkMemory::DoMemoryCheck()
{
    JUINT32 idx = 0;
    JUINT32 idx2 = 0;
    JUINT32 idx3 = 0;
    JTRUNK_HDR* pBlkHdr = JNULL;
    JTRUNK_TAL* pBlkTal = JNULL;
    JUINT32 uiHdrPadSize = sizeof(JTRUNK_HDR);
    JCHAR* pAddr = JNULL;

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_MEMORY, "JTrunkMemory::DoMemoryCheck");

    for (idx=0; idx<m_blkTypeNum; idx++)
    {
        for (idx2=0; idx2<m_blkNum[idx]; idx2++)
        {
            pBlkHdr = reinterpret_cast<JTRUNK_HDR*>(m_blkStartAddr[idx] + (m_blkTotalSize[idx] * idx2));
            if (pBlkHdr->ubIsUsed)
            {
                pBlkTal = reinterpret_cast<JTRUNK_TAL*>(reinterpret_cast<JUINT32>(pBlkHdr) + uiHdrPadSize + m_blkSize[idx]);
                if (pBlkHdr->uiAdjust != 0xA55A || 
                    pBlkTal->uiAdjust != 0xA55A)
                {
                    pAddr = reinterpret_cast<JCHAR*>(reinterpret_cast<JUINT32>(pBlkHdr) + uiHdrPadSize);

                    JSingleton<JLog>::instance2() << set(JLOG_MOD_MEMORY, JLOG_ERROR_LEVEL) 
                        << "JTrunkMemory::DoMemoryCheck memory adjust had been changed\n";

                    JSingleton<JLog>::instance2() << set(JLOG_MOD_MEMORY, JLOG_ERROR_LEVEL) 
                        << "JTrunkMemory::DoMemoryCheck addr is:"
                        << reinterpret_cast<JUINT32>(pAddr)
                        << ", alloc size is:"
                        << pBlkHdr->usAllocSize
                        << "\n";

                    for (idx3=0; idx3<JTMEM_CALL_TRACE_NUM; idx3++)
                    {
                        if (pBlkHdr->uiCallTrace[idx3])
                        {
                            JSingleton<JLog>::instance2() << set(JLOG_MOD_MEMORY, JLOG_ERROR_LEVEL) 
                                << "JTrunkMemory::DoMemoryCheck call function trace "
                                << idx3
                                << " is"
                                << pBlkHdr->uiCallTrace[idx3]
                                << "\n";
                        }
                    }

                    JSingleton<JLog>::instance2() << set(JLOG_MOD_MEMORY, JLOG_ERROR_LEVEL) 
                        << "\n";
                }
            }
        }
    }

	return JSUCCESS;
}

JUINT32 JTrunkMemory::SetMemorySize(JMEMORY_SIZE eSizeType)
{
    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_MEMORY, "JTrunkMemory::SetMemorySize");

    m_eSizeType = eSizeType;

	return JSUCCESS;
}


//heap memory section
JHeapMemory::JHeapMemory(JMEMORY_SIZE eSizeType):m_eSizeType(eSizeType)
{
    m_pHeapMemory = JNULL;

	return;
}

JHeapMemory::~JHeapMemory()
{
    if (m_pHeapMemory)
        delete [] m_pHeapMemory;

	return;
}

JUINT32 JHeapMemory::Init()
{
    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_MEMORY, "JHeapMemory::Init");

    m_pHeapMemory = new char[JHMEM_MAX_HEAP_LEN/m_eSizeType];
    if (m_pHeapMemory)
        SafeMemset(m_pHeapMemory, 0, JHMEM_MAX_HEAP_LEN/m_eSizeType);

    return JSUCCESS;
}

JCHAR* JHeapMemory::Alloc(const JUINT32 uiLen)
{
    JCHAR* ptrAddr = JNULL;
    JListItem<JHEAP_ALLOC_INFO>* pItem1 = JNULL;
    JListItem<JHEAP_ALLOC_INFO>* pItem2 = JNULL;
    JHEAP_ALLOC_INFO* ptrInfo1 = JNULL;
    JHEAP_ALLOC_INFO* ptrInfo2 = JNULL;
    JUINT32 uiPadSize = sizeof(JHEAP_HDR)+sizeof(JHEAP_TAL);
    JUINT32 uiTmpLen = 0;

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_MEMORY, "JHeapMemory::Alloc");

    if (m_AllocInfo.GetItemNum())   //m_AllocInfo list isn't null
    {
        JListIterator<JHEAP_ALLOC_INFO> clsListIter1(m_AllocInfo);
        JListIterator<JHEAP_ALLOC_INFO> clsListIter2(m_AllocInfo);
        for (clsListIter1.First(), clsListIter2.First(); 
            clsListIter1.Done(); 
            clsListIter1.Next())
        {
            pItem1 = clsListIter1.Item();

            clsListIter2.Next();

            if (clsListIter2.Done())    //pItem1 isn't the last item of list
            {
                pItem2 = clsListIter2.Item();

                ptrInfo1 = pItem1->GetData();
                ptrInfo2 = pItem2->GetData();
                if (ptrInfo1 && ptrInfo2)
                {
                    uiTmpLen = reinterpret_cast<JUINT32>(ptrInfo1->pEnd)+uiLen+uiPadSize;
                    if (uiTmpLen <= reinterpret_cast<JUINT32>(ptrInfo2->pStart))
                    {
                        ptrAddr = privAlloc(ptrInfo1->pEnd, uiLen, pItem1);
                        if (!ptrAddr)
                        {
                            JSingleton<JLog>::instance2() << set(JLOG_MOD_MEMORY, JLOG_ERROR_LEVEL) 
                                << "JHeapMemory::Alloc alloc memory failure1\n";
                            return JNULL;
                        }
                        else
                        {
                            return ptrAddr;
                        }
                    }
                }
            }
            else    //pItem1 is the last item of list
            {
                ptrInfo1 = pItem1->GetData();
                if (ptrInfo1)
                {
                    uiTmpLen = reinterpret_cast<JUINT32>(ptrInfo1->pEnd)+uiLen+uiPadSize;
                    if ((uiTmpLen - reinterpret_cast<JUINT32>(m_pHeapMemory)) <= 
                        (JHMEM_MAX_HEAP_LEN/static_cast<JUINT32>(m_eSizeType))
                        )
                    {
                        ptrAddr = privAlloc(ptrInfo1->pEnd, uiLen, pItem1);
                        if (!ptrAddr)
                        {
                            JSingleton<JLog>::instance2() << set(JLOG_MOD_MEMORY, JLOG_ERROR_LEVEL) 
                                << "JHeapMemory::Alloc alloc memory failure2\n";
                            return JNULL;
                        }
                        else
                        {
                            return ptrAddr;
                        }
                    }
                }
            }
        }
    }
    else    /* m_AllocInfo list is null */
    {
        ptrAddr = privAlloc(m_pHeapMemory, uiLen, NULL);
        if (!ptrAddr)
        {
            JSingleton<JLog>::instance2() << set(JLOG_MOD_MEMORY, JLOG_ERROR_LEVEL) 
                << "JHeapMemory::Alloc alloc memory failure3\n";
            return JNULL;
        }
        else
        {
            return ptrAddr;
        }
    }

    JSingleton<JLog>::instance2() << set(JLOG_MOD_MEMORY, JLOG_ERROR_LEVEL) 
        << "JHeapMemory::Alloc alloc memory failure4\n";
    return JNULL;
}

JVOID JHeapMemory::Free(JCHAR* ptrAddr)
{
    JUINT32 uiIdx = 0;
    JListItem<JHEAP_ALLOC_INFO>* pItem = JNULL;
    JHEAP_ALLOC_INFO* ptrInfo = JNULL;
    JUINT32 uiPadSize = sizeof(JHEAP_HDR)+sizeof(JTRUNK_TAL);
    JUINT32 uiHeadPadSize = sizeof(JHEAP_HDR);
    JHEAP_HDR* ptrHdr = JNULL;

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_MEMORY, "JHeapMemory::Free");

    if (!ptrAddr)
    {
        JSingleton<JLog>::instance2() << set(JLOG_MOD_MEMORY, JLOG_ERROR_LEVEL) 
            << "JHeapMemory::Free input param is NULL\n";
        return;
    }

    JListIterator<JHEAP_ALLOC_INFO> clsListIter(m_AllocInfo);
    for (clsListIter.First(); clsListIter.Done(); clsListIter.Next())
    {
        pItem = clsListIter.Item();

        ptrInfo = pItem->GetData();
        if (ptrInfo)
        {
            ptrHdr = reinterpret_cast<JHEAP_HDR*>(ptrInfo->pStart);
            if((ptrInfo->pStart+uiHeadPadSize) == ptrAddr)
            {
                if (ptrHdr->ucMemoryType != JMEM_TYPE_HEAP)
                {
                    JSingleton<JLog>::instance2() << set(JLOG_MOD_MEMORY, JLOG_ERROR_LEVEL) 
                        << "JHeapMemory::Free this memory has been overwrite\n";
                    return;
                }

                m_AllocInfo.RemoveItem(pItem);
                delete pItem;

                for (uiIdx=0; uiIdx<JTMEM_CALL_TRACE_NUM; uiIdx++)
                {
                    if (ptrHdr->uiCallTrace[uiIdx])
                    {
                        delete [] ptrHdr->uiCallTrace[uiIdx];
                        ptrHdr->uiCallTrace[uiIdx] = 0;
                    }
                }

                SafeMemset(ptrInfo->pStart, 0, ptrHdr->usAllocSize+uiPadSize);
                return;
            }
        }
    }

    JSingleton<JLog>::instance2() << set(JLOG_MOD_MEMORY, JLOG_ERROR_LEVEL)
        << "JHeapMemory::Free this memory isn't found, ptrAddr is:" 
        << ptrAddr
        << "\n";

	return;
}

JCHAR* JHeapMemory::privAlloc(JCHAR* ptrStart, const JUINT32 uiLen, JListItem<JHEAP_ALLOC_INFO>* ptrItem)
{
    JUINT32 uiPadSize = sizeof(JHEAP_HDR)+sizeof(JHEAP_TAL);
    JUINT32 uiHeadPadSize = sizeof(JHEAP_HDR);
    JCHAR* ptrAddr = JNULL;
    JHEAP_ALLOC_INFO* ptrInfo = JNULL;
    JListItem<JHEAP_ALLOC_INFO>* ptrNewItem = JNULL;
    JHEAP_HDR*  ptrHdr = JNULL;
    JHEAP_TAL*  ptrTal = JNULL;

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_MEMORY, "JHeapMemory::privAlloc");

    if (!ptrStart)
    {
        JSingleton<JLog>::instance2() << set(JLOG_MOD_MEMORY, JLOG_ERROR_LEVEL) 
            << "JHeapMemory::privAlloc input param is NULL\n";
        return JNULL;
    }

    SafeMemset(ptrStart, 0, uiLen+uiPadSize);

    ptrInfo = reinterpret_cast<JHEAP_ALLOC_INFO*>(ptrStart);
    ptrInfo->pStart = ptrStart;
    ptrInfo->pEnd = ptrStart+uiLen+uiPadSize;

    ptrNewItem = new JListItem<JHEAP_ALLOC_INFO>(ptrInfo);
    m_AllocInfo.InsertItem(ptrNewItem, ptrItem);

    ptrHdr = reinterpret_cast<JHEAP_HDR*>(ptrStart);
    ptrHdr->usAllocSize = uiLen;
    ptrHdr->ucMemoryType = JMEM_TYPE_HEAP;
    JSingleton<JLog>::instance()->GetFuncTrace(JHMEM_CALL_TRACE_NUM, ptrHdr->uiCallTrace);
    ptrHdr->uiAdjust = 0xA55A;

    ptrTal = reinterpret_cast<JHEAP_TAL*>(ptrStart + uiHeadPadSize + uiLen);
    ptrTal->uiAdjust = 0xA55A;

    ptrAddr = ptrStart + uiHeadPadSize;

    return ptrAddr;
}

JUINT32 JHeapMemory::DumpMemory()
{
    JListItem<JHEAP_ALLOC_INFO>* pItem = JNULL;
    JHEAP_ALLOC_INFO* ptrInfo = JNULL;
    JHEAP_HDR*  ptrHdr = JNULL;
    JUINT32 uiHeadPadSize = sizeof(JHEAP_HDR);
    JUINT32 uiIdx = 0;
    JUINT32 uiAddr = 0;

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_MEMORY, "JHeapMemory::DumpMemory");

    JSingleton<JLog>::instance2() << set(JLOG_MOD_MEMORY, JLOG_DEBUG_LEVEL) 
        << "JHeapMemory::DumpMemory nofree memory dump\n"
        << "==========================================\n";

    JListIterator<JHEAP_ALLOC_INFO> clsListIter(m_AllocInfo);
    for (clsListIter.First(); clsListIter.Done(); clsListIter.Next())
    {
        pItem = clsListIter.Item();

        ptrInfo = pItem->GetData();
        if (ptrInfo)
        {
            ptrHdr = reinterpret_cast<JHEAP_HDR*>(ptrInfo);

            uiAddr = reinterpret_cast<JUINT32>(ptrInfo->pStart+uiHeadPadSize);
            JSingleton<JLog>::instance2() << set(JLOG_MOD_MEMORY, JLOG_DEBUG_LEVEL) 
                << "addr "
                << uiAddr
                << " size "
                << ptrHdr->usAllocSize
                << " calltrace:\n";

            for (uiIdx=0; uiIdx<JTMEM_CALL_TRACE_NUM; uiIdx++)
            {
                if (ptrHdr->uiCallTrace[uiIdx] && SafeStrlen(ptrHdr->uiCallTrace[uiIdx]))
                {
                    JSingleton<JLog>::instance2() << set(JLOG_MOD_MEMORY, JLOG_DEBUG_LEVEL) 
                        << ptrHdr->uiCallTrace[uiIdx]
                        << "\n";
                }
                else
                {
                    break;
                }
            }

            JSingleton<JLog>::instance2() << set(JLOG_MOD_MEMORY, JLOG_DEBUG_LEVEL) 
                << "\n";
        }
    }

    return JSUCCESS;
}

JUINT32 JHeapMemory::DoMemoryCheck()
{
    JUINT32 idx = 0;
    JListItem<JHEAP_ALLOC_INFO>* pItem = JNULL;
    JHEAP_ALLOC_INFO* ptrInfo = JNULL;
    JHEAP_HDR* ptrHdr = JNULL;
    JHEAP_TAL* ptrTal = JNULL;
    JUINT32 uiHeadPadSize = sizeof(JHEAP_HDR);
    JCHAR* pAddr = JNULL;

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_MEMORY, "JHeapMemory::DoMemoryCheck");

    JListIterator<JHEAP_ALLOC_INFO> clsListIter(m_AllocInfo);
    for (clsListIter.First(); clsListIter.Done(); clsListIter.Next())
    {
        pItem = clsListIter.Item();

        ptrInfo = pItem->GetData();
        if (ptrInfo)
        {
            ptrHdr = reinterpret_cast<JHEAP_HDR*>(ptrInfo->pStart);
            ptrTal = reinterpret_cast<JHEAP_TAL*>(ptrInfo->pStart + uiHeadPadSize + ptrHdr->usAllocSize);
            if (ptrHdr->uiAdjust != 0xA55A || 
                ptrTal->uiAdjust != 0xA55A)
            {
                pAddr = reinterpret_cast<JCHAR*>(ptrInfo->pStart + uiHeadPadSize);

                JSingleton<JLog>::instance2() << set(JLOG_MOD_MEMORY, JLOG_ERROR_LEVEL) 
                    << "JHeapMemory::DoMemoryCheck memory adjust had been changed\n";

                JSingleton<JLog>::instance2() << set(JLOG_MOD_MEMORY, JLOG_ERROR_LEVEL) 
                    << "JHeapMemory::DoMemoryCheck addr is:"
                    << reinterpret_cast<JUINT32>(pAddr)
                    << ", alloc size is:"
                    << ptrHdr->usAllocSize
                    << "\n";

                for (idx=0; idx<JTMEM_CALL_TRACE_NUM; idx++)
                {
                    if (ptrHdr->uiCallTrace[idx])
                    {
                        JSingleton<JLog>::instance2() << set(JLOG_MOD_MEMORY, JLOG_ERROR_LEVEL) 
                            << "JHeapMemory::DoMemoryCheck call function trace "
                            << idx
                            << " is"
                            << ptrHdr->uiCallTrace[idx]
                            << "\n";
                    }
                }

                JSingleton<JLog>::instance2() << set(JLOG_MOD_MEMORY, JLOG_ERROR_LEVEL) 
                    << "\n";
            }
        }
    }

	return JSUCCESS;
}

JUINT32 JHeapMemory::SetMemorySize(JMEMORY_SIZE eSizeType)
{
    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_MEMORY, "JHeapMemory::SetMemorySize");

    m_eSizeType = eSizeType;

	return JSUCCESS;
}


//static memory section
JMEMORY_TYPE g_eMemoryType[JMEMORY_NUM] = {
JMEM_TYPE_TRUNK,
JMEM_TYPE_HEAP
};

JUINT32 OnCheckMemory(JVOID* pData)
{
    JStaticMemory* pStaticMemory = reinterpret_cast<JStaticMemory*>(pData);
    JEvent* pEvent = JNULL;
    JCommEngine* pComEngine = JNULL;
    JThread* pThread = JNULL;
    JModuleThread* pModuleThread = JNULL;

    if (!pStaticMemory)
    {
        return JFAILURE;
    }

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_MEMORY, "OnCheckMemory");

    pEvent = new JEvent(JEVT_MEMORY_CHECK);
    if (pEvent)
    {
        pEvent->SetFromProc(JS_P_JMAINPROC);
        pEvent->SetFromThrd(JS_T_JMAINTHREAD);
        pEvent->SetFromMod(JS_M_JMEMORY);
        pEvent->SetToProc(JS_P_JMAINPROC);
        pEvent->SetToThrd(JS_T_JMAINTHREAD);
        pEvent->SetToMod(JS_M_JMEMORY);
        pEvent->SetBody(JNULL);

        pThread = JSingleton<JThreadManager>::instance()->GetThread(JS_T_JMAINTHREAD);
        pModuleThread = dynamic_cast<JModuleThread*>(pThread);
        if (pModuleThread)
        {
    	    pComEngine = pModuleThread->GetNotifyCommEngine();
    	    if (pComEngine)
    	    {
    	        pComEngine->SendEvent(pEvent);
    	    }
	    }
    }

	return JSUCCESS;
}

JStaticMemory::JStaticMemory(JMEMORY_SIZE eSizeType):m_eSizeType(eSizeType)
{
    JUINT32 uiIdx = 0;

    m_timeHandler = 0;

    for (uiIdx=0; uiIdx<JMEMORY_NUM; uiIdx++)
    {
        m_eMemoryType[uiIdx] = g_eMemoryType[uiIdx];

        if (m_eMemoryType[uiIdx] == JMEM_TYPE_TRUNK)
        {
            m_pMemory[uiIdx] = new JTrunkMemory(eSizeType);
        }
        else if (m_eMemoryType[uiIdx] == JMEM_TYPE_HEAP)
        {
            m_pMemory[uiIdx] = new JHeapMemory(eSizeType);
        }

        if (m_pMemory[uiIdx])
        {
            m_pMemory[uiIdx]->Init();
        }
    }

	return;
}

JStaticMemory::~JStaticMemory()
{
    JUINT32 uiIdx = 0;

    if (m_timeHandler && JSingleton<JTimer>::instance())
    {
        JSingleton<JTimer>::instance()->StopTimer(m_timeHandler);
        JSingleton<JTimer>::instance()->DestoryTimer(m_timeHandler);
    }

    for (uiIdx=0; uiIdx<JMEMORY_NUM; uiIdx++)
    {
        if (m_pMemory[uiIdx])
        {
            delete m_pMemory[uiIdx];
        }
    }

	return;
}

JCHAR* JStaticMemory::Alloc(const JUINT32 uiLen)
{
    JUINT32 uiIdx = 0;
    JCHAR* ptrAddr = JNULL;

    JAutoPtr<JLock> clsLockAutoPtr(&m_Lock);

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_MEMORY, "JStaticMemory::Alloc");

    if (!uiLen)
    {
        JSingleton<JLog>::instance2() << set(JLOG_MOD_MEMORY, JLOG_ERROR_LEVEL) 
            << "JStaticMemory::Alloc input param is NULL\n";
        return JNULL;
    }

    for (uiIdx=0; uiIdx<JMEMORY_NUM; uiIdx++)
    {
        if (m_pMemory[uiIdx])
        {
            ptrAddr = m_pMemory[uiIdx]->Alloc(uiLen);
            if (ptrAddr)
            {
                return ptrAddr;
            }
        }
    }

    JSingleton<JLog>::instance2() << set(JLOG_MOD_MEMORY, JLOG_ERROR_LEVEL) 
        << "JStaticMemory::Alloc alloc memory failure\n";

    return JNULL;
}

JVOID JStaticMemory::Free(JCHAR* ptrAddr)
{
    JUINT32 uiIdx = 0;
    JUINT32 uiOffset = 0;
    JMEMORY_TYPE* ptrMemType = JNULL;

    JAutoPtr<JLock> clsLockAutoPtr(&m_Lock);

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_MEMORY, "JStaticMemory::Free");

    if (!ptrAddr)
    {
        JSingleton<JLog>::instance2() << set(JLOG_MOD_MEMORY, JLOG_ERROR_LEVEL) 
            << "JStaticMemory::Free input param is NULL\n";
        return;
    }

    uiOffset = sizeof(JUINT32) + sizeof(JMEMORY_TYPE);
    ptrMemType = reinterpret_cast<JMEMORY_TYPE*>(ptrAddr-uiOffset);
    if (ptrMemType)
    {
        for (uiIdx=0; uiIdx<JMEMORY_NUM; uiIdx++)
        {
            if (m_eMemoryType[uiIdx] == *ptrMemType && m_pMemory[uiIdx])
            {
                m_pMemory[uiIdx]->Free(ptrAddr);
                return;
            }
        }
    }

    JSingleton<JLog>::instance2() << set(JLOG_MOD_MEMORY, JLOG_ERROR_LEVEL)
        << "JStaticMemory::Free this memory type is wrong, ptrAddr is:" << ptrAddr 
        << "type is:" << static_cast<JUINT8>(*ptrMemType) 
        << "\n";

	return;
}

JUINT32 JStaticMemory::InitFunc()
{
    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_MEMORY, "JStaticMemory::InitFunc");

    m_timeHandler = JSingleton<JTimer>::instance()->CreateTimer(JTIMER_TYPE_PERIODIC, 
                                        OnCheckMemory, 
                                        this, 
                                        JMEMORY_CHECK_INTERVAL);
    if (m_timeHandler)
    {
        JSingleton<JTimer>::instance()->StartTimer(m_timeHandler);
    }

    return JSUCCESS;
}

JUINT32 JStaticMemory::EventProcFunc(JEvent* pEvent)
{
    JEVT_TYPE type = JEVT_NONE;

    JAutoPtr<JLock> clsLockAutoPtr(&m_Lock);

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_MEMORY, "JStaticMemory::EventProcFunc");

    if (!pEvent)
    {
        JSingleton<JLog>::instance2()<< set(JLOG_MOD_MEMORY, JLOG_ERROR_LEVEL) 
            << "JStaticMemory::EventProcFunc input param is NULL\n";
        return JFAILURE;
    }

    type = pEvent->GetEventType();
    switch(type)
    {
        case JEVT_MEMORY_CHECK:
        {
            DoMemoryCheck();
        }
        break;

        case JEVT_MEMORY_DUMP_MEMORY:
        {
            DumpMemory();
        }
        break;

        default:
        break;
    };

	return JSUCCESS;
}

JEventBody* JStaticMemory::MakeEventBody(JUINT32 uiType)
{
	JEventBody* pEventBody = JNULL;

	return pEventBody;
}

JUINT32 JStaticMemory::DumpMemory()
{
    JUINT32 uiIdx = 0;

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_MEMORY, "JStaticMemory::DumpMemory");

    for (uiIdx=0; uiIdx<JMEMORY_NUM; uiIdx++)
    {
        if (m_pMemory[uiIdx])
        {
            m_pMemory[uiIdx]->DumpMemory();
        }
    }

	return JSUCCESS;
}

JUINT32 JStaticMemory::DoMemoryCheck()
{
    JUINT32 uiIdx = 0;

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_MEMORY, "JStaticMemory::DoMemoryCheck");

    for (uiIdx=0; uiIdx<JMEMORY_NUM; uiIdx++)
    {
        if (m_pMemory[uiIdx])
        {
            m_pMemory[uiIdx]->DoMemoryCheck();
        }
    }

	return JSUCCESS;
}

JUINT32 JStaticMemory::SetMemorySize(JMEMORY_SIZE eSizeType)
{
    JUINT32 uiIdx = 0;

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_MEMORY, "JStaticMemory::SetMemorySize");

    m_eSizeType = eSizeType;

    for (uiIdx=0; uiIdx<JMEMORY_NUM; uiIdx++)
    {
        if (m_pMemory[uiIdx])
        {
            m_pMemory[uiIdx]->SetMemorySize(eSizeType);
        }
    }

	return JSUCCESS;
}

}

