#pragma once

template <class T>
class Vector2
{
private:
	T x;
	T y;

public:
	
	Vector2()
	{

	}

	void Set(T x_, T y_)
	{
		x = x_;
		y = y_;
	}
	T& X() { return x; }
	T& Y() { return y; }

	const Vector2<T> operator+(const Vector2<T>& other)const
	{
		Vector2<T> tmp;
		tmp.Set(x + other.x, y + other.y);
		return tmp;
	}

	const Vector2<T> operator-(const Vector2<T>& other)const
	{
		Vector2<T> tmp;
		tmp.Set(x - other.x, y - other.y);
		return tmp;
	}

	const Vector2<T> operator+=(const Vector2<T>& other)
	{
		x += other.x;
		y += other.y;

		return *(this);
	}

	const Vector2<T> operator-=(const Vector2<T>& other)
	{
		x -= other.x;
		y -= other.y;

		return *(this);
	}

	const bool operator==(const Vector2<T>& other)const
	{
		return (x == other.x&&y == other.y);
	}
};