#ifndef AGL_BYTES_HPP
#define AGL_BYTES_HPP

#include <vector>
#include <string>
#include <stdint.h>

namespace Agl
{

typedef std::vector< uint8_t > Bytes;

}

// Add operators
inline Agl::Bytes operator+(const Agl::Bytes& v0, const Agl::Bytes& v1);
inline Agl::Bytes operator+(const Agl::Bytes& v, const std::string& s);
inline Agl::Bytes operator+(const std::string& s, const Agl::Bytes& v);
inline Agl::Bytes operator+=(Agl::Bytes& v0, const Agl::Bytes& v1);
inline Agl::Bytes operator+=(Agl::Bytes& v, const std::string& s);

inline Agl::Bytes operator+(const Agl::Bytes& v0, const Agl::Bytes& v1)
{
	Agl::Bytes new_v;
	new_v.reserve(v0.size() + v1.size());
	new_v.insert(new_v.end(), v0.begin(), v0.end());
	new_v.insert(new_v.end(), v1.begin(), v1.end());
	return new_v;
}

inline Agl::Bytes operator+(const Agl::Bytes& v, const std::string& s)
{
	Agl::Bytes new_v;
	new_v.reserve(v.size() + s.size());
	new_v.insert(new_v.end(), v.begin(), v.end());
	new_v.insert(new_v.end(), s.begin(), s.end());
	return new_v;
}

inline Agl::Bytes operator+(const std::string& s, const Agl::Bytes& v)
{
	Agl::Bytes new_v;
	new_v.reserve(s.size() + v.size());
	new_v.insert(new_v.end(), s.begin(), s.end());
	new_v.insert(new_v.end(), v.begin(), v.end());
	return new_v;
}

inline Agl::Bytes operator+=(Agl::Bytes& v0, const Agl::Bytes& v1)
{
	v0.insert(v0.end(), v1.begin(), v1.end());
	return v0;
}

inline Agl::Bytes operator+=(Agl::Bytes& v, const std::string& s)
{
	v.insert(v.end(), s.begin(), s.end());
	return v;
}

#endif
