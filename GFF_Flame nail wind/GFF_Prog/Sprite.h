#pragma once

#include "Direct3D.h"

class Texture;

class Sprite
{
	friend class Direct3D;

private:
	D3DXVECTOR2 pos;
	int width;
	int height;

	float angle;

	float alpha;

public:
	Sprite();
	virtual ~Sprite();

	void SetPos(float x, float y);

	void SetSize(float width, float height);

	void SetAngle(float rad);

	void SetAlpha(float alpha);

	void Draw(Texture& tex);

	D3DXVECTOR2 GetCenterPos() { return pos; }

	float GetWidth() { return width; }

	float GetHeight() { return height; }

	float GetAngle_Rad() { return angle; }

	float GetAlpha() { return alpha; }
};