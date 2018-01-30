//==============================================================================
//                      Buffer.h
//
//begin   : 2016-08-23
//describe: ������������
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

#ifdef LINUX_LIMITED //�ڴ桢���������Ƶ�linuxϵͳ��ʹ��

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
	char* GetBuffer();			// �������ÿռ��׵�ַ
	char* GetAvailableBuf();	// ���ؿ��ÿռ��׵�ַ
	int GetBufSize();			// ���ط���ռ��С
	int	GetAvailableSize();		// ���ؿ��ÿռ��С������ռ��С - ���ÿռ��С��
	int	GetUsed() const;		// �������ÿռ��С

	void SetWParam(int w);
	int GetWParam();
	void SetLParam(int l);
	int	GetLParam();
	void SetFlag(int nFlag);
	int GetFlag() const;
	void SetDesc(const char* szDesc);
	string GetDesc() const;

	// ʹ�����ÿռ��С����iUsed��Ҫ�����Ӻ�����ÿռ��С���ܴ��ڷ���ռ��С
	void Use(int nUsed);

	// ִ�гɹ����û����Ա�֤���ÿռ���ڵ���nSizeNeed
	bool Extend(int nSizeNeed = EXTENDSIZE);
	
	/* ����ռ�������ÿռ���к��ʵļ�С���ͷŶ����ڴ棬�ٷ���һ��С���ڴ棩
	�·�����ڴ��С���������ÿռ䣬����Ϊ��ʼ�����С��������
	һ�������ÿռ��С��С�󣨱���Release�ͷ��ˣ����������Ҫ */
	bool Shrink();

	// ������ݵ����ÿռ��β����������ÿռ䲻���ˣ��Զ������㹻�Ŀռ�
	// ���ʧ���򷵻�-1���ɹ��򷵻���Ӻ�����ÿռ��С
	int AddData(CBuffer* pBuf);
	int	AddData(const char *pData, int nSize);
	int	AddData(void *pData, int nBlockSize, int nBlockCount);

	// �����ÿռ��λ��nPos���������ݣ�ʧ�ܷ���-1���ɹ����ز��������ÿռ��С
	int	Insert(int nPos, void *pData, int nSize);
	
	// �����ÿռ��λ��nPos������nSize�ֽڿհ����ݣ�0����ʧ�ܷ���-1���ɹ����ز��������ÿռ��С
	int	Interspace(int nPos, int nSize);
	
	// �����ÿռ��λ��nPos��ɾ����nSize�ֽڵ����ݣ�����������Ƶ�nPos����ǰ������ݽ���
	void Cut(int nPos, int nSize);
	
	/* �����ÿռ�Ŀ�ʼλ���𿽱�nSplit�ֽڵ����ݵ��·����һ��buffer�����
	���ұ�buffer��������ÿռ��СnSplit
	���ʧ�ܣ�����NULL���ɹ������·����buffer�����ָ�� */
	CBuffer* Split(int nSplit);
	
	// ���Ƴ�һ���µ�buffer����
	CBuffer* Dup(); 

	// ����������allocator��active list�н�pBuf�Ƴ���
	// �����ʱfree list�Ĵ�Сû�е������ֵ����ô��pBuf���뵽free list��ͷ��������delete pBuf
	void Release();

	// �����ÿռ��С����Ϊ0
	void Empty();

private:
	char*		m_pBuf;			// ����Ŀռ���׵�ַ
	int			m_nBufSize;		// ����ռ�Ĵ�С
	int			m_nBufUsed;		// ���ÿռ�Ĵ�С
	int			m_nExtendSize;	// �����ڴ����չ��λ�������ڴ�Ϊ����������
	int			m_nInitSize;	// �������ķ���ռ��ʼ����С�����߽г�ʼ����ռ��С

	int			m_nWParam;		// buffer����Ķ�����������ڴ�����ݰ���ת����ʽ������ת���� bool bIsSupportLocalDispatch
								// Ĭ��Ϊ0����ʾ����Ҫ����ת����1����ʾ��Ҫ����ת����
	int			m_nLParam;		// buffer����Ķ������
	int			m_nFlag;		// buffer����Ķ������
	string		m_strDesc;		// buffer����Ķ������

	CAllocator&	m_objAllocator;	// buffer����ķ���������
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
	// ��active list�н�pBuf�Ƴ���
	// �����ʱfree list�Ĵ�Сû�е������ֵ����ô��pBuf���뵽free list��ͷ��������delete pBuf
	void Release(CBuffer *pBuf);
	
public:
	// ���free list�ǿգ���ô����ͷ��ȡ��һ��buffer������newһ��buffer��
	// ����buffer���뵽active list��ͷ�������ظ�buffer
	CBuffer *Allocate(const char* szDesc = "");

	// ����free list���ֵ����Χ��0��MAXFREEBUFS
	void SetMaxFreeBufs(unsigned int nMaxFreeBufs); 

	void CleanFreeList();
	int GetFreeSize() const;
	int GetActiveSize() const;

	string GetDumpStr();

protected:
	int m_nBufSize; 			// ������CBuffer�ĳ�ʼ������
	unsigned int m_nExtendSize; // ������CBuffer�ĳ�ʼ������
	
	list<CBuffer *> m_lstFreeBuf;
	list<CBuffer *> m_lstActiveBuf;

	// free list����󳤶ȣ�����ӳ��һ��CAllocator�Ļ�������
	// ���һ��CAllocator��free list�����������ֵ����ô����CAllocator������
	// ����������Щ�ڴ��һֱ���������ᱻ�ͷ�
	unsigned int m_nMaxFreeBufs;

	CPthMutex m_objMutex;
};


#endif
