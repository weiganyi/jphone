/**********************************************************
* Author: weiganyi
* File: JSerialization:.cpp
* Date: 20120504
* Description: 
*
**********************************************************/

#include "JFrameWork.h"


namespace JFrameWork{

JPersistence::JPersistence()
{
	return;
}

JPersistence::~JPersistence()
{
	return;
}

JIni::JIni()
{
	return;
}

JIni::~JIni()
{
	return;
}

JUINT32 JIni::SetList(JList<JPER_RECORD> &clsList, JCHAR* pName)
{
    JListItem<JPER_RECORD>* pItem = JNULL;
    JPER_RECORD* pData = JNULL;
    JCHAR strFileName[JMAX_STRING_LEN] = {0};
    FILE* hFileHandler = JNULL;

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_SERIALIZATION, "JIni::SetList");

    if (!pName)
    {
        JSingleton<JLog>::instance2() << set(JLOG_MOD_SERIALIZATION, JLOG_ERROR_LEVEL) 
            << "JIni::SetList input param is NULL\n";
        return JFAILURE;
    }

    SafeSprintf(strFileName, JMAX_STRING_LEN, "%s.ini", pName);
    hFileHandler = SafeFopen(strFileName, "w+");
    if (!hFileHandler)
    {
        JSingleton<JLog>::instance2() << set(JLOG_MOD_SERIALIZATION, JLOG_ERROR_LEVEL) 
            << "JIni::SetList fopen file "
            << pName
            << "failure\n";
        return JFAILURE;
    }

    if (clsList.GetItemNum() == 0)
    {
        return JSUCCESS;
    }

    JListIterator<JPER_RECORD> clsListIter(clsList);
    for (clsListIter.Last(); clsListIter.Done(); clsListIter.Prev())
    {
        pItem = clsListIter.Item();

        pData = pItem->GetData();
        if (pData)
        {
            SafeFprintf(hFileHandler, "%s=%s\n", pData->strKey, pData->strValue);
            SafeFflush(hFileHandler);
        }
    }

    SafeFclose(hFileHandler);

    return JSUCCESS;
}

JList<JPER_RECORD> JIni::GetList(JCHAR* pName)
{
    JUINT32 uiLen = 0;
    JUINT32 uiLen2 = 0;
    JList<JPER_RECORD> clsList;
    JListItem<JPER_RECORD>* prevItem = JNULL;
    JListItem<JPER_RECORD>* pItem = JNULL;
    JPER_RECORD* pData = JNULL;
    JCHAR strKey[JMAX_STRING_LEN] = {0};
    JCHAR strValue[JMAX_STRING_LEN] = {0};
    JCHAR strFileName[JMAX_STRING_LEN] = {0};
    FILE* hFileHandler = JNULL;
    JCHAR strLine[JMAX_STRING_LEN] = {0};
    JCHAR* pOffset = JNULL;
    JCHAR* pOffset2 = JNULL;
    JCHAR cEqualChar = '=';
    JCHAR cCRChar = '\n';

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_SERIALIZATION, "JIni::GetList");

    if (!pName)
    {
        JSingleton<JLog>::instance2() << set(JLOG_MOD_SERIALIZATION, JLOG_ERROR_LEVEL) 
            << "JIni::GetList input param is NULL\n";
        return clsList;
    }

    SafeSprintf(strFileName, JMAX_STRING_LEN, "%s.ini", pName);
    hFileHandler = SafeFopen(strFileName, "r");
    if (!hFileHandler)
    {
        JSingleton<JLog>::instance2() << set(JLOG_MOD_SERIALIZATION, JLOG_ERROR_LEVEL) 
            << "JIni::GetList fopen file "
            << pName
            << "failure\n";
        return clsList;
    }

    while(SafeFgets(strLine, JMAX_STRING_LEN, hFileHandler) != JNULL)
    {
        if ((pOffset = SafeStrchr(strLine, cEqualChar)) != 0)
        {
            SafeStrncpy(strKey, strLine, pOffset-strLine, JMAX_STRING_LEN);
            if ((pOffset2 = SafeStrchr(strLine, cCRChar)) != 0)
            {
                uiLen = pOffset2-strLine;
            }
            else
            {
                uiLen = SafeStrlen(strLine);
            }
            uiLen2 = pOffset-strLine;
            SafeStrncpy(strValue, pOffset+1, uiLen-(uiLen2+1), JMAX_STRING_LEN);

            uiLen = SafeStrlen(strKey);
            uiLen2 = SafeStrlen(strValue);
            if (uiLen && uiLen2)
            {
                prevItem = pItem;

                uiLen = sizeof(JPER_RECORD);
                pData = 
                    reinterpret_cast<JPER_RECORD*>(JSingleton<JStaticMemory>::instance()->Alloc(uiLen+1));
                if (pData)
                {
		            SafeMemset(reinterpret_cast<JCHAR*>(pData), 0, uiLen+1);

	                if (SafeStrlen(strKey) < JMAX_STRING_LEN)
	                {
	                    SafeStrcpy(pData->strKey, strKey, JMAX_STRING_LEN);
	                }
	                if (SafeStrlen(strValue) < JMAX_STRING_LEN)
	                {
	                    SafeStrcpy(pData->strValue, strValue, JMAX_STRING_LEN);
	                }

	                pItem = new JListItem<JPER_RECORD>(pData);
	                pItem->SetDataLength(uiLen);
	                clsList.InsertItem(pItem, prevItem);
                }
            }
        }
    };

    SafeFclose(hFileHandler);

    return clsList;
}

