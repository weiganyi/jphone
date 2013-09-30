/**********************************************************
* Author: weiganyi
* File: JSipUa.cpp
* Date: 20110718
* Description: 
*
**********************************************************/

#include "JFrameWork.h"

using namespace JFrameWork;

#include "JSipUa.h"


JSIPUA_KEY_MAP gKeyMap[] = {
 {JSIPUA_KEY_0, '0'},
 {JSIPUA_KEY_1, '1'},
 {JSIPUA_KEY_2, '2'},
 {JSIPUA_KEY_3, '3'},
 {JSIPUA_KEY_4, '4'},
 {JSIPUA_KEY_5, '5'},
 {JSIPUA_KEY_6, '6'},
 {JSIPUA_KEY_7, '7'},
 {JSIPUA_KEY_8, '8'},
 {JSIPUA_KEY_9, '9'},
 {JSIPUA_KEY_0, '0'},
 {JSIPUA_KEY_STAR, 0x2A},
 {JSIPUA_KEY_POUND, 0x23},
 {JSIPUA_KEY_OFFHOOK, 0x11},
 {JSIPUA_KEY_ONHOOK, 0x12},
 {JSIPUA_KEY_MAX, 0x00},
};

JSIPUA_STATUS_MAP gStatusMap[] = {
{JSIPUA_STATUS_IDLE, JSIPUA_S_STATUS_IDLE},
{JSIPUA_STATUS_REGISTERING, JSIPUA_S_STATUS_REGISTERING},
{JSIPUA_STATUS_REGISTERED, JSIPUA_S_STATUS_REGISTERED},
{JSIPUA_STATUS_COLLECT, JSIPUA_S_STATUS_COLLECT},
{JSIPUA_STATUS_OUTGOING, JSIPUA_S_STATUS_OUTGOING},
{JSIPUA_STATUS_INCOMING, JSIPUA_S_STATUS_INCOMING},
{JSIPUA_STATUS_CONNECTED, JSIPUA_S_STATUS_CONNECTED},
{JSIPUA_STATUS_HOLDING, JSIPUA_S_STATUS_HOLDING},
};

JPjSipUaCfg::JPjSipUaCfg()
{
	return;
}

JPjSipUaCfg::~JPjSipUaCfg()
{
	return;
}

JUINT32 JPjSipUaCfg::GetLength()
{
	JUINT32 uiLen = 0;

	uiLen += m_Number.GetLength()+sizeof(JUINT32);
	uiLen += m_AuthName.GetLength()+sizeof(JUINT32);
	uiLen += m_AuthPasswd.GetLength()+sizeof(JUINT32);
	uiLen += m_ProxyAddr.GetLength()+sizeof(JUINT32);

	return uiLen;
}

JEventBody* JPjSipUaCfg::Clone()
{
	JPjSipUaCfg* pPjSipUaCfg = JNULL;

	pPjSipUaCfg = new JPjSipUaCfg();
	if (pPjSipUaCfg)
	{
		pPjSipUaCfg->SetNumber(m_Number);
		pPjSipUaCfg->SetAuthName(m_AuthName);
		pPjSipUaCfg->SetAuthPasswd(m_AuthPasswd);
		pPjSipUaCfg->SetProxyAddr(m_ProxyAddr);
	}

	return pPjSipUaCfg;
}

JUINT32 JPjSipUaCfg::Clear()
{
	m_Number.Clear();
	m_AuthName.Clear();
	m_AuthPasswd.Clear();
	m_ProxyAddr.Clear();

	return JSUCCESS;
}

JUINT32 JPjSipUaCfg::Serialize(JCHAR* pBuf, JUINT32 uiMaxNum)
{
	JUINT32 uiLen = 0;
	JUINT32* pLen = JNULL;
	JCHAR* pOffset = JNULL;
	JUINT32 tmpMaxNum = 0;

	if (!pBuf || !uiMaxNum)
	{
        JSingleton<JLog>::instance2() << set(JLOG_MOD_SIPUA, JLOG_ERROR_LEVEL) 
            << "JPjSipUaCfg::Serialize input param is NULL\n";
        return JFAILURE;
	}

	pOffset = pBuf;
	tmpMaxNum = uiMaxNum;

	uiLen = m_Number.GetLength();
	pLen = reinterpret_cast<JUINT32*>(pOffset);
	*pLen = uiLen;
	pOffset += sizeof(JUINT32);
	tmpMaxNum -= sizeof(JUINT32);
	if (uiLen)
	{
		SafeMemcpy(pOffset, m_Number.c_str(), uiLen, tmpMaxNum);

		pOffset += uiLen;
		tmpMaxNum -= uiLen;
	}

	uiLen = m_AuthName.GetLength();
	pLen = reinterpret_cast<JUINT32*>(pOffset);
	*pLen = uiLen;
	pOffset += sizeof(JUINT32);
	tmpMaxNum -= sizeof(JUINT32);
	if (uiLen)
	{
		SafeMemcpy(pOffset, m_AuthName.c_str(), uiLen, tmpMaxNum);

		pOffset += uiLen;
		tmpMaxNum -= uiLen;
	}

	uiLen = m_AuthPasswd.GetLength();
	pLen = reinterpret_cast<JUINT32*>(pOffset);
	*pLen = uiLen;
	pOffset += sizeof(JUINT32);
	tmpMaxNum -= sizeof(JUINT32);
	if (uiLen)
	{
		SafeMemcpy(pOffset, m_AuthPasswd.c_str(), uiLen, tmpMaxNum);

		pOffset += uiLen;
		tmpMaxNum -= uiLen;
	}

	uiLen = m_ProxyAddr.GetLength();
	pLen = reinterpret_cast<JUINT32*>(pOffset);
	*pLen = uiLen;
	pOffset += sizeof(JUINT32);
	tmpMaxNum -= sizeof(JUINT32);
	if (uiLen)
	{
		SafeMemcpy(pOffset, m_ProxyAddr.c_str(), uiLen, tmpMaxNum);

		pOffset += uiLen;
		tmpMaxNum -= uiLen;
	}

	return JSUCCESS;
}

JUINT32 JPjSipUaCfg::DeSerialize(JCHAR* pBuf)
{
	JUINT32* pLen = 0;
	JCHAR* pOffset = JNULL;
	JCHAR strBuffer[JMAX_STRING_LEN] = {0};

	if (!pBuf)
	{
        JSingleton<JLog>::instance2() << set(JLOG_MOD_SIPUA, JLOG_ERROR_LEVEL) 
            << "JPjSipUaCfg::DeSerialize input param is NULL\n";
        return JFAILURE;
	}

	pOffset = pBuf;

	pLen = reinterpret_cast<JUINT32*>(pOffset);
	pOffset += sizeof(JUINT32);
	if (*pLen)
	{
		SafeStrncpy(strBuffer, pOffset, *pLen, JMAX_STRING_LEN);
		m_Number = strBuffer;
		pOffset += *pLen;
	}

	pLen = reinterpret_cast<JUINT32*>(pOffset);
	pOffset += sizeof(JUINT32);
	if (*pLen)
	{
		SafeStrncpy(strBuffer, pOffset, *pLen, JMAX_STRING_LEN);
		m_AuthName = strBuffer;
		pOffset += *pLen;
	}

	pLen = reinterpret_cast<JUINT32*>(pOffset);
	pOffset += sizeof(JUINT32);
	if (*pLen)
	{
		SafeStrncpy(strBuffer, pOffset, *pLen, JMAX_STRING_LEN);
		m_AuthPasswd = strBuffer;
		pOffset += *pLen;
	}

	pLen = reinterpret_cast<JUINT32*>(pOffset);
	pOffset += sizeof(JUINT32);
	if (*pLen)
	{
		SafeStrncpy(strBuffer, pOffset, *pLen, JMAX_STRING_LEN);
		m_ProxyAddr = strBuffer;
		pOffset += *pLen;
	}

	return JSUCCESS;
}

JUINT32 JPjSipUaCfg::operator=(JPjSipUaCfg& rPjSipUaCfg)
{
	m_Number = rPjSipUaCfg.GetNumber();
	m_AuthName = rPjSipUaCfg.GetAuthName();
	m_AuthPasswd = rPjSipUaCfg.GetAuthPasswd();
	m_ProxyAddr = rPjSipUaCfg.GetProxyAddr();

	return JSUCCESS;
}

JString& JPjSipUaCfg::GetNumber()
{
	return m_Number;
}

JUINT32 JPjSipUaCfg::SetNumber(JString& rStr)
{
	m_Number = rStr;

	return JSUCCESS;
}

JString& JPjSipUaCfg::GetAuthName()
{
	return m_AuthName;
}

JUINT32 JPjSipUaCfg::SetAuthName(JString& rStr)
{
	m_AuthName = rStr;

	return JSUCCESS;
}

JString& JPjSipUaCfg::GetAuthPasswd()
{
	return m_AuthPasswd;
}

JUINT32 JPjSipUaCfg::SetAuthPasswd(JString& rStr)
{
	m_AuthPasswd = rStr;

	return JSUCCESS;
}

JString& JPjSipUaCfg::GetProxyAddr()
{
	return m_ProxyAddr;
}

JUINT32 JPjSipUaCfg::SetProxyAddr(JString& rStr)
{
	m_ProxyAddr = rStr;

	return JSUCCESS;
}

JPjSipUaKey::JPjSipUaKey()
{
	return;
}

JPjSipUaKey::~JPjSipUaKey()
{
	return;
}

JUINT32 JPjSipUaKey::GetLength()
{
	JUINT32 uiLen = 0;

	uiLen += m_Key.GetLength()+sizeof(JUINT32);

	return uiLen;
}

JEventBody* JPjSipUaKey::Clone()
{
	JPjSipUaKey* pPjSipUaKey = JNULL;

	pPjSipUaKey = new JPjSipUaKey();
	if (pPjSipUaKey)
	{
		pPjSipUaKey->SetKey(m_Key);
	}

	return pPjSipUaKey;
}

JUINT32 JPjSipUaKey::Clear()
{
	m_Key.Clear();

	return JSUCCESS;
}

JUINT32 JPjSipUaKey::Serialize(JCHAR* pBuf, JUINT32 uiMaxNum)
{
	JUINT32 uiLen = 0;
	JUINT32* pLen = JNULL;
	JCHAR* pOffset = JNULL;
	JUINT32 tmpMaxNum = 0;

	if (!pBuf || !uiMaxNum)
	{
        JSingleton<JLog>::instance2() << set(JLOG_MOD_SIPUA, JLOG_ERROR_LEVEL) 
            << "JPjSipUaKey::Serialize input param is NULL\n";
        return JFAILURE;
	}

	pOffset = pBuf;
	tmpMaxNum = uiMaxNum;

	uiLen = m_Key.GetLength();
	pLen = reinterpret_cast<JUINT32*>(pOffset);
	*pLen = uiLen;
	pOffset += sizeof(JUINT32);
	tmpMaxNum -= sizeof(JUINT32);
	if (uiLen)
	{
		SafeMemcpy(pOffset, m_Key.c_str(), uiLen, tmpMaxNum);

		pOffset += uiLen;
		tmpMaxNum -= uiLen;
	}

	return JSUCCESS;
}

JUINT32 JPjSipUaKey::DeSerialize(JCHAR* pBuf)
{
	JUINT32* pLen = 0;
	JCHAR* pOffset = JNULL;
	JCHAR strBuffer[JMAX_STRING_LEN] = {0};

	if (!pBuf)
	{
        JSingleton<JLog>::instance2() << set(JLOG_MOD_SIPUA, JLOG_ERROR_LEVEL) 
            << "JPjSipUaKey::DeSerialize input param is NULL\n";
        return JFAILURE;
	}

	pOffset = pBuf;

	pLen = reinterpret_cast<JUINT32*>(pOffset);
	pOffset += sizeof(JUINT32);
	if (*pLen)
	{
		SafeStrncpy(strBuffer, pOffset, *pLen, JMAX_STRING_LEN);
		m_Key = strBuffer;
		pOffset += *pLen;
	}

	return JSUCCESS;
}

JUINT32 JPjSipUaKey::operator=(JPjSipUaKey& rPjSipUaKey)
{
	m_Key = rPjSipUaKey.GetKey();

	return JSUCCESS;
}

JString& JPjSipUaKey::GetKey()
{
	return m_Key;
}

JUINT32 JPjSipUaKey::SetKey(JString& rStr)
{
	m_Key = rStr;

	return JSUCCESS;
}

JPjSipUaClickContact::JPjSipUaClickContact()
{
	return;
}

JPjSipUaClickContact::~JPjSipUaClickContact()
{
	return;
}

JUINT32 JPjSipUaClickContact::GetLength()
{
	JUINT32 uiLen = 0;

	uiLen += m_ContactNumber.GetLength()+sizeof(JUINT32);

	return uiLen;
}

JEventBody* JPjSipUaClickContact::Clone()
{
	JPjSipUaClickContact* pPjSipUaClickContact = JNULL;

	pPjSipUaClickContact = new JPjSipUaClickContact();
	if (pPjSipUaClickContact)
	{
		pPjSipUaClickContact->SetContactNumber(m_ContactNumber);
	}

	return pPjSipUaClickContact;
}

JUINT32 JPjSipUaClickContact::Clear()
{
	m_ContactNumber.Clear();

	return JSUCCESS;
}

