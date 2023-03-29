#include "Position.h"

Position::Position() : x(), y(), z()
{
}

Position::~Position()
{
}

Position Position::operator +(DirectX::XMFLOAT3 fl)
{
	Position pos;
	pos.x = this->x + fl.x;
	pos.y = this->y + fl.y;
	pos.z = this->z + fl.z;
	return pos;
}

Position Position::operator +(Position fl)
{
	Position pos;
	pos.x = this->x + fl.x;
	pos.y = this->y + fl.y;
	pos.z = this->z + fl.z;
	return pos;
}

Position Position::operator -(DirectX::XMFLOAT3 fl)
{
	Position pos;
	pos.x = this->x - fl.x;
	pos.y = this->y - fl.y;
	pos.z = this->z - fl.z;
	return pos;
}

Position Position::operator -(Position fl)
{
	Position pos;
	pos.x = this->x - fl.x;
	pos.y = this->y - fl.y;
	pos.z = this->z - fl.z;
	return pos;
}

Position& Position::operator =(const DirectX::XMFLOAT3 fl)
{
	x = fl.x;
	y = fl.y;
	z = fl.z;
	return *this;
}

Position& Position::operator =(const Position& fl)
{
	x = fl.x;
	y = fl.y;
	z = fl.z;
	return *this;
}

Position& Position::operator +=(DirectX::XMFLOAT3 fl)
{
	x += fl.x;
	y += fl.y;
	z += fl.z;
	return *this;
}

Position& Position::operator +=(Position& fl)
{
	x += fl.x;
	y += fl.y;
	z += fl.z;
	return *this;
}

Position& Position::operator -=(DirectX::XMFLOAT3 fl)
{
	x -= fl.x;
	y -= fl.y;
	z -= fl.z;
	return *this;
}

Position& Position::operator -=(Position fl)
{
	x -= fl.x;
	y -= fl.y;
	z -= fl.z;
	return *this;
}

bool Position::operator ==(const DirectX::XMFLOAT3 fl) const
{
	return (x == fl.x && y == fl.y && z == fl.z);
}

bool Position::operator ==(const Position& fl) const
{
	return (x == fl.x && y == fl.y && z == fl.z);
}

bool Position::operator !=(DirectX::XMFLOAT3 fl) const
{
	return !(*this == fl);
}

bool Position::operator !=(Position fl) const
{
	return !(*this == fl);
}

Position::operator DirectX::XMFLOAT3() const
{
	return DirectX::XMFLOAT3(x, y, z);
}