JMySql::JMySql(JCHAR* pServer, 
            JCHAR* pUser, 
            JCHAR* pPassword, 
            JUINT32 uiPort, 
            JCHAR* pDataBase)
{
    JUINT32 uiRet = JSUCCESS;
    JUINT32 uiLen = 0;
    JCHAR errInfo[JMAX_BUFFER_LEN] = {0};

    m_uiPort = 0;

    uiLen = SafeStrlen(pServer);
    if (uiLen)
    {
        m_strServer = pServer;
    }

    uiLen = SafeStrlen(pUser);
    if (uiLen)
    {
        m_strUser = pUser;
    }

    uiLen = SafeStrlen(pPassword);
    if (uiLen)
    {
        m_strPassword = pPassword;
    }

    m_uiPort = uiPort;

    uiLen = SafeStrlen(pDataBase);
    if (uiLen)
    {
        m_strDataBase = pDataBase;
    }

    uiRet = OpenDataBase(m_strServer.c_str(), m_strUser.c_str(), m_strPassword.c_str(), 
        m_uiPort, m_strDataBase.c_str());
    if (uiRet != JSUCCESS)
    {
        SafeSprintf(errInfo, JMAX_BUFFER_LEN, 
            "JMySql::JMySql OpenDataBase failure\n");
        throw JException(errInfo);
    }

	return;
}

JMySql::~JMySql()
{
	return;
}

JUINT32 JMySql::SetList(JList<JPER_RECORD> &clsList, JCHAR* pName)
{
    JCHAR sqlBuf[JMAX_BUFFER_LEN] = {0};
    JINT32 state = 0;
    MYSQL mysql;
    MYSQL* pHandler;
    MYSQL_RES* result = JNULL;
    JListItem<JPER_RECORD>* pItem = JNULL;
    JPER_RECORD* pData = JNULL;
    JCHAR errInfo[JMAX_BUFFER_LEN] = {0};
    JCHAR strTableName[JMAX_STRING_LEN] = {0};

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_SERIALIZATION, "JMySql::SetList");

    if (!pName)
    {
        JSingleton<JLog>::instance2() << set(JLOG_MOD_SERIALIZATION, JLOG_ERROR_LEVEL) 
            << "JMySql::SetList input param is NULL\n";
        return JFAILURE;
    }

    if (clsList.GetItemNum() == 0)
    {
        return JSUCCESS;
    }

    try
    {
        SafeSprintf(strTableName, JMAX_STRING_LEN, "t_%s", pName);

        mysql_init(&mysql);
        pHandler = mysql_real_connect(&mysql, m_strServer.c_str(), m_strUser.c_str(), 
            m_strPassword.c_str(), m_strDataBase.c_str(), m_uiPort, 0, 0);
        if (!pHandler)
        {
            SafeSprintf(errInfo, JMAX_BUFFER_LEN, 
                "mysql_real_connect return err:%s\n", mysql_error(pHandler));
            throw JException(errInfo);
        }

        SafeSprintf(sqlBuf, JMAX_BUFFER_LEN, "use %s", m_strDataBase.c_str());
        state = mysql_query(pHandler, sqlBuf);
        if (state != 0)
        {
            SafeSprintf(errInfo, JMAX_BUFFER_LEN, 
                "mysql_query return err:%s, sqlBuf:%s", mysql_error(pHandler), sqlBuf);
            throw JException(errInfo);
        }

        SafeSprintf(sqlBuf, JMAX_BUFFER_LEN, "select * from %s", strTableName);
        state = mysql_query(pHandler, sqlBuf);
        if (state != 0)
        {
            SafeSprintf(sqlBuf, JMAX_BUFFER_LEN, 
                "create table %s (id char(64) not null primary key, number char(64) not null)", 
                strTableName);
            state = mysql_query(pHandler, sqlBuf);
            if (state != 0)
            {
                SafeSprintf(errInfo, JMAX_BUFFER_LEN, 
                    "mysql_query return err:%s, sqlBuf:%s", mysql_error(pHandler), sqlBuf);
                throw JException(errInfo);
            }
        }
        else
        {
            result = mysql_store_result(pHandler);
            if (result)
            {
                mysql_free_result(result);
                result = JNULL;
            }
        }

        SafeSprintf(sqlBuf, JMAX_BUFFER_LEN, "delete from %s", strTableName);
        state = mysql_query(pHandler, sqlBuf);
        if (state != 0)
        {
            SafeSprintf(errInfo, JMAX_BUFFER_LEN, 
                "mysql_query return err:%s, sqlBuf:%s", mysql_error(pHandler), sqlBuf);
            throw JException(errInfo);
        }

        JListIterator<JPER_RECORD> clsListIter(clsList);
        for (clsListIter.First(); clsListIter.Done(); clsListIter.Next())
        {
            pItem = clsListIter.Item();

            pData = pItem->GetData();
            if (pData)
            {
                SafeSprintf(sqlBuf, JMAX_BUFFER_LEN, 
                    "insert into %s values(\"%s\", \"%s\")", strTableName, pData->strKey, pData->strValue);
                state = mysql_query(pHandler, sqlBuf);
                if (state != 0)
                {
                    SafeSprintf(errInfo, JMAX_BUFFER_LEN, 
                        "mysql_query return err:%s, sqlBuf:%s", mysql_error(pHandler), sqlBuf);
                    throw JException(errInfo);
                }
            }
        }
    }
    catch(JException &e)
    {
        JCHAR* pStr = JNULL;

        pStr = e.GetErrInfo();
        if (pStr)
        {
            JSingleton<JLog>::instance2() << set(JLOG_MOD_SERIALIZATION, JLOG_ERROR_LEVEL) 
                << "JMySql::SetList " 
                << pStr;
        }
        else
        {
            JSingleton<JLog>::instance2() << set(JLOG_MOD_SERIALIZATION, JLOG_ERROR_LEVEL) 
                << "JMySql::SetList return failue\n";
        }

        if (result)
        {
            mysql_free_result(result);
        }
        mysql_close(pHandler);

        return JFAILURE;
    }

    if (result)
    {
        mysql_free_result(result);
    }
    mysql_close(pHandler);

    return JSUCCESS;
}

