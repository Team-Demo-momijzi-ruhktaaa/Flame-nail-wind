#include "Direct3D.h"

#include "Sprite.h"
#include "Texture.h"
#include "MeshX.h"

Direct3D* Direct3D::pInstance = nullptr;

Direct3D::Direct3D()
{
	pD3D9 = NULL;
	pDevice3D = NULL;
}

Direct3D::~Direct3D()
{
	Release();
}

void Direct3D::Release()
{
	if (pDevice3D != NULL)
	{
		pDevice3D->Release();
		pD3D9->Release();

		pDevice3D = NULL;
		pD3D9 = NULL;
	}
}

bool Direct3D::TryCreate(HWND hWnd)
{
	if (pDevice3D != nullptr)
	{
		return false;
	}

	if (!Create(hWnd))
	{
		return false;
	}

	return true;
}

bool Direct3D::Create(HWND hWnd)
{
	RECT rect;
	GetClientRect(hWnd, &rect);

	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;

	pD3D9 = Direct3DCreate9(D3D_SDK_VERSION);

	D3DDISPLAYMODE Display;
	pD3D9->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &Display);

	D3DPRESENT_PARAMETERS D3DParam;
	D3DParam.BackBufferWidth = width;
	D3DParam.BackBufferHeight = height;
	D3DParam.BackBufferFormat = Display.Format;

	D3DParam.BackBufferCount = 1;
	D3DParam.MultiSampleType = D3DMULTISAMPLE_NONE;

	D3DParam.MultiSampleQuality = 0;
	D3DParam.SwapEffect = D3DSWAPEFFECT_DISCARD;
	D3DParam.hDeviceWindow = hWnd;
	D3DParam.Windowed = TRUE;
	D3DParam.EnableAutoDepthStencil = TRUE;

	D3DParam.AutoDepthStencilFormat = D3DFMT_D24S8;
	D3DParam.Flags = 0;
	D3DParam.FullScreen_RefreshRateInHz = 0;

	D3DParam.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;

	if (FAILED(pD3D9->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		D3DParam.hDeviceWindow,
		D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,
		&D3DParam, &pDevice3D)))
	{
		if (FAILED(pD3D9->CreateDevice(
			D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			D3DParam.hDeviceWindow,
			D3DCREATE_MIXED_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,
			&D3DParam, &pDevice3D)))
		{
			if (FAILED(pD3D9->CreateDevice(
				D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				D3DParam.hDeviceWindow,
				D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,
				&D3DParam, &pDevice3D)))
			{
				if (FAILED(pD3D9->CreateDevice(
					D3DADAPTER_DEFAULT,
					D3DDEVTYPE_REF,
					D3DParam.hDeviceWindow,
					D3DCREATE_MIXED_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,
					&D3DParam, &pDevice3D)))
				{

					pD3D9->Release();
					pD3D9 = NULL;
					pDevice3D = NULL;

					return false;
				}
			}
		}
	}

	this->hWnd = hWnd;

	return true;
}

void Direct3D::SetRenderState(RENDERSTATE state)
{
	if (pDevice3D == nullptr)return;

	switch (state)
	{
	case RENDER_DEFAULT:
		pDevice3D->SetRenderState(D3DRS_ALPHATESTENABLE, false);
		pDevice3D->SetRenderState(D3DRS_ALPHABLENDENABLE, false);

		break;
	case RENDER_ALPHATEST:
		pDevice3D->SetRenderState(D3DRS_ALPHATESTENABLE, true);
		pDevice3D->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
		pDevice3D->SetRenderState(D3DRS_ALPHAREF, 0x80);
		pDevice3D->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);

		break;
	case RENDER_ALPHABLEND:

		pDevice3D->SetRenderState(D3DRS_ALPHATESTENABLE, false);
		pDevice3D->SetRenderState(D3DRS_ALPHABLENDENABLE, false);

		pDevice3D->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice3D->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		break;
	case RENDER_MESH_X:

		SetProjectionMatrixs();

		pDevice3D->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
		pDevice3D->SetRenderState(D3DRS_LIGHTING, TRUE);

		pDevice3D->SetRenderState(D3DRS_ZENABLE, TRUE);

		D3DLIGHT9 light;
		ZeroMemory(&light, sizeof(D3DLIGHT9));
		light.Type = D3DLIGHT_DIRECTIONAL;
		light.Diffuse.r = 1.0f;
		light.Diffuse.g = 1.0f;
		light.Diffuse.b = 1.0f;
		light.Direction = D3DXVECTOR3(-0.5f, -1.0f, 0.5f);
		light.Range = 1000.0f;

		pDevice3D->SetLight(0, &light);

		ZeroMemory(&light, sizeof(D3DLIGHT9));
		light.Type = D3DLIGHT_DIRECTIONAL;
		light.Diffuse.r = 1.0f;
		light.Diffuse.g = 1.0f;
		light.Diffuse.b = 1.0f;
		light.Direction = D3DXVECTOR3(0.5f, -1.0f, 0.5f);
		light.Range = 1000.0f;

		pDevice3D->SetLight(1, &light);

		pDevice3D->LightEnable(0, TRUE);
		pDevice3D->LightEnable(1, TRUE);

		pDevice3D->SetRenderState(D3DRS_AMBIENT, 0x00444444);

	}
}

HRESULT Direct3D::BeginScene()
{
	if (pDevice3D != nullptr)
	{
		return pDevice3D->BeginScene();
	}
	else
	{
		return S_FALSE;
	}
}

HRESULT Direct3D::EndScene()
{
	if (pDevice3D != nullptr)
	{
		return pDevice3D->EndScene();
	}
	else
	{
		return S_FALSE;
	}
}

HRESULT Direct3D::ClearScreen()
{
	if (pDevice3D != nullptr)
	{
		D3DCOLOR Color = D3DCOLOR_XRGB(0, 0, 0);

		return pDevice3D->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_STENCIL | D3DCLEAR_ZBUFFER, Color, 1.0f, 0);
	}
	else
	{
		return S_FALSE;
	}
}

HRESULT Direct3D::Present()
{
	if (pDevice3D != nullptr)
	{
		return pDevice3D->Present(NULL, NULL, NULL, NULL);
	}
	else
	{
		return S_FALSE;
	}
}

void Direct3D::DrawSprite(Sprite& sprite, Texture& tex)
{
	if (pDevice3D == nullptr)return;

	D3DXVECTOR2 pos = sprite.GetCenterPos();
	float width = sprite.GetWidth();
	float height = sprite.GetHeight();
	float alpha = sprite.GetAlpha();

	char alphaC = static_cast<char>(255 * alpha);

	float u = tex.numU;
	float v = tex.numV;
	float du = tex.divU;
	float dv = tex.divV;

	SpriteVertex vertex[4] =
	{
		{D3DXVECTOR3(width / 2,-height / 2,0),1.0f,D3DCOLOR_RGBA(255,255,255,alphaC),(u + 1) / du,v / dv},
		{D3DXVECTOR3(width / 2,height / 2,0),1.0f,D3DCOLOR_RGBA(255,255,255,alphaC),(u + 1) / du,(v + 1) / du},
		{D3DXVECTOR3(-width / 2,-height / 2,0),1.0f,D3DCOLOR_RGBA(255,255,255,alphaC,u / du,v / dv)},
		{D3DXVECTOR3(-width / 2,height / 2,0),1.0f,D3DCOLOR_RGBA(255,255,255,alphaC),u / du,(v + 1) / dv},
	};

	float angle = sprite.GetAngle_Rad();

	for (int i = 0; i < 4; i++)
	{
		float x = vertex[i].pos.x*cosf(angle) - vertex[i].pos.y*sinf(angle);

		float y = vertex[i].pos.x*sinf(angle) + vertex[i].pos.y*cosf(angle);

		D3DXVECTOR3 vPos;
		vPos.x = pos.x + x;
		vPos.y = pos.y + y;
		vPos.z = 0;
		vertex[i].pos = vPos;
	}
	pDevice3D->SetTexture(0, tex.pTexture);
	pDevice3D->SetFVF(SPRITE_FVF);

	pDevice3D->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vertex, sizeof(SpriteVertex));

}

bool Direct3D::LoadTexture(Texture& tex, TCHAR* FilePath)
{
	if (pDevice3D != nullptr)
	{
		if (SUCCEEDED(D3DXCreateTextureFromFile(pDevice3D, FilePath, &(tex.pTexture))))
		{
			return true;
		}
	}
	return false;
}

bool Direct3D::LoadMeshX(MeshX& mesh, TCHAR*path)
{
	CHAR dir[_MAX_DIR];
	_splitpath_s(path, NULL, 0, dir, _MAX_DIR, NULL, 0, NULL, 0);

	LPD3DXBUFFER pBufferMaterial;

	if (D3DXLoadMeshFromX(path, D3DXMESH_SYSTEMMEM, pDevice3D, NULL, &pBufferMaterial, NULL, &mesh.numMaterials, &mesh.pMesh) != D3D_OK)
	{
		return false;
	}

	if (mesh.numMaterials > 0)
	{
		mesh.pMaterials = new D3DMATERIAL9[mesh.numMaterials];
		mesh.ppTextures = new LPDIRECT3DTEXTURE9[mesh.numMaterials];

		D3DXMATERIAL* d3dxmaterials = (D3DXMATERIAL*)pBufferMaterial->GetBufferPointer();

		for (unsigned int i = 0; i < mesh.numMaterials; i++)
		{
			mesh.pMaterials[i] = d3dxmaterials[i].MatD3D;
			mesh.pMaterials[i].Ambient = mesh.pMaterials[i].Diffuse;

			mesh.ppTextures[i] = nullptr;

			if (d3dxmaterials[i].pTextureFilename != nullptr)
			{
				CHAR textureFilepath[1024];
				ZeroMemory(textureFilepath, sizeof(textureFilepath));

				lstrcat(textureFilepath, dir);

				lstrcat(textureFilepath, d3dxmaterials[i].pTextureFilename);

				if (D3DXCreateTextureFromFile(pDevice3D, textureFilepath, &mesh.ppTextures[i]) != D3D_OK)
				{
					mesh.ppTextures[i] = nullptr;
				}
			}
		}
	}

	pBufferMaterial->Release();

	return true;
}

void Direct3D::DrawMeshX(MeshX& mesh, D3DXMATRIXA16& matWorld)
{
	if (mesh.pMesh != nullptr)
	{
		pDevice3D->SetTransform(D3DTS_WORLD, &matWorld);

		pDevice3D->SetVertexShader(NULL);

		pDevice3D->SetFVF(mesh.pMesh->GetFVF());

		for (unsigned int i = 0; i < mesh.numMaterials; i++)
		{
			pDevice3D->SetMaterial(&mesh.pMaterials[i]);

			pDevice3D->SetTexture(0, mesh.ppTextures[i]);

			mesh.pMesh->DrawSubset(i);
		}
	}
}

void Direct3D::SetViewMatrix(D3DXMATRIXA16& matView)
{
	pDevice3D->SetTransform(D3DTS_VIEW, &matView);
}

void Direct3D::SetProjectionMatrixs()
{
	RECT client;

	GetClientRect(hWnd, &client);
	float w = client.right - client.left;
	float h = client.bottom - client.top;

	D3DXMATRIXA16 matProj;

	D3DXMatrixPerspectiveFovLH(&matProj, (float)(D3DX_PI / 4.0), w / h, 1.0f, 100.0f);
	pDevice3D->SetTransform(D3DTS_PROJECTION, &matProj);
}