JUINT32 JPjSipUaClickContact::Serialize(JCHAR* pBuf, JUINT32 uiMaxNum)
{
	JUINT32 uiLen = 0;
	JUINT32* pLen = JNULL;
	JCHAR* pOffset = JNULL;
	JUINT32 tmpMaxNum = 0;

	if (!pBuf || !uiMaxNum)
	{
        JSingleton<JLog>::instance2() << set(JLOG_MOD_SIPUA, JLOG_ERROR_LEVEL) 
            << "JPjSipUaClickContact::Serialize input param is NULL\n";
        return JFAILURE;
	}

	pOffset = pBuf;
	tmpMaxNum = uiMaxNum;

	uiLen = m_ContactNumber.GetLength();
	pLen = reinterpret_cast<JUINT32*>(pOffset);
	*pLen = uiLen;
	pOffset += sizeof(JUINT32);
	tmpMaxNum -= sizeof(JUINT32);
	if (uiLen)
	{
		SafeMemcpy(pOffset, m_ContactNumber.c_str(), uiLen, tmpMaxNum);

		pOffset += uiLen;
		tmpMaxNum -= uiLen;
	}

	return JSUCCESS;
}

JUINT32 JPjSipUaClickContact::DeSerialize(JCHAR* pBuf)
{
	JUINT32* pLen = 0;
	JCHAR* pOffset = JNULL;
	JCHAR strBuffer[JMAX_STRING_LEN] = {0};

	if (!pBuf)
	{
        JSingleton<JLog>::instance2() << set(JLOG_MOD_SIPUA, JLOG_ERROR_LEVEL) 
            << "JPjSipUaClickContact::DeSerialize input param is NULL\n";
        return JFAILURE;
	}

	pOffset = pBuf;

	pLen = reinterpret_cast<JUINT32*>(pOffset);
	pOffset += sizeof(JUINT32);
	if (*pLen)
	{
		SafeStrncpy(strBuffer, pOffset, *pLen, JMAX_STRING_LEN);
		m_ContactNumber = strBuffer;
		pOffset += *pLen;
	}

	return JSUCCESS;
}

JUINT32 JPjSipUaClickContact::operator=(JPjSipUaClickContact& rPjSipUaClickContact)
{
	m_ContactNumber = rPjSipUaClickContact.GetContactNumber();

	return JSUCCESS;
}

JString& JPjSipUaClickContact::GetContactNumber()
{
	return m_ContactNumber;
}

JUINT32 JPjSipUaClickContact::SetContactNumber(JString& rStr)
{
	m_ContactNumber = rStr;

	return JSUCCESS;
}

JPjSipUaCallStatus::JPjSipUaCallStatus()
{
	return;
}

JPjSipUaCallStatus::~JPjSipUaCallStatus()
{
	return;
}

JUINT32 JPjSipUaCallStatus::GetLength()
{
	JUINT32 uiLen = 0;

	uiLen += m_Number.GetLength()+sizeof(JUINT32);
	uiLen += m_Status.GetLength()+sizeof(JUINT32);

	return uiLen;
}

JEventBody* JPjSipUaCallStatus::Clone()
{
	JPjSipUaCallStatus* pPjSipUaCallStatus = JNULL;

	pPjSipUaCallStatus = new JPjSipUaCallStatus();
	if (pPjSipUaCallStatus)
	{
		pPjSipUaCallStatus->SetNumber(m_Number);
		pPjSipUaCallStatus->SetStatus(m_Status);
	}

	return pPjSipUaCallStatus;
}

JUINT32 JPjSipUaCallStatus::Clear()
{
	m_Number.Clear();
	m_Status.Clear();

	return JSUCCESS;
}

JUINT32 JPjSipUaCallStatus::Serialize(JCHAR* pBuf, JUINT32 uiMaxNum)
{
	JUINT32 uiLen = 0;
	JUINT32* pLen = JNULL;
	JCHAR* pOffset = JNULL;
	JUINT32 tmpMaxNum = 0;

	if (!pBuf || !uiMaxNum)
	{
        JSingleton<JLog>::instance2() << set(JLOG_MOD_SIPUA, JLOG_ERROR_LEVEL) 
            << "JPjSipUaCallStatus::Serialize input param is NULL\n";
        return JFAILURE;
	}

	pOffset = pBuf;
	tmpMaxNum = uiMaxNum;

	uiLen = m_Number.GetLength();
	pLen = reinterpret_cast<JUINT32*>(pOffset);
	*pLen = uiLen;
	pOffset += sizeof(JUINT32);
	tmpMaxNum -= sizeof(JUINT32);
	if (uiLen)
	{
		SafeMemcpy(pOffset, m_Number.c_str(), uiLen, tmpMaxNum);

		pOffset += uiLen;
		tmpMaxNum -= uiLen;
	}

	uiLen = m_Status.GetLength();
	pLen = reinterpret_cast<JUINT32*>(pOffset);
	*pLen = uiLen;
	pOffset += sizeof(JUINT32);
	tmpMaxNum -= sizeof(JUINT32);
	if (uiLen)
	{
		SafeMemcpy(pOffset, m_Status.c_str(), uiLen, tmpMaxNum);

		pOffset += uiLen;
		tmpMaxNum -= uiLen;
	}

	return JSUCCESS;
}

JUINT32 JPjSipUaCallStatus::DeSerialize(JCHAR* pBuf)
{
	JUINT32* pLen = 0;
	JCHAR* pOffset = JNULL;
	JCHAR strBuffer[JMAX_STRING_LEN] = {0};

	if (!pBuf)
	{
        JSingleton<JLog>::instance2() << set(JLOG_MOD_SIPUA, JLOG_ERROR_LEVEL) 
            << "JPjSipUaCallStatus::DeSerialize input param is NULL\n";
        return JFAILURE;
	}

	pOffset = pBuf;

	pLen = reinterpret_cast<JUINT32*>(pOffset);
	pOffset += sizeof(JUINT32);
	if (*pLen)
	{
		SafeStrncpy(strBuffer, pOffset, *pLen, JMAX_STRING_LEN);
		m_Number = strBuffer;
		pOffset += *pLen;
	}

	pLen = reinterpret_cast<JUINT32*>(pOffset);
	pOffset += sizeof(JUINT32);
	if (*pLen)
	{
		SafeStrncpy(strBuffer, pOffset, *pLen, JMAX_STRING_LEN);
		m_Status = strBuffer;
		pOffset += *pLen;
	}

	return JSUCCESS;
}

JUINT32 JPjSipUaCallStatus::operator=(JPjSipUaCallStatus& rPjSipUaCallStatus)
{
	m_Number = rPjSipUaCallStatus.GetNumber();
	m_Status = rPjSipUaCallStatus.GetStatus();

	return JSUCCESS;
}

JString& JPjSipUaCallStatus::GetNumber()
{
	return m_Number;
}

JUINT32 JPjSipUaCallStatus::SetNumber(JString& rStr)
{
	m_Number = rStr;

	return JSUCCESS;
}

JString& JPjSipUaCallStatus::GetStatus()
{
	return m_Status;
}

JUINT32 JPjSipUaCallStatus::SetStatus(JString& rStr)
{
	m_Status = rStr;

	return JSUCCESS;
}

JPjSipUaContactList::JPjSipUaContactList()
{
	return;
}

JPjSipUaContactList::~JPjSipUaContactList()
{
	return;
}

JUINT32 JPjSipUaContactList::GetLength()
{
	JUINT32 uiLen = 0;
	JUINT32 uiIdx = 0;

	for (uiIdx=0; uiIdx<JPJSIP_MAX_NUMBER; uiIdx++)
	{
		if(!m_ContactNumber[uiIdx].GetLength())
		{
			break;
		}

		uiLen += m_ContactNumber[uiIdx].GetLength()+sizeof(JUINT32);
	}

	return uiLen;
}

JEventBody* JPjSipUaContactList::Clone()
{
	JUINT32 uiIdx = 0;
	JPjSipUaContactList* pPjSipUaContactList = JNULL;

	pPjSipUaContactList = new JPjSipUaContactList();
	if (pPjSipUaContactList)
	{
		for (uiIdx=0; uiIdx<JPJSIP_MAX_NUMBER; uiIdx++)
		{
			if(!m_ContactNumber[uiIdx].GetLength())
			{
				break;
			}

			pPjSipUaContactList->SetContactNumber(uiIdx, m_ContactNumber[uiIdx]);
		}
	}

	return pPjSipUaContactList;
}

JUINT32 JPjSipUaContactList::Clear()
{
	JUINT32 uiIdx = 0;

	for (uiIdx=0; uiIdx<JPJSIP_MAX_NUMBER; uiIdx++)
	{
		m_ContactNumber[uiIdx].Clear();
	}

	return JSUCCESS;
}

JUINT32 JPjSipUaContactList::Serialize(JCHAR* pBuf, JUINT32 uiMaxNum)
{
	JUINT32 uiLen = 0;
	JUINT32* pLen = JNULL;
	JCHAR* pOffset = JNULL;
	JUINT32 tmpMaxNum = 0;
	JUINT32 uiIdx = 0;

	if (!pBuf || !uiMaxNum)
	{
        JSingleton<JLog>::instance2() << set(JLOG_MOD_SIPUA, JLOG_ERROR_LEVEL) 
            << "JPjSipUaContactList::Serialize input param is NULL\n";
        return JFAILURE;
	}

	pOffset = pBuf;
	tmpMaxNum = uiMaxNum;

	for (uiIdx=0; uiIdx<JPJSIP_MAX_NUMBER; uiIdx++)
	{
		if(!m_ContactNumber[uiIdx].GetLength())
		{
			break;
		}

		uiLen = m_ContactNumber[uiIdx].GetLength();
		pLen = reinterpret_cast<JUINT32*>(pOffset);
		*pLen = uiLen;
		pOffset += sizeof(JUINT32);
		tmpMaxNum -= sizeof(JUINT32);
		if (uiLen)
		{
			SafeMemcpy(pOffset, m_ContactNumber[uiIdx].c_str(), uiLen, tmpMaxNum);

			pOffset += uiLen;
			tmpMaxNum -= uiLen;
		}
	}

	return JSUCCESS;
}

JUINT32 JPjSipUaContactList::DeSerialize(JCHAR* pBuf)
{
	JUINT32* pLen = 0;
	JCHAR* pOffset = JNULL;
	JCHAR strBuffer[JMAX_STRING_LEN] = {0};
	JUINT32 uiIdx = 0;

	if (!pBuf)
	{
        JSingleton<JLog>::instance2() << set(JLOG_MOD_SIPUA, JLOG_ERROR_LEVEL) 
            << "JPjSipUaContactList::DeSerialize input param is NULL\n";
        return JFAILURE;
	}

	pOffset = pBuf;

	while(pOffset)
	{
		pLen = reinterpret_cast<JUINT32*>(pOffset);
		pOffset += sizeof(JUINT32);
		if (*pLen)
		{
			SafeStrncpy(strBuffer, pOffset, *pLen, JMAX_STRING_LEN);
			m_ContactNumber[uiIdx] = strBuffer;
			pOffset += *pLen;
			uiIdx++;
		}
		else
		{
			break;
		}
	}

	return JSUCCESS;
}

JUINT32 JPjSipUaContactList::operator=(JPjSipUaContactList& rPjSipUaContactList)
{
	JUINT32 uiIdx = 0;

	for (uiIdx=0; uiIdx<JPJSIP_MAX_NUMBER; uiIdx++)
	{
		if(!m_ContactNumber[uiIdx].GetLength())
		{
			break;
		}

		m_ContactNumber[uiIdx] = rPjSipUaContactList.GetContactNumber(uiIdx);
	}

	return JSUCCESS;
}

JString& JPjSipUaContactList::GetContactNumber(JUINT32 uiIdx)
{
	if (uiIdx < JPJSIP_MAX_NUMBER)
	{
		return m_ContactNumber[uiIdx];
	}
	else
	{
        JSingleton<JLog>::instance2() << set(JLOG_MOD_SIPUA, JLOG_ERROR_LEVEL) 
            << "JPjSipUaContactList::GetContactNumber uiIdx is over range\n";
		return m_ContactNumber[uiIdx%JPJSIP_MAX_NUMBER];
	}
}

JUINT32 JPjSipUaContactList::SetContactNumber(JUINT32 uiIdx, JString& rStr)
{
	if (uiIdx < JPJSIP_MAX_NUMBER)
	{
		m_ContactNumber[uiIdx] = rStr;
		return JSUCCESS;
	}
	else
	{
        JSingleton<JLog>::instance2() << set(JLOG_MOD_SIPUA, JLOG_ERROR_LEVEL) 
            << "JPjSipUaContactList::SetContactNumber uiIdx is over range\n";
		return JFAILURE;
	}
}

JPjSipCallback::JPjSipCallback()
{
	return;
}

JPjSipCallback::~JPjSipCallback()
{
	return;
}

JUINT32 JPjSipCallback::GetLength()
{
	JUINT32 uiLen = 0;

	uiLen += sizeof(JSIPUA_CB_TYPE);
	uiLen += sizeof(JUINT32);
	uiLen += sizeof(pjsip_inv_state);
	uiLen += sizeof(pjsua_acc_id);
	uiLen += sizeof(pjsua_call_id);
	uiLen += pRemoteNumber.GetLength()+sizeof(JUINT32);

	return uiLen;
}