JList<JPER_RECORD> JMySql::GetList(JCHAR* pName)
{
    JCHAR sqlBuf[JMAX_BUFFER_LEN] = {0};
    JINT32 state = 0;
    MYSQL mysql;
    MYSQL* pHandler;
    MYSQL_RES* result = JNULL;
    MYSQL_ROW row;
    JList<JPER_RECORD> clsList;
    JListItem<JPER_RECORD>* pItem = JNULL;
    JListItem<JPER_RECORD>* prevItem = JNULL;
    JPER_RECORD* pData = JNULL;
    JUINT32 uiLen = 0;
    JCHAR errInfo[JMAX_BUFFER_LEN] = {0};
    JCHAR strTableName[JMAX_STRING_LEN] = {0};

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_SERIALIZATION, "JMySql::GetList");

    if (!pName)
    {
        JSingleton<JLog>::instance2() << set(JLOG_MOD_SERIALIZATION, JLOG_ERROR_LEVEL) 
            << "JMySql::GetList input param is NULL\n";
        return clsList;
    }

    try
    {
        SafeSprintf(strTableName, JMAX_STRING_LEN, "t_%s", pName);

        mysql_init(&mysql);
        pHandler = mysql_real_connect(&mysql, m_strServer.c_str(), m_strUser.c_str(), 
            m_strPassword.c_str(), m_strDataBase.c_str(), m_uiPort, 0, 0);
        if (!pHandler)
        {
            SafeSprintf(errInfo, JMAX_BUFFER_LEN, 
                "mysql_real_connect return err:%s", mysql_error(pHandler));
            throw JException(errInfo);
        }

        SafeSprintf(sqlBuf, JMAX_BUFFER_LEN, "use %s", m_strDataBase.c_str());
        state = mysql_query(pHandler, sqlBuf);
        if (state != 0)
        {
            SafeSprintf(errInfo, JMAX_BUFFER_LEN, 
                "mysql_query return err:%s, sqlBuf:%s", mysql_error(pHandler), sqlBuf);
            throw JException(errInfo);
        }

        SafeSprintf(sqlBuf, JMAX_BUFFER_LEN, "select * from %s", strTableName);
        state = mysql_query(pHandler, sqlBuf);
        if (state != 0)
        {
            SafeSprintf(errInfo, JMAX_BUFFER_LEN, 
                "mysql_query return err:%s, sqlBuf:%s", mysql_error(pHandler), sqlBuf);
            throw JException(errInfo);
        }
        result = mysql_store_result(pHandler);
        while ((row = mysql_fetch_row(result)) != JNULL)
        {
            prevItem = pItem;

            uiLen = sizeof(JPER_RECORD);
            pData = 
                reinterpret_cast<JPER_RECORD*>(JSingleton<JStaticMemory>::instance()->Alloc(uiLen+1));
            if (pData)
            {
	            SafeMemset(reinterpret_cast<JCHAR*>(pData), 0, uiLen+1);

	            if (SafeStrlen(row[0]) < JMAX_STRING_LEN)
	            {
	                SafeStrcpy(pData->strKey, row[0], JMAX_STRING_LEN);
	            }
	            if (SafeStrlen(row[1]) < JMAX_STRING_LEN)
	            {
	                SafeStrcpy(pData->strValue, row[1], JMAX_STRING_LEN);
	            }
	            pItem = new JListItem<JPER_RECORD>(pData);
	            pItem->SetDataLength(uiLen);
	            clsList.InsertItem(pItem, prevItem);
            }
        }
    }
    catch(JException &e)
    {
        JCHAR* pStr = JNULL;

        pStr = e.GetErrInfo();
        if (pStr)
        {
            JSingleton<JLog>::instance2() << set(JLOG_MOD_SERIALIZATION, JLOG_ERROR_LEVEL) 
                << "JMySql::GetList " 
                << pStr;
        }
        else
        {
            JSingleton<JLog>::instance2() << set(JLOG_MOD_SERIALIZATION, JLOG_ERROR_LEVEL) 
                << "JMySql::GetList return failue\n";
        }

        if (result)
        {
            mysql_free_result(result);
        }
        mysql_close(pHandler);

        return clsList;
    }

    if (result)
    {
        mysql_free_result(result);
    }
    mysql_close(pHandler);

    return clsList;
}

