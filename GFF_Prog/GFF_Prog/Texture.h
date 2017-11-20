#pragma once

#include "Direct3D.h"

class Texture
{
	friend class Direct3D;
private:
	IDirect3DTexture9* pTexture;

	unsigned int divU;
	unsigned int divV;
	unsigned int numU;
	unsigned int numV;

public:
	Texture();
	~Texture();

	void Release();

	bool Load(TCHAR* FilePath);

	void SetDivide(unsigned int DivU, unsigned int DivV);

	void SetNum(unsigned int NumU, unsigned int NumV);

};