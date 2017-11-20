#include "MeshX.h"
#include <tchar.h>

MeshX::MeshX()
{
	pMesh = nullptr;
	ppTextures = nullptr;
	pMaterials = nullptr;
	numMaterials = 0;
}

MeshX::~MeshX()
{
	if (ppTextures)
	{
		for (unsigned int i = 0; i < numMaterials; i++)
		{
			if (ppTextures[i])
			{
				ppTextures[i]->Release();
			}
		}
		delete[] ppTextures;
	}

	if (pMaterials)
	{
		delete[] pMaterials;
	}

	if (pMesh)
	{
		pMesh->Release();
	}
}

bool MeshX::Load(TCHAR* path)
{
	Direct3D& pD3d = Direct3D::GetInstance();

	return pD3d.LoadMeshX(*this, path);
}

bool MeshX::Load(std::string path)
{
	const TCHAR* path_t = _T(path.c_str());

	return Load(path_t);
}

void MeshX::Draw(D3DXMATRIXA16& matTransform,
	D3DXMATRIXA16& matRotation,
	D3DXMATRIXA16& matScale)
{
	D3DXMATRIXA16 matWorld;

	D3DXMatrixIdentity(&matWorld);

	D3DXMATRIXA16 matTmp;
	D3DXMatrixIdentity(&matTmp);

	D3DXMatrixMultiply(&matTmp, &matScale, &matRotation);

	D3DXMatrixMultiply(&matWorld, &matTmp, &matTransform);

	Direct3D& d3d = Direct3D::GetInstance();

	d3d.DrawMeshX(*this, matWorld);
}