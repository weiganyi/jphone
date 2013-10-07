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
    //fixed trunk for memory alloc
    JMEM_TYPE_TRUNK,
    //dynamic heap for memory alloc
    JMEM_TYPE_HEAP,
    JMEM_TYPE_MAX
};

enum JMEMORY_SIZE{
    JMEM_SIZE_NONE,
    //memory size for normal mode, used in memory module
    JMEM_SIZE_NORMAL=1,
    //memory size for small mode, used in the other individual thread
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
//size for memory trunk block
#define JTMEM_BLK_SIZE1         16
#define JTMEM_BLK_SIZE2         64
#define JTMEM_BLK_SIZE3         256
#define JTMEM_BLK_SIZE4         1024

//number for memory trunk block
#define JTMEM_BLK_NUM1          128
#define JTMEM_BLK_NUM2          128
#define JTMEM_BLK_NUM3          64
#define JTMEM_BLK_NUM4          16

//func trace deep for memory alloc
#define JTMEM_CALL_TRACE_NUM    8

typedef struct _JTRUNK_MEMORY_CFG{
    JUINT32 uiSize;
    JUINT32 uiNum;
}JTRUNK_MEMORY_CFG;

//trunk header struct be put in the front of memory block
typedef struct _JTRUNK_HDR{
    //whether this block already used
    JBOOL   ubIsUsed;
    //size for the memory alloc
    JUINT16 usAllocSize;
    //func trace for memory alloc
    JCHAR* uiCallTrace[JTMEM_CALL_TRACE_NUM];
    //memory alloc type: trunk or heap
    JMEMORY_TYPE ucMemoryType;
    //head adjust key
    JUINT32 uiAdjust;
}JTRUNK_HDR;

//trunk tail struct be put in the tail of memory block
typedef struct _JTRUNK_TAL{
    //tail adjust key
    JUINT32 uiAdjust;
}JTRUNK_TAL;

//number of the memory block for different type
#define JTMEM_MAX_BLK_TYPE_NUM  10

class JTrunkMemory: public JMemory{
public:
    //the default type is small mode
    JTrunkMemory(JMEMORY_SIZE eSizeType=JMEM_SIZE_NORMAL);
    ~JTrunkMemory();

    JUINT32 Init();
    JCHAR* Alloc(const JUINT32 uiLen);
    JVOID Free(JCHAR* ptrAddr);

    JUINT32 DumpMemory();
    JUINT32 DoMemoryCheck();
    JUINT32 SetMemorySize(JMEMORY_SIZE eSizeType);

private:
    //type number for different block
    JUINT32 m_blkTypeNum;
    //start address for all block type
    JCHAR* m_blkStartAddr[JTMEM_MAX_BLK_TYPE_NUM];
    //size for all block type
    JUINT32 m_blkSize[JTMEM_MAX_BLK_TYPE_NUM];
    //total size of one block type for all block type
    JUINT32 m_blkTotalSize[JTMEM_MAX_BLK_TYPE_NUM];
    //number for all block type
    JUINT32 m_blkNum[JTMEM_MAX_BLK_TYPE_NUM];

    //max block number already alloc
    JUINT32 m_blkMaxAllocNum[JTMEM_MAX_BLK_TYPE_NUM];
    //free block number currently
    JUINT32 m_blkFreeNum[JTMEM_MAX_BLK_TYPE_NUM];

    //start address for trunk memory
    JCHAR* m_pTrunkMemory;

    //stored memory type
    JMEMORY_SIZE m_eSizeType;
};


//JHeapMemory definication
//size for heap momory
#define JHMEM_MAX_HEAP_LEN        1024*1024

//func trace deep for memory alloc
#define JHMEM_CALL_TRACE_NUM    8

//info for heap alloc
typedef struct _JHEAP_ALLOC_INFO{
    //start address for heap alloc
    JCHAR* pStart;
    //end address for heap alloc
    JCHAR* pEnd;
}JHEAP_ALLOC_INFO;

//heap header struct be put in the front of memory alloc
typedef struct _JHEAP_HDR{
    //info for heap alloc
    JHEAP_ALLOC_INFO stInfo;
    //size for memoy alloc
    JUINT16 usAllocSize;
    //func trace for memory alloc
    JCHAR* uiCallTrace[JHMEM_CALL_TRACE_NUM];
    //memory type
    JMEMORY_TYPE ucMemoryType;
    //head adjust key
    JUINT32 uiAdjust;
}JHEAP_HDR;

//heap tail struct be put in the tail of memory alloc
typedef struct _JHEAP_TAL{
    //tail adjust key
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
    //start address for total heap memory
    JCHAR*     m_pHeapMemory;
    //info list for memory alloc
    JList<JHEAP_ALLOC_INFO> m_AllocInfo;

    //size type for heap memory
    JMEMORY_SIZE m_eSizeType;

    JCHAR* privAlloc(JCHAR* ptrStart, const JUINT32 uiLen, JListItem<JHEAP_ALLOC_INFO>* ptrItem);
};


//JStaticMemory definication
//inteval for checking memory overwrite
#define JMEMORY_CHECK_INTERVAL      1000*5
//type number for memory
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
    //memory type
    JMEMORY_TYPE m_eMemoryType[JMEMORY_NUM];
    //memory object
    JMemory* m_pMemory[JMEMORY_NUM];

    //lock to protect memory alloc
    JLock m_Lock;

    ////size type for memory
    JMEMORY_SIZE m_eSizeType;

    //timer handler to check memory overwrite
    JUINT32 m_timeHandler;

    JUINT32 DumpMemory();
    JUINT32 DoMemoryCheck();
};

}


#endif