JUINT32 JMySql::OpenDataBase(JCHAR* pServer, 
                            JCHAR* pUser, 
                            JCHAR* pPassword, 
                            JUINT32 uiPort, 
                            JCHAR* pDataBase)
{
    MYSQL mysql;
    MYSQL* pHandler;
    JINT32 state = 0;
    JCHAR sqlBuf[JMAX_BUFFER_LEN] = {0};
    JCHAR errInfo[JMAX_BUFFER_LEN] = {0};

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_SERIALIZATION, "JMySql::OpenDataBase");

    if (!pServer || !pDataBase)
    {
        JSingleton<JLog>::instance2() << set(JLOG_MOD_SERIALIZATION, JLOG_ERROR_LEVEL) 
            << "JMySql::OpenDataBase input param is NULL\n";
        return JFAILURE;
    }

    try
    {
        mysql_init(&mysql);
        pHandler = mysql_real_connect(&mysql, pServer, pUser, pPassword, pDataBase, uiPort, 0, 0);
        if (!pHandler)
        {
            /* if want to create a new database, first we need connect to another database, 
                then create that new database by this connected database */
            mysql_init(&mysql);
            pHandler = mysql_real_connect(&mysql, pServer, pUser, pPassword, JSQL_DEFAULT_DB, uiPort, 0, 0);
            if (!pHandler)
            {
                SafeSprintf(errInfo, JMAX_BUFFER_LEN, 
                    "mysql_real_connect return err:%s", mysql_error(&mysql));
                throw JException(errInfo);
            }
            else
            {
                SafeSprintf(sqlBuf, JMAX_BUFFER_LEN, "create database %s", pDataBase);
                state = mysql_query(pHandler, sqlBuf);
                if (state != 0)
                {
                    SafeSprintf(errInfo, JMAX_BUFFER_LEN, 
                        "mysql_query return err:%s, sqlBuf:%s", mysql_error(pHandler), sqlBuf);
                    throw JException(errInfo);
                }
            }
        }
    }
    catch(JException &e)
    {
        JCHAR* pStr = JNULL;

        pStr = e.GetErrInfo();
        if (pStr)
        {
            JSingleton<JLog>::instance2() << set(JLOG_MOD_SERIALIZATION, JLOG_ERROR_LEVEL) 
                << "JMySql::OpenDataBase " 
                << pStr
                << "\n";
        }
        else
        {
            JSingleton<JLog>::instance2() << set(JLOG_MOD_SERIALIZATION, JLOG_ERROR_LEVEL) 
                << "JMySql::OpenDataBase return failue\n";
        }

        if (pHandler)
            mysql_close(pHandler);

        return JFAILURE;
    }

    if (pHandler)
        mysql_close(pHandler);

    return JSUCCESS;
}

mxml_type_t vtp_type_cb(vtp_tree_node* p_node)
{
    const JCHAR* p_type = NULL;			/* Type string */

    if ((p_type = mxmlElementGetAttr(p_node, "type")) == NULL)
        return (MXML_OPAQUE);

    if (!strcmp(p_type, "integer"))
        return (MXML_INTEGER);
    else if (!strcmp(p_type, "opaque") || !strcmp(p_type, "pre"))
        return (MXML_OPAQUE);
    else if (!strcmp(p_type, "real"))
        return (MXML_REAL);
    else if (!strcmp(p_type, "text"))
    	return (MXML_TEXT);
    else
    	return (MXML_OPAQUE);
}

