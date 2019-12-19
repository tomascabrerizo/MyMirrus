#include "mrTexture.h"

/*static texture members*/
mrUInt32 mrTexture::m_iActiveTexture = -1;
mrUInt32 mrTexture::m_iCurrentID = 0;

mrTexture::mrTexture(void)
{
	m_pkD3DTexture = NULL;
	m_pkRawImage = NULL;
	m_iHasAlpha = mrFalse;
	m_iID = 0;
}

mrTexture::~mrTexture(void)
{
	if (m_pkD3DTexture != NULL)
	{
		m_pkD3DTexture->Release();
		m_pkD3DTexture = NULL;
	}
	if (m_pkRawImage != NULL)
	{
		delete m_pkRawImage;
		m_pkRawImage = NULL;
	}
}

mrError32 mrTexture::Create(mrRGBAImage* pkRawImage)
{
	/*Update methods*/
	m_iID = m_iCurrentID;
	m_iCurrentID++;

	/*Set the texture raw image and update the direct3d texture*/
	SetRawImage(pkRawImage);

	return Update();
}

mrError32 mrTexture::Update(void)
{
	if (m_pkRawImage == NULL)
	{
		return mrErrorInvalidRawImage;
	}

	if (m_pkD3DTexture != NULL)
	{
		m_pkD3DTexture->Release();
		m_pkD3DTexture = NULL;
	}

	m_iHasAlpha = mrTrue;
	/*We are in 32 bits mode*/
	if (mrScreen::GetSingleton()->GetBitdepth() == 32)
	{
		/*try to create alpha texture*/
		if (FAILED(mrScreen::GetSingleton()->GetDevice()->CreateTexture(
			m_pkRawImage->GetWidth(),
			m_pkRawImage->GetHeight(),
			0, 0, 
			D3DFMT_A8R8G8B8,
			D3DPOOL_MANAGED,
			&m_pkD3DTexture, NULL)))
		{
			/*if falied use backbuffer format for textures*/
			if (FAILED(mrScreen::GetSingleton()->GetDevice()->CreateTexture(
				m_pkRawImage->GetWidth(),
				m_pkRawImage->GetHeight(),
				0, 0,
				(D3DFORMAT)mrScreen::GetSingleton()->GetFormat(),
				D3DPOOL_MANAGED,
				&m_pkD3DTexture, NULL)))
			{
				m_pkD3DTexture = NULL;
				m_iHasAlpha = mrFalse;
				return mrErrorCreateTexture;
			}
		}
	}

	/*if we are in 16 bits mode*/
	if (mrScreen::GetSingleton()->GetBitdepth() == 16)
	{
		/*try to create alpha texture*/
		if (FAILED(mrScreen::GetSingleton()->GetDevice()->CreateTexture(
			m_pkRawImage->GetWidth(),
			m_pkRawImage->GetHeight(),
			0, 0,
			D3DFMT_A4R4G4B4,
			D3DPOOL_MANAGED,
			&m_pkD3DTexture, NULL)))
		{
			m_iHasAlpha = mrFalse;
			/*if falied use backbuffer format for textures*/
			if (FAILED(mrScreen::GetSingleton()->GetDevice()->CreateTexture(
				m_pkRawImage->GetWidth(),
				m_pkRawImage->GetHeight(),
				0, 0,
				(D3DFORMAT)mrScreen::GetSingleton()->GetFormat(),
				D3DPOOL_MANAGED,
				&m_pkD3DTexture, NULL)))
			{
				m_pkD3DTexture = NULL;
				return mrErrorCreateTexture;
			}
		}
	}

	/*Lock Texture*/
	D3DLOCKED_RECT kLockedRect;
	m_pkD3DTexture->LockRect(0, &kLockedRect, NULL, 0);

	/*Pointer to locked textures*/
	WORD* pi16TextureBuffer = NULL;
	DWORD* pi32TextureBuffer = NULL;

	/*Use the correct pointer depending on depth*/
	if (mrScreen::GetSingleton()->GetBitdepth() == 16)
	{
		/*Cast the 16-bits pointer to point to the first pixel*/
		pi16TextureBuffer = (WORD*)kLockedRect.pBits;
	}
	else
	{
		/*Cast the 32-bits pointer to point to the first pixel*/
		pi32TextureBuffer = (DWORD*)kLockedRect.pBits;
	}

	/*Fill the Texture*/
	mrUInt32 iX, iY;
	mrUInt32 iDepth;

	/*Get depth in bytes and calculate pitch*/
	iDepth = mrScreen::GetSingleton()->GetBitdepth() / 8;
	mrUInt32 iPitch = kLockedRect.Pitch / iDepth;

	mrUInt8 iRed, iGreen, iBlue, iAlpha;
	mrUInt32 iColor;

	for (iY = 0; iY < m_pkRawImage->GetHeight(); iY++)
	{
		for (iX = 0; iX < m_pkRawImage->GetWidth(); iX++)
		{
			/*Get color component*/
			iColor = m_pkRawImage->GetColor(iX, iY);
			
			iRed = (mrUInt8)((iColor & 0xFF000000) >> 24);
			iGreen = (mrUInt8)((iColor & 0x00FF0000) >> 16);
			iBlue = (mrUInt8)((iColor & 0x0000FF00) >> 8);
			iAlpha = (mrUInt8)((iColor & 0x000000FF));

			/*Write color to Texture buffer according to mode*/
			if (mrScreen::GetSingleton()->GetBitdepth() == 16)
			{
				if (m_iHasAlpha == mrTrue)
				{
					pi16TextureBuffer[iX + (iY * iPitch)] =
						(mrUInt16)((iAlpha * (1 << 4) / 256 << 12) |
								   (iRed * (1 << 4) / 256 << 8) |
								   (iGreen * (1 << 4) / 256 << 4) | 
								   (iBlue * (1 << 4) / 256));
				}
				else
				{
					if (mrScreen::GetSingleton()->GetFormat() == D3DFMT_R5G6B5)
					{
						pi16TextureBuffer[iX + (iY * iPitch)] =
							(mrUInt16)((iRed * (1 << 5) / 256 << 11) |
									   (iGreen * (1 << 6) / 256 << 5) |
									   (iBlue * (1 << 5) / 256));
					}
					if (mrScreen::GetSingleton()->GetFormat() == D3DFMT_X1R5G5B5)
					{
						pi16TextureBuffer[iX + (iY * iPitch)] =
							(mrUInt16)((iRed * (1 << 5) / 256 << 10) |
							(iGreen * (1 << 5) / 256 << 5) |
								(iBlue * (1 << 5) / 256));
					}
					if (mrScreen::GetSingleton()->GetFormat() == D3DFMT_A1R5G5B5)
					{
						pi16TextureBuffer[iX + (iY * iPitch)] =
							(mrUInt16)(((iAlpha > 0) ? 1 : 0 << 15) |
							(iRed * (1 << 5) / 256 << 10) |
							(iGreen * (1 << 5) / 256 << 5) |
								(iBlue * (1 << 5) / 256));
					}
				}
			}
			else
			{
				pi32TextureBuffer[iX + iY * iPitch] =
					D3DCOLOR_ARGB(iAlpha, iRed, iGreen, iBlue);
			}
		}
	}

	/*Unlock*/
	m_pkD3DTexture->UnlockRect(0);

	return mrNoError;
}

void mrTexture::SetRawImage(mrRGBAImage* pkRawImage)
{
	if (m_pkRawImage == NULL)
	{
		m_pkRawImage = new mrRGBAImage();
	}

	m_pkRawImage->SetWidth(pkRawImage->GetWidth());
	m_pkRawImage->SetHegiht(pkRawImage->GetHeight());
	m_pkRawImage->SetImageBuffer(pkRawImage->GetImageBuffer());
}

mrRGBAImage* mrTexture::GetRawImage(void)
{
	return m_pkRawImage;
}

void mrTexture::SetActiveTexture(void)
{
	/*Set Direct3D Active texture*/
	if (m_iActiveTexture != m_iID)
	{
		mrScreen::GetSingleton()->GetDevice()->SetTexture(0, m_pkD3DTexture);
		m_iActiveTexture = m_iID;
	}
}

mrUInt32 mrTexture::GetID(void)
{
	return m_iID;
}

mrUInt32 mrTexture::GetActiveTexture(void)
{
	return m_iActiveTexture;
}


