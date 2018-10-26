#ifndef _BUFFER_h
#define _BUFFER_h

#include <Arduino.h>

class  ByteBuffer {
private:
	ByteBuffer(ByteBuffer &) = delete;
	uint8_t size;
	bool outer = false;
	byte * buf;
	uint8_t pos_w = 0;
	uint8_t pos_r = 0;
	uint8_t lim = 64;

protected:
public:
	union int16u {
		byte bytes[2];
		int16_t int16;
		uint16_t uint16;
	};

	union int32u {
		byte bytes[4];
		int32_t int32;
		uint32_t uint32;
	};

public:
	ByteBuffer(uint8_t size);
	ByteBuffer(byte * buf, uint8_t size);
	~ByteBuffer();

	inline void setBuffer(byte * buf, uint8_t size);
	inline void resize(uint8_t size);
	inline void newInterBuffer(uint8_t size);

	inline void write(byte b);
	inline void write(byte* bytes, uint8_t offset, uint8_t size);
	inline void writeStr(const char* msg);

	inline void rewind();
	inline void reset();

	inline uint8_t available();
	inline byte getByte();
	inline int8_t getInt8();
	inline uint8_t getUInt8();
	inline int16_t getInt16();
	inline uint16_t getUInt16();
	inline int32_t getInt32();
	inline uint32_t getUInt32();

	inline void getBytes(byte* arr, int16_t offset, int8_t size);

	inline void write_to(Stream& out);
	inline void write_left_to(Stream& out);

	inline void print_to(Print& out);
	inline void print_to(Print& out, char spl);
};

inline  ByteBuffer::ByteBuffer(uint8_t size) : buf(new byte[size]), size(size)
{
}

inline ByteBuffer::ByteBuffer(byte * buf, uint8_t size) : buf(buf), size(size), outer(true)
{
}

inline  ByteBuffer::~ByteBuffer()
{
	delete[] buf;
}

inline void ByteBuffer::setBuffer(byte * buf, uint8_t size)
{
	if (!outer) {
		delete[] buf;
		outer = true;
	}
	this->buf = buf;
	this->size = size;
}

inline void ByteBuffer::resize(uint8_t size)
{
	if (!outer) {
		delete[] buf;
		this->size = size;
		buf = new byte[size];
	}
}

inline void ByteBuffer::newInterBuffer(uint8_t size)
{
	if (!outer) {
		delete[] buf;
		outer = false;
	}
	buf = new byte[size];
}

inline void ByteBuffer::write(byte b)
{
	if (pos_w < size) {
		buf[pos_w] = b;
		pos_w++;
	}
}

inline void ByteBuffer::write(byte * bytes, uint8_t offset, uint8_t size)
{
	if (size - pos_w >= size) {
		for (uint8_t i = 0; i < size; i++)
		{
			buf[pos_w] = bytes[i];
			pos_w++;
		}
	}
}

inline void ByteBuffer::writeStr(const char * msg)
{
	char c = msg[0];
	for (uint8_t i = 0; c != 0; i++) {
		c = msg[i];

		write(c);
	}
}

inline void ByteBuffer::rewind()
{
	pos_r = 0;
}

inline void ByteBuffer::reset()
{
	pos_w = 0;
	pos_r = 0;
}

inline uint8_t ByteBuffer::available()
{
	return pos_w - pos_r;
}

inline byte ByteBuffer::getByte()
{
	return buf[pos_r++];
}

inline int8_t ByteBuffer::getInt8()
{
	return buf[pos_r++];
}

inline uint8_t ByteBuffer::getUInt8()
{
	return buf[pos_r++];
}

inline int16_t ByteBuffer::getInt16()
{
	int16u a;
	a.bytes[0] = getByte();
	a.bytes[1] = getByte();
	return a.int16;
}

inline uint16_t ByteBuffer::getUInt16()
{
	int16u a;
	a.bytes[0] = getByte();
	a.bytes[1] = getByte();
	return a.uint16;
}

inline int32_t ByteBuffer::getInt32()
{
	int32u a;
	a.bytes[0] = getByte();
	a.bytes[1] = getByte();
	a.bytes[2] = getByte();
	a.bytes[3] = getByte();
	return a.int32;
}

inline uint32_t ByteBuffer::getUInt32()
{
	int32u a;
	a.bytes[0] = getByte();
	a.bytes[1] = getByte();
	a.bytes[2] = getByte();
	a.bytes[3] = getByte();
	return a.uint32;
}

inline void ByteBuffer::getBytes(byte * arr, const int16_t offset, const int8_t size)
{
	for (int8_t i = offset; i < offset + size; i++)
	{
		arr[i] = buf[pos_r++];
	}
}

inline void ByteBuffer::write_to(Stream & out)
{
	out.write(buf, pos_w);
}

inline void ByteBuffer::write_left_to(Stream & out)
{
	for (; pos_r < pos_w;)
	{
		out.write(buf[pos_r++]);
	}
}

inline void ByteBuffer::print_to(Print& out)
{
	for (uint8_t i = 0; i < pos_w; i++)
	{
		out.print(buf[i]);
	}
}

inline void ByteBuffer::print_to(Print & out, char spl)
{
	if (pos_w > 0) {
		out.print(buf[0]);
	}
	for (uint8_t i = 1; i < pos_w; i++)
	{
		out.print(spl);
		out.print(buf[i]);
	}
}

#endif