const JCHAR* vtp_whitespace_cb(vtp_tree_node* p_node, JINT32 where)
{
    vtp_tree_node	*parent = NULL;		/* Parent node */
    JINT32		level = 0;			/* Indentation level */
    JCHAR* p_name = NULL;			/* Name of element */
    static JCHAR *tabs = "\t\t\t\t\t\t\t\t";    /* Tabs for indentation */

    p_name = p_node->value.element.name;

    if (!strcmp(p_name, "html") || !strcmp(p_name, "head") || !strcmp(p_name, "body") ||
        !strcmp(p_name, "pre") || !strcmp(p_name, "p") ||
        !strcmp(p_name, "h1") || !strcmp(p_name, "h2") || !strcmp(p_name, "h3") ||
        !strcmp(p_name, "h4") || !strcmp(p_name, "h5") || !strcmp(p_name, "h6"))
    {
        if (where == MXML_WS_BEFORE_OPEN || where == MXML_WS_AFTER_CLOSE)
            return ("\n");
    }
    else if (!strcmp(p_name, "dl") || !strcmp(p_name, "ol") || !strcmp(p_name, "ul"))
    {
        return ("\n");
    }
    else if (!strcmp(p_name, "dd") || !strcmp(p_name, "dt") || !strcmp(p_name, "li"))
    {
        if (where == MXML_WS_BEFORE_OPEN)
            return ("\t");
        else if (where == MXML_WS_AFTER_CLOSE)
            return ("\n");
    }
    else if (!strncmp(p_name, "?xml", 4))
    {
        if (where == MXML_WS_AFTER_OPEN)
            return ("\n");
        else
            return (NULL);
    }
    else if (where == MXML_WS_BEFORE_OPEN &&
             ((!strcmp(p_name, "choice") || !strcmp(p_name, "option")) &&
     	    where == MXML_WS_BEFORE_CLOSE))
    {
        for (level = -1, parent = p_node->parent;
            parent;
        level ++, parent = parent->parent);
  
        if (level > 8)
            level = 8;
        else if (level < 0)
            level = 0;
  
        return (tabs + 8 - level);
    }
    else if (where == MXML_WS_AFTER_CLOSE &&
             ((!strcmp(p_name, "group") || !strcmp(p_name, "option") ||
      	     !strcmp(p_name, "choice")) &&
              where == MXML_WS_AFTER_OPEN))
        return ("\n");

    return (0);
}

JMiniXML::JMiniXML()
{
    mxmlSetWrapMargin(0);

	return;
}

JMiniXML::~JMiniXML()
{
	return;
}

JUINT32 JMiniXML::SetList(JList<JPER_RECORD> &clsList, JCHAR* pName)
{
    JListItem<JPER_RECORD>* pItem = JNULL;
    JPER_RECORD* pData = JNULL;
    vtp_tree_node* pTreeNode = JNULL;
    JCHAR strFileName[JMAX_STRING_LEN] = {0};

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_SERIALIZATION, "JMiniXML::SetList");

    if (!pName)
    {
        JSingleton<JLog>::instance2() << set(JLOG_MOD_SERIALIZATION, JLOG_ERROR_LEVEL) 
            << "JMiniXML::SetList input param is NULL\n";
        return JFAILURE;
    }

    if (clsList.GetItemNum() == 0)
    {
        return JSUCCESS;
    }

    pTreeNode = LoadString(JXML_FILE_TEMPLATE);

    JListIterator<JPER_RECORD> clsListIter(clsList);
    for (clsListIter.Last(); clsListIter.Done(); clsListIter.Prev())
    {
        pItem = clsListIter.Item();

        pData = pItem->GetData();
        if (pData)
        {
            AddNode(pTreeNode, pData->strKey, pData->strValue);
        }
    }

    if (clsList.GetItemNum())
    {
        SafeSprintf(strFileName, JMAX_STRING_LEN, "%s.xml", pName);
        SaveFile(pTreeNode, strFileName);
        DeleteNode(pTreeNode);
    }

    return JSUCCESS;
}

JList<JPER_RECORD> JMiniXML::GetList(JCHAR* pName)
{
    JUINT32 uiLen = 0;
    JList<JPER_RECORD> clsList;
    vtp_tree_node* pTreeNode = JNULL;
    vtp_tree_node* pRootNode = JNULL;
    vtp_tree_node* pSubNode = JNULL;
    JListItem<JPER_RECORD>* prevItem = JNULL;
    JListItem<JPER_RECORD>* pItem = JNULL;
    JPER_RECORD* pData = JNULL;
    JCHAR strKey[JMAX_STRING_LEN] = {0};
    JCHAR strValue[JMAX_STRING_LEN] = {0};
    JCHAR strFileName[JMAX_STRING_LEN] = {0};

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_SERIALIZATION, "JMiniXML::GetList");

    if (!pName)
    {
        JSingleton<JLog>::instance2() << set(JLOG_MOD_SERIALIZATION, JLOG_ERROR_LEVEL) 
            << "JMiniXML::GetList input param is NULL\n";
        return clsList;
    }

    SafeSprintf(strFileName, JMAX_STRING_LEN, "%s.xml", pName);
    pTreeNode = LoadFile(strFileName);
    if (pTreeNode)
    {
        pRootNode = FindNode(pTreeNode, pTreeNode, JXML_ROOT_NODE, strValue, JMAX_STRING_LEN);
        if (pRootNode)
        {
            while(HasChildNode(pRootNode))
            {
                pSubNode = NextNode(pRootNode, pRootNode, strKey, JMAX_STRING_LEN, 
                                    strValue, JMAX_STRING_LEN);
                if (pSubNode)
                {
                    prevItem = pItem;

                    uiLen = sizeof(JPER_RECORD);
                    pData = 
                        reinterpret_cast<JPER_RECORD*>(JSingleton<JStaticMemory>::instance()->Alloc(uiLen+1));
                    if (pData)
                    {
			            SafeMemset(reinterpret_cast<JCHAR*>(pData), 0, uiLen+1);

	                    if (SafeStrlen(strKey) < JMAX_STRING_LEN)
	                    {
	                        SafeStrcpy(pData->strKey, strKey, JMAX_STRING_LEN);
	                    }
	                    if (SafeStrlen(strValue) < JMAX_STRING_LEN)
	                    {
	                        SafeStrcpy(pData->strValue, strValue, JMAX_STRING_LEN);
	                    }
	                    pItem = new JListItem<JPER_RECORD>(pData);
	                    pItem->SetDataLength(uiLen);
	                    clsList.InsertItem(pItem, prevItem);
                    }
                }

                DeleteNode(pSubNode);
            }

            DeleteNode(pTreeNode);
        }
    }

    return clsList;
}

