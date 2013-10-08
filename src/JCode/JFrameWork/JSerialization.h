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

//JPersistence definition
typedef struct{
    //key for one persistence record
    JCHAR  strKey[JMAX_STRING_LEN];
    //value for one persistence record
    JCHAR  strValue[JMAX_STRING_LEN];
}JPER_RECORD;

class JPersistence: public JObject{
public:
    JPersistence();
    virtual ~JPersistence();

    virtual JUINT32 SetList(JList<JPER_RECORD> &clsList, JCHAR* pName) = 0;
    virtual JList<JPER_RECORD> GetList(JCHAR* pName) = 0;

private:
};


//JIni definition
class JIni: public JPersistence{
public:
    JIni();
    ~JIni();

    JUINT32 SetList(JList<JPER_RECORD> &clsList, JCHAR* pName);
    JList<JPER_RECORD> GetList(JCHAR* pName);

private:
};


//JMySql definition
//default sql database name
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
    //sql database server
    JString m_strServer;
    //sql database user
    JString m_strUser;
    //sql database password
    JString m_strPassword;
    //sql database port
    JUINT32 m_uiPort;
    //sql database name
    JString m_strDataBase;

    JUINT32 JMySql::OpenDataBase(JCHAR* pServer, 
                                JCHAR* pUser, 
                                JCHAR* pPassword, 
                                JUINT32 uiPort, 
                                JCHAR* pDataBase);
};


//JMiniXML definition
//xml node type
typedef mxml_node_t vtp_tree_node;

//root node template
#define JXML_FILE_TEMPLATE  "<ROOT></ROOT>"
//root node name
#define JXML_ROOT_NODE      "ROOT"

//lineend token
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


//JSerialization definition
typedef enum{
    JSER_PERSISTENCE_NONE     = 0x0,
    //persistence through ini file
    JSER_PERSISTENCE_INI      = 0x1,
    //persistence through sql databse
    JSER_PERSISTENCE_SQL      = 0x1<<1,
    //persistence through xml file
    JSER_PERSISTENCE_XML      = 0x1<<2
}JSER_PERSISTENCE_TYPE;

//default sql databse server
#define JSER_SQL_LOCALHOST      "localhost"
//default sql databse user
#define JSER_SQL_USER           "root"
//default sql databse password
#define JSER_SQL_PASSWORD       ""
//default sql databse port
#define JSER_SQL_PORT           3306
//default sql databse name
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
    //persistence handler
    JPersistence* m_pPersistence;
};

}

#endif