JEventBody* JPjSipCallback::Clone()
{
	JPjSipCallback* pPjSipCallback = JNULL;

	pPjSipCallback = new JPjSipCallback();
	if (pPjSipCallback)
	{
		pPjSipCallback->SetCbType(eType);
		pPjSipCallback->SetCode(uiCode);
		pPjSipCallback->SetState(state);
		pPjSipCallback->SetAccId(acc_id);
		pPjSipCallback->SetCallId(call_id);
		pPjSipCallback->SetRemoteNumber(pRemoteNumber);
	}

	return pPjSipCallback;
}

JUINT32 JPjSipCallback::Clear()
{
	eType = JSIPUA_CB_TYPE_NONE;
	uiCode = 0;
	state = PJSIP_INV_STATE_NULL;
	acc_id = 0;
	call_id = 0;
	pRemoteNumber.Clear();

	return JSUCCESS;
}

JUINT32 JPjSipCallback::Serialize(JCHAR* pBuf, JUINT32 uiMaxNum)
{
	JUINT32 uiLen = 0;
	JUINT32* pLen = JNULL;
	JCHAR* pOffset = JNULL;
	JUINT32 tmpMaxNum = 0;

	JSIPUA_CB_TYPE* pType = JNULL;
	JUINT32* pCode = JNULL;
	pjsip_inv_state* pState = JNULL;
	pjsua_acc_id* pAccId = JNULL;
	pjsua_call_id* pCallId = JNULL;

	if (!pBuf || !uiMaxNum)
	{
        JSingleton<JLog>::instance2() << set(JLOG_MOD_SIPUA, JLOG_ERROR_LEVEL) 
            << "JPjSipCallback::Serialize input param is NULL\n";
        return JFAILURE;
	}

	pOffset = pBuf;
	tmpMaxNum = uiMaxNum;

	{
		uiLen = sizeof(JSIPUA_CB_TYPE);

		pType = reinterpret_cast<JSIPUA_CB_TYPE*>(pOffset);
		*pType = eType;

		pOffset += uiLen;
		tmpMaxNum -= uiLen;
	}

	{
		uiLen = sizeof(JUINT32);

		pCode = reinterpret_cast<JUINT32*>(pOffset);
		*pCode = uiCode;

		pOffset += uiLen;
		tmpMaxNum -= uiLen;
	}

	{
		uiLen = sizeof(pjsip_inv_state);

		pState = reinterpret_cast<pjsip_inv_state*>(pOffset);
		*pState = state;

		pOffset += uiLen;
		tmpMaxNum -= uiLen;
	}

	{
		uiLen = sizeof(pjsua_acc_id);

		pAccId = reinterpret_cast<pjsua_acc_id*>(pOffset);
		*pAccId = acc_id;

		pOffset += uiLen;
		tmpMaxNum -= uiLen;
	}

	{
		uiLen = sizeof(pjsua_call_id);

		pCallId = reinterpret_cast<pjsua_call_id*>(pOffset);
		*pCallId = call_id;

		pOffset += uiLen;
		tmpMaxNum -= uiLen;
	}

	uiLen = pRemoteNumber.GetLength();
	pLen = reinterpret_cast<JUINT32*>(pOffset);
	*pLen = uiLen;
	pOffset += sizeof(JUINT32);
	tmpMaxNum -= sizeof(JUINT32);
	if (uiLen)
	{
		SafeMemcpy(pOffset, pRemoteNumber.c_str(), uiLen, tmpMaxNum);

		pOffset += uiLen;
		tmpMaxNum -= uiLen;
	}

	return JSUCCESS;
}

JUINT32 JPjSipCallback::DeSerialize(JCHAR* pBuf)
{
	JUINT32* pLen = 0;
	JCHAR* pOffset = JNULL;
	JCHAR strBuffer[JMAX_STRING_LEN] = {0};

	JSIPUA_CB_TYPE* pType = JNULL;
	JUINT32* pCode = JNULL;
	pjsip_inv_state* pState = JNULL;
	pjsua_acc_id* pAccId = JNULL;
	pjsua_call_id* pCallId = JNULL;

	if (!pBuf)
	{
        JSingleton<JLog>::instance2() << set(JLOG_MOD_SIPUA, JLOG_ERROR_LEVEL) 
            << "JPjSipCallback::DeSerialize input param is NULL\n";
        return JFAILURE;
	}

	pOffset = pBuf;

	pType = reinterpret_cast<JSIPUA_CB_TYPE*>(pOffset);
	eType = *pType;
	pOffset += sizeof(JSIPUA_CB_TYPE);

	pCode = reinterpret_cast<JUINT32*>(pOffset);
	uiCode = *pCode;
	pOffset += sizeof(JUINT32);

	pState = reinterpret_cast<pjsip_inv_state*>(pOffset);
	state = *pState;
	pOffset += sizeof(pjsip_inv_state);

	pAccId = reinterpret_cast<pjsua_acc_id*>(pOffset);
	acc_id = *pAccId;
	pOffset += sizeof(pjsua_acc_id);

	pCallId = reinterpret_cast<pjsua_call_id*>(pOffset);
	call_id = *pCallId;
	pOffset += sizeof(pjsua_call_id);

	pLen = reinterpret_cast<JUINT32*>(pOffset);
	pOffset += sizeof(JUINT32);
	if (*pLen)
	{
		SafeStrncpy(strBuffer, pOffset, *pLen, JMAX_STRING_LEN);
		pRemoteNumber = strBuffer;
		pOffset += *pLen;
	}

	return JSUCCESS;
}

JUINT32 JPjSipCallback::operator=(JPjSipCallback& rPjSipCallback)
{
	eType = rPjSipCallback.GetCbType();
	uiCode = rPjSipCallback.GetCode();
	state = rPjSipCallback.GetState();
	acc_id = rPjSipCallback.GetAccId();
	call_id = rPjSipCallback.GetCallId();
	pRemoteNumber = rPjSipCallback.GetRemoteNumber();

	return JSUCCESS;
}

JSIPUA_CB_TYPE JPjSipCallback::GetCbType()
{
	return eType;
}

JUINT32 JPjSipCallback::SetCbType(JSIPUA_CB_TYPE type)
{
	eType = type;

	return JSUCCESS;
}

JUINT32 JPjSipCallback::GetCode()
{
	return uiCode;
}

JUINT32 JPjSipCallback::SetCode(JUINT32 code)
{
	uiCode = code;

	return JSUCCESS;
}

pjsip_inv_state JPjSipCallback::GetState()
{
	return state;
}

JUINT32 JPjSipCallback::SetState(pjsip_inv_state eState)
{
	state = eState;

	return JSUCCESS;
}

pjsua_acc_id JPjSipCallback::GetAccId()
{
	return acc_id;
}

JUINT32 JPjSipCallback::SetAccId(pjsua_acc_id eId)
{
	acc_id = eId;

	return JSUCCESS;
}

pjsua_call_id JPjSipCallback::GetCallId()
{
	return call_id;
}

JUINT32 JPjSipCallback::SetCallId(pjsua_call_id eId)
{
	call_id = eId;

	return JSUCCESS;
}

JString& JPjSipCallback::GetRemoteNumber()
{
	return pRemoteNumber;
}

JUINT32 JPjSipCallback::SetRemoteNumber(JString& rStr)
{
	pRemoteNumber = rStr;

	return JSUCCESS;
}

JPjSip::JPjSip()
{
    m_accId = JNULL;
    m_callId = JNULL;

    return;
}

JPjSip::~JPjSip()
{
    return;
}

JUINT32 JPjSip::SipUaCreate()
{
    pj_status_t status;
	pjsua_config cfg;
	pjsua_logging_config log_cfg;
	pjsua_transport_config tran_cfg;
    pjsua_media_config media_cfg;

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_SIPUA, "JPjSip::SipUaCreate");

    //create pjsua first
    status = pjsua_create();
    if (status != PJ_SUCCESS)
    {
        JSingleton<JLog>::instance2() << set(JLOG_MOD_SIPUA, JLOG_ERROR_LEVEL) 
            << "JPjSip::SipUaCreate pjsua_create return failure, status="
            << status 
            << "\n";
        return JFAILURE;
    }

    //init pjsua
	pjsua_config_default(&cfg);
	cfg.cb.on_incoming_call = &OnIncomingCall;
	cfg.cb.on_call_media_state = &OnCallMediaState;
	cfg.cb.on_call_state = &OnCallState;
	cfg.cb.on_reg_state2 = &OnRegState2;
	cfg.cb.on_dtmf_digit = &OnIncomingDtmf;

	pjsua_logging_config_default(&log_cfg);
	log_cfg.console_level = JPJSIP_CONSOLE_LEVEL;

    pjsua_media_config_default(&media_cfg);
    media_cfg.ec_options = PJMEDIA_ECHO_SIMPLE;
    media_cfg.ec_tail_len = 256;
    media_cfg.ptime = 20;
	status = pjsua_init(&cfg, &log_cfg, &media_cfg);
    if (status != PJ_SUCCESS)
    {
        JSingleton<JLog>::instance2() << set(JLOG_MOD_SIPUA, JLOG_ERROR_LEVEL) 
            << "JPjSip::SipUaCreate pjsua_init return failure, status="
            << status 
            << "\n";
        return JFAILURE;
    }

    //add udp transport
	pjsua_transport_config_default(&tran_cfg);
	tran_cfg.port = JPJSIP_LOCAL_PORT;
	status = pjsua_transport_create(PJSIP_TRANSPORT_UDP, &tran_cfg, NULL);
	if (status != PJ_SUCCESS)
    {
        JSingleton<JLog>::instance2() << set(JLOG_MOD_SIPUA, JLOG_ERROR_LEVEL) 
            << "JPjSip::SipUaCreate pjsua_transport_create return failure, status="
            << status 
            << "\n";
        return JFAILURE;
    }

    //initialization is done, now start pjsua
    status = pjsua_start();
    if (status != PJ_SUCCESS)
    {
        JSingleton<JLog>::instance2() << set(JLOG_MOD_SIPUA, JLOG_ERROR_LEVEL) 
            << "JPjSip::SipUaCreate pjsua_start return failure, status="
            << status 
            << "\n";
        return JFAILURE;
    }

	return JSUCCESS;
}

JUINT32 JPjSip::SipUaDestory()
{
    pj_status_t status;

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_SIPUA, "JPjSip::SipUaDestory");

    HangUp();

    //destroy pjsua
    status = pjsua_destroy();
    if (status != PJ_SUCCESS)
    {
        JSingleton<JLog>::instance2() << set(JLOG_MOD_SIPUA, JLOG_ERROR_LEVEL) 
            << "JPjSip::SipUaDestory pjsua_destroy return failure, status="
            << status 
            << "\n";
    	return JFAILURE;
    }

	return JSUCCESS;
}

JUINT32 JPjSip::Register(JString& rNumber,
                        JString& rAuthName,
                        JString& rAuthPasswd,
                        JString& rProxyAddr)
{
	JCHAR id[JMAX_STRING_LEN] = {0};
	JCHAR reg_uri[JMAX_STRING_LEN] = {0};
    pj_status_t status;
	pjsua_acc_config cfg;

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_SIPUA, "JPjSip::Register");

    if (!rNumber.GetLength() || !rProxyAddr.GetLength())
    {
        JSingleton<JLog>::instance2() << set(JLOG_MOD_SIPUA, JLOG_ERROR_LEVEL) 
            << "JPjSip::Register input param is NULL\n";
        return JFAILURE;
    }

	pjsua_acc_config_default(&cfg);
	SafeSprintf(id, JMAX_STRING_LEN, "sip:%s@%s", rNumber.c_str(), rProxyAddr.c_str());
	cfg.id = pj_str(id);
	SafeSprintf(reg_uri, JMAX_STRING_LEN, "sip:%s", rProxyAddr.c_str());
	cfg.reg_uri = pj_str(reg_uri);
	if (rAuthName.GetLength()&& rAuthPasswd.GetLength())
	{
    	cfg.cred_count = 1;
    	cfg.cred_info[0].realm = pj_str("*");
    	cfg.cred_info[0].scheme = pj_str("digest");
    	cfg.cred_info[0].username = pj_str(rAuthName.c_str());
    	cfg.cred_info[0].data_type = PJSIP_CRED_DATA_PLAIN_PASSWD;
    	cfg.cred_info[0].data = pj_str(rAuthPasswd.c_str());
	}

	status = pjsua_acc_add(&cfg, PJ_TRUE, &m_accId);
	if (status != PJ_SUCCESS)
    {
        JSingleton<JLog>::instance2() << set(JLOG_MOD_SIPUA, JLOG_ERROR_LEVEL) 
            << "JPjSip::Register pjsua_acc_add return failure, status="
            << status 
            << "\n";
		return JFAILURE;
    }

	return JSUCCESS;
}

JUINT32 JPjSip::UnRegister()
{
    pj_status_t status;

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_SIPUA, "JPjSip::UnRegister");

    if (m_accId<0)
    {
        JSingleton<JLog>::instance2() << set(JLOG_MOD_SIPUA, JLOG_ERROR_LEVEL) 
            << "JPjSip::UnRegister m_accId isn't correct\n";
        return JFAILURE;
    }

    status = pjsua_acc_del(m_accId);
	if (status != PJ_SUCCESS)
    {
        JSingleton<JLog>::instance2() << set(JLOG_MOD_SIPUA, JLOG_ERROR_LEVEL) 
            << "JPjSip::UnRegister pjsua_acc_del return failure, status="
            << status 
            << "\n";
		return JFAILURE;
    }

    m_accId = JNULL;

	return JSUCCESS;
}

