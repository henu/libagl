#ifndef AGL_Rbuf_HPP
#define AGL_Rbuf_HPP

#include <cstring>
#include <cstdlib>
#include <stdexcept>

namespace Agl
{

template< typename T >
class Rbuf
{

public:

	inline Rbuf();
	inline ~Rbuf();

	inline void clear();
	inline bool empty() const;
	inline size_t size() const;

	inline void insert(T const* begin, T const* end);
	inline void read(T* result, size_t amount);

	inline void push(T const& t);
	inline T pop();

	inline T front() const;

	inline void swap(Rbuf< T >& rbuf);

private:

	size_t res;
	size_t items;
	T* write_pos;
	T* read_pos;
	T* buf;

	inline void ensureSpace(size_t req);

};

template< typename T >
inline Rbuf< T >::Rbuf() :
	res(0),
	items(0)
{
}

template< typename T >
inline Rbuf< T >::~Rbuf()
{
	if (res > 0) {
		delete[] buf;
	}
}

template< typename T >
inline void Rbuf< T >::clear()
{
	if (res > 0) {
		delete[] buf;
	}
	res = 0;
	items = 0;
}

template< typename T >
inline bool Rbuf< T >::empty() const
{
	return items == 0;
}

template< typename T >
inline size_t Rbuf< T >::size() const
{
	return items;
}

template< typename T >
inline void Rbuf< T >::insert(T const* begin, T const* end)
{
	//assert(end >= begin, "Invalid range!");
	if (begin == end) {
		return;
	}
	size_t add = end - begin;
	ensureSpace(items + add);
	if (write_pos + add <= buf + res) {
		memcpy(write_pos, begin, add * sizeof(T));
		write_pos += add;
	} else {
		size_t amount = buf + res - write_pos;
		memcpy(write_pos, begin, amount * sizeof(T));
		//assert(add >= amount, "Fail!");
		size_t amount2 = add - amount;
		memcpy(buf, begin + amount, amount2 * sizeof(T));
		write_pos = buf + amount2;
	}
	items += add;
}

template< typename T >
inline void Rbuf< T >::read(T* result, size_t amount)
{
	if (amount == 0) return;
	if (amount > items) {
		throw std::runtime_error("Trying to read too much!");
	}
	// If everything can be read using one copy
	if (read_pos < write_pos || size_t(buf + res - read_pos) >= amount) {
		memcpy(result, read_pos, amount * sizeof(T));
		read_pos += amount;
		if (read_pos == buf + res) {
			read_pos = buf;
		}
		items -= amount;
// TODO: Check if it would be good idea to reduce space!
	}
	// We need two read steps
	else {
		size_t first_copy_amount = buf + res - read_pos;
		memcpy(result, read_pos, first_copy_amount * sizeof(T));
		memcpy(result + first_copy_amount, buf, (amount - first_copy_amount) * sizeof(T));
		items -= amount;
		read_pos = buf + amount - first_copy_amount;
	}
}

template< typename T >
inline void Rbuf< T >::push(T const& t)
{
	ensureSpace(items + 1);
	//assert(write_pos != buf + res, "Write points to the end of buffer!");
	*write_pos = t;
	write_pos ++;
	if (write_pos == buf + res) {
		write_pos = buf;
	}
	items ++;
}

template< typename T >
inline T Rbuf< T >::pop()
{
	//assert(items > 0, "Queue is empty!");
	//assert(read_pos != buf + res, "Fail!");
	T result = *read_pos;
	read_pos ++;
	if (read_pos == buf + res) {
		read_pos = buf;
	}
	items --;
// TODO: Check if it would be good idea to reduce space!
	return result;
}

template< typename T >
inline T Rbuf< T >::front() const
{
	//assert(items > 0, "No items!");
	return *read_pos;
}

template< typename T >
inline void Rbuf< T >::swap(Rbuf< T >& rbuf)
{
	size_t swap_res = rbuf.res;
	size_t swap_items = rbuf.items;
	T* swap_write_pos = rbuf.write_pos;
	T* swap_read_pos = rbuf.read_pos;
	T* swap_buf = rbuf.buf;

	rbuf.res = res;
	rbuf.items = items;
	rbuf.write_pos = write_pos;
	rbuf.read_pos = read_pos;
	rbuf.buf = buf;

	res = swap_res;
	items = swap_items;
	write_pos = swap_write_pos;
	read_pos = swap_read_pos;
	buf = swap_buf;
}

template< typename T >
inline void Rbuf< T >::ensureSpace(size_t req)
{
	if (res >= req) {
		return;
	}
	req *= 2;
	T* newbuf = new T[req];
	if (items > 0) {
		if (read_pos < write_pos || write_pos == buf) {
			//assert(read_pos + items <= buf + res, "Overflow!");
			memcpy(newbuf, read_pos, items * sizeof(T));
		} else {
			size_t amount = buf + res - read_pos;
			memcpy(newbuf, read_pos, amount * sizeof(T));
			//assert(items >= amount, "Fail!");
			size_t amount2 = items - amount;
			memcpy(newbuf + amount, buf, amount2 * sizeof(T));
		}
	}
	if (res > 0) {
		delete[] buf;
	}
	res = req;
	buf = newbuf;
	read_pos = newbuf;
	write_pos = newbuf + items;
}

}

#endif
