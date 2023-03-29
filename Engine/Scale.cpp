#include "Scale.h"

Scale::Scale() : x(1), y(1), z(1)
{
}

Scale::~Scale()
{
}

Scale Scale::operator*(const float i)
{
	Scale sc;
	sc.x = this->x * i;
	sc.y = this->y * i;
	sc.z = this->z * i;
	return sc;
}

Scale Scale::operator/(const float i)
{
	Scale sc;
	sc.x = this->x / i;
	sc.y = this->y / i;
	sc.z = this->z / i;
	return sc;
}

Scale& Scale::operator=(const DirectX::XMFLOAT3 fl)
{
	x = fl.x;
	y = fl.y;
	z = fl.z;
	return *this;
}

Scale& Scale::operator=(const Scale& fl)
{
	x = fl.x;
	y = fl.y;
	z = fl.z;
	return *this;
}

Scale& Scale::operator=(const float i)
{
	x = i;
	y = i;
	z = i;
	return *this;
}

Scale Scale::operator*=(const float i)
{
	x *= i;
	y *= i;
	z *= i;
	return *this;
}

Scale Scale::operator/=(const float i)
{
	x /= i;
	y /= i;
	z /= i;
	return *this;
}

bool Scale::operator==(const DirectX::XMFLOAT3 fl) const
{
	return (x == fl.x && y == fl.y && z == fl.z);
}

bool Scale::operator==(const Scale& fl) const
{
	return (x == fl.x && y == fl.y && z == fl.z);
}

bool Scale::operator!=(const DirectX::XMFLOAT3 fl) const
{
	return !(*this == fl);
}

bool Scale::operator!=(const Scale& fl) const
{
	return !(*this == fl);
}

Scale::operator DirectX::XMFLOAT3() const
{
	return DirectX::XMFLOAT3(x, y, z);
}