JUINT32 JPjSip::StartCall(JString& rNumber, JString& rProxyAddr)
{
    pj_status_t status;
    pj_str_t uri;
	JCHAR ptrRmtAddr[JMAX_STRING_LEN] = {0};

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_SIPUA, "JPjSip::StartCall");

    if (!rNumber.GetLength())
    {
        JSingleton<JLog>::instance2() << set(JLOG_MOD_SIPUA, JLOG_ERROR_LEVEL) 
            << "JPjSip::StartCall pNumber is NULL\n";
        return JFAILURE;
    }

	SafeSprintf(ptrRmtAddr, JMAX_STRING_LEN, "sip:%s@%s", rNumber.c_str(), rProxyAddr.c_str());
	uri = pj_str(ptrRmtAddr);

	status = pjsua_call_make_call(m_accId, &uri, 0, NULL, NULL, &m_callId);
	if (status != PJ_SUCCESS)
    {
        JSingleton<JLog>::instance2() << set(JLOG_MOD_SIPUA, JLOG_ERROR_LEVEL) 
            << "JPjSip::StartCall pjsua_call_make_call return failure, status="
            << status 
            << "\n";
		return JFAILURE;
    }

	return JSUCCESS;
}

JUINT32 JPjSip::DialDtmf(JCHAR cKey)
{
    pj_status_t status;
    pj_str_t digits;
    JCHAR buf[JMAX_STRING_LEN] = {0};

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_SIPUA, "JPjSip::DialDtmf");

    SafeSprintf(buf, JMAX_STRING_LEN, "%c", cKey);
    digits = pj_str(buf);

	status = pjsua_call_dial_dtmf(m_callId, &digits);
	if (status != PJ_SUCCESS)
    {
        JSingleton<JLog>::instance2() << set(JLOG_MOD_SIPUA, JLOG_ERROR_LEVEL) 
            << "JPjSip::DialDtmf pjsua_call_dial_dtmf return failure, status="
            << status 
            << "\n";
		return JFAILURE;
    }

	return JSUCCESS;
}

JUINT32 JPjSip::Answer(JUINT32 uiCode)
{
    pj_status_t status;

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_SIPUA, "JPjSip::Answer");

    status = pjsua_call_answer(m_callId, 200, NULL, NULL);
	if (status != PJ_SUCCESS)
    {
        JSingleton<JLog>::instance2() << set(JLOG_MOD_SIPUA, JLOG_ERROR_LEVEL) 
            << "JPjSip::Answer pjsua_call_answer return failure, status="
            << status 
            << "\n";
		return JFAILURE;
    }

	return JSUCCESS;
}

JUINT32 JPjSip::Hold()
{
    pj_status_t status;

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_SIPUA, "JPjSip::Hold");

    status = pjsua_call_set_hold(m_callId, NULL);
	if (status != PJ_SUCCESS)
    {
        JSingleton<JLog>::instance2() << set(JLOG_MOD_SIPUA, JLOG_ERROR_LEVEL) 
            << "JPjSip::Hold pjsua_call_set_hold return failure, status="
            << status 
            << "\n";
		return JFAILURE;
    }

	return JSUCCESS;
}

JUINT32 JPjSip::UnHold()
{
    pj_status_t status;

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_SIPUA, "JPjSip::UnHold");

    status = pjsua_call_reinvite(m_callId, PJ_TRUE, NULL);
	if (status != PJ_SUCCESS)
    {
        JSingleton<JLog>::instance2() << set(JLOG_MOD_SIPUA, JLOG_ERROR_LEVEL) 
            << "JPjSip::UnHold pjsua_call_set_hold return failure, status="
            << status 
            << "\n";
		return JFAILURE;
    }

	return JSUCCESS;
}

JUINT32 JPjSip::HangUp()
{
    pj_status_t status;

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_SIPUA, "JPjSip::HangUp");

    status = pjsua_call_hangup(m_callId, 0, NULL, NULL);
	if (status != PJ_SUCCESS)
    {
        JSingleton<JLog>::instance2() << set(JLOG_MOD_SIPUA, JLOG_ERROR_LEVEL) 
            << "JPjSip::HangUp pjsua_call_hangup return failure, status="
            << status 
            << "\n";
		return JFAILURE;
    }

	return JSUCCESS;
}

JUINT32 JPjSip::SetCodecPriority()
{
    pj_status_t status;
    pjsua_codec_info c[32];
    JUINT32 count;
    JUINT32 idx=0;

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_SIPUA, "JPjSip::SetCodecPriority");

    pjsua_enum_codecs(c, &count);

    for (idx=0; idx<count; ++idx)
    {
        if (!(SafeStrstr(c[idx].codec_id.ptr, "PCMU") != NULL ||
            SafeStrstr(c[idx].codec_id.ptr, "PCMA") != NULL))
        {
            status = pjsua_codec_set_priority(&(c[idx].codec_id), PJMEDIA_CODEC_PRIO_DISABLED);
        }
    }

	return JSUCCESS;
}

JUINT32 JPjSip::GetRemoteNumber(JCHAR* pRemoteInfo, JCHAR* pNumber)
{
    JCHAR* pOffset1 = 0;
    JCHAR* pOffset2 = 0;
    JUINT32 uiLen = 0;

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_SIPUA, "JPjSip::GetRemoteNumber");

    if (!pRemoteInfo || !pNumber)
    {
        JSingleton<JLog>::instance2() << set(JLOG_MOD_SIPUA, JLOG_ERROR_LEVEL) 
            << "JPjSip::GetRemoteNumber input param is NULL\n";
        return JFAILURE;
    }

    pOffset1 = SafeStrstr(pRemoteInfo, "\"");
    if (pOffset1)
    {
        pOffset2 = SafeStrstr(pOffset1+1, "\"");
        if (pOffset2)
        {
            uiLen = pOffset2-(pOffset1+1);
            SafeStrncpy(pNumber, pOffset1+1, uiLen, JMAX_STRING_LEN);

        	return JSUCCESS;
        }
    }

    pOffset1 = SafeStrstr(pRemoteInfo, ":");
    if (pOffset1)
    {
        pOffset2 = SafeStrstr(pOffset1+1, "@");
        if (pOffset2)
        {
            uiLen = pOffset2-(pOffset1+1);
            SafeStrncpy(pNumber, pOffset1+1, uiLen, JMAX_STRING_LEN);

        	return JSUCCESS;
        }
    }

	return JFAILURE;
}

JVOID JPjSip::ProcOnIncomingCall(JPjSipCallback* pPjSipCallback, JPjSipUa* pPjSipUa)
{
    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_SIPUA, "JPjSip::ProcOnIncomingCall");

    if (!pPjSipCallback)
    {
        JSingleton<JLog>::instance2() << set(JLOG_MOD_SIPUA, JLOG_ERROR_LEVEL) 
            << "JPjSip::ProcOnIncomingCall input param is invalid\n";
        return;
    }

	return;
}

JVOID JPjSip::ProcOnCallMediaState(JPjSipCallback* pPjSipCallback, JPjSipUa* pPjSipUa)
{
    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_SIPUA, "JPjSip::ProcOnCallMediaState");

    if (!pPjSipCallback)
    {
        JSingleton<JLog>::instance2() << set(JLOG_MOD_SIPUA, JLOG_ERROR_LEVEL) 
            << "JPjSip::ProcOnCallMediaState input param is invalid\n";
        return;
    }

	return;
}

JVOID JPjSip::ProcOnCallState(JPjSipCallback* pPjSipCallback, JPjSipUa* pPjSipUa)
{
    JUINT32 uiState = 0;

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_SIPUA, "JPjSip::ProcOnCallState");

    if (!pPjSipCallback || !pPjSipUa)
    {
        JSingleton<JLog>::instance2() << set(JLOG_MOD_SIPUA, JLOG_ERROR_LEVEL) 
            << "JPjSip::ProcOnCallState input param is invalid\n";
        return;
    }

    uiState = pPjSipCallback->GetState();
    JSingleton<JLog>::instance2() << set(JLOG_MOD_SIPUA, JLOG_DEBUG_LEVEL) 
        << "JPjSip::ProcOnCallState change state to "
        << uiState
        << "\n";

    switch(uiState)
    {
        case PJSIP_INV_STATE_CALLING:
        {
            m_callId = pPjSipCallback->GetCallId();

            pPjSipUa->StateChange(JSIPUA_STATUS_OUTGOING);
        }
        break;

        case PJSIP_INV_STATE_EARLY:
        {
            if (pPjSipUa->GetStatus() == JSIPUA_STATUS_OUTGOING)
            {
                break;
            }
        }
        case PJSIP_INV_STATE_INCOMING:
        {
            pPjSipUa->SetCurrNumber(pPjSipCallback->GetRemoteNumber());

            pPjSipUa->StoreContactNumber();

            m_callId = pPjSipCallback->GetCallId();

            pPjSipUa->StateChange(JSIPUA_STATUS_INCOMING);
        }
        break;

        case PJSIP_INV_STATE_CONFIRMED:
        case PJSIP_INV_STATE_CONNECTING:
        {
            pPjSipUa->StateChange(JSIPUA_STATUS_CONNECTED);
        }
        break;

        case PJSIP_INV_STATE_DISCONNECTED:
        {
            m_callId = JNULL;

            pPjSipUa->StateChange(JSIPUA_STATUS_REGISTERED);
        }
        break;

        case PJSIP_INV_STATE_NULL:
        {
            m_callId = JNULL;

            pPjSipUa->StateChange(JSIPUA_STATUS_REGISTERED);
        }
        break;

        default:
        break;
    }

	return;
}

JVOID JPjSip::ProcOnRegState2(JPjSipCallback* pPjSipCallback, JPjSipUa* pPjSipUa)
{
    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_SIPUA, "JPjSip::ProcOnRegState2");

    if (!pPjSipCallback)
    {
        JSingleton<JLog>::instance2() << set(JLOG_MOD_SIPUA, JLOG_ERROR_LEVEL) 
            << "JPjSip::ProcOnRegState2 input param is invalid\n";
        return;
    }

    if (pPjSipCallback->GetCode()== 200)
    {
        pPjSipUa->StateChange(JSIPUA_STATUS_REGISTERED);
    }
    else if (pPjSipCallback->GetCode()== 401 || pPjSipCallback->GetCode()== 407)
    {
        pPjSipUa->StateChange(JSIPUA_STATUS_REGISTERING);
    }
    else
    {
        pPjSipUa->StateChange(JSIPUA_STATUS_IDLE);
    }

	return;
}

JVOID JPjSip::ProcOnIncomingDtmf(JPjSipCallback* pPjSipCallback, JPjSipUa* pPjSipUa)
{
    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_SIPUA, "JPjSip::ProcOnIncomingDtmf");

    if (!pPjSipCallback)
    {
        JSingleton<JLog>::instance2() << set(JLOG_MOD_SIPUA, JLOG_ERROR_LEVEL) 
            << "JPjSip::ProcOnIncomingDtmf input param is invalid\n";
        return;
    }

	return;
}

JUINT32 OnFlushContactList(JVOID* pData)
{
    JPjSipUa* pSipUa = reinterpret_cast<JPjSipUa*>(pData);
    JEvent* pEvent = JNULL;
    JCommEngine* pComEngine = JNULL;
    JThread* pThread = JNULL;
    JModuleThread* pModuleThread = JNULL;

    if (!pSipUa)
    {
        return JFAILURE;
    }

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_SIPUA, "OnFlushContactList");

    pEvent = new JEvent(JEVT_SIPUA_FLUAH_CONTACT_LIST);
    if (pEvent)
    {
        pEvent->SetFromProc(JS_P_JMAINPROC);
        pEvent->SetFromThrd(JS_T_JMAINTHREAD);
        pEvent->SetFromMod(JS_M_JSIPUA);
        pEvent->SetToProc(JS_P_JMAINPROC);
        pEvent->SetToThrd(JS_T_JMAINTHREAD);
        pEvent->SetToMod(JS_M_JSIPUA);
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

JVOID OnIncomingCall(pjsua_acc_id acc_id, 
                        pjsua_call_id call_id,
			            pjsip_rx_data *rdata)
{
    JPjSipUa* pSipUa = 
        dynamic_cast<JPjSipUa*>(GetRegisterModule(JS_T_JMAINTHREAD, JS_M_JSIPUA));
    pj_status_t status;

    if (!pSipUa)
    {
        return;
    }

    JAutoPtr<JLock> clsLockAutoPtr(&pSipUa->m_Lock);

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_SIPUA, "OnIncomingCall");

    status = pjsua_call_answer(call_id, 180, NULL, NULL);
	if (status != PJ_SUCCESS)
    {
        JSingleton<JLog>::instance2() << set(JLOG_MOD_SIPUA, JLOG_ERROR_LEVEL) 
            << "OnIncomingCall pjsua_call_answer return failure, status="
            << status 
            << "\n";
    }

	return;
}

