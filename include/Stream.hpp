#ifndef AGL_STREAM_HPP
#define AGL_STREAM_HPP

#include "Bytes.hpp"
#include "Rbuf.hpp"

#include <string>
#include <stdexcept>
#include <cstring>
#include <stdint.h>

namespace Agl
{

class Stream
{

public:

	class StreamInputClosed : public std::runtime_error
	{
	public:
		inline StreamInputClosed() : std::runtime_error("Stream input already closed!") { }
		inline virtual ~StreamInputClosed() throw () { }
		inline virtual const char* what() const throw () { return "Stream input already closed!"; }
	};

	inline Stream();
	inline ~Stream();

	inline void push(const Bytes& bytes);
	inline void push(const std::string& str);
	inline void push(const char* bytes, uint64_t size);

	// Informs stream, that all data is got. No more data will be pushed.
	inline void setEndOfData();

	// Functions to read data that Stream has processed
	inline Bytes readBytes(size_t limit = 0);
	inline std::string readString(size_t limit = 0);

protected:

	// Reads chunk from input data. If limit
	// is not given, then everything is read.
	// Data is written to the end of "result".
	inline void readInputData(Bytes& result, size_t limit = 0);

	// Writes to outputdata
	inline void writeOutputData(uint8_t* begin, uint8_t* end);

private:

	Rbuf< uint8_t > input;
	Rbuf< uint8_t > output;

	bool end_of_data;

	// Ensures there is specific amount of unused space in ring buffer
	inline void ensureEmptySpace(uint64_t size);

	// This virtual function informs subclass when there is
	// new data available, or when end of data has been set.
	virtual void newDataAvailable(uint64_t amount, bool end_of_data) = 0;

};

inline Stream::Stream() :
	end_of_data(false)
{
}

inline Stream::~Stream()
{
}

inline void Stream::push(const Bytes& bytes)
{
	push((const char*)&bytes[0], bytes.size());
}

inline void Stream::push(const std::string& str)
{
	push(str.c_str(), str.size());
}

inline void Stream::push(const char* bytes, uint64_t size)
{
	if (end_of_data) throw StreamInputClosed();

	input.insert((uint8_t*)bytes, (uint8_t*)bytes + size);

	newDataAvailable(input.size(), false);
}

inline void Stream::setEndOfData()
{
	if (end_of_data) throw StreamInputClosed();
	end_of_data = true;

	newDataAvailable(input.size(), true);
}

inline Bytes Stream::readBytes(size_t limit)
{
	size_t amount_to_copy;
	if (limit == 0 || limit > output.size()) amount_to_copy = output.size();
	else amount_to_copy = limit;

	Bytes result(amount_to_copy, 0);
	output.read(&result[0], amount_to_copy);
	return result;
}

inline std::string Stream::readString(size_t limit)
{
	size_t amount_to_copy;
	if (limit == 0 || limit > output.size()) amount_to_copy = output.size();
	else amount_to_copy = limit;

	std::string result(amount_to_copy, ' ');
	output.read((uint8_t*)&result[0], amount_to_copy);
	return result;
}

inline void Stream::readInputData(Bytes& result, size_t limit)
{
	size_t amount_to_copy;
	if (limit == 0 || limit > input.size()) amount_to_copy = input.size();
	else amount_to_copy = limit;

	uint8_t* readbuf = new uint8_t[amount_to_copy];
	input.read(readbuf, amount_to_copy);
	result.insert(result.end(), readbuf, readbuf + amount_to_copy);
	delete[] readbuf;
}

inline void Stream::writeOutputData(uint8_t* begin, uint8_t* end)
{
	output.insert(begin, end);
}

}

#endif
