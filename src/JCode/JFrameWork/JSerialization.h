/**********************************************************
* Author: weiganyi
* File: JSerialization:.h
* Date: 20120504
* Description: 
*
**********************************************************/

#ifndef _JSERIALIZATION_H_
#define _JSERIALIZATION_H_


#include <mxml.h>
#include <mysql.h>

namespace JFrameWork{

//JPersistence definication
typedef struct{
    JCHAR  strKey[JMAX_STRING_LEN];
    JCHAR  strValue[JMAX_STRING_LEN];
    JUINT8 bSysMemory;
}JPER_RECORD;

class JPersistence: public JObject{
public:
    JPersistence();
    virtual ~JPersistence();

    virtual JUINT32 SetList(JList<JPER_RECORD> &clsList, JCHAR* pName) = 0;
    virtual JList<JPER_RECORD> GetList(JCHAR* pName) = 0;

private:
};


//JIni definication
class JIni: public JPersistence{
public:
    JIni();
    ~JIni();

    JUINT32 SetList(JList<JPER_RECORD> &clsList, JCHAR* pName);
    JList<JPER_RECORD> GetList(JCHAR* pName);

private:
};


//JMySql definication
#define JSQL_DEFAULT_DB     "mysql"

class JMySql: public JPersistence{
public:
    JMySql(JCHAR* pServer, 
            JCHAR* pUser, 
            JCHAR* pPassword, 
            JUINT32 uiPort, 
            JCHAR* pDataBase);
    ~JMySql();

    JUINT32 SetList(JList<JPER_RECORD> &clsList, JCHAR* pName);
    JList<JPER_RECORD> GetList(JCHAR* pName);

private:
    JString m_strServer;
    JString m_strUser;
    JString m_strPassword;
    JString m_strDataBase;
    JUINT32 m_uiPort;

    JUINT32 JMySql::OpenDataBase(JCHAR* pServer, 
                                JCHAR* pUser, 
                                JCHAR* pPassword, 
                                JUINT32 uiPort, 
                                JCHAR* pDataBase);
};


//JMiniXML definication
typedef mxml_node_t vtp_tree_node;

#define JXML_FILE_TEMPLATE  "<ROOT></ROOT>"
#define JXML_ROOT_NODE      "ROOT"

#define JXML_LINE_FEED      0x0D

class JMiniXML: public JPersistence{
public:
    JMiniXML();
    ~JMiniXML();

    JUINT32 SetList(JList<JPER_RECORD> &clsList, JCHAR* pName);
    JList<JPER_RECORD> GetList(JCHAR* pName);

    friend mxml_type_t vtp_type_cb(vtp_tree_node* p_node);
    friend const JCHAR* vtp_whitespace_cb(vtp_tree_node* p_node, JINT32 where);

private:
    JINT32 JMiniXML::SaveFile(vtp_tree_node* p_tree, JCHAR* file);
    vtp_tree_node* JMiniXML::LoadFile(JCHAR* file);
    vtp_tree_node* JMiniXML::LoadString(JCHAR*  p_str);
    vtp_tree_node* JMiniXML::AddNode(vtp_tree_node*  p_node,
                                    JCHAR*          p_key,
                                    JCHAR*          p_value);
    JVOID JMiniXML::DeleteNode(vtp_tree_node* p_node);
    JBOOL JMiniXML::HasChildNode(vtp_tree_node*  p_node);
    vtp_tree_node* JMiniXML::FindNode(vtp_tree_node*     p_node,
                                        vtp_tree_node*   p_top,
                                        JCHAR*          p_key,
                                        JCHAR*          p_value,
                                        JUINT32          value_len);
    vtp_tree_node* JMiniXML::NextNode(vtp_tree_node*     p_node,
                                    vtp_tree_node*   p_top,
                                    JCHAR*         p_key,
                                    JUINT32          key_len,
                                    JCHAR*         p_value,
                                    JUINT32          value_len);
};


//JSerialization definication
typedef enum{
    JSER_PERSISTENCE_NONE     = 0x0,
    JSER_PERSISTENCE_INI      = 0x1,
    JSER_PERSISTENCE_SQL      = 0x1<<1,
    JSER_PERSISTENCE_XML      = 0x1<<2
}JSER_PERSISTENCE_TYPE;

#define JSER_SQL_LOCALHOST      "localhost"
#define JSER_SQL_USER           "root"
#define JSER_SQL_PASSWORD       ""
#define JSER_SQL_PORT           3306
#define JSER_SQL_DB_NAME        "db_jphone"

class JSerialization: public JObject{
public:
    JSerialization(JSER_PERSISTENCE_TYPE ePersistenceType, 
                    JCHAR* pServer = JSER_SQL_LOCALHOST, 
                    JCHAR* pUser = JSER_SQL_USER, 
                    JCHAR* pPassword = JSER_SQL_PASSWORD, 
                    JUINT32 uiPort = JSER_SQL_PORT, 
                    JCHAR* pDataBase = JSER_SQL_DB_NAME);
    ~JSerialization();

    JUINT32 SetList(JList<JPER_RECORD> &clsList, JCHAR* pName);
    JList<JPER_RECORD> GetList(JCHAR* pName) ;

private:
    JPersistence* m_pPersistence;
};

}

#endif