JVOID OnCallMediaState(pjsua_call_id call_id)
{
    JPjSipUa* pSipUa = 
        dynamic_cast<JPjSipUa*>(GetRegisterModule(JS_T_JMAINTHREAD, JS_M_JSIPUA));
    pj_status_t status;
    pjsua_call_info ci;

    if (!pSipUa)
    {
        return;
    }

    JAutoPtr<JLock> clsLockAutoPtr(&pSipUa->m_Lock);

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_SIPUA, "OnCallMediaState");

    status = pjsua_call_get_info(call_id, &ci);
	if (status != PJ_SUCCESS)
    {
        JSingleton<JLog>::instance2() << set(JLOG_MOD_SIPUA, JLOG_ERROR_LEVEL) 
            << "OnCallMediaState pjsua_call_get_info return failure, status="
            << status 
            << "\n";
    	return;
    }

    if (ci.media_status == PJSUA_CALL_MEDIA_ACTIVE)
    {
    	// When media is active, connect call to sound device.
    	status = pjsua_conf_connect(ci.conf_slot, 0);
    	if (status != PJ_SUCCESS)
        {
            JSingleton<JLog>::instance2() << set(JLOG_MOD_SIPUA, JLOG_ERROR_LEVEL) 
                << "OnCallMediaState pjsua_conf_connect return failure, status="
                << status 
                << "\n";
        	return;
        }

    	status = pjsua_conf_connect(0, ci.conf_slot);
    	if (status != PJ_SUCCESS)
        {
            JSingleton<JLog>::instance2() << set(JLOG_MOD_SIPUA, JLOG_ERROR_LEVEL) 
                << "OnCallMediaState pjsua_conf_connect return failure, status="
                << status 
                << "\n";
        	return;
        }
    }

	return;
}

JVOID OnCallState(pjsua_call_id call_id, pjsip_event *e)
{
    pj_status_t status;
    pjsua_call_info ci;

    JPjSipUa* pSipUa = 
        dynamic_cast<JPjSipUa*>(GetRegisterModule(JS_T_JMAINTHREAD, JS_M_JSIPUA));
    JEvent* pEvent = JNULL;
    JPjSipCallback* pPjSipCallbackBody = JNULL;
    JCHAR number[JMAX_STRING_LEN] = {0};
    JCommEngine* pComEngine = JNULL;
    JThread* pThread = JNULL;
    JAgentThread* pAgentThread = JNULL;

    if (!pSipUa)
    {
        return;
    }

    JAutoPtr<JLock> clsLockAutoPtr(&pSipUa->m_Lock);

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_SIPUA, "OnCallState");

    status = pjsua_call_get_info(call_id, &ci);
	if (status != PJ_SUCCESS)
    {
        JSingleton<JLog>::instance2() << set(JLOG_MOD_SIPUA, JLOG_ERROR_LEVEL) 
            << "OnCallState pjsua_call_get_info return failure, status="
            << status 
            << "\n";
    	return;
    }

    pEvent = new JEvent(JEVT_SIPUA_CALLBACK);
    pPjSipCallbackBody = new JPjSipCallback;
	if (pEvent && pPjSipCallbackBody)
	{
		pPjSipCallbackBody->SetCbType(JSIPUA_CB_TYPE_CALL_STATE);
		pPjSipCallbackBody->SetState(ci.state);
		pPjSipCallbackBody->SetCallId(call_id);
        pSipUa->GetPjSip()->GetRemoteNumber(ci.remote_info.ptr, number);
        JString strNumber = number;
		pPjSipCallbackBody->SetRemoteNumber(strNumber);

	    pEvent->SetFromProc(JS_P_JMAINPROC);
	    pEvent->SetFromThrd(JS_T_JSIPUA);
	    pEvent->SetFromMod(JNULL);
	    pEvent->SetToProc(JS_P_JMAINPROC);
	    pEvent->SetToThrd(JS_T_JMAINTHREAD);
	    pEvent->SetToMod(JS_M_JSIPUA);
	    pEvent->SetBody(pPjSipCallbackBody);

        pThread = JSingleton<JThreadManager>::instance()->GetThread(JS_T_JSIPUA);
        pAgentThread = dynamic_cast<JAgentThread*>(pThread);
        if (pAgentThread)
        {
    	    pComEngine = pAgentThread->GetNotifyCommEngine();
    	    if (pComEngine)
    	    {
    	        pComEngine->SendEvent(pEvent);
    	    }
	    }
    }
    else
    {
        JSingleton<JLog>::instance2() << set(JLOG_MOD_SIPUA, JLOG_ERROR_LEVEL) 
            << "OnCallState alloc body failure\n";
    }

	return;
}

JVOID OnRegState2(pjsua_acc_id acc_id, pjsua_reg_info *info)
{
    JPjSipUa* pSipUa = 
        dynamic_cast<JPjSipUa*>(GetRegisterModule(JS_T_JMAINTHREAD, JS_M_JSIPUA));
    JEvent* pEvent = JNULL;
    JPjSipCallback* pPjSipCallbackBody = JNULL;
    JCommEngine* pComEngine = JNULL;
    JThread* pThread = JNULL;
    JAgentThread* pAgentThread = JNULL;

    if (!info)
    {
        JSingleton<JLog>::instance2() << set(JLOG_MOD_SIPUA, JLOG_ERROR_LEVEL) 
           << "OnRegState2 input param is NULL\n";
        return;
    }

    if (!pSipUa)
    {
        return;
    }

    JAutoPtr<JLock> clsLockAutoPtr(&pSipUa->m_Lock);

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_SIPUA, "OnRegState2");

    pEvent = new JEvent(JEVT_SIPUA_CALLBACK);
    pPjSipCallbackBody = new JPjSipCallback;
	if (pEvent && pPjSipCallbackBody)
	{
		pPjSipCallbackBody->SetCbType(JSIPUA_CB_TYPE_REG_STATE2);
		pPjSipCallbackBody->SetCode(info->cbparam->code);
		pPjSipCallbackBody->SetAccId(acc_id);

	    pEvent->SetFromProc(JS_P_JMAINPROC);
	    pEvent->SetFromThrd(JS_T_JSIPUA);
	    pEvent->SetFromMod(JNULL);
	    pEvent->SetToProc(JS_P_JMAINPROC);
	    pEvent->SetToThrd(JS_T_JMAINTHREAD);
	    pEvent->SetToMod(JS_M_JSIPUA);
	    pEvent->SetBody(pPjSipCallbackBody);

        pThread = JSingleton<JThreadManager>::instance()->GetThread(JS_T_JSIPUA);
        pAgentThread = dynamic_cast<JAgentThread*>(pThread);
        if (pAgentThread)
        {
    	    pComEngine = pAgentThread->GetNotifyCommEngine();
    	    if (pComEngine)
    	    {
    	        pComEngine->SendEvent(pEvent);
    	    }
	    }
    }
    else
    {
        JSingleton<JLog>::instance2() << set(JLOG_MOD_SIPUA, JLOG_ERROR_LEVEL) 
            << "OnRegState2 alloc body failure\n";
    }

	return;
}

JVOID OnIncomingDtmf(pjsua_call_id call_id, JINT32 iDtmf)
{
    JPjSipUa* pSipUa = 
        dynamic_cast<JPjSipUa*>(GetRegisterModule(JS_T_JMAINTHREAD, JS_M_JSIPUA));
    JCHAR errInfo[JMAX_BUFFER_LEN] = {0};
	pjmedia_tone_digit digits[1];
	pj_status_t status;
	pj_pool_t   *pool;
	pjmedia_port *port;
	JINT32 iDtmfSlot = 0;
    pjsua_call_info ci;
    pjsua_conf_port_info info;

    if (pSipUa->IsDtmf(iDtmf) == JFALSE)
    {
        JSingleton<JLog>::instance2() << set(JLOG_MOD_SIPUA, JLOG_ERROR_LEVEL) 
            << "OnIncomingDtmf input param is wrong\n";
        return;
    }

    if (!pSipUa)
    {
        return;
    }

    JAutoPtr<JLock> clsLockAutoPtr(&pSipUa->m_Lock);

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_SIPUA, "OnIncomingDtmf");

    try
    {
        pool = pjsua_pool_create("jphone_dtmf", 1000, 1000);

        status = pjsua_call_get_info(call_id, &ci);
    	if (status != PJ_SUCCESS)
        {
            sprintf(errInfo,  
                "OnIncomingDtmf pjsua_call_get_info return failure, status=%s\n", status);
            throw JException(errInfo);
        }

        //get current activte codec samples per frame
        status = pjsua_conf_get_port_info(ci.conf_slot, &info);
    	if (status != PJ_SUCCESS)
        {
            sprintf(errInfo,  
                "OnIncomingDtmf pjsua_conf_get_port_info return failure, status=%s\n", status);
            throw JException(errInfo);
        }

        //create media port
        status = pjmedia_tonegen_create(pool, 8000, 1, info.samples_per_frame, 16, 0, &port);
    	if (status != PJ_SUCCESS)
        {
            sprintf(errInfo,  
                "OnIncomingDtmf pjmedia_tonegen_create return failure, status=%s\n", status);
            throw JException(errInfo);
        }

    	digits[0].digit = iDtmf;
    	digits[0].on_msec = JSIPUA_ON_DURATION;
    	digits[0].off_msec = JSIPUA_OFF_DURATION;

        //set media port param of dtmf digit playing
    	status = pjmedia_tonegen_play_digits(port, 1, digits, 0);
    	if (status != PJ_SUCCESS)
        {
            sprintf(errInfo, 
                "OnIncomingDtmf pjmedia_tonegen_play_digits return failure, status=%s\n", status);
            throw JException(errInfo);
        }

        //add media port to pjsip conference bridge, return dtmf solt id
    	status = pjsua_conf_add_port(pool, port, &iDtmfSlot);
    	if (status != PJ_SUCCESS)
        {
            sprintf(errInfo, 
                "OnIncomingDtmf pjsua_conf_add_port return failure, status=%s\n", status);
            throw JException(errInfo);
        }

        //connect dtmf solt id to the sound device
    	status = pjsua_conf_connect(iDtmfSlot, 0);
    	if (status != PJ_SUCCESS)
        {
            sprintf(errInfo, 
                "OnIncomingDtmf pjsua_conf_connect return failure, status=%s\n", status);
            throw JException(errInfo);
        }

    	status = pjsua_conf_connect(0, iDtmfSlot);
    	if (status != PJ_SUCCESS)
        {
            sprintf(errInfo, 
                "OnIncomingDtmf pjsua_conf_connect return failure, status=%s\n", status);
            throw JException(errInfo);
        }
    }
    catch(JException &e)
    {
        JCHAR* pStr = JNULL;

        pStr = e.GetErrInfo();
        if (pStr)
        {
            JSingleton<JLog>::instance2() << set(JLOG_MOD_SIPUA, JLOG_ERROR_LEVEL) 
                << "OnIncomingDtmf " << pStr;
        }
        else
        {
            JSingleton<JLog>::instance2() << set(JLOG_MOD_SIPUA, JLOG_ERROR_LEVEL) 
                << "OnIncomingDtmf return failue\n";
        }
    }

	return;
}

JPjSipUa::JPjSipUa()
{
	JTHRD_PUB_DATA thrdPubData;
    JString strThrdName;
    JString strLocalAddr;

    StateChange(JSIPUA_STATUS_IDLE);

    m_contactListNumber = 0;
    m_timeHandler = 0;

    m_ePersistenceType = JSER_PERSISTENCE_NONE;
    m_pSerialization = JNULL;

    //create JAgentThread and add into JThreadManager
    strThrdName = JS_T_JSIPUA;
    strLocalAddr = JSIPUA_LOCAL_ADDR;
	m_pAgentThread = new JAgentThread(&strThrdName, &strLocalAddr, JSIPUA_LOCAL_PORT);

    JThreadManager* pThreadManager = JSingleton<JThreadManager>::instance();
    thrdPubData.strThrdName = JS_T_JSIPUA;
    thrdPubData.pThread = m_pAgentThread;
    pThreadManager->RegisterThread(thrdPubData);

	return;
}

JPjSipUa::~JPjSipUa()
{
    m_pPjSip->SipUaDestory();

    if (m_timeHandler)
    {
        JSingleton<JTimer>::instance()->StopTimer(m_timeHandler);
        JSingleton<JTimer>::instance()->DestoryTimer(m_timeHandler);
    }

    SetContactList(m_contactList);
    m_contactList.RemoveAllItem();

    if (m_pSerialization)
    {
        delete m_pSerialization;
    }

    if (m_pAgentThread)
    {
        delete m_pAgentThread;
    }

	return;
}

JUINT32 JPjSipUa::SetPersistenceType(JSER_PERSISTENCE_TYPE eType)
{
    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_SIPUA, "JPjSipUa::SetPersistenceType");

    if (eType != JSER_PERSISTENCE_SQL && 
        eType != JSER_PERSISTENCE_XML)
    {
        JSingleton<JLog>::instance2() << set(JLOG_MOD_SIPUA, JLOG_ERROR_LEVEL) 
            << "JPjSipUa::SetPersistenceType input param is wrong\n";
        return JFAILURE;
    }

    m_ePersistenceType = eType;

    return JSUCCESS;
}

