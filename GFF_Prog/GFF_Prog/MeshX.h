#pragma once

#include "Direct3D.h"

#include "string"

class MeshX
{
	friend class Direct3D;
private:
	LPD3DXMESH pMesh;
	DWORD numMaterials;

	LPDIRECT3DTEXTURE9* ppTextures;
	D3DMATERIAL9* pMaterials;

public:
	MeshX();
	~MeshX();

	void Draw(D3DXMATRIXA16& matTransform,
		D3DXMATRIXA16& matRotation,
		D3DXMATRIXA16& matScale);

	bool Load(TCHAR* path);
	bool Load(std::string path);
};