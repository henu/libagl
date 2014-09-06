#ifndef AGL_MATH_VECTOR2_HPP
#define AGL_MATH_VECTOR2_HPP

#include <cmath>
#include <ostream>

namespace Agl
{

namespace Math
{

template<typename T>
class Vector2
{

public:

	inline Vector2();
	inline Vector2(const Vector2<T>& v);
	inline Vector2(const T& x, const T& y);

	inline void set(const Vector2<T>& v);
	inline void set(const T& x, const T& y);

	// Miscellaneous functions
	inline T length() const;
	inline T lengthTo2() const;
	inline void normalize();
	inline Vector2<T> normalized() const;

	//inline void rotateAroundX(angle);
	//inline void rotateAroundY(angle);
	//inline void rotateAroundZ(angle);

	// Operators between Vector2s
	inline Vector2<T> operator-() const;
	inline Vector2<T> operator+(const Vector2<T>& v) const;
	inline Vector2<T> operator-(const Vector2<T>& v) const;
	inline Vector2<T> operator*(const Vector2<T>& v) const;
	inline Vector2<T>& operator+=(const Vector2<T>& v);
	inline Vector2<T>& operator-=(const Vector2<T>& v);
	inline Vector2<T>& operator*=(const Vector2<T>& v);

	// Operators with other types
	inline Vector2<T> operator*(float f) const;
	inline Vector2<T> operator/(float f) const;
	inline Vector2<T>& operator*=(float f);
	inline Vector2<T>& operator/=(float f);

	// Comparison operators
	inline bool operator==(Vector2<T> const& v) const;
	inline bool operator!=(Vector2<T> const& v) const;

	// Returns vector that is perpendicular (that is, 90 degrees) to this
	// one. Basically result is this vector that is rotated 90 degrees
	// counter clockwise when X axis is right and Y axis up.
	inline Vector2<T> perp() const;

	T x, y;

};

typedef Vector2< float > Vector2f;
typedef Vector2< int > Vector2i;

template<typename T>
inline std::ostream& operator<<(std::ostream& strm, Vector2<T> const& v);

// More operators with other types
template<typename T>
inline Vector2<T> operator*(float f, Vector2<T> const& v);


// ----------------------------------------
// Implementations of inline functions
// ----------------------------------------

template<typename T>
inline Vector2<T>::Vector2()
{
}

template<typename T>
inline Vector2<T>::Vector2(const Vector2<T>& v) :
	x(v.x), y(v.y)
{
}

template<typename T>
inline Vector2<T>::Vector2(const T& x, const T& y) :
	x(x), y(y)
{
}

template<typename T>
inline void Vector2<T>::set(const Vector2<T>& v)
{
	x = v.x;
	y = v.y;
}

template<typename T>
inline void Vector2<T>::set(const T& x, const T& y)
{
	this->x = x;
	this->y = y;
}

template<typename T>
inline T Vector2<T>::length() const
{
	return sqrt(x * x + y * y);
}

template<typename T>
inline T Vector2<T>::lengthTo2() const
{
	return x * x + y * y;
}

template<typename T>
inline void Vector2<T>::normalize()
{
	T len = length();
	//assert(len != 0.0, "Division by zero!");
	x /= len;
	y /= len;
}

template<typename T>
inline Vector2<T> Vector2<T>::normalized() const
{
	Vector2<T> result;
	T len = length();
	//assert(len != 0.0, "Division by zero!");
	result.x = x / len;
	result.y = y / len;
	return result;
}

template<typename T>
inline Vector2<T> Vector2<T>::operator-() const
{
	return Vector2<T>(-x, -y);
}

template<typename T>
inline Vector2<T> Vector2<T>::operator+(Vector2<T> const& v) const
{
	return Vector2<T>(x + v.x, y + v.y);
}

template<typename T>
inline Vector2<T> Vector2<T>::operator-(Vector2<T> const& v) const
{
	return Vector2<T>(x - v.x, y - v.y);
}

template<typename T>
inline Vector2<T> Vector2<T>::operator*(Vector2<T> const& v) const
{
	return Vector2<T>(x * v.x, y * v.y);
}

template<typename T>
inline Vector2<T>& Vector2<T>::operator+=(Vector2<T> const& v)
{
	x += v.x;
	y += v.y;
	return *this;
}

template<typename T>
inline Vector2<T>& Vector2<T>::operator-=(Vector2<T> const& v)
{
	x -= v.x;
	y -= v.y;
	return *this;
}

template<typename T>
inline Vector2<T>& Vector2<T>::operator*=(Vector2 const& v)
{
	x *= v.x;
	y *= v.y;
	return *this;
}

template<typename T>
inline Vector2<T> Vector2<T>::operator*(float f) const
{
	return Vector2<T>(x * f, y * f);
}

template<typename T>
inline Vector2<T> Vector2<T>::operator/(float f) const
{
	//assert(f != 0.0, "Division by zero!");
	return Vector2<T>(x / f, y / f);
}

template<typename T>
inline Vector2<T>& Vector2<T>::operator*=(float f)
{
	x *= f;
	y *= f;
	return *this;
}

template<typename T>
inline Vector2<T>& Vector2<T>::operator/=(float f)
{
	//assert(f != 0.0, "Division by zero!");
	x /= f;
	y /= f;
	return *this;
}

template<typename T>
inline bool Vector2<T>::operator==(Vector2<T> const& v) const
{
	return x == v.x && y == v.y;
}

template<typename T>
inline bool Vector2<T>::operator!=(Vector2<T> const& v) const
{
	return x != v.x || y != v.y;
}

template<typename T>
inline Vector2<T> Vector2<T>::perp() const
{
	return Vector2<T>(-y, x);
}

template<typename T>
inline std::ostream& operator<<(std::ostream& strm, Vector2<T> const& v)
{
	strm << "(" << v.x << ", " << v.y << ")";
	return strm;
}

template<typename T>
inline Vector2<T> operator*(float f, Vector2<T> const& v)
{
	return Vector2<T>(f * v.x, f * v.y);
}

}

}

#endif