JINT32 JMiniXML::SaveFile(vtp_tree_node* p_tree, JCHAR* file)
{
    FILE*   fp = NULL;
    JINT32  ret = 0;

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_SERIALIZATION, "JMiniXML::SaveFile");

    if (!p_tree || !file)
    {
        JSingleton<JLog>::instance2() << set(JLOG_MOD_SERIALIZATION, JLOG_ERROR_LEVEL) 
            << "JMiniXML::SaveFile input param is NULL\n";
        return JFAILURE;
    }

    fp = SafeFopen(file, "w+");
    if (fp == NULL)
    {
        JSingleton<JLog>::instance2() << set(JLOG_MOD_SERIALIZATION, JLOG_ERROR_LEVEL) 
            << "JMiniXML::SaveFile fopen failure\n";
        return JFAILURE;
    }

    ret = mxmlSaveFile(p_tree, fp, vtp_whitespace_cb);

    SafeFclose(fp);

    return ret;
}

vtp_tree_node* JMiniXML::LoadFile(JCHAR* file)
{
    FILE*           fp = NULL;
    vtp_tree_node*  p_tree = NULL;

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_SERIALIZATION, "JMiniXML::LoadFile");

    if (!file)
    {
        JSingleton<JLog>::instance2() << set(JLOG_MOD_SERIALIZATION, JLOG_ERROR_LEVEL) 
            << "JMiniXML::LoadFile input param is NULL\n";
        return NULL;
    }

    fp = SafeFopen(file, "r");
    if (fp == NULL)
    {
        JSingleton<JLog>::instance2() << set(JLOG_MOD_SERIALIZATION, JLOG_ERROR_LEVEL) 
            << "JMiniXML::LoadFile fopen failure\n";
        return NULL;
    }

    p_tree = mxmlLoadFile(NULL, fp, vtp_type_cb);

    SafeFclose(fp);

    return p_tree;
}

vtp_tree_node* JMiniXML::LoadString(JCHAR*  p_str)
{
    vtp_tree_node*  p_tree = NULL;

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_SERIALIZATION, "JMiniXML::LoadString");

    if (!p_str)
    {
        JSingleton<JLog>::instance2() << set(JLOG_MOD_SERIALIZATION, JLOG_ERROR_LEVEL) 
            << "JMiniXML::LoadString input param is NULL\n";
        return NULL;
    }

    p_tree = mxmlLoadString(NULL, p_str, vtp_type_cb);

    return p_tree;
}

