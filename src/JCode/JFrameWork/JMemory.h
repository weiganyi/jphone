/**********************************************************
* Author: weiganyi
* File: JMemory.h
* Date: 20110718
* Description: 
*
**********************************************************/

#ifndef _JMEMORY_H_
#define _JMEMORY_H_


namespace JFrameWork{

//JMemory definication
enum JMEMORY_TYPE{
    JMEM_TYPE_NONE,
    JMEM_TYPE_TRUNK,
    JMEM_TYPE_HEAP,
    JMEM_TYPE_MAX
};

enum JMEMORY_SIZE{
    JMEM_SIZE_NONE,
    JMEM_SIZE_NORMAL=1,
    JMEM_SIZE_SMALL =1<<4
};

class JMemory: public JObject{
public:
    JMemory();
    virtual ~JMemory();

    virtual JUINT32 Init() = 0;
    virtual JCHAR* Alloc(const JUINT32 uiLen) = 0;
    virtual JVOID Free(JCHAR* ptrAddr) = 0;
    virtual JUINT32 DoMemoryCheck() = 0;
    virtual JUINT32 DumpMemory() = 0;
    virtual JUINT32 SetMemorySize(JMEMORY_SIZE eSizeType) = 0;

private:
};


//JTrunkMemory definication
#define JTMEM_BLK_SIZE1         16
#define JTMEM_BLK_SIZE2         64
#define JTMEM_BLK_SIZE3         256
#define JTMEM_BLK_SIZE4         1024

#define JTMEM_BLK_NUM1          128
#define JTMEM_BLK_NUM2          128
#define JTMEM_BLK_NUM3          64
#define JTMEM_BLK_NUM4          16

#define JTMEM_CALL_TRACE_NUM    8

typedef struct _JTRUNK_MEMORY_CFG{
    JUINT32 uiSize;
    JUINT32 uiNum;
}JTRUNK_MEMORY_CFG;

typedef struct _JTRUNK_HDR{
    JBOOL   ubIsUsed;
    JUINT16 usAllocSize;
    JCHAR* uiCallTrace[JTMEM_CALL_TRACE_NUM];
    JMEMORY_TYPE ucMemoryType;
    JUINT32 uiAdjust;
}JTRUNK_HDR;

typedef struct _JTRUNK_TAL{
    JUINT32 uiAdjust;
}JTRUNK_TAL;

#define JTMEM_MAX_BLK_TYPE_NUM  10

class JTrunkMemory: public JMemory{
public:
    JTrunkMemory(JMEMORY_SIZE eSizeType=JMEM_SIZE_NORMAL);
    ~JTrunkMemory();

    JUINT32 Init();
    JCHAR* Alloc(const JUINT32 uiLen);
    JVOID Free(JCHAR* ptrAddr);
    JUINT32 DumpMemory();
    JUINT32 DoMemoryCheck();
    JUINT32 SetMemorySize(JMEMORY_SIZE eSizeType);

private:
    JUINT32 m_blkTypeNum;
    JCHAR* m_blkStartAddr[JTMEM_MAX_BLK_TYPE_NUM];
    JUINT32 m_blkSize[JTMEM_MAX_BLK_TYPE_NUM];
    JUINT32 m_blkTotalSize[JTMEM_MAX_BLK_TYPE_NUM];
    JUINT32 m_blkNum[JTMEM_MAX_BLK_TYPE_NUM];

    JUINT32 m_blkMaxAllocNum[JTMEM_MAX_BLK_TYPE_NUM];
    JUINT32 m_blkFreeNum[JTMEM_MAX_BLK_TYPE_NUM];

    JCHAR* m_pTrunkMemory;

    JMEMORY_SIZE m_eSizeType;
};


//JHeapMemory definication
#define JHMEM_MAX_HEAP_LEN        1024*1024

#define JHMEM_CALL_TRACE_NUM    8

typedef struct _JHEAP_ALLOC_INFO{
    JCHAR* pStart;
    JCHAR* pEnd;
}JHEAP_ALLOC_INFO;

typedef struct _JHEAP_HDR{
    JHEAP_ALLOC_INFO stInfo;
    JUINT16 usAllocSize;
    JCHAR* uiCallTrace[JHMEM_CALL_TRACE_NUM];
    JMEMORY_TYPE ucMemoryType;
    JUINT32 uiAdjust;
}JHEAP_HDR;

typedef struct _JHEAP_TAL{
    JUINT32 uiAdjust;
}JHEAP_TAL;

class JHeapMemory: public JMemory{
public:
    JHeapMemory(JMEMORY_SIZE eSizeType=JMEM_SIZE_NORMAL);
    ~JHeapMemory();

    JUINT32 Init();
    JCHAR* Alloc(const JUINT32 uiLen);
    JVOID Free(JCHAR*);
    JUINT32 DumpMemory();
    JUINT32 DoMemoryCheck();
    JUINT32 SetMemorySize(JMEMORY_SIZE eSizeType);

private:
    JCHAR*     m_pHeapMemory;
    JList<JHEAP_ALLOC_INFO> m_AllocInfo;

    JMEMORY_SIZE m_eSizeType;

    JCHAR* privAlloc(JCHAR* ptrStart, const JUINT32 uiLen, JListItem<JHEAP_ALLOC_INFO>* ptrItem);
};


//JStaticMemory definication
#define JMEMORY_CHECK_INTERVAL      1000*5
#define JMEMORY_NUM                 2

class JStaticMemory: public JModule{
public:
    JStaticMemory(JMEMORY_SIZE eSizeType=JMEM_SIZE_NORMAL);
    ~JStaticMemory();

    JCHAR* Alloc(const JUINT32 uiLen);
    JVOID Free(JCHAR* ptrAddr);

    JUINT32 SetMemorySize(JMEMORY_SIZE eSizeType);

    JUINT32 InitFunc();
    JUINT32 EventProcFunc(JEvent* pEvent);
    JEventBody* MakeEventBody(JUINT32 uiType);

    friend JUINT32 OnCheckMemory(JVOID* pData);

private:
    JMEMORY_TYPE m_eMemoryType[JMEMORY_NUM];
    JMemory* m_pMemory[JMEMORY_NUM];

    JLock m_Lock;

    JMEMORY_SIZE m_eSizeType;

    JUINT32 m_timeHandler;

    JUINT32 DumpMemory();
    JUINT32 DoMemoryCheck();
};

}


#endif

