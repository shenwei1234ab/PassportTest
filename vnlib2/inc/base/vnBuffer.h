//
//  vnBuffer.h
//  vnlib
//
//  Created by Wayne on 14-5-11.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#ifndef vnlib_vnBuffer_h
#define vnlib_vnBuffer_h

#include "vnRefCounted.h"
#include "vnDataStream.h"

_vn_begin

class Buffer : public RefCounted {
public:
    virtual Buffer * clone() const = 0;
    virtual void * lock() = 0;
    virtual void unlock() = 0;
    virtual size_t size() const = 0;
    virtual size_t read(size_t offset, void *data, size_t size) const = 0;
    virtual size_t write(size_t offset, const void *data, size_t size) = 0;
};

typedef RefCountedPtr<Buffer> BufferPtr;

class BufferStream : public DataStream {
public:
    BufferStream(BufferPtr buffer);
    
    const BufferPtr & buffer() const;
    
    virtual size_t read(void *data, size_t size);
	virtual size_t write(const void *data, size_t size);
	virtual bool seek(s64 offset, SeekDir way = kBegin);
	virtual s64 tell();
private:
    BufferPtr m_buffer;
    size_t m_offset = 0;
};

class FixedBuffer : public Buffer {
public:
	explicit FixedBuffer(size_t size, bool extraNull = false);
	FixedBuffer(const FixedBuffer &);
	virtual ~FixedBuffer();
	virtual FixedBuffer * clone() const;
	virtual void * lock();
	virtual void unlock();
	virtual size_t size() const;
    virtual size_t read(size_t offset, void *data, size_t size) const;
	virtual size_t write(size_t offset, const void *data, size_t size);
private:
	void *m_data;
	size_t m_size;
	bool m_extraNull;
};

class ProxyBuffer : public Buffer {
public:
	ProxyBuffer(void *data, size_t size);
	virtual ProxyBuffer * clone() const;
	virtual void * lock();
	virtual void unlock();
	virtual size_t size() const;
    virtual size_t read(size_t offset, void *data, size_t size) const;
	virtual size_t write(size_t offset, const void *data, size_t size);
private:
	void *m_data;
	size_t m_size;
};

_vn_end

#endif
