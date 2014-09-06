#ifndef AGL_MATH_VECTOR3_HPP
#define AGL_MATH_VECTOR3_HPP

#include <cmath>
#include <ostream>

namespace Agl
{

namespace Math
{

template<typename T>
class Vector3
{

public:

	inline Vector3();
	inline Vector3(const Vector3<T>& v);
	inline Vector3(const T& x, const T& y, const T& z);

	inline void set(const Vector3<T>& v);
	inline void set(const T& x, const T& y, const T& z);

	// Miscellaneous functions
	inline T length() const;
	inline T lengthTo2() const;
	inline void normalize();
	inline Vector3<T> normalized() const;

	//inline void rotateAroundX(angle);
	//inline void rotateAroundY(angle);
	//inline void rotateAroundZ(angle);

	// Operators between Vector3s
	inline Vector3<T> operator-() const;
	inline Vector3<T> operator+(const Vector3<T>& v) const;
	inline Vector3<T> operator-(const Vector3<T>& v) const;
	inline Vector3<T> operator*(const Vector3<T>& v) const;
	inline Vector3<T>& operator+=(const Vector3<T>& v);
	inline Vector3<T>& operator-=(const Vector3<T>& v);
	inline Vector3<T>& operator*=(const Vector3<T>& v);

	// Operators with other types
	inline Vector3<T> operator*(float f) const;
	inline Vector3<T> operator/(float f) const;
	inline Vector3<T>& operator*=(float f);
	inline Vector3<T>& operator/=(float f);

	// Comparison operators
	inline bool operator==(Vector3<T> const& v) const;
	inline bool operator!=(Vector3<T> const& v) const;

	// Returns vector that is perpendicular (that is, 90 degrees) to this
	// one. The length of resulting vector might be smaller than this one,
	// but not greater.
	inline Vector3<T> perp() const;

	T x, y, z;

};

typedef Vector3< float > Vector3f;
typedef Vector3< int > Vector3i;

template<typename T>
inline std::ostream& operator<<(std::ostream& strm, Vector3<T> const& v);

// More operators with other types
template<typename T>
inline Vector3<T> operator*(float f, Vector3<T> const& v);


// ----------------------------------------
// Implementations of inline functions
// ----------------------------------------

template<typename T>
inline Vector3<T>::Vector3()
{
}

template<typename T>
inline Vector3<T>::Vector3(const Vector3<T>& v) :
	x(v.x), y(v.y), z(v.z)
{
}

template<typename T>
inline Vector3<T>::Vector3(const T& x, const T& y, const T& z) :
	x(x), y(y), z(z)
{
}

template<typename T>
inline void Vector3<T>::set(const Vector3<T>& v)
{
	x = v.x;
	y = v.y;
	z = v.z;
}

template<typename T>
inline void Vector3<T>::set(const T& x, const T& y, const T& z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

template<typename T>
inline T Vector3<T>::length() const
{
	return sqrt(x * x + y * y + z * z);
}

template<typename T>
inline T Vector3<T>::lengthTo2() const
{
	return x * x + y * y + z * z;
}

template<typename T>
inline void Vector3<T>::normalize()
{
	T len = length();
	//assert(len != 0.0, "Division by zero!");
	x /= len;
	y /= len;
	z /= len;
}

template<typename T>
inline Vector3<T> Vector3<T>::normalized() const
{
	Vector3<T> result;
	T len = length();
	//assert(len != 0.0, "Division by zero!");
	result.x = x / len;
	result.y = y / len;
	result.z = z / len;
	return result;
}

template<typename T>
inline Vector3<T> Vector3<T>::operator-() const
{
	return Vector3<T>(-x, -y, -z);
}

template<typename T>
inline Vector3<T> Vector3<T>::operator+(Vector3<T> const& v) const
{
	return Vector3<T>(x + v.x, y + v.y, z + v.z);
}

template<typename T>
inline Vector3<T> Vector3<T>::operator-(Vector3<T> const& v) const
{
	return Vector3<T>(x - v.x, y - v.y, z - v.z);
}

template<typename T>
inline Vector3<T> Vector3<T>::operator*(Vector3<T> const& v) const
{
	return Vector3<T>(x * v.x, y * v.y, z * v.z);
}

template<typename T>
inline Vector3<T>& Vector3<T>::operator+=(Vector3<T> const& v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

template<typename T>
inline Vector3<T>& Vector3<T>::operator-=(Vector3<T> const& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}

template<typename T>
inline Vector3<T>& Vector3<T>::operator*=(Vector3 const& v)
{
	x *= v.x;
	y *= v.y;
	z *= v.z;
	return *this;
}

template<typename T>
inline Vector3<T> Vector3<T>::operator*(float f) const
{
	return Vector3<T>(x * f, y * f, z * f);
}

template<typename T>
inline Vector3<T> Vector3<T>::operator/(float f) const
{
	//assert(f != 0.0, "Division by zero!");
	return Vector3<T>(x / f, y / f, z / f);
}

template<typename T>
inline Vector3<T>& Vector3<T>::operator*=(float f)
{
	x *= f;
	y *= f;
	z *= f;
	return *this;
}

template<typename T>
inline Vector3<T>& Vector3<T>::operator/=(float f)
{
	//assert(f != 0.0, "Division by zero!");
	x /= f;
	y /= f;
	z /= f;
	return *this;
}

template<typename T>
inline bool Vector3<T>::operator==(Vector3<T> const& v) const
{
	return x == v.x && y == v.y && z == v.z;
}

template<typename T>
inline bool Vector3<T>::operator!=(Vector3<T> const& v) const
{
	return x != v.x || y != v.y || z != v.z;
}

template<typename T>
inline Vector3<T> Vector3<T>::perp() const
{
	T x_abs = (x >= 0) ? x : -x;
	T y_abs = (y >= 0) ? y : -y;
	if (x_abs < y_abs) {
		return Vector3<T>(0, z, -y);
	} else {
		return Vector3<T>(-z, 0, x);
	}
}

template<typename T>
inline std::ostream& operator<<(std::ostream& strm, Vector3<T> const& v)
{
	strm << "(" << v.x << ", " << v.y << ", " << v.z << ")";
	return strm;
}

template<typename T>
inline Vector3<T> operator*(float f, Vector3<T> const& v)
{
	return Vector3<T>(f * v.x, f * v.y, f * v.z);
}

}

}

#endif
