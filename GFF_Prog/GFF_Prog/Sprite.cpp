#include "Sprite.h"

Sprite::Sprite()
{
	pos.x = pos.y = 0.0f;
	width = height = 0.0f;

	angle = 0.0f;

	alpha = 1.0f;
}

Sprite::~Sprite()
{

}

void Sprite::SetPos(float x, float y)
{
	pos.x = x;
	pos.y = y;
}

void Sprite::SetSize(float width, float height)
{
	this->width = width;
	this->height = height;
}

void Sprite::SetAngle(float rad)
{
	angle = rad;
}

void Sprite::SetAlpha(float a)
{
	if (a <= 0.0f)
	{
		alpha = 0.0f;
	}
	else if (a >= 1.0f)
	{
		alpha = 1;
	}
	else
	{
		alpha = a;
	}
}

void Sprite::Draw(Texture& tex)
{
	Direct3D& d3d = Direct3D::GetInstance();

	d3d.DrawSprite(*this, tex);
}