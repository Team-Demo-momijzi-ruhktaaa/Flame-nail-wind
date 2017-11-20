#include "Texture.h"

Texture::Texture()
{
	pTexture = nullptr;

	divU = divV = 1;

	numU = numV = 0;
}

Texture::~Texture()
{
	Release();
}

void Texture::Release()
{
	if (pTexture != nullptr)
	{
		pTexture->Release();

		pTexture = nullptr;

		divU = divV = 1;
		numU = numV = 0;
	}
}

bool Texture::Load(TCHAR* FilePath)
{
	Direct3D &d3d = Direct3D::GetInstance();

	return d3d.LoadTexture((*this), FilePath);

}

void Texture::SetDivide(unsigned int DivU, unsigned int DivV)
{
	divU = (DivU != 0) ? DivU : 1;
	divV = (DivV != 0) ? DivV : 1;

	numU = (numU >= divU) ? 0 : numU;
	numV = (numV >= divV) ? 0 : numV;
}

void Texture::SetNum(unsigned int NumU, unsigned int NumV)
{
	numU = (numU >= divU) ? divU - 1 : NumU;
	numV = (numV >= divV) ? divV - 1 : NumV;
}