vtp_tree_node* JMiniXML::AddNode(vtp_tree_node*  p_node,
                                JCHAR*          p_key,
                                JCHAR*          p_value)
{
    vtp_tree_node*  p_key_node = NULL;
    vtp_tree_node*  p_value_node = NULL;
    vtp_tree_node*  p_format_node = NULL;

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_SERIALIZATION, "JMiniXML::AddNode");

    if (!p_node || !p_key)
    {
        JSingleton<JLog>::instance2() << set(JLOG_MOD_SERIALIZATION, JLOG_ERROR_LEVEL) 
            << "JMiniXML::AddNode input param is NULL\n";
        return NULL;
    }

    if (p_node->child && 
        p_node->child->type == MXML_OPAQUE &&
        p_node->child->value.opaque[0] != NULL)
    {
        p_format_node = reinterpret_cast<vtp_tree_node*>(calloc(1, sizeof(vtp_tree_node)));
        if (p_format_node)
        {
            p_format_node->type = MXML_OPAQUE;
            p_format_node->value.opaque = strdup(p_node->child->value.opaque);
        }
        else
        {
            JSingleton<JLog>::instance2() << set(JLOG_MOD_SERIALIZATION, JLOG_ERROR_LEVEL) 
                << "JMiniXML::AddNode calloc format_node failure\n";
            return NULL;
        }
    }

    p_key_node = reinterpret_cast<vtp_tree_node*>(calloc(1, sizeof(vtp_tree_node)));
    if (p_key_node)
    {
        p_key_node->type = MXML_ELEMENT;
        p_key_node->value.element.name = strdup(p_key);
        mxmlAdd(p_node, MXML_ADD_BEFORE, MXML_ADD_TO_PARENT, p_key_node);
    }
    else
    {
        JSingleton<JLog>::instance2() << set(JLOG_MOD_SERIALIZATION, JLOG_ERROR_LEVEL) 
            << "JMiniXML::AddNode calloc key_node failure\n";
        return NULL;
    }

    if (p_value)
    {
        p_value_node = mxmlNewOpaque(p_key_node, p_value);
        if (p_value_node == NULL)
        {
            JSingleton<JLog>::instance2() << set(JLOG_MOD_SERIALIZATION, JLOG_ERROR_LEVEL) 
                << "JMiniXML::AddNode invoke mxmlNewOpaque failure\n";
            return NULL;
        }
    }

    if (p_format_node)
    {
        mxmlAdd(p_node, MXML_ADD_BEFORE, MXML_ADD_TO_PARENT, p_format_node);
    }

    return p_key_node;
}

JVOID JMiniXML::DeleteNode(vtp_tree_node* p_node)
{
    vtp_tree_node*  p_parent_node = NULL;
    vtp_tree_node*  p_prev_node = NULL;

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_SERIALIZATION, "JMiniXML::DeleteNode");

    if (!p_node)
    {
        JSingleton<JLog>::instance2() << set(JLOG_MOD_SERIALIZATION, JLOG_ERROR_LEVEL) 
            << "JMiniXML::DeleteNode input param is NULL\n";
        return;
    }

    p_parent_node = p_node->parent;
    p_prev_node = p_node;
    while(1)    /* delete the space/LF opaque node before the key element node */
    {
        p_prev_node = mxmlWalkPrev(p_node, p_parent_node, MXML_DESCEND);
        if (p_prev_node && p_prev_node->type != MXML_ELEMENT)
            mxmlDelete(p_prev_node);
        else
            break;
    }
    mxmlDelete(p_node);

    return;
}

JBOOL JMiniXML::HasChildNode(vtp_tree_node*  p_node)
{
    vtp_tree_node*  p_child_node = NULL;
    vtp_tree_node*  p_next_node = NULL;

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_SERIALIZATION, "JMiniXML::HasChildNode");

    if (!p_node)
    {
        JSingleton<JLog>::instance2() << set(JLOG_MOD_SERIALIZATION, JLOG_ERROR_LEVEL) 
            << "JMiniXML::HasChildNode input param is NULL\n";
        return JFALSE;
    }

    p_child_node = p_node->child;
    if (p_child_node)
    {
        if (p_child_node->type == MXML_ELEMENT)
        {
            return JTRUE;
        }
    }
    else
    {
        return JFALSE;
    }

    p_next_node = p_child_node->next;
    while(p_next_node)
    {
        if (p_next_node->type == MXML_ELEMENT)
        {
            return JTRUE;
        }

        p_next_node = p_next_node->next;
    }

    return JFALSE;
}

vtp_tree_node* JMiniXML::FindNode(vtp_tree_node*     p_node,
                                    vtp_tree_node*   p_top,
                                    JCHAR*          p_key,
                                    JCHAR*          p_value,
                                    JUINT32          value_len)
{
    vtp_tree_node*  p_key_node = NULL;
    vtp_tree_node*  p_value_node = NULL;

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_SERIALIZATION, "JMiniXML::FindNode");

    if (!p_node || !p_top || !p_key)
    {
        JSingleton<JLog>::instance2() << set(JLOG_MOD_SERIALIZATION, JLOG_ERROR_LEVEL) 
            << "JMiniXML::FindNode input param is NULL\n";
        return NULL;
    }

    if (p_value && value_len)
        memset(p_value, 0, value_len);

    if (p_node->type == MXML_ELEMENT && 
        p_node->value.element.name &&
        strcmp(p_node->value.element.name, p_key) == 0)
    {
        p_key_node = p_node;
    }
    else
    {
        p_key_node = mxmlFindElement(p_node, p_top, p_key, NULL, NULL, MXML_DESCEND);
        if (p_key_node == NULL)
        {
            return NULL;
        }
    }

    p_value_node = p_key_node->child;
    if (p_value_node && 
        p_value_node->type == MXML_OPAQUE && 
        p_value_node->value.opaque[0] != JXML_LINE_FEED)
    {
        if (p_value && value_len)
        {
            if (strlen(p_value_node->value.opaque) >= value_len)
            {
                JSingleton<JLog>::instance2() << set(JLOG_MOD_SERIALIZATION, JLOG_ERROR_LEVEL) 
                    << "JMiniXML::FindNode input param p_value buffer is too short\n";
                return NULL;
            }
            else
                strcpy(p_value, p_value_node->value.opaque);
        }
    }

    return p_key_node;
}

