
#include "mrWindow.h"
#include <math.h>
#include <d3d9.h>

#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)

class CustomVertex
{
public:
	//transformed positions
	FLOAT x, y, z, rhw;
	//Color
	DWORD Color;
	//Texture coordenates
	FLOAT tU, tV;
};

class D3DWindow : public mrWindow
{
	//Direct 3D interfaces
	LPDIRECT3D9 m_pD3D;
	LPDIRECT3DDEVICE9 m_pD3DDevice;

	//Direct 3D texture interface
	LPDIRECT3DTEXTURE9 m_pD3DTexture;

	//Direct 3D current format
	mrUInt32 m_iD3DFormat;

	//Vertices
	CustomVertex m_kVertex[4];

	//Direct 3D surface interface
	LPDIRECT3DSURFACE9 m_pD3DSurface;

public: 
	D3DWindow(void) {};
	~D3DWindow(void) {};

	//Setup and shut down Direct3D
	HRESULT SetupDirect3D(void);
	HRESULT KillDirect3D(void);

	//Window Manipulation function
	mrBool32 Frame(void);
};

HRESULT D3DWindow::SetupDirect3D(void)
{
	//Create the direct3D object
	if ((m_pD3D = Direct3DCreate9(D3D_SDK_VERSION)) == NULL)
	{
		return E_FAIL;
	}

	//Get the current display mode so we can know what bitdeph we are
	D3DDISPLAYMODE d3ddm;
	if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	//Fill in the present parameters
	D3DPRESENT_PARAMETERS d3dpp;
	//initialize d3dpp with zeros
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	//We want windowed mode
	d3dpp.Windowed = TRUE;
	//Discard this
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	//Same format at the current format
	d3dpp.BackBufferFormat = d3ddm.Format;

	//Create the Device
	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
		m_hWindow,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp,
		&m_pD3DDevice)))
	{
		return E_FAIL;
	}

	//Create the texture
	if (FAILED(m_pD3DDevice->CreateTexture(200, 200, 0, 0, D3DFMT_X8R8G8B8, D3DPOOL_MANAGED, &m_pD3DTexture, NULL)))
	{
		return E_FAIL;
	}

	//Lock Texture
	D3DLOCKED_RECT kLockedRect;
	m_pD3DTexture->LockRect(0, &kLockedRect, NULL, 0);

	//Cast a pointer to piont the first pixel
	DWORD* piSurfaceBuffer = (DWORD*)kLockedRect.pBits;

	//Fill the Texture
	int iX, iY;
	for (iY = 0; iY < 200; iY++)
	{
		for (iX = 0; iX < 200; iX++)
		{
			//Form a patterm
			int iPower = (int)(sin(iY * iX) * 32 + cos(iY * -iX) * 32);
			piSurfaceBuffer[iX + iY * (kLockedRect.Pitch >> 2)] =
				D3DCOLOR_XRGB(iPower, iPower, iPower);
		}
	}

	//Unlock
	m_pD3DTexture->UnlockRect(0);

	//Setup temporal vertices  information
	CustomVertex kVertices[] = 
	{ /* x, y, z, w, color, texture coordinates (u,v) */
		{0.0f, 0.0f, 0.5f, 1.0f, D3DCOLOR_ARGB(255, 0, 255, 0), 0, 0},
		{400.0f, 0.0f, 0.5f, 1.0f, D3DCOLOR_ARGB(255, 255, 255, 255), 1, 0},
		{400.0f, 400.0f, 0.5f, 1.0f, D3DCOLOR_ARGB(255, 255, 0, 0), 1, 1},
		{0.0f, 400.0f, 0.5f, 1.0f, D3DCOLOR_ARGB(255, 0, 0, 255), 0, 1}
	};

	//Copy the vertices 
	memcpy(m_kVertex, kVertices, sizeof(CustomVertex) * 4);

	//Set Render States
	//Dont cull polygons
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	//Dont use lighting
	m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	//Set Texture States
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);

	return D3D_OK;
}

HRESULT D3DWindow::KillDirect3D(void)
{
	//If any of the direct 3D objects exist, release them
	if (m_pD3D != NULL)
	{
		m_pD3D->Release();
	}
	if (m_pD3DDevice != NULL)
	{
		m_pD3DDevice->Release();
	}
	if (m_pD3DSurface != NULL)
	{
		m_pD3DSurface->Release();
	}
	if (m_pD3DTexture != NULL)
	{
		m_pD3DTexture->Release();
	}
	return D3D_OK;
}

mrBool32 D3DWindow::Frame(void)
{
	//Clear the screen to blue
	m_pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET,
		D3DCOLOR_XRGB(0, 0, 255), 1.0f, 0);

	//Start Rendering
	m_pD3DDevice->BeginScene();

	/*Set Texture Sorce*/
	m_pD3DDevice->SetTexture(0, m_pD3DTexture);
	
	/*Set vertex source*/
	m_pD3DDevice->SetVertexShader(NULL);
	m_pD3DDevice->SetFVF(D3DFVF_CUSTOMVERTEX);
	m_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, m_kVertex, sizeof(CustomVertex));

	m_pD3DDevice->EndScene();

	//Present the rendered screen to the screen
	m_pD3DDevice->Present(NULL, NULL, NULL, NULL);

	return mrTrue;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nShowCmd)
{
	D3DWindow window;

	//Create Window
	window.Create(hInstance, "Game_Engine_DEMO");
	window.SetPosition(10, 10);
	window.SetSize(800, 600);

	window.SetupDirect3D();
	//Enter MessageLoop
	window.Run();

	window.KillDirect3D();

	return 0;
}