JUINT32 JPjSipUa::InitFunc()
{
    JUINT32 uiRet = JSUCCESS;

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_SIPUA, "JPjSipUa::InitFunc");

    m_pPjSip = new JPjSip;
    if (JPjSipUa)
    {
        uiRet= m_pPjSip->SipUaCreate();
        if (!uiRet)
        {
            JSingleton<JLog>::instance2() << set(JLOG_MOD_SIPUA, JLOG_ERROR_LEVEL) 
                << "JPjSipUa::InitFunc SipUaCreate return failure\n";
            return uiRet;
        }
    }

    m_pSerialization = new JSerialization(m_ePersistenceType);

    if (m_pSerialization)
    {
        GetContactList(m_contactList);
        m_contactListNumber = m_contactList.GetItemNum();

        GetCfgList(&m_cfg);

        if (m_cfg.GetNumber().GetLength() &&
            m_cfg.GetProxyAddr().GetLength())
        {
            if (m_status>=JSIPUA_STATUS_OUTGOING)
            {
                m_pPjSip->HangUp();
            }

            if (m_status>=JSIPUA_STATUS_REGISTERED)
            {
                m_pPjSip->UnRegister();
            }

            StateChange(JSIPUA_STATUS_REGISTERING);

            m_pPjSip->Register(m_cfg.GetNumber(), m_cfg.GetAuthName(), 
                m_cfg.GetAuthPasswd(), m_cfg.GetProxyAddr());

            StateChange(JSIPUA_STATUS_REGISTERING);
        }
    }

    m_timeHandler = JSingleton<JTimer>::instance()->CreateTimer(JTIMER_TYPE_PERIODIC, 
                                                    OnFlushContactList, 
                                                    this, 
                                                    JSIPUA_FLUSH_INTERVAL);
    if (m_timeHandler)
    {
        JSingleton<JTimer>::instance()->StartTimer(m_timeHandler);
    }

    m_pPjSip->SetCodecPriority();

    return JSUCCESS;
}

JUINT32 JPjSipUa::EventProcFunc(JEvent* pEvent)
{
    JUINT32 uiRet = JFAILURE;
    JEVT_TYPE type = JEVT_NONE;
    JPjSipUaKey* pJPjSipUaKey = JNULL;
    JPjSipUaClickContact* pPjSipUaClickContact = JNULL;
    JPjSipUaCfg* pPjSipUaCfg = JNULL;
    JPjSipCallback* pPjSipCallback = JNULL;

    JAutoPtr<JLock> clsLockAutoPtr(&m_Lock);

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_SIPUA, "JPjSipUa::EventProcFunc");

    if (!pEvent)
    {
        JSingleton<JLog>::instance2() << set(JLOG_MOD_SIPUA, JLOG_ERROR_LEVEL) 
            << "JPjSipUa::EventProcFunc input param is NULL\n";
        return JFAILURE;
    }

    type = pEvent->GetEventType();
    switch(type)
    {
        case JEVT_SIPUA_SET_CFG:
        {
            pPjSipUaCfg = dynamic_cast<JPjSipUaCfg*>(pEvent->GetBody());
            if (pPjSipUaCfg)
            {
                uiRet = ProcSetCfgEvent(pPjSipUaCfg);
            }
        }
        break;

        case JEVT_SIPUA_GET_CFG:
        {
            uiRet = ProcGetCfgEvent(pEvent);
        }
        break;

        case JEVT_SIPUA_PRESS_KEY:
        {
            pJPjSipUaKey = dynamic_cast<JPjSipUaKey*>(pEvent->GetBody());
            if (pJPjSipUaKey)
            {
                if (m_status != JSIPUA_STATUS_IDLE && m_status != JSIPUA_STATUS_REGISTERING)
                    uiRet = ProcPressKeyEvent(pJPjSipUaKey);
            }
        }
        break;

        case JEVT_SIPUA_CLICK_CONTACT:
        {
            pPjSipUaClickContact = dynamic_cast<JPjSipUaClickContact*>(pEvent->GetBody());
            if (pPjSipUaClickContact)
            {
                if (m_status != JSIPUA_STATUS_IDLE && m_status != JSIPUA_STATUS_REGISTERING)
                    uiRet = ProcClickContactEvent(pPjSipUaClickContact);
            }
        }
        break;

        case JEVT_SIPUA_GET_CALL_STATUS:
        {
            uiRet = ProcGetCallStatusEvent(pEvent);
        }
        break;

        case JEVT_SIPUA_GET_CONTACT_LIST:
        {
            uiRet = ProcGetContactListEvent(pEvent);
        }
        break;

        case JEVT_SIPUA_FLUAH_CONTACT_LIST:
        {
            uiRet = ProcOnFlushContactList();
        }
        break;

        case JEVT_SIPUA_CALLBACK:
        {
            pPjSipCallback = dynamic_cast<JPjSipCallback*>(pEvent->GetBody());
            if (pPjSipCallback)
            {
                uiRet = ProcCallBackEvent(pPjSipCallback);
            }
        }
        break;

        default:
        break;
    };

	return uiRet;
}

JEventBody* JPjSipUa::MakeEventBody(JUINT32 uiType)
{
	JEventBody* pEventBody = JNULL;

	switch(uiType)
	{
		case JEVT_SIPUA_SET_CFG:
		case JEVT_SIPUA_GET_CFG_RSP:
			pEventBody = new JPjSipUaCfg;
			break;

		case JEVT_SIPUA_PRESS_KEY:
			pEventBody = new JPjSipUaKey;
			break;

		case JEVT_SIPUA_CLICK_CONTACT:
			pEventBody = new JPjSipUaClickContact;
			break;

		case JEVT_SIPUA_GET_CALL_STATUS_RSP:
			pEventBody = new JPjSipUaCallStatus;
			break;

		case JEVT_SIPUA_GET_CONTACT_LIST_RSP:
			pEventBody = new JPjSipUaContactList;
			break;

		case JEVT_SIPUA_CALLBACK:
			pEventBody = new JPjSipCallback;
			break;

		default:
			break;
	};

	return pEventBody;
}

JCHAR JPjSipUa::KeyEnum2Char(JSIPUA_KEY eKey)
{
    JUINT32 uiSize = sizeof(gKeyMap)/sizeof(JSIPUA_KEY_MAP);
    JUINT32 uiIdx = 0;

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_SIPUA, "JPjSipUa::KeyEnum2Char");

    if (eKey<=JSIPUA_KEY_NONE || eKey>=JSIPUA_KEY_MAX)
    {
        JSingleton<JLog>::instance2() << set(JLOG_MOD_SIPUA, JLOG_ERROR_LEVEL) 
            << "JPjSipUa::KeyEnum2Char input param is invalid\n";
        return JNULL;
    }

    for (uiIdx=0; uiIdx<uiSize; uiIdx++)
    {
        if (gKeyMap[uiIdx].eKey == eKey)
        {
            return gKeyMap[uiIdx].cKey;
        }
    }

	return JNULL;
}

JSIPUA_KEY JPjSipUa::KeyChar2Enum(JCHAR* pKey)
{
    JUINT32 uiSize = sizeof(gKeyMap)/sizeof(JSIPUA_KEY_MAP);
    JUINT32 uiIdx = 0;

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_SIPUA, "JPjSipUa::KeyEnum2Char");

    if (!pKey)
    {
        JSingleton<JLog>::instance2() << set(JLOG_MOD_SIPUA, JLOG_ERROR_LEVEL) 
            << "JPjSipUa::KeyEnum2Char input param is invalid\n";
        return JSIPUA_KEY_NONE;
    }

    for (uiIdx=0; uiIdx<uiSize; uiIdx++)
    {
        if (gKeyMap[uiIdx].cKey == *pKey)
        {
            return gKeyMap[uiIdx].eKey;
        }
    }

	return JSIPUA_KEY_NONE;
}

JUINT32 JPjSipUa::StateChange(JSIPUA_STATUS eStatus)
{
    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_SIPUA, "JPjSipUa::StateChange");

    switch(eStatus)
    {
        case JSIPUA_STATUS_IDLE:
        case JSIPUA_STATUS_REGISTERED:
        {
            m_strCurrNumber.Clear();
        }
        break;

        default:
        break;
    }

    m_status = eStatus;

	return JSUCCESS;
}

JUINT32 JPjSipUa::StoreContactNumber()
{
    JBOOL bExist = JFALSE;
    JListItem<JCHAR>* pItem = JNULL;
    JCHAR* pContactNumber = JNULL;
    JUINT32 uiLen = 0;
    JCHAR* pData = JNULL;

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_SIPUA, "JPjSipUa::StoreContactNumber");

    JListIterator<JCHAR> clsListIter(m_contactList);
    for (clsListIter.First(); clsListIter.Done(); clsListIter.Next())
    {
        pItem = clsListIter.Item();

        pContactNumber = pItem->GetData();
        if (pContactNumber)
        {
            if (m_strCurrNumber == pContactNumber)
            {
                bExist = JTRUE;
                break;
            }
        }
    }

    if (bExist)
    {
        m_contactList.RemoveItem(pItem);
        m_contactList.InsertItem(pItem, JNULL);
    }
    else
    {
        if (m_contactListNumber >= JPJSIP_MAX_NUMBER)
        {
            m_contactList.RemoveTailItem();

            m_contactListNumber--;
        }

        uiLen = m_strCurrNumber.GetLength();
        pData = JSingleton<JStaticMemory>::instance()->Alloc(uiLen+1);
        if (pData)
        {
            SafeMemset(pData, 0, uiLen+1);
            SafeStrcpy(pData, m_strCurrNumber.c_str(), uiLen+1);
            pItem = new JListItem<JCHAR>(pData);
            if (pItem)
            {
                m_contactList.InsertItem(pItem, JNULL);
                m_contactListNumber++;
            }
            else
            {
                JSingleton<JLog>::instance2() << set(JLOG_MOD_SIPUA, JLOG_ERROR_LEVEL) 
                    << "JPjSipUa::StoreContactNumber alloc JListItem failure\n";
            }
        }
        else
        {
            JSingleton<JLog>::instance2() << set(JLOG_MOD_SIPUA, JLOG_ERROR_LEVEL) 
                << "JPjSipUa::StoreContactNumber m_pStaticMemory Alloc failure\n";
        }
    }

	return JSUCCESS;
}

JUINT32 JPjSipUa::SetCurrNumber(JString& rNumber)
{
    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_SIPUA, "JPjSipUa::SetCurrNumber");

    m_strCurrNumber = rNumber;

    return JSUCCESS;
}

JSIPUA_STATUS JPjSipUa::GetStatus()
{
    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_SIPUA, "JPjSipUa::GetStatus");

    return m_status;
}

JPjSip* JPjSipUa::GetPjSip()
{
    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_SIPUA, "JPjSipUa::GetPjSip");

    return m_pPjSip;
}

JUINT32 JPjSipUa::ProcOnFlushContactList()
{
    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_SIPUA, "JPjSipUa::ProcOnFlushContactList");

    SetContactList(m_contactList);

	return JSUCCESS;
}

JUINT32 JPjSipUa::ProcCallBackEvent(JPjSipCallback* pPjSipCallback)
{
    JUINT32 uiLen = 0;

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_SIPUA, "JPjSipUa::ProcCallBackEvent");

    if (!pPjSipCallback)
    {
        JSingleton<JLog>::instance2() << set(JLOG_MOD_SIPUA, JLOG_ERROR_LEVEL) 
            << "JPjSipUa::ProcCallBackEvent pCallBackInfo is NULL\n";
        return JFAILURE;
    }

    switch(pPjSipCallback->GetCbType())
    {
        case JSIPUA_CB_TYPE_INCOMING_CALL:
        {
            m_pPjSip->ProcOnIncomingCall(pPjSipCallback, this);
        }
        break;

        case JSIPUA_CB_TYPE_CALL_MEDIA_STATE:
        {
            m_pPjSip->ProcOnCallMediaState(pPjSipCallback, this);
        }
        break;

        case JSIPUA_CB_TYPE_CALL_STATE:
        {
            m_pPjSip->ProcOnCallState(pPjSipCallback, this);
        }
        break;

        case JSIPUA_CB_TYPE_REG_STATE2:
        {
            m_pPjSip->ProcOnRegState2(pPjSipCallback, this);
        }
        break;

        case JSIPUA_CB_TYPE_INCOMING_DTMF:
        {
            m_pPjSip->ProcOnIncomingDtmf(pPjSipCallback, this);
        }
        break;

        default:
        break;
    };

	return JSUCCESS;
}

JUINT32 JPjSipUa::ProcSetCfgEvent(JPjSipUaCfg* pPjSipUaCfg)
{
    JUINT32 uiLen = 0;
    JBOOL bChanged = JFALSE;

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_SIPUA, "JPjSipUa::ProcSetCfgEvent");

    if (!pPjSipUaCfg)
    {
        JSingleton<JLog>::instance2() << set(JLOG_MOD_SIPUA, JLOG_ERROR_LEVEL) 
            << "JPjSipUa::ProcSetCfgEvent pAccount is NULL\n";
        return JFAILURE;
    }

    if (pPjSipUaCfg->GetNumber() != m_cfg.GetNumber())
    {
        m_cfg.GetNumber() = pPjSipUaCfg->GetNumber();
        bChanged = JTRUE;
    }

    if (pPjSipUaCfg->GetAuthName() != m_cfg.GetAuthName())
    {
        m_cfg.GetAuthName() = pPjSipUaCfg->GetAuthName();
        bChanged = JTRUE;
    }

    if (pPjSipUaCfg->GetAuthPasswd() != m_cfg.GetAuthPasswd())
    {
        m_cfg.GetAuthPasswd() = pPjSipUaCfg->GetAuthPasswd();
        bChanged = JTRUE;
    }

    if (pPjSipUaCfg->GetProxyAddr() != m_cfg.GetProxyAddr())
    {
        m_cfg.GetProxyAddr() = pPjSipUaCfg->GetProxyAddr();
        bChanged = JTRUE;
    }

    if (bChanged)
    {
        if (m_status>=JSIPUA_STATUS_OUTGOING)
        {
            m_pPjSip->HangUp();
        }

        if (m_status>=JSIPUA_STATUS_REGISTERED)
        {
            m_pPjSip->UnRegister();
        }

        StateChange(JSIPUA_STATUS_REGISTERING);

        m_pPjSip->Register(m_cfg.GetNumber(), m_cfg.GetAuthName(), 
            m_cfg.GetAuthPasswd(), m_cfg.GetProxyAddr());

        StateChange(JSIPUA_STATUS_REGISTERING);

        SetCfgList(&m_cfg);
    }

	return JSUCCESS;
}

JUINT32 JPjSipUa::ProcGetCfgEvent(JEvent* pEvent)
{
    JEvent* pNewEvent = JNULL;
    JPjSipUaCfg* pPjSipUaCfg = JNULL;
    JCommEngine* pComEngine = JNULL;
    JThread* pThread = JNULL;
    JModuleThread* pModuleThread = JNULL;

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_SIPUA, "JPjSipUa::ProcGetCfgEvent");

    pNewEvent = new JEvent(JEVT_SIPUA_GET_CFG_RSP);
    pPjSipUaCfg = new JPjSipUaCfg;
    if (pNewEvent && pPjSipUaCfg)
    {
    	pPjSipUaCfg->SetNumber(m_cfg.GetNumber());
    	pPjSipUaCfg->SetAuthName(m_cfg.GetAuthName());
    	pPjSipUaCfg->SetAuthPasswd(m_cfg.GetAuthPasswd());
    	pPjSipUaCfg->SetProxyAddr(m_cfg.GetProxyAddr());

	    pNewEvent->SetFromProc(pEvent->GetToProc().c_str());
	    pNewEvent->SetFromThrd(pEvent->GetToThrd().c_str());
	    pNewEvent->SetFromMod(pEvent->GetToMod().c_str());
	    pNewEvent->SetToProc(pEvent->GetFromProc().c_str());
	    pNewEvent->SetToThrd(pEvent->GetFromThrd().c_str());
	    pNewEvent->SetToMod(pEvent->GetFromMod().c_str());
	    pNewEvent->SetBody(pPjSipUaCfg);

        pThread = JSingleton<JThreadManager>::instance()->GetThread(JS_T_JMAINTHREAD);
        pModuleThread = dynamic_cast<JModuleThread*>(pThread);
        if (pModuleThread)
        {
    	    pComEngine = pModuleThread->GetNotifyCommEngine();
    	    if (pComEngine)
    	    {
    	        pComEngine->SendEvent(pNewEvent);
    	    }
	    }
    }

	return JSUCCESS;
}

JUINT32 JPjSipUa::ProcPressKeyEvent(JPjSipUaKey* pPjSipUaKey)
{
	JSIPUA_KEY eKey = JSIPUA_KEY_NONE;
    JUINT32 uiRet = 0;

	if (!pPjSipUaKey)
	{
        JSingleton<JLog>::instance2() << set(JLOG_MOD_SIPUA, JLOG_ERROR_LEVEL) 
            << "JPjSipUa::ProcPressKeyEvent pPjSipUaKey is NULL\n";
        return JFAILURE;
	}

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_SIPUA, "JPjSipUa::ProcPressKeyEvent");

	eKey = JPjSipUa::KeyChar2Enum(pPjSipUaKey->GetKey().c_str());

    switch(eKey)
    {
        case JSIPUA_KEY_0:
        case JSIPUA_KEY_1:
        case JSIPUA_KEY_2:
        case JSIPUA_KEY_3:
        case JSIPUA_KEY_4:
        case JSIPUA_KEY_5:
        case JSIPUA_KEY_6:
        case JSIPUA_KEY_7:
        case JSIPUA_KEY_8:
        case JSIPUA_KEY_9:
        case JSIPUA_KEY_STAR:
        case JSIPUA_KEY_POUND:
        {
            JCHAR cKey = JNULL;
		    JCHAR buf[JMAX_STRING_LEN] = {0};

            if (m_status == JSIPUA_STATUS_REGISTERED|| 
                m_status == JSIPUA_STATUS_COLLECT)
            {
                cKey = KeyEnum2Char(eKey);
                if (cKey != JNULL)
                {
                    SafeSprintf(buf, JMAX_STRING_LEN, "%c", cKey);
                    m_strCurrNumber += buf;
                }
                else
                {
                    JSingleton<JLog>::instance2() << set(JLOG_MOD_SIPUA, JLOG_ERROR_LEVEL) 
                        << "JPjSipUa::ProcPressKeyEvent KeyEnum2Char return failure\n";
                }

                StateChange(JSIPUA_STATUS_COLLECT);
            }
            else if(m_status == JSIPUA_STATUS_CONNECTED)
            {
                cKey = KeyEnum2Char(eKey);
                if (cKey != JNULL)
                {
                    uiRet = m_pPjSip->DialDtmf(cKey);
            		if (uiRet != JSUCCESS)
                    {
                        JSingleton<JLog>::instance2() << set(JLOG_MOD_SIPUA, JLOG_ERROR_LEVEL) 
                            << "JPjSipUa::ProcPressKeyEvent DialDtmf return failure, status="
                            << uiRet 
                            << "\n";
                		return JFAILURE;
                    }
                }
                else
                {
                    JSingleton<JLog>::instance2() << set(JLOG_MOD_SIPUA, JLOG_ERROR_LEVEL) 
                        << "JPjSipUa::ProcPressKeyEvent KeyEnum2Char return failure\n";
                }
    		}
    		else
            {
                JSingleton<JLog>::instance2() << set(JLOG_MOD_SIPUA, JLOG_ERROR_LEVEL) 
                    << "JPjSipUa::ProcPressKeyEvent call_id is NULL\n";
            }
        }
        break;

        case JSIPUA_KEY_OFFHOOK:
        {
            JUINT32 uiLen = 0;

            if (m_status == JSIPUA_STATUS_COLLECT)
            {
                uiLen = m_strCurrNumber.GetLength();
                if (uiLen != 0)
                {
                    uiRet = m_pPjSip->StartCall(m_strCurrNumber, m_cfg.GetProxyAddr());
                    if (uiRet != JSUCCESS)
                    {
                        JSingleton<JLog>::instance2() << set(JLOG_MOD_SIPUA, JLOG_ERROR_LEVEL) 
                            << "JPjSipUa::ProcPressKeyEvent StartCall return failure\n";
                    }

                    StoreContactNumber();
                }
            }
            else if (m_status == JSIPUA_STATUS_INCOMING)
            {
                uiRet = m_pPjSip->Answer(200);
        		if (uiRet != JSUCCESS)
                {
                    JSingleton<JLog>::instance2() << set(JLOG_MOD_SIPUA, JLOG_ERROR_LEVEL) 
                        << "JPjSipUa::ProcPressKeyEvent Answer return failure, status="
                        << uiRet 
                        << "\n";
            		return JFAILURE;
                }            
            }
            else if (m_status == JSIPUA_STATUS_CONNECTED)
            {
                uiRet = m_pPjSip->Hold();
        		if (uiRet != JSUCCESS)
                {
                    JSingleton<JLog>::instance2() << set(JLOG_MOD_SIPUA, JLOG_ERROR_LEVEL) 
                        << "JPjSipUa::ProcPressKeyEvent Hold return failure, status="
                        << uiRet 
                        << "\n";
            		return JFAILURE;
                }

                StateChange(JSIPUA_STATUS_HOLDING);
            }
            else if (m_status == JSIPUA_STATUS_HOLDING)
            {
                uiRet = m_pPjSip->UnHold();
        		if (uiRet != JSUCCESS)
                {
                    JSingleton<JLog>::instance2() << set(JLOG_MOD_SIPUA, JLOG_ERROR_LEVEL) 
                        << "JPjSipUa::ProcPressKeyEvent UnHold return failure, status="
                        << uiRet 
                        << "\n";
            		return JFAILURE;
                }

                StateChange(JSIPUA_STATUS_CONNECTED);
            }
        }
        break;

        case JSIPUA_KEY_ONHOOK:
        {
            if (m_status == JSIPUA_STATUS_COLLECT)
            {
                StateChange(JSIPUA_STATUS_REGISTERED);
            }
            else if (m_status == JSIPUA_STATUS_OUTGOING || 
                    m_status == JSIPUA_STATUS_INCOMING || 
                    m_status == JSIPUA_STATUS_CONNECTED || 
                    m_status == JSIPUA_STATUS_HOLDING)
            {
                m_pPjSip->HangUp();
            }
        }
        break;

        default:
        break;
    };

	return JSUCCESS;
}

JUINT32 JPjSipUa::ProcClickContactEvent(JPjSipUaClickContact* pPjSipUaClickContact)
{
    JUINT32 uiRet = JFAILURE;

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_SIPUA, "JPjSipUa::ProcClickContactEvent");

    if (m_status == JSIPUA_STATUS_REGISTERED|| 
        m_status == JSIPUA_STATUS_COLLECT)
    {
        m_strCurrNumber = pPjSipUaClickContact->GetContactNumber();

        uiRet = m_pPjSip->StartCall(m_strCurrNumber, m_cfg.GetProxyAddr());
        if (uiRet)
        {
            JSingleton<JLog>::instance2() << set(JLOG_MOD_SIPUA, JLOG_ERROR_LEVEL) 
                << "JPjSipUa::ProcClickContactEvent StartCall return failure\n";
        }

        StoreContactNumber();
    }

	return JSUCCESS;
}

JUINT32 JPjSipUa::ProcGetCallStatusEvent(JEvent* pEvent)
{
    JEvent* pNewEvent = JNULL;
    JPjSipUaCallStatus* pPjSipUaCallStatus = JNULL;
    JCommEngine* pComEngine = JNULL;
    JThread* pThread = JNULL;
    JModuleThread* pModuleThread = JNULL;

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_SIPUA, "JPjSipUa::ProcGetCallStatusEvent");

    pNewEvent = new JEvent(JEVT_SIPUA_GET_CALL_STATUS_RSP);
    pPjSipUaCallStatus = new JPjSipUaCallStatus;
    if (pNewEvent && pPjSipUaCallStatus)
    {
    	pPjSipUaCallStatus->SetNumber(m_strCurrNumber);
    	JString strStatus = StatusEnum2Char(m_status);
    	pPjSipUaCallStatus->SetStatus(strStatus);

	    pNewEvent->SetFromProc(pEvent->GetToProc().c_str());
	    pNewEvent->SetFromThrd(pEvent->GetToThrd().c_str());
	    pNewEvent->SetFromMod(pEvent->GetToMod().c_str());
	    pNewEvent->SetToProc(pEvent->GetFromProc().c_str());
	    pNewEvent->SetToThrd(pEvent->GetFromThrd().c_str());
	    pNewEvent->SetToMod(pEvent->GetFromMod().c_str());
	    pNewEvent->SetBody(pPjSipUaCallStatus);

        pThread = JSingleton<JThreadManager>::instance()->GetThread(JS_T_JMAINTHREAD);
        pModuleThread = dynamic_cast<JModuleThread*>(pThread);
        if (pModuleThread)
        {
    	    pComEngine = pModuleThread->GetNotifyCommEngine();
    	    if (pComEngine)
    	    {
    	        pComEngine->SendEvent(pNewEvent);
    	    }
	    }
    }

	return JSUCCESS;
}

JUINT32 JPjSipUa::ProcGetContactListEvent(JEvent* pEvent)
{
    JEvent* pNewEvent = JNULL;
    JPjSipUaContactList* pJPjSipUaContactList = JNULL;
    JCommEngine* pComEngine = JNULL;
    JUINT32 uiIdx = 0;
    JListItem<JCHAR>* pItem = JNULL;
    JCHAR* pData = JNULL;
    JUINT32 uiContactListNumber = m_contactListNumber;
    JThread* pThread = JNULL;
    JModuleThread* pModuleThread = JNULL;

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_SIPUA, "JPjSipUa::ProcGetContactListEvent");

    pNewEvent = new JEvent(JEVT_SIPUA_GET_CONTACT_LIST_RSP);
    pJPjSipUaContactList = new JPjSipUaContactList;
    if (pNewEvent && pJPjSipUaContactList)
    {
        JListIterator<JCHAR> clsListIter(m_contactList);
        for (clsListIter.First(); 
            clsListIter.Done() && uiContactListNumber && uiIdx<JPJSIP_MAX_NUMBER; 
            clsListIter.Next())
        {
            pItem = clsListIter.Item();

            pData = pItem->GetData();
            if (pData)
            {
    			JString strNumber = pData;
            	pJPjSipUaContactList->SetContactNumber(uiIdx, strNumber);
            }

            uiIdx++;
            uiContactListNumber--;
        }

	    pNewEvent->SetFromProc(pEvent->GetToProc().c_str());
	    pNewEvent->SetFromThrd(pEvent->GetToThrd().c_str());
	    pNewEvent->SetFromMod(pEvent->GetToMod().c_str());
	    pNewEvent->SetToProc(pEvent->GetFromProc().c_str());
	    pNewEvent->SetToThrd(pEvent->GetFromThrd().c_str());
	    pNewEvent->SetToMod(pEvent->GetFromMod().c_str());
	    pNewEvent->SetBody(pJPjSipUaContactList);

        pThread = JSingleton<JThreadManager>::instance()->GetThread(JS_T_JMAINTHREAD);
        pModuleThread = dynamic_cast<JModuleThread*>(pThread);
        if (pModuleThread)
        {
    	    pComEngine = pModuleThread->GetNotifyCommEngine();
    	    if (pComEngine)
    	    {
    	        pComEngine->SendEvent(pNewEvent);
    	    }
	    }
    }

	return JSUCCESS;
}

JCHAR* JPjSipUa::StatusEnum2Char(JSIPUA_STATUS eStatus)
{
    JUINT32 uiSize = sizeof(gStatusMap)/sizeof(JSIPUA_STATUS_MAP);
    JUINT32 uiIdx = 0;

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_SIPUA, "JPjSipUa::StatusEnum2Char");

    if (eStatus<JSIPUA_STATUS_IDLE || eStatus>JSIPUA_STATUS_HOLDING)
    {
        JSingleton<JLog>::instance2() << set(JLOG_MOD_SIPUA, JLOG_ERROR_LEVEL) 
            << "JPjSipUa::StatusEnum2Char input param is invalid\n";
        return JNULL;
    }

    for (uiIdx=0; uiIdx<uiSize; uiIdx++)
    {
        if (gStatusMap[uiIdx].eStatus == eStatus)
        {
            return gStatusMap[uiIdx].pStatus;
        }
    }

	return JNULL;
}

JUINT32 JPjSipUa::SetContactList(JList<JCHAR>& rSrcList)
{
    JUINT32 uiLen = 0;
    JUINT32 uiNum = 0;
    JList<JPER_RECORD> clsDstList;
    JListItem<JCHAR>* pSrcItem = JNULL;
    JListItem<JPER_RECORD>* pDstItem = JNULL;
    JListItem<JPER_RECORD>* prevDstItem = JNULL;
    JCHAR* pSrcData = JNULL;
    JPER_RECORD* pDstData = JNULL;

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_SIPUA, "JPjSipUa::SetContactList");

    if (m_pSerialization)
    {
        JListIterator<JCHAR> clsListIter(rSrcList);
        for (clsListIter.First(); clsListIter.Done(); clsListIter.Next())
        {
            pSrcItem = clsListIter.Item();

            uiNum++;
            pDstData = JNULL;

            pSrcData = pSrcItem->GetData();
            uiLen = SafeStrlen(pSrcData);
            if (uiLen)
            {
                pDstData = 
                    reinterpret_cast<JPER_RECORD*>(JSingleton<JStaticMemory>::instance()->Alloc(sizeof(JPER_RECORD)+1));
                if (pDstData)
                {
	            	SafeMemset(reinterpret_cast<JCHAR*>(pDstData), 0, sizeof(JPER_RECORD)+1);
                    SafeSprintf(pDstData->strKey, JMAX_STRING_LEN, "%d", uiNum);
                    SafeStrcpy(pDstData->strValue, pSrcData, uiLen+1);
                }
                else
                {
                    JSingleton<JLog>::instance2() << set(JLOG_MOD_SIPUA, JLOG_ERROR_LEVEL) << "JPjSipUa::SetContactList memory alloc failure\n";
                    return JFAILURE;
                }
            }

            pDstItem = new JListItem<JPER_RECORD>(pDstData);
            pDstItem->SetDataLength(sizeof(JPER_RECORD));
            clsDstList.InsertItem(pDstItem, prevDstItem);

            prevDstItem = pDstItem;
        }

        m_pSerialization->SetList(clsDstList, JSIPUA_CONTACT_FILE);
    }

	return JSUCCESS;
}

JUINT32 JPjSipUa::GetContactList(JList<JCHAR>& rDstList)
{
    JListItem<JCHAR>* pItem = JNULL;
    JListItem<JCHAR>* prevItem = JNULL;
    JUINT32 uiLen = 0;
    JList<JPER_RECORD> clsSrcList;
    JListItem<JPER_RECORD>* pSrcItem = JNULL;
    JListItem<JCHAR>* pDstItem = JNULL;
    JListItem<JCHAR>* prevDstItem = JNULL;
    JPER_RECORD* pSrcData = JNULL;
    JCHAR* pDstData = JNULL;

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_SIPUA, "JPjSipUa::GetContactList");

    rDstList.RemoveAllItem();

    if (m_pSerialization)
    {
        clsSrcList = m_pSerialization->GetList(JSIPUA_CONTACT_FILE);

        JListIterator<JPER_RECORD> clsListIter(clsSrcList);
        for (clsListIter.First(); clsListIter.Done(); clsListIter.Next())
        {
            pSrcItem = clsListIter.Item();

            pDstData = JNULL;

            pSrcData = pSrcItem->GetData();
            uiLen = SafeStrlen(pSrcData->strValue);
            if (uiLen)
            {
                pDstData = JSingleton<JStaticMemory>::instance()->Alloc(uiLen+1);
                if (pDstData)
                {
	            	SafeMemset(pDstData, 0, uiLen+1);
                    SafeStrcpy(pDstData, pSrcData->strValue, uiLen+1);
                }
                else
                {
                    JSingleton<JLog>::instance2() << set(JLOG_MOD_SIPUA, JLOG_ERROR_LEVEL) 
                        << "JPjSipUa::GetContactList memory alloc failure\n";
                    return JFAILURE;
                }
            }

            pDstItem = new JListItem<JCHAR>(pDstData);
            rDstList.InsertItem(pDstItem, prevDstItem);

            prevDstItem = pDstItem;
        }
    }

	return JSUCCESS;
}

JUINT32 JPjSipUa::SetCfgList(JPjSipUaCfg* pPjSipUaCfg)
{
    JUINT32 uiLen = 0;
    JList<JPER_RECORD> clsDstList;
    JListItem<JPER_RECORD>* pDstItem = JNULL;
    JListItem<JPER_RECORD>* prevDstItem = JNULL;
    JPER_RECORD* pDstData = JNULL;

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_SIPUA, "JPjSipUa::SetCfgList");

    if (m_pSerialization)
    {
        uiLen = pPjSipUaCfg->GetNumber().GetLength();
        if (uiLen)
        {
            pDstData = 
                reinterpret_cast<JPER_RECORD*>(JSingleton<JStaticMemory>::instance()->Alloc(sizeof(JPER_RECORD)+1));
            if (pDstData)
            {
	            SafeMemset(reinterpret_cast<JCHAR*>(pDstData), 0, sizeof(JPER_RECORD)+1);
                SafeSprintf(pDstData->strKey, JMAX_STRING_LEN, "%s", JSIPUA_NUMBER);
                SafeStrcpy(pDstData->strValue, pPjSipUaCfg->GetNumber().c_str(), uiLen+1);
                pDstItem = new JListItem<JPER_RECORD>(pDstData);
                pDstItem->SetDataLength(sizeof(JPER_RECORD));
                clsDstList.InsertItem(pDstItem, prevDstItem);
                prevDstItem = pDstItem;
            }
            else
            {
                JSingleton<JLog>::instance2() << set(JLOG_MOD_SIPUA, JLOG_ERROR_LEVEL) << "JPjSipUa::SetCfgList memory alloc failure\n";
                return JFAILURE;
            }
        }

        uiLen = pPjSipUaCfg->GetAuthName().GetLength();
        if (uiLen)
        {
            pDstData = 
                reinterpret_cast<JPER_RECORD*>(JSingleton<JStaticMemory>::instance()->Alloc(sizeof(JPER_RECORD)+1));
            if (pDstData)
            {
	            SafeMemset(reinterpret_cast<JCHAR*>(pDstData), 0, sizeof(JPER_RECORD)+1);
                SafeSprintf(pDstData->strKey, JMAX_STRING_LEN, "%s", JSIPUA_AUTH_NAME);
                SafeStrcpy(pDstData->strValue, pPjSipUaCfg->GetAuthName().c_str(), uiLen+1);
                pDstItem = new JListItem<JPER_RECORD>(pDstData);
                pDstItem->SetDataLength(sizeof(JPER_RECORD));
                clsDstList.InsertItem(pDstItem, prevDstItem);
                prevDstItem = pDstItem;
            }
            else
            {
                JSingleton<JLog>::instance2() << set(JLOG_MOD_SIPUA, JLOG_ERROR_LEVEL) << "JPjSipUa::SetCfgList memory alloc failure\n";
                return JFAILURE;
            }
        }

        uiLen = pPjSipUaCfg->GetAuthPasswd().GetLength();
        if (uiLen)
        {
            pDstData = 
                reinterpret_cast<JPER_RECORD*>(JSingleton<JStaticMemory>::instance()->Alloc(sizeof(JPER_RECORD)+1));
            if (pDstData)
            {
	            SafeMemset(reinterpret_cast<JCHAR*>(pDstData), 0, sizeof(JPER_RECORD)+1);
                SafeSprintf(pDstData->strKey, JMAX_STRING_LEN, "%s", JSIPUA_AUTH_PASSWORD);
                SafeStrcpy(pDstData->strValue, pPjSipUaCfg->GetAuthPasswd().c_str(), uiLen+1);
                pDstItem = new JListItem<JPER_RECORD>(pDstData);
                pDstItem->SetDataLength(sizeof(JPER_RECORD));
                clsDstList.InsertItem(pDstItem, prevDstItem);
                prevDstItem = pDstItem;
            }
            else
            {
                JSingleton<JLog>::instance2() << set(JLOG_MOD_SIPUA, JLOG_ERROR_LEVEL) << "JPjSipUa::SetCfgList memory alloc failure\n";
                return JFAILURE;
            }
        }

        uiLen = pPjSipUaCfg->GetProxyAddr().GetLength();
        if (uiLen)
        {
            pDstData = 
                reinterpret_cast<JPER_RECORD*>(JSingleton<JStaticMemory>::instance()->Alloc(sizeof(JPER_RECORD)+1));
            if (pDstData)
            {
	            SafeMemset(reinterpret_cast<JCHAR*>(pDstData), 0, sizeof(JPER_RECORD)+1);
                SafeSprintf(pDstData->strKey, JMAX_STRING_LEN, "%s", JSIPUA_PROXY_ADDR);
                SafeStrcpy(pDstData->strValue, pPjSipUaCfg->GetProxyAddr().c_str(), uiLen+1);
                pDstItem = new JListItem<JPER_RECORD>(pDstData);
                pDstItem->SetDataLength(sizeof(JPER_RECORD));
                clsDstList.InsertItem(pDstItem, prevDstItem);
                prevDstItem = pDstItem;
            }
            else
            {
                JSingleton<JLog>::instance2() << set(JLOG_MOD_SIPUA, JLOG_ERROR_LEVEL) << "JPjSipUa::SetCfgList memory alloc failure\n";
                return JFAILURE;
            }
        }

        m_pSerialization->SetList(clsDstList, JSIPUA_CFG_FILE);
    }

	return JSUCCESS;
}

JUINT32 JPjSipUa::GetCfgList(JPjSipUaCfg* pPjSipUaCfg)
{
    JUINT32 uiLen = 0;
    JUINT32 uiLen2 = 0;
    JList<JPER_RECORD> clsSrcList;
    JListItem<JPER_RECORD>* pSrcItem = JNULL;
    JPER_RECORD* pSrcData = JNULL;

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_SIPUA, "JPjSipUa::GetCfgList");

    if (m_pSerialization)
    {
        clsSrcList = m_pSerialization->GetList(JSIPUA_CFG_FILE);

        JListIterator<JPER_RECORD> clsListIter(clsSrcList);
        for (clsListIter.First(); clsListIter.Done(); clsListIter.Next())
        {
            pSrcItem = clsListIter.Item();

            pSrcData = pSrcItem->GetData();

            uiLen = SafeStrlen(pSrcData->strKey);
            uiLen2 = SafeStrlen(pSrcData->strValue);
            if (uiLen && uiLen2)
            {
                if (SafeStrcmp(pSrcData->strKey, JSIPUA_NUMBER) == 0)
                {
                    pPjSipUaCfg->GetNumber() = pSrcData->strValue;
                }
                else if (SafeStrcmp(pSrcData->strKey, JSIPUA_AUTH_NAME) == 0)
                {
                    pPjSipUaCfg->GetAuthName() = pSrcData->strValue;
                }
                else if (SafeStrcmp(pSrcData->strKey, JSIPUA_AUTH_PASSWORD) == 0)
                {
                    pPjSipUaCfg->GetAuthPasswd() = pSrcData->strValue;
                }
                else if (SafeStrcmp(pSrcData->strKey, JSIPUA_PROXY_ADDR) == 0)
                {
                    pPjSipUaCfg->GetProxyAddr() = pSrcData->strValue;
                }
            }
        }
    }

	return JSUCCESS;
}

