#ifndef _CHARBUFFER_h
#define _CHARBUFFER_h

#include <Arduino.h>
#include "datatypes.h"

class CharBuffer {
private:
	CharBuffer(CharBuffer &) = delete;

	const bool outerbuf;
protected:

public:
	const uint32_t siz;
	char *buf;
	uint32_t lim;

	uint32_t pos = 0;
	bool packed = false;

	CharBuffer(const uint32_t size);
	CharBuffer(char* bufptr, const uint32_t size);
	~CharBuffer();

	/*!
	* 寫入 1 byte
	*/
	void write(const byte b);
	/*!
	* 寫入 1 char
	*/
	void write(const char c);
	/*!
	* 寫入多個 byte
	* @param  [in] bytes 來源
	* @param  [in] offset 偏移
	* @param  [in] size 長度
	*/
	void write(const byte* bytes, const uint32_t offset, const uint32_t size);
	/*!
	* 寫入字串
	* @param  [in] str 來源
	* @param  [in] offset 偏移
	* @param  [in] size 長度
	*/
	void write(const char* str, const uint32_t offset, const uint32_t size);
	/*!
	* 寫入字串
	* @param  [in] msg 來源
	*/
	void write(const char msg[]);

	/*!
	* 打包
	*/
	void pack();
	/*!
	* 解包
	*/
	void unpack();
	void reset();

	char getChar();
	int8_t getInt8();
	uint8_t getUint8();
	int16_t getInt16();
	uint16_t getUint16();
	int32_t getInt32();
	uint32_t getUint32();
	void getStr(byte* arr, const uint32_t offset, const uint32_t size);

	void copy(char* buf);
};

inline CharBuffer::CharBuffer(const uint32_t size) : buf(new char[size + 1]), siz(size), lim(size), outerbuf(false)
{
}

inline CharBuffer::CharBuffer(char * bufptr, const uint32_t size) : buf(bufptr), siz(size), lim(size), outerbuf(true)
{
}

inline CharBuffer::~CharBuffer()
{
	if (!outerbuf) {
		delete[] buf;
	}
}

inline void CharBuffer::write(const byte b)
{
	if (pos < lim) {
		buf[pos] = b;
		pos++;
	}
}

inline void CharBuffer::write(const char c)
{
	if (pos < lim) {
		buf[pos] = c;
		pos++;
	}
}

inline void CharBuffer::write(const byte * bytes, const uint32_t offset, const uint32_t size)
{
	if (lim - pos >= size) {
		for (uint32_t i = offset; i < offset + size; i++)
		{
			buf[pos] = bytes[i];
			pos++;
		}
	}
}

inline void CharBuffer::write(const char * str, const uint32_t offset, const uint32_t size)
{
	if (lim - pos >= size) {
		for (uint32_t i = offset; i < offset + size; i++)
		{
			buf[pos] = str[i];
			pos++;
		}
	}
}

inline void CharBuffer::write(const char msg[])
{
	char c = msg[0];
	for (uint32_t i = 0; c != 0; i++) {
		c = msg[i];

		write(c);
	}
}

inline void CharBuffer::pack()
{
	if (packed) {
		return;
	}
	lim = pos;
	pos = 0;
	packed = true;
}

inline void CharBuffer::unpack()
{
	if (!packed) {
		return;
	}
	const uint32_t lefts = lim - pos;
	for (uint32_t i = 0; i < lefts; i++)
	{
		buf[i] = buf[pos + i];
	}
	pos = lefts;
	lim = siz;
	packed = false;
	buf[pos] = 0;
}

inline void CharBuffer::reset()
{
	pos = 0;
	lim = siz;
	packed = false;
	buf[0] = 0;
}

inline char CharBuffer::getChar()
{
	return buf[pos++];
}

inline int8_t CharBuffer::getInt8()
{
	return (int8_t)buf[pos++];
}

inline uint8_t CharBuffer::getUint8()
{
	return (uint8_t)buf[pos++];
}

inline int16_t CharBuffer::getInt16()
{
	u16buf u;
	u.b[0] = buf[pos++];
	u.b[1] = buf[pos++];
	return u.i;
}

inline uint16_t CharBuffer::getUint16()
{
	u16buf u;
	u.b[0] = buf[pos++];
	u.b[1] = buf[pos++];
	return u.ui;
}

inline int32_t CharBuffer::getInt32()
{
	u32buf u;
	u.b[0] = buf[pos++];
	u.b[1] = buf[pos++];
	u.b[2] = buf[pos++];
	u.b[3] = buf[pos++];
	return u.i;
}

inline uint32_t CharBuffer::getUint32()
{
	u32buf u;
	u.b[0] = buf[pos++];
	u.b[1] = buf[pos++];
	u.b[2] = buf[pos++];
	u.b[3] = buf[pos++];
	return u.ui;
}

inline void CharBuffer::getStr(byte * arr, const uint32_t offset, const uint32_t size)
{
	for (uint32_t i = offset; i < offset + size; i++)
	{
		arr[i] = buf[pos++];
	}
}

inline void CharBuffer::copy(char * newbuf)
{
	buf[pos] = 0;
	for (uint32_t i = 0; i <= pos; i++) {
		newbuf[i] = buf[i];
	}
}

#endif