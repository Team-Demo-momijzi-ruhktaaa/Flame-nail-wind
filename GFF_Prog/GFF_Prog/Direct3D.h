#pragma once

#include <d3d9.h>
#include <d3dx9.h>

#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"d3dxof.lib")
#pragma comment(lib,"dxguid.lib")

#include <windows.h>

#pragma comment(lib,"winmm.lib")

#include <tchar.h>

struct SpriteVertex
{
	D3DXVECTOR3 pos;

	float rhw;
	
	D3DCOLOR color;

	float u, v;
};

static const DWORD SPRITE_FVF
= D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1;

enum RENDERSTATE
{
	RENDER_DEFAULT,
	RENDER_ALPHATEST,
	RENDER_ALPHABLEND,
	RENDER_MESH_X
};

class Sprite;

class Texture;

class MeshX;

class Direct3D
{
private:

	Direct3D();
	~Direct3D();

	Direct3D(const Direct3D& obj) {};
	void operator=(const Direct3D& obj) {};

	static Direct3D* pInstance;

public:

	static Direct3D& GetInstance()
	{
		if (pInstance == nullptr)
		{
			pInstance = new Direct3D();
		}
		return *pInstance;
	}

	static void DestroyInstance()
	{
		if (pInstance != nullptr)
		{
			delete pInstance;
			pInstance = nullptr;
		}
	}

	void Release();

	void SetRenderState(RENDERSTATE state);

private:

	IDirect3DDevice9*pDevice3D;

	IDirect3D9* pD3D9;

	HWND hWnd;

private:
	bool Create(HWND hWnd);

public:

	bool TryCreate(HWND hWnd);

	HRESULT BeginScene();

	HRESULT EndScene();

	HRESULT ClearScreen();

	HRESULT Present();

	void SetViewMatrix(D3DXMATRIXA16& matView);

	void SetProjectionMatrixs();

	bool LoadTexture(Texture& tex, TCHAR*FilePath);

	void DrawSprite(Sprite& sprite, Texture& tex);

	bool LoadMeshX(MeshX& mesh, TCHAR*path);

	void DrawMeshX(MeshX& mesh, D3DXMATRIXA16& matWorld);
};