/**********************************************************
* Author: weiganyi
* File: JObject.cpp
* Date: 20110718
* Description: 
*
**********************************************************/

#include "JFrameWork.h"


namespace JFrameWork{

JObject::JObject()
{
	return;
}

JObject::~JObject()
{
	return;
}

JUINT32 JObject::SafeStrlen(const JCHAR* str)
{
    if (!str)
    {
        cout << "JObject::SafeStrlen: input param is NULL" << endl;
        return JFAILURE;
    }

    return strlen(str);
}

JUINT32 JObject::SafeStrcmp(JCHAR* dst, const JCHAR* src)
{
    if (!dst || !src)
    {
        cout << "JObject::SafeStrcmp: input param is NULL" << endl;
        return JFAILURE;
    }

    return strcmp(dst, src);
}

JCHAR* JObject::SafeStrcpy(JCHAR* dst, const JCHAR* src, const JUINT32 maxLen)
{
    if (!dst || !src || !maxLen)
    {
        cout << "JObject::SafeStrcpy: input param is NULL" << endl;
        return JNULL;
    }

    if (strlen(src) >= maxLen)
    {
        cout << "JObject::SafeStrcpy: dst buffer is too short" << endl;
        return JNULL;
    }

    SafeMemset(dst, 0, maxLen);

    return strcpy(dst, src);
}

JVOID* JObject::SafeStrncpy(JCHAR* dst, const JCHAR* src, const JUINT32 len, const JUINT32 maxLen)
{
    if (!dst || !src || !maxLen)
    {
        cout << "JObject::SafeStrcpy: input param is NULL" << endl;
        return JNULL;
    }

    if (len > maxLen)
    {
        cout << "JObject::SafeStrcpy: dst buffer is too short" << endl;
        return JNULL;
    }

    SafeMemset(dst, 0, maxLen);

    return strncpy(dst, src, len);
}

JVOID* JObject::SafeStrncat(JCHAR* dst, const JCHAR* src, const JUINT32 len, const JUINT32 maxLen)
{
    if (!dst || !src || !maxLen)
    {
        cout << "JObject::SafeStrncat: input param is NULL" << endl;
        return JNULL;
    }

    if (len >= (maxLen-SafeStrlen(dst)))
    {
        cout << "JObject::SafeStrncat: dst buffer is too short" << endl;
        return JNULL;
    }

    return strncat(dst, src, len);
}

JVOID* JObject::SafeMemset(JCHAR* dst, JCHAR ch, const JUINT32 len)
{
    if (!dst)
    {
        cout << "JObject::SafeMemset: input param is NULL" << endl;
        return JNULL;
    }

    return memset(dst, ch, len);
}

JVOID* JObject::SafeMemcpy(JCHAR* dst, const JCHAR* src, const JUINT32 len, const JUINT32 maxLen)
{
    if (!dst || !src || !maxLen)
    {
        cout << "JObject::SafeMemcpy: input param is NULL" << endl;
        return JNULL;
    }

    if (len > maxLen)
    {
        cout << "JObject::SafeMemcpy: dst buffer is too short" << endl;
        return JNULL;
    }

    SafeMemset(dst, 0, maxLen);

    return memcpy(dst, src, len);
}

JVOID* JObject::SafeMemcat(JCHAR* dst, const JCHAR* src, const JUINT32 len, const JUINT32 maxLen)
{
    JUINT32 uiLen = 0;

    if (!dst || !src || !maxLen)
    {
        cout << "JObject::SafeMemcat: input param is NULL" << endl;
        return JNULL;
    }

    uiLen = SafeStrlen(dst);
    if (len >= (maxLen-uiLen))
    {
        cout << "JObject::SafeMemcat: dst buffer is too short" << endl;
        return JNULL;
    }

    return memcpy(dst+uiLen, src, len);
}

JUINT32 JObject::SafeSprintf(JCHAR* dst, JUINT32 maxLen, const JCHAR* fmt, ...)
{
    va_list args;
    JINT32 ret = 0;

    if (!dst || !fmt)
    {
        cout << "JObject::SafeSprintf: input param is NULL" << endl;
        return JFAILURE;
    }

    SafeMemset(dst, 0, maxLen);

    va_start(args, fmt);
    ret = _vsnprintf(dst, maxLen, fmt, args);
    va_end(args);

    return ret;
}

JCHAR* JObject::SafeStrdup(JCHAR* dst)
{
    JCHAR* ucNewObject = JNULL;

    if (!dst)
    {
        cout << "JObject::SafeStrdup: input param is NULL" << endl;
        return JNULL;
    }

    ucNewObject = strdup(dst);

    return ucNewObject;
}

JCHAR* JObject::SafeStrstr(const JCHAR* src, const JCHAR* substr)
{
    JCHAR* pOffset = JNULL;

    if (!src || !substr)
    {
        cout << "JObject::SafeStrstr: input param is NULL" << endl;
        return JNULL;
    }

    pOffset = strstr(src, substr);

    return pOffset;
}

JCHAR* JObject::SafeStrchr(const JCHAR* src, const JUINT32 ch)
{
    JCHAR* pOffset = JNULL;

    if (!src || !ch)
    {
        cout << "JObject::SafeStrstr: input param is NULL" << endl;
        return JNULL;
    }

    pOffset = strchr(src, ch);

    return pOffset;
}

FILE* JObject::SafeFopen(const JCHAR* name, const JCHAR* param)
{
    FILE* hFileHandler = JNULL;

    if (!name || !param)
    {
        cout << "JObject::SafeFopen: input param is NULL" << endl;
        return JNULL;
    }

    hFileHandler = fopen(name, param);

    return hFileHandler;
}

JUINT32 JObject::SafeFclose(FILE* fd)
{
    if (!fd)
    {
        cout << "JObject::SafeFclose: input param is NULL" << endl;
        return JNULL;
    }

    return fclose(fd);
}

JUINT32 JObject::SafeFprintf(FILE* fd, const JCHAR* fmt, ...)
{
    va_list args;
    JINT32 ret = 0;

    if (!fd || !fmt)
    {
        cout << "JObject::SafeFprintf: input param is NULL" << endl;
        return JFAILURE;
    }

    va_start(args, fmt);
    ret = vfprintf(fd, fmt, args);
    va_end(args);

    return ret;
}

JUINT32 JObject::SafeFflush(FILE* fd)
{
    if (!fd)
    {
        cout << "JObject::SafeFflush: input param is NULL" << endl;
        return JNULL;
    }

    return fflush(fd);
}

JCHAR* JObject::SafeFgets(JCHAR* buf, const JUINT32 len, FILE* fd)
{
    if (!buf)
    {
        cout << "JObject::SafeFgets: input param is NULL" << endl;
        return JNULL;
    }

    return fgets(buf, len, fd);
}

JUINT32 JObject::GetSysTime(JSYSTIME *ptrSysTime)
{
    SYSTEMTIME systemTime;

    if (!ptrSysTime)
    {
        cout << "JObject::GetSysTime: input param is NULL" << endl;
        return JFAILURE;
    }

    ::GetSystemTime(&systemTime);

    ptrSysTime->uiYear = systemTime.wYear;
    ptrSysTime->uiMonth = systemTime.wMonth;
    ptrSysTime->uiDayOfWeek = systemTime.wDayOfWeek;
    ptrSysTime->uiDay = systemTime.wDay;
    ptrSysTime->uiHour = systemTime.wHour;
    ptrSysTime->uiMinute = systemTime.wMinute;
    ptrSysTime->uiSecond = systemTime.wSecond;
    ptrSysTime->uiMilliseconds = systemTime.wMilliseconds;

    return JSUCCESS;
}

JINT32 JObject::CmpSysTime(JSYSTIME *srcSysTime, JSYSTIME *dstSysTime)
{
    JINT32 result = SYSTIME_EQUAL;

    if (!srcSysTime || !dstSysTime)
    {
        cout << "JObject::CmpSysTime: input param is NULL" << endl;
        return SYSTIME_INVALID;
    }

    do
    {
        if (srcSysTime->uiYear > dstSysTime->uiYear)
        {
            result = SYSTIME_LARGER;
            break;
        }
        else if (srcSysTime->uiYear < dstSysTime->uiYear)
        {
            result = SYSTIME_SMALLER;
            break;
        }

        if (srcSysTime->uiMonth > dstSysTime->uiMonth)
        {
            result = SYSTIME_LARGER;
            break;
        }
        else if (srcSysTime->uiMonth < dstSysTime->uiMonth)
        {
            result = SYSTIME_SMALLER;
            break;
        }

        if (srcSysTime->uiDay > dstSysTime->uiDay)
        {
            result = SYSTIME_LARGER;
            break;
        }
        else if (srcSysTime->uiDay < dstSysTime->uiDay)
        {
            result = SYSTIME_SMALLER;
            break;
        }

        if (srcSysTime->uiHour > dstSysTime->uiHour)
        {
            result = SYSTIME_LARGER;
            break;
        }
        else if (srcSysTime->uiHour < dstSysTime->uiHour)
        {
            result = SYSTIME_SMALLER;
            break;
        }

        if (srcSysTime->uiMinute > dstSysTime->uiMinute)
        {
            result = SYSTIME_LARGER;
            break;
        }
        else if (srcSysTime->uiMinute < dstSysTime->uiMinute)
        {
            result = SYSTIME_SMALLER;
            break;
        }

        if (srcSysTime->uiSecond > dstSysTime->uiSecond)
        {
            result = SYSTIME_LARGER;
            break;
        }
        else if (srcSysTime->uiSecond < dstSysTime->uiSecond)
        {
            result = SYSTIME_SMALLER;
            break;
        }

        if (srcSysTime->uiMilliseconds > dstSysTime->uiMilliseconds)
        {
            result = SYSTIME_LARGER;
            break;
        }
        else if (srcSysTime->uiMilliseconds < dstSysTime->uiMilliseconds)
        {
            result = SYSTIME_SMALLER;
            break;
        }

        result = SYSTIME_EQUAL;
        break;
    }while(1);

    return result;
}

JSYSTIME* JObject::AddSysTime(JSYSTIME *ptrSysTime, JUINT32 milliseconds)
{
    JUINT32 uiYear; 
    JUINT32 uiMonth; 
    JUINT32 uiDay; 
    JUINT32 uiHour; 
    JUINT32 uiMinute; 
    JUINT32 uiSecond; 

    if (!ptrSysTime)
    {
        cout << "JObject::AddSysTime: input param is NULL" << endl;
        return JNULL;
    }

    uiSecond = (ptrSysTime->uiMilliseconds+milliseconds)/1000;
    ptrSysTime->uiMilliseconds = (ptrSysTime->uiMilliseconds+milliseconds)%1000;

    uiMinute = (ptrSysTime->uiSecond+uiSecond)/60;
    ptrSysTime->uiSecond = (ptrSysTime->uiSecond+uiSecond)%60;

    uiHour = (ptrSysTime->uiMinute+uiMinute)/60;
    ptrSysTime->uiMinute = (ptrSysTime->uiMinute+uiMinute)%60;

    uiDay = (ptrSysTime->uiHour+uiHour)/24;
    ptrSysTime->uiHour = (ptrSysTime->uiHour+uiHour)%24;

    uiMonth = (ptrSysTime->uiDay+uiDay)/31;
    ptrSysTime->uiDay = (ptrSysTime->uiDay+uiDay)%31;

    uiYear = (ptrSysTime->uiMonth+uiMonth)/13;
    ptrSysTime->uiMonth = (ptrSysTime->uiMonth+uiMonth)%13;

    ptrSysTime->uiYear = ptrSysTime->uiYear+uiYear;

    return JNULL;
}

JBOOL JObject::IsDtmf(JINT32 iDigit)
{
    if ((iDigit>=0x30 && iDigit<=0x39) ||
        iDigit == 0x2A || 
        iDigit == 0x23)
    {
    	return JTRUE;
	}
	else
	{
    	return JFALSE;
	}
}

JUINT32 JObject::GetLocalAddr(JString& rAddr, JString& rIfAddr)
{
    struct hostent *pHostEnt = JNULL;
    JCHAR strHostAddr[16] = {0};
    JUINT8 ucAddr[4] = {0};
    JUINT32 uiIdx = 0;
    JUINT32 uiMatchIdx = 0;

	//get address list for local mechine
    pHostEnt = gethostbyname(strHostAddr);
    if (pHostEnt)
    {
        SafeMemset(reinterpret_cast<JCHAR*>(ucAddr), 0, sizeof(JUINT8)*4);
        sscanf(rAddr.c_str(), "%d.%d.%d.%d", &ucAddr[0], &ucAddr[1], &ucAddr[2], &ucAddr[3]);

        uiIdx = 0;
        uiMatchIdx = 0;
        while(pHostEnt->h_addr_list[++uiIdx])
        {
            if ((pHostEnt->h_addr_list[uiMatchIdx][0] & ucAddr[0]) > 
                (pHostEnt->h_addr_list[uiIdx][0] & ucAddr[0]))
            {
                continue;
            }
            else if ((pHostEnt->h_addr_list[uiMatchIdx][0] & ucAddr[0]) < 
                (pHostEnt->h_addr_list[uiIdx][0] & ucAddr[0]))
            {
                uiMatchIdx = uiIdx;
                continue;
            }

            if ((pHostEnt->h_addr_list[uiMatchIdx][1] & ucAddr[1]) > 
                (pHostEnt->h_addr_list[uiIdx][1] & ucAddr[1]))
            {
                continue;
            }
            else if ((pHostEnt->h_addr_list[uiMatchIdx][1] & ucAddr[1]) < 
                (pHostEnt->h_addr_list[uiIdx][1] & ucAddr[1]))
            {
                uiMatchIdx = uiIdx;
                continue;
            }

            if ((pHostEnt->h_addr_list[uiMatchIdx][2] & ucAddr[2]) > 
                (pHostEnt->h_addr_list[uiIdx][2] & ucAddr[2]))
            {
                continue;
            }
            else if ((pHostEnt->h_addr_list[uiMatchIdx][2] & ucAddr[2]) < 
                (pHostEnt->h_addr_list[uiIdx][2] & ucAddr[2]))
            {
                uiMatchIdx = uiIdx;
                continue;
            }

            if ((pHostEnt->h_addr_list[uiMatchIdx][3] & ucAddr[3]) > 
                (pHostEnt->h_addr_list[uiIdx][3] & ucAddr[3]))
            {
                continue;
            }
            else if ((pHostEnt->h_addr_list[uiMatchIdx][3] & ucAddr[3]) < 
                (pHostEnt->h_addr_list[uiIdx][3] & ucAddr[3]))
            {
                uiMatchIdx = uiIdx;
                continue;
            }
        }

        SafeMemset(strHostAddr, 0, 16);
        SafeSprintf(strHostAddr, 16, "%d.%d.%d.%d",
            pHostEnt->h_addr_list[uiMatchIdx][0] & 0x00ff,
            pHostEnt->h_addr_list[uiMatchIdx][1] & 0x00ff,
            pHostEnt->h_addr_list[uiMatchIdx][2] & 0x00ff,
            pHostEnt->h_addr_list[uiMatchIdx][3] & 0x00ff);
        rIfAddr = strHostAddr;

        return JSUCCESS;
    }

    return JFAILURE;
}

JModule::JModule()
{
	return;
}

JModule::~JModule()
{
	return;
}

JException::JException()
{
    m_pErrInfo = JNULL;

	return;
}

JException::JException(JCHAR* pStr):m_pErrInfo(JNULL)
{
    JUINT32 uiLen = 0;
    JCHAR* pAlloc = JNULL;

    uiLen = SafeStrlen(pStr);
    pAlloc = JSingleton<JStaticMemory>::instance()->Alloc(uiLen+1);
    if (pAlloc)
    {
        SafeMemset(pAlloc, 0, uiLen+1);
        SafeStrcpy(pAlloc, pStr, uiLen+1);
        m_pErrInfo = pAlloc;
    }        

	return;
}

JException::~JException()
{
    if (m_pErrInfo)
    {
        JSingleton<JStaticMemory>::instance()->Free(m_pErrInfo);
    }

	return;
}

JCHAR* JException::GetErrInfo()
{
    if (m_pErrInfo)
	    return m_pErrInfo;
	else
	    return JNULL;
}

JEventBody::JEventBody()
{
	return;
}

JEventBody::~JEventBody()
{
	return;
}

}

