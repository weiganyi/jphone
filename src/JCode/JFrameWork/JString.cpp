/**********************************************************
* Author: weiganyi
* File: JString.cpp
* Date: 20121112
* Description: 
*
**********************************************************/

#include "JFrameWork.h"


namespace JFrameWork{

JString::JString(JCHAR* pStr, JStaticMemory* pAllocator):m_pAllocator(pAllocator)
{
    m_uiLen = 0;
    m_pData = 0;

    if (!m_pAllocator)
    {
        m_pAllocator = JSingleton<JStaticMemory>::try_instance();
    }

	Copy(pStr);

	return;
}

JString::~JString()
{
    if (m_uiLen && m_pAllocator)
    {
        m_pAllocator->Free(m_pData);
    }

	return;
}

JString::JString(JString& rString):m_pAllocator(rString.m_pAllocator)
{
    Copy(rString);

	return;
}

JUINT32 JString::operator=(JString& rString)
{
    m_pAllocator = rString.m_pAllocator;

    return Copy(rString);
}

JUINT32 JString::operator=(JCHAR* pStr)
{
    m_pAllocator = JSingleton<JStaticMemory>::try_instance();

    return Copy(pStr);
}

JBOOL JString::operator==(JString& rString)
{
    JBOOL bRet = JFALSE;

    bRet = NoEqual(rString);

    return !bRet;
}

JBOOL JString::operator==(JCHAR* pStr)
{
    JBOOL bRet = JFALSE;

    bRet = NoEqual(pStr);

    return !bRet;
}

JBOOL JString::operator!=(JString& rString)
{
    JBOOL bRet = JFALSE;

    bRet = NoEqual(rString);

    return bRet;
}

JBOOL JString::operator!=(JCHAR* pStr)
{
    JBOOL bRet = JFALSE;

    bRet = NoEqual(pStr);

    return bRet;
}

JString& JString::operator+=(JString& rString)
{
    JCHAR* pData = JNULL;
    JUINT32 uiLen = 0;

    if (m_uiLen && m_pData)
    {
        pData = m_pData;
        uiLen = m_uiLen;
    }

    if (rString.m_uiLen && rString.m_pData && m_pAllocator)
    {
        uiLen += rString.m_uiLen;
        m_pData = m_pAllocator->Alloc(uiLen+1);
        if (m_pData)
        {
            SafeMemset(m_pData, 0, uiLen+1);
            if (pData)
            {
                SafeMemcpy(m_pData, pData, m_uiLen, uiLen+1);
            }
            SafeMemcat(m_pData, rString.m_pData, rString.m_uiLen, uiLen+1);
            m_uiLen = uiLen;

            if (pData)
            {
                m_pAllocator->Free(pData);
            }

            return *this;
        }
    }

	return *this;
}

JString& JString::operator+=(JCHAR* pStr)
{
    JCHAR* pData = JNULL;
    JUINT32 uiLen = 0;

	if (!pStr)
	{
		return *this;
	}

    if (m_uiLen && m_pData)
    {
        pData = m_pData;
        uiLen = m_uiLen;
    }

    if (SafeStrlen(pStr) && m_pAllocator)
    {
        uiLen += SafeStrlen(pStr);
        m_pData = m_pAllocator->Alloc(uiLen+1);
        if (m_pData)
        {
            SafeMemset(m_pData, 0, uiLen+1);
            if (pData)
            {
                SafeMemcpy(m_pData, pData, m_uiLen, uiLen+1);
            }
            SafeMemcat(m_pData, pStr, SafeStrlen(pStr), uiLen+1);
            m_uiLen = uiLen;

            if (pData)
            {
                m_pAllocator->Free(pData);
            }

            return *this;
        }
    }

	return *this;
}

JUINT32 JString::Clear()
{
    if (m_uiLen && m_pData && m_pAllocator)
    {
        m_pAllocator->Free(m_pData);
		m_pData = JNULL;
		m_uiLen = 0;
    }

    return JSUCCESS;
}

JCHAR* JString::c_str()
{
    return m_pData;
}

JUINT32 JString::GetLength()
{
    return m_uiLen;
}

JStaticMemory* JString::GetAllocator()
{
    return m_pAllocator;
}

JUINT32 JString::SetAllocator(JStaticMemory* pAllocator)
{
    m_pAllocator = pAllocator;

    return JSUCCESS;
}

JUINT32 JString::Copy(JString& rString)
{
    if (m_uiLen && m_pData && m_pAllocator)
    {
        m_pAllocator->Free(m_pData);
		m_pData = JNULL;
		m_uiLen = 0;
    }

    if (rString.m_uiLen && rString.m_pData && m_pAllocator)
    {
        m_pData = m_pAllocator->Alloc(rString.m_uiLen+1);
        if (m_pData)
        {
            SafeMemset(m_pData, 0, rString.m_uiLen+1);
            SafeMemcpy(m_pData, rString.m_pData, rString.m_uiLen, rString.m_uiLen+1);
            m_uiLen = rString.m_uiLen;

            return JSUCCESS;
        }
    }

	return JFAILURE;
}

JUINT32 JString::Copy(JCHAR* pStr)
{
	JUINT32 uiLen = 0;

	if (!pStr)
	{
		cout << "JString::Copy input param is NULL\n" << endl;
		return JFAILURE;
	}

    if (m_uiLen && m_pData && m_pAllocator)
    {
        m_pAllocator->Free(m_pData);
		m_pData = JNULL;
		m_uiLen = 0;
    }

	uiLen = SafeStrlen(pStr);
    if (uiLen && m_pAllocator)
    {
        m_pData = m_pAllocator->Alloc(uiLen+1);
        if (m_pData)
        {
            SafeMemset(m_pData, 0, uiLen+1);
            SafeMemcpy(m_pData, pStr, uiLen, uiLen+1);
            m_uiLen = uiLen;

            return JSUCCESS;
        }
    }

	return JFAILURE;
}

JBOOL JString::NoEqual(JString& rString)
{
    if (rString.m_uiLen != m_uiLen)
    {
        return JTRUE;
    }

    if (rString.m_pData && m_pData)
    {
        return SafeStrcmp(rString.m_pData, m_pData);
    }

	return JFALSE;
}

JBOOL JString::NoEqual(JCHAR* pStr)
{
	if (!pStr)
	{
		return JTRUE;
	}

    if (SafeStrlen(pStr) != m_uiLen)
    {
        return JTRUE;
    }

    if (m_pData)
    {
        return SafeStrcmp(pStr, m_pData);
    }

	return JFALSE;
}

}

