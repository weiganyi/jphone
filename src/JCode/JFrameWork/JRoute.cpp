/**********************************************************
* Author: weiganyi
* File: JRoute.cpp
* Date: 20110718
* Description: 
*
**********************************************************/

#include "JFrameWork.h"


namespace JFrameWork{

JHash::JHash()
{
	return;
}

JHash::~JHash()
{
	return;
}

JUINT32 JHash::CalcHashValue(JCHAR* pName)
{
    JCHAR* pOffset = JNULL;
    JUINT32 uiVal = 0;

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_ROUTE, "JHash::CalcHashValue");

    if (!pName)
    {
        JSingleton<JLog>::instance2() << set(JLOG_MOD_ROUTE, JLOG_ERROR_LEVEL) 
            << "JHash::CalcHashValue input param is NULL\n";
        return JINVALID;
    }

    pOffset = pName;
    uiVal = *pOffset;
    if (uiVal)
    {
        for (pOffset+=1; *pOffset!=0; ++pOffset)
        {
            uiVal = (uiVal<<5) - uiVal + *pOffset;
        }
    }

    return uiVal;
}

JHashData::JHashData()
{
	return;
}

JHashData::~JHashData()
{
	return;
}

JString& JHashData::GetProcName()
{
	return m_strProcName;
}

JUINT32 JHashData::SetProcName(JString& rStr)
{
	m_strProcName = rStr;

	return JSUCCESS;
}

JString& JHashData::GetThrdName()
{
	return m_strThrdName;
}

JUINT32 JHashData::SetThrdName(JString& rStr)
{
	m_strThrdName = rStr;

	return JSUCCESS;
}

JSOCKADDR_IN* JHashData::GetAddr()
{
	return &m_stAddr;
}

JUINT32 JHashData::SetAddr(JSOCKADDR_IN* pAddr)
{
    if (!pAddr)
    {
        return JFAILURE;
    }

	SafeMemcpy(reinterpret_cast<JCHAR*>(&m_stAddr), reinterpret_cast<JCHAR*>(pAddr), 
		sizeof(JSOCKADDR_IN), sizeof(JSOCKADDR_IN));

	return JSUCCESS;
}

JRoute::JRoute()
{
	return;
}

JRoute::~JRoute()
{
	return;
}

JUINT32 JRoute::AddRoute(JCHAR* pProcName, JCHAR* pThrdName, JSOCKADDR_IN* pAddr)
{
    JUINT32 uiRet = 0;

    JAutoPtr<JLock> clsLockAutoPtr(&m_Lock);

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_ROUTE, "JRoute::AddRoute");

    if (!pProcName && !pThrdName && !pAddr)
    {
        JSingleton<JLog>::instance2() << set(JLOG_MOD_ROUTE, JLOG_ERROR_LEVEL) 
            << "JRoute::AddRoute input param is NULL\n";
        return JFAILURE;
    }

    uiRet = privAddRoute(pProcName, pThrdName, pAddr);

    return uiRet;
}

JUINT32 JRoute::DelRoute(JCHAR* pProcName, JCHAR* pThrdName)
{
    JUINT32 iOffset = 0;
    JUINT32 iPos = 0;
    JListItem<JHashData>* pListItem = JNULL;
    JHashData* pData = JNULL;
    JCHAR strName[JMAX_STRING_LEN] = {0};
    JUINT32 uiProcLen = 0;
    JUINT32 uiThrdLen = 0;

    JAutoPtr<JLock> clsLockAutoPtr(&m_Lock);

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_ROUTE, "JRoute::DelRoute");

    if (!pProcName && !pThrdName)
    {
        JSingleton<JLog>::instance2() << set(JLOG_MOD_ROUTE, JLOG_ERROR_LEVEL) 
            << "JRoute::DelRoute input param is NULL\n";
        return JFAILURE;
    }

    uiProcLen = SafeStrlen(pProcName);
    if (uiProcLen)
    {
        SafeStrncat(strName, pProcName, uiProcLen, JMAX_STRING_LEN);
    }

    uiThrdLen = SafeStrlen(pThrdName);
    if (uiThrdLen)
    {
        SafeStrncat(strName, pThrdName, uiThrdLen, JMAX_STRING_LEN);
    }

    iOffset = JSingleton<JHash>::instance()->CalcHashValue(strName);
    if (iOffset)
    {
        iPos = iOffset % JROUTE_MAX_HASH_TABLE;

        JListIterator<JHashData> clsListIter(m_pHashTable[iPos]);
        for (clsListIter.First(); clsListIter.Done(); clsListIter.Next())
        {
            pListItem = clsListIter.Item();

            pData = pListItem->GetData();
            if (pData)
            {
                if (pData->GetProcName() == pProcName &&
                    pData->GetThrdName() == pThrdName)
                {
                    m_pHashTable[iPos].RemoveItem(pListItem);
                    delete pListItem;

                    return JSUCCESS;
                }
            }
        }
    }

    return JFAILURE;
}

JROUTE_TYPE JRoute::GetRouteType(JCHAR* pFromProcName, 
                                JCHAR* pFromThrdName,
                                JCHAR* pToProcName, 
                                JCHAR* pToThrdName)
{
    JAutoPtr<JLock> clsLockAutoPtr(&m_Lock);

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_ROUTE, "JRoute::IsQueueRoute");

    if (!pFromProcName && !pFromThrdName && !pToProcName && !pToThrdName)
    {
        JSingleton<JLog>::instance2() << set(JLOG_MOD_ROUTE, JLOG_ERROR_LEVEL) 
            << "JRoute::IsQueueRoute input param is NULL\n";
        return ROUTE_TYPE_NONE;
    }

    if (pFromProcName && pToProcName && SafeStrcmp(pFromProcName, pToProcName) == 0)
    {
        if (pFromThrdName && pToThrdName && SafeStrcmp(pFromThrdName, pToThrdName) == 0)
        {
            return ROUTE_TYPE_MODULE;
        }
        else
        {
            return ROUTE_TYPE_THREAD;
        }
    }
    else
    {
        return ROUTE_TYPE_PROCESS;
    }

    return ROUTE_TYPE_NONE;
}

JUINT32 JRoute::FindRoute(JCHAR* pProcName, JCHAR* pThrdName)
{
    JUINT32 iOffset = 0;
    JUINT32 iPos = 0;
    JListItem<JHashData>* pListItem = JNULL;
    JHashData* pData = JNULL;
    JCHAR strName[JMAX_STRING_LEN] = {0};
    JUINT32 uiProcLen = 0;
    JUINT32 uiThrdLen = 0;

    JAutoPtr<JLock> clsLockAutoPtr(&m_Lock);

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_ROUTE, "JRoute::FindRoute");

    if (!pProcName && !pThrdName)
    {
        JSingleton<JLog>::instance2() << set(JLOG_MOD_ROUTE, JLOG_ERROR_LEVEL) 
            << "JRoute::FindRoute input param is NULL\n";
        return JFAILURE;
    }

    uiProcLen = SafeStrlen(pProcName);
    if (uiProcLen)
    {
        SafeStrncat(strName, pProcName, uiProcLen, JMAX_STRING_LEN);
    }

    uiThrdLen = SafeStrlen(pThrdName);
    if (uiThrdLen)
    {
        SafeStrncat(strName, pThrdName, uiThrdLen, JMAX_STRING_LEN);
    }

    iOffset = JSingleton<JHash>::instance()->CalcHashValue(strName);
    if (iOffset)
    {
        iPos = iOffset % JROUTE_MAX_HASH_TABLE;

        JListIterator<JHashData> clsListIter(m_pHashTable[iPos]);
        for (clsListIter.First(); clsListIter.Done(); clsListIter.Next())
        {
            pListItem = clsListIter.Item();

            pData = pListItem->GetData();
            if (pData)
            {
                if (pData->GetProcName() == pProcName &&
                    pData->GetThrdName() == pThrdName)
                {
                    return JSUCCESS;
                }
            }
        }
    }

    return JFAILURE;
}

JUINT32 JRoute::GetRoute(JCHAR* pProcName, JCHAR* pThrdName, JSOCKADDR_IN* pAddr)
{
    JUINT32 iOffset = 0;
    JUINT32 iPos = 0;
    JListItem<JHashData>* pListItem = JNULL;
    JHashData* pData = JNULL;
    JCHAR strName[JMAX_STRING_LEN] = {0};
    JUINT32 uiProcLen = 0;
    JUINT32 uiThrdLen = 0;

    JAutoPtr<JLock> clsLockAutoPtr(&m_Lock);

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_ROUTE, "JRoute::GetRoute");

    if (!pProcName && !pThrdName && !pAddr)
    {
        JSingleton<JLog>::instance2() << set(JLOG_MOD_ROUTE, JLOG_ERROR_LEVEL) 
            << "JRoute::GetRoute input param is NULL\n";
        return JFAILURE;
    }

    uiProcLen = SafeStrlen(pProcName);
    if (uiProcLen)
    {
        SafeStrncat(strName, pProcName, uiProcLen, JMAX_STRING_LEN);
    }

    uiThrdLen = SafeStrlen(pThrdName);
    if (uiThrdLen)
    {
        SafeStrncat(strName, pThrdName, uiThrdLen, JMAX_STRING_LEN);
    }

    iOffset = JSingleton<JHash>::instance()->CalcHashValue(strName);
    if (iOffset)
    {
        iPos = iOffset % JROUTE_MAX_HASH_TABLE;

        JListIterator<JHashData> clsListIter(m_pHashTable[iPos]);
        for (clsListIter.First(); clsListIter.Done(); clsListIter.Next())
        {
            pListItem = clsListIter.Item();

            pData = pListItem->GetData();
            if (pData)
            {
                if (pData->GetProcName() == pProcName &&
                    pData->GetThrdName() == pThrdName)
                {
                    SafeMemcpy(reinterpret_cast<JCHAR*>(pAddr), 
                        reinterpret_cast<JCHAR*>(pData->GetAddr()), 
                        sizeof(JSOCKADDR_IN), sizeof(JSOCKADDR_IN));

                    return JSUCCESS;
                }
            }
        }
    }

    return JFAILURE;
}

JUINT32 JRoute::privAddRoute(JCHAR* pProcName, 
                            JCHAR* pThrdName, 
                            JSOCKADDR_IN* pAddr)
{
    JUINT32 iOffset = 0;
    JUINT32 iPos = 0;
    JListItem<JHashData>* pListItem = JNULL;
    JHashData* pData = JNULL;
    JUINT32 uiProcLen = 0;
    JUINT32 uiThrdLen = 0;
    JUINT32 uiExist = JFAILURE;
    JCHAR strName[JMAX_STRING_LEN] = {0};

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_ROUTE, "JRoute::privAddRoute");

    if (!pProcName && !pThrdName && !pAddr)
    {
        JSingleton<JLog>::instance2() << set(JLOG_MOD_ROUTE, JLOG_ERROR_LEVEL) 
            << "JRoute::privAddRoute input param is NULL\n";
        return JFAILURE;
    }

    uiExist = FindRoute(pProcName, pThrdName);
    if (uiExist)
    {
        JSingleton<JLog>::instance2() << set(JLOG_MOD_ROUTE, JLOG_INFO_LEVEL) 
            << "JRoute::privAddRoute this route is already exist\n";
        return JSUCCESS;
    }

    uiProcLen = SafeStrlen(pProcName);
    if (uiProcLen)
    {
        SafeStrncat(strName, pProcName, uiProcLen, JMAX_STRING_LEN);
    }

    uiThrdLen = SafeStrlen(pThrdName);
    if (uiThrdLen)
    {
        SafeStrncat(strName, pThrdName, uiThrdLen, JMAX_STRING_LEN);
    }

    iOffset = JSingleton<JHash>::instance()->CalcHashValue(strName);
    if (iOffset)
    {
        iPos = iOffset % JROUTE_MAX_HASH_TABLE;
        pData = new JHashData;
        if (pData)
        {
            if (uiProcLen)
            {
                JString strName = pProcName;
                pData->SetProcName(strName);
            }
            if (uiThrdLen)
            {
                JString strName = pThrdName;
                pData->SetThrdName(strName);
            }
            if (pAddr)
            {
                pData->SetAddr(pAddr);
            }

            pListItem = new JListItem<JHashData>(pData);
            if (pListItem)
            {
                m_pHashTable[iPos].InsertItem(pListItem, JNULL);
            }
            else
            {
                delete pData;
            }
        }
    }

    return JSUCCESS;
}

}

