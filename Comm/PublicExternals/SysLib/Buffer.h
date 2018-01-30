//==============================================================================
//                      Buffer.h
//
//begin   : 2016-08-23
//describe: 缓存分配与管理
//==============================================================================
#ifndef SELFDEF_BUFFER_H 
#define SELFDEF_BUFFER_H

#pragma warning(disable : 4251)

#include <assert.h>
#include <list>
#include <string>
#include <string.h>

#include "PthMutex.h"
#include "TypeDef.h"

using namespace std;

#ifdef LINUX_LIMITED //内存、磁盘受限制的linux系统可使用

	#define IOBUFSIZE	128
	#define EXTENDSIZE	128
	#define MAXFREEBUFS 1

#else

	#define IOBUFSIZE	1024
	#define EXTENDSIZE	1024
	#define MAXFREEBUFS 10

#endif

//==============================================================================
//                 
//                    CBuffer
//                    
//
//==============================================================================
class MY_EXT_CLASS CBuffer 
{
public:
	class CAllocator;
	friend class CAllocator;

protected:
	CBuffer(CAllocator& objAlloc, int nSize = IOBUFSIZE, int nExtend = EXTENDSIZE);
	~CBuffer();

public:
	char* GetBuffer();			// 返回已用空间首地址
	char* GetAvailableBuf();	// 返回可用空间首地址
	int GetBufSize();			// 返回分配空间大小
	int	GetAvailableSize();		// 返回可用空间大小（分配空间大小 - 已用空间大小）
	int	GetUsed() const;		// 返回已用空间大小

	void SetWParam(int w);
	int GetWParam();
	void SetLParam(int l);
	int	GetLParam();
	void SetFlag(int nFlag);
	int GetFlag() const;
	void SetDesc(const char* szDesc);
	string GetDesc() const;

	// 使得已用空间大小增加iUsed，要求增加后的已用空间大小不能大于分配空间大小
	void Use(int nUsed);

	// 执行成功后，用户可以保证可用空间大于等于nSizeNeed
	bool Extend(int nSizeNeed = EXTENDSIZE);
	
	/* 分配空间根据已用空间进行合适的减小（释放多余内存，再分配一个小的内存）
	新分配的内存大小能容下已用空间，并且为初始分配大小的整数倍
	一般是已用空间大小减小后（比如Release释放了）才有这个需要 */
	bool Shrink();

	// 添加数据到已用空间的尾部，如果可用空间不够了，自动分配足够的空间
	// 如果失败则返回-1，成功则返回添加后的已用空间大小
	int AddData(CBuffer* pBuf);
	int	AddData(const char *pData, int nSize);
	int	AddData(void *pData, int nBlockSize, int nBlockCount);

	// 在已用空间的位置nPos处插入数据，失败返回-1，成功返回插入后的已用空间大小
	int	Insert(int nPos, void *pData, int nSize);
	
	// 在已用空间的位置nPos处插入nSize字节空白数据（0），失败返回-1，成功返回插入后的已用空间大小
	int	Interspace(int nPos, int nSize);
	
	// 在已用空间的位置nPos处删除掉nSize字节的数据，后面的数据移到nPos处和前面的数据接上
	void Cut(int nPos, int nSize);
	
	/* 从已用空间的开始位置起拷贝nSplit字节的数据到新分配的一个buffer对象里，
	并且本buffer对象的已用空间减小nSplit
	如果失败，返回NULL，成功返回新分配的buffer对象的指针 */
	CBuffer* Split(int nSplit);
	
	// 复制出一个新的buffer对象
	CBuffer* Dup(); 

	// 从其隶属的allocator的active list中将pBuf移除掉
	// 如果此时free list的大小没有到其最大值，那么将pBuf插入到free list的头部，否则delete pBuf
	void Release();

	// 将已用空间大小设置为0
	void Empty();

private:
	char*		m_pBuf;			// 分配的空间的首地址
	int			m_nBufSize;		// 分配空间的大小
	int			m_nBufUsed;		// 已用空间的大小
	int			m_nExtendSize;	// 分配内存的扩展单位，分配内存为它的整数倍
	int			m_nInitSize;	// 对象构造后的分配空间初始化大小，或者叫初始分配空间大小

	int			m_nWParam;		// buffer对象的额外参数，用于存放数据包的转发方式（本地转发） bool bIsSupportLocalDispatch
								// 默认为0：表示不需要本地转发；1：表示需要本地转发。
	int			m_nLParam;		// buffer对象的额外参数
	int			m_nFlag;		// buffer对象的额外参数
	string		m_strDesc;		// buffer对象的额外参数

	CAllocator&	m_objAllocator;	// buffer对象的分配器对象
};

//==============================================================================
//                 
//                    CBuffer::CAllocator
//                    
//
//==============================================================================
class MY_EXT_CLASS CBuffer::CAllocator
{
	friend class CBuffer;
public:
	CAllocator(int nBufSize = IOBUFSIZE, 
			   		 int nExtend = EXTENDSIZE, 
			   		 int nMaxFreeBufs = MAXFREEBUFS);
	virtual ~CAllocator();
	
protected:
	// 从active list中将pBuf移除掉
	// 如果此时free list的大小没有到其最大值，那么将pBuf插入到free list的头部，否则delete pBuf
	void Release(CBuffer *pBuf);
	
public:
	// 如果free list非空，那么从其头部取出一个buffer，否则new一个buffer，
	// 将该buffer插入到active list的头部，返回该buffer
	CBuffer *Allocate(const char* szDesc = "");

	// 设置free list最大值，范围从0到MAXFREEBUFS
	void SetMaxFreeBufs(unsigned int nMaxFreeBufs); 

	void CleanFreeList();
	int GetFreeSize() const;
	int GetActiveSize() const;

	string GetDumpStr();

protected:
	int m_nBufSize; 			// 分配新CBuffer的初始化参数
	unsigned int m_nExtendSize; // 分配新CBuffer的初始化参数
	
	list<CBuffer *> m_lstFreeBuf;
	list<CBuffer *> m_lstActiveBuf;

	// free list的最大长度，它反映了一个CAllocator的缓存容量
	// 如果一个CAllocator的free list到达了其最大值，那么除非CAllocator析构，
	// 否则程序的这些内存会一直保留，不会被释放
	unsigned int m_nMaxFreeBufs;

	CPthMutex m_objMutex;
};


#endif