vtp_tree_node* JMiniXML::NextNode(vtp_tree_node*    p_node,
                                vtp_tree_node*      p_top,
                                JCHAR*              p_key,
                                JUINT32             key_len,
                                JCHAR*              p_value,
                                JUINT32             value_len)
{
    vtp_tree_node*  p_key_node = NULL;
    vtp_tree_node*  p_value_node = NULL;

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_SERIALIZATION, "JMiniXML::NextNode");

    if (!p_top)
    {
        JSingleton<JLog>::instance2() << set(JLOG_MOD_SERIALIZATION, JLOG_ERROR_LEVEL) 
            << "JMiniXML::NextNode input param is NULL\n";
        return NULL;
    }

    if (p_key && key_len)
        memset(p_key, 0, key_len);
    if (p_value && value_len)
        memset(p_value, 0, value_len);

    if (!p_node)
        p_key_node = p_top;
    else
        p_key_node = p_node;

    while(p_key_node)
    {
        p_key_node = mxmlWalkNext(p_key_node, p_top, MXML_DESCEND);
        if (p_key_node && p_key_node->type == MXML_ELEMENT)
            break;
    }

    if (p_key_node && p_key_node->type == MXML_ELEMENT)
    {
        if (p_key && key_len)
        {
            if (strlen(p_key_node->value.element.name) >= key_len)
            {
                JSingleton<JLog>::instance2() << set(JLOG_MOD_SERIALIZATION, JLOG_ERROR_LEVEL) 
                    << "JMiniXML::NextNode input param p_key buffer is too short\n";
                return NULL;
            }
            else
                strcpy(p_key, p_key_node->value.element.name);
        }

        p_value_node = p_key_node->child;
        if (p_value_node && 
            p_value_node->type == MXML_OPAQUE && 
            p_value_node->value.opaque[0] != JXML_LINE_FEED)
        {
            if (p_value && value_len)
            {
                if (strlen(p_value_node->value.opaque) >= value_len)
                {
                    JSingleton<JLog>::instance2() << set(JLOG_MOD_SERIALIZATION, JLOG_ERROR_LEVEL) 
                        << "JMiniXML::NextNode input param p_value buffer is too short\n";
                    return NULL;
                }
                else
                    strcpy(p_value, p_value_node->value.opaque);
            }
        }

        return p_key_node;
    }

    return NULL;
}

JSerialization::JSerialization(JSER_PERSISTENCE_TYPE ePersistenceType, 
                                JCHAR* pServer, 
                                JCHAR* pUser, 
                                JCHAR* pPassword, 
                                JUINT32 uiPort, 
                                JCHAR* pDataBase)
{
    m_pPersistence = JNULL;

    if (ePersistenceType == JSER_PERSISTENCE_INI)
    {
        m_pPersistence = new JIni();
    }
    else if (ePersistenceType == JSER_PERSISTENCE_SQL)
    {
        m_pPersistence = new JMySql(pServer, pUser, pPassword, uiPort, pDataBase);
    }
    else if (ePersistenceType == JSER_PERSISTENCE_XML)
    {
        m_pPersistence = new JMiniXML();
    }
    #if 0
    else if (ePersistenceType == JSER_PERSISTENCE_QUERY)
    {
        try
        {
            m_pPersistence = new JMySql(pServer, pUser, pPassword, uiPort, pDataBase);
        }
        catch(JException &)
        {
            if (m_pPersistence)
            {
                delete m_pPersistence;
            }

            m_pPersistence = new JMiniXML();
        }
    }
    #endif

	return;
}

JSerialization::~JSerialization()
{
    if (m_pPersistence)
    {
        delete m_pPersistence;
    }

	return;
}

JUINT32 JSerialization::SetList(JList<JPER_RECORD> &clsList, JCHAR* pName)
{
    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_SERIALIZATION, "JSerialization::SetList");

    if (!pName)
    {
        JSingleton<JLog>::instance2() << set(JLOG_MOD_SERIALIZATION, JLOG_ERROR_LEVEL) 
            << "JSerialization::SetList input param is NULL\n";
        return JFAILURE;
    }

    if (m_pPersistence)
    {
        m_pPersistence->SetList(clsList, pName);
    }

    return JSUCCESS;
}

JList<JPER_RECORD> JSerialization::GetList(JCHAR* pName)
{
    JList<JPER_RECORD> clsList;

    JLogAutoPtr clsLogAutoPtr(JSingleton<JLog>::instance(), 
        JLOG_MOD_SERIALIZATION, "JSerialization::GetList");

    if (!pName)
    {
        JSingleton<JLog>::instance2() << set(JLOG_MOD_SERIALIZATION, JLOG_ERROR_LEVEL) 
            << "JSerialization::GetList input param is NULL\n";
        return clsList;
    }

    if (m_pPersistence)
    {
        return m_pPersistence->GetList(pName);
    }

    return clsList;
}

}

