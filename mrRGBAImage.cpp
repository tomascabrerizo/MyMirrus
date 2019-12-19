#include "mrRGBAImage.h"

mrRGBAImage::mrRGBAImage(void)
{
	m_iWidth = 0;
	m_iHeight = 0;
	m_piImageBUffer = NULL;
}

mrRGBAImage::~mrRGBAImage(void)
{
	m_iWidth = 0;
	m_iHeight = 0;
	/*If memory was allocate release it*/
	if (m_piImageBUffer != NULL)
	{
		delete[] m_piImageBUffer;
		m_piImageBUffer = NULL;
	}
}

mrRGBAImage& mrRGBAImage::operator=(mrRGBAImage& rkImage)
{
	m_iWidth = rkImage.GetWidth();
	m_iHeight = rkImage.GetHeight();

	SetImageBuffer(rkImage.GetImageBuffer());

	/*Return instance of this class*/
	return *this;
}

mrError32 mrRGBAImage::LoadFromBitmap(LPSTR lpszFilename)
{
	std::fstream kBitmap;
	kBitmap.open(lpszFilename, std::ios::binary, std::ios::in);

	if (kBitmap.is_open())
	{
		mrUInt16 iType;
		kBitmap.read((char*)&iType, sizeof(mrUInt16));

		/*Get bitmap size*/
		if (0x4D42 != iType)
		{
			return mrErrorNotBitmapFile;
		}
		/*Ignore eight bytes*/
		kBitmap.seekg(8, std::ios::cur);
		/*Get the position of the start of the bitmap buffer*/
		mrUInt32 iStartBuffer;
		kBitmap.read((char*)&iStartBuffer, sizeof(mrUInt32));
		/*ignore four bytes*/
		kBitmap.seekg(4, std::ios::cur);
		/*Get bit and height of the bitmap*/
		kBitmap.read((char*)&m_iWidth, sizeof(mrUInt32));
		kBitmap.read((char*)&m_iHeight, sizeof(mrUInt32));
		/*ignore 2 bytes*/
		kBitmap.seekg(2, std::ios::cur);
		/*Get bit count*/
		mrUInt16 iBitCount;
		kBitmap.read((char*)&iBitCount, sizeof(mrUInt16));
		/*if not 24 mode not sopported, return error*/
		if (iBitCount != 24)
		{
			return mrErrorBitmapNotSupported;
		}
		/*Get Compression*/
		mrUInt32 iCompression;
		kBitmap.read((char*)&iCompression, sizeof(mrUInt32));
		/*if compressed not sopported return error*/
		if (iCompression != BI_RGB)
		{
			return mrErrorBitmapNotSupported;
		}
		/*Move to bitmap buffer*/
		kBitmap.seekg(iStartBuffer, std::ios::beg);
		/*Read image buffer from file*/
		mrUInt8* piBuffer = new mrUInt8[m_iWidth * m_iHeight * 3];
		kBitmap.read((char*)piBuffer, m_iWidth * m_iHeight * 3 * sizeof(mrUInt8));
		/*Allocate memory from image buffer*/
		if (m_piImageBUffer != NULL)
		{
			delete[] m_piImageBUffer;
		}
		m_piImageBUffer = new mrUInt32[m_iWidth * m_iHeight];
		/*Get each pixel color component  and fill image buffer*/
		mrUInt32 iX, iY;
		for (iY = 0; iY < m_iHeight; iY++)
		{
			for (iX = 0; iX < m_iWidth; iX++)
			{
				/*Need to be flippled*/
				if (m_iHeight > 0)
				{
					m_piImageBUffer[iX + (m_iHeight - 1 - iY) * m_iWidth] = 255|
						(piBuffer[(iX + iY * (m_iWidth)) * 3 + 0] << 8)|
						(piBuffer[(iX + iY * (m_iWidth)) * 3 + 1] << 16)|
						(piBuffer[(iX + iY * (m_iWidth)) * 3 + 2] << 24); 
				}
				else 
				{
					m_piImageBUffer[iX + (iY * m_iWidth)] = 255|
						(piBuffer[iX + (iY * m_iWidth) * 3 + 0] << 8)|
						(piBuffer[iX + (iY * m_iWidth) * 3 + 1] << 16)|
						(piBuffer[iX + (iY * m_iWidth) * 3 + 2] << 24);
				}
			}
		}

		/*Close file, release memory and return no error*/
		if (piBuffer != NULL)
		{
			delete[] piBuffer;
		}
		kBitmap.close();
	}
	return mrNoError;
}

mrError32 mrRGBAImage::LoadFromTarga(LPSTR lpszFilename)
{
	std::fstream kTarga;

	kTarga.open(lpszFilename, std::ios::binary, std::ios::in);

	if (kTarga.is_open())
	{
		/*read field description size*/
		mrUInt8 iFieldDescSize;
		kTarga.read((char*)&iFieldDescSize, sizeof(mrUInt8));
		/*Ignore one bit*/
		kTarga.seekg(1, std::ios::cur);
		/*Read image color code*/
		mrUInt8 iImageCode;
		kTarga.read((char*)&iImageCode, sizeof(mrUInt8));
		if (iImageCode != 2)
		{
			return mrErrorTargaNotSupported;
		}
		/*ignore two bytes*/
		kTarga.seekg(2, std::ios::cur);
		/*Read color map*/
		mrUInt16 iMapLenght;
		kTarga.read((char*)&iMapLenght, sizeof(mrUInt16));
		/*ignore one byte*/
		kTarga.seekg(1, std::ios::cur);
		/*Read image start position*/
		mrUInt16 iXstart;
		kTarga.read((char*)&iXstart, sizeof(mrUInt16));
		mrUInt16 iYstart;
		kTarga.read((char*)&iYstart, sizeof(mrUInt16));
		/*Read image size*/
		mrUInt16 iWidth;
		kTarga.read((char*)&iWidth, sizeof(mrUInt16));
		mrUInt16 iHeight;
		kTarga.read((char*)&iHeight, sizeof(mrUInt16));

		m_iWidth = iWidth;
		m_iHeight = iHeight;

		/*Read image bit depth*/
		mrUInt8 iImageBits;
		kTarga.read((char*)&iImageBits,sizeof(mrUInt8));
		if (iImageBits != 32)
		{
			return mrErrorTargaNotSupported;
		}

		/*Read image description*/
		mrUInt8 iImageDesc;
		kTarga.read((char*)&iImageDesc, sizeof(mrUInt8));

		/*Find the start of image data*/
		kTarga.seekg(iFieldDescSize, std::ios::cur);
		/*ignore colorMap*/
		kTarga.seekg(iMapLenght*4, std::ios::cur);
		/*Read image buffer from file*/
		mrUInt32* piBuffer = new mrUInt32[m_iWidth * m_iHeight];
		kTarga.read((char*)piBuffer, m_iWidth * m_iHeight * 4 * sizeof(mrUInt8));
		/*Allocate memory for image buffer*/
		if (m_piImageBUffer != NULL)
		{
			delete[] m_piImageBUffer;
		}
		m_piImageBUffer = new mrUInt32[m_iWidth * m_iHeight];
		/*Copy image data to our buffer*/
		mrUInt8 iRed, iGreen, iBlue, iAlpha;
		mrUInt32 iColor;
		/*Get each pixel color component and fill image buffer*/
		mrUInt32 iX, iY;
		
		for (iY = 0; iY < m_iHeight; iY++)
		{
			for (iX = 0; iX < m_iWidth; iX++)
			{
				/*Doenst need to be flipped*/
				if ((iImageDesc & 1) << 4)
				{
					/*Get color component*/
					iColor = piBuffer[iX + (iY * m_iWidth)];
					
					iAlpha = (mrUInt8)((iColor & 0xFF000000) >> 24);
					iRed = (mrUInt8)((iColor & 0x00FF0000) >> 16);
					iGreen = (mrUInt8)((iColor & 0x0000FF00) >> 8);
					iBlue = (mrUInt8)((iColor & 0x000000FF));

					/*Copy flipped positions*/
					m_piImageBUffer[iX + (iY * m_iWidth)] = iAlpha << 0 | iBlue << 8 | iGreen << 16 | iRed << 24;
				}
				else
				{
					/*Needs to be flipped*/
					/*Get color component*/
					iColor = piBuffer[iX + (iY * m_iWidth)];

					iAlpha = (mrUInt8)((iColor & 0xFF000000) >> 24);
					iRed = (mrUInt8)((iColor & 0x00FF0000) >> 16);
					iGreen = (mrUInt8)((iColor & 0x0000FF00) >> 8);
					iBlue = (mrUInt8)((iColor & 0x000000FF));

					/*Copy positions*/
					m_piImageBUffer[iX + (m_iHeight - 1 - iY) * (m_iWidth)] = iAlpha << 0 | iBlue << 8 | iGreen << 16 | iRed << 24;

				}
			}
		}
		/*Close file, release memory and return no error*/
		if (piBuffer != NULL)
		{
			delete[] piBuffer;
		}
		kTarga.close();
	}

	return mrNoError;
}

void mrRGBAImage::SetColorKey(mrUInt8 iRed, mrUInt8 iGreen, mrUInt8 iBlue)
{
	/*Get each pixel color component and set color key*/
	mrUInt32 iX, iY;
	mrUInt8 iOriRed, iOriGreen, iOriBlue;

	for (iY = 0; iY < m_iHeight; iY++) {
		for (iX = 0; iX < m_iWidth; iX++) {
			iOriRed = (mrUInt8)((m_piImageBUffer[iX + (iY * m_iWidth)] & 0xFF000000) >> 24);
			iOriGreen = (mrUInt8)((m_piImageBUffer[iX + (iY * m_iWidth)] & 0x00FF0000) >> 16);
			iOriBlue = (mrUInt8)((m_piImageBUffer[iX + (iY * m_iWidth)] & 0x0000FF00 >> 8));

			/*if color match set alpha to cero*/
			if ((iOriRed == iRed) && (iOriGreen == iGreen) && (iOriBlue == iBlue))
			{
				m_piImageBUffer[iX + (iY * m_iWidth)] = iOriRed << 24 | iOriGreen << 16 | iOriBlue << 8 | 0;
			}
		}
	}
}

void mrRGBAImage::SetWidth(mrUInt32 iWidth)
{
	this->m_iWidth = iWidth;
}

void mrRGBAImage::SetHegiht(mrUInt32 iHeight)
{
	this->m_iHeight = iHeight;
}

void mrRGBAImage::SetColor(mrUInt32 iX, mrUInt32 iY, mrUInt8 iRed, mrUInt8 iGreen, mrUInt8 iBlue, mrUInt8 iAlpha)
{
	/*Set Color at given position*/
	mrUInt32 iColor;
	iColor = D3DCOLOR_RGBA(iRed, iGreen, iBlue, iAlpha);
	m_piImageBUffer[iX + (iY * m_iWidth - 1)] = iColor;
}

void mrRGBAImage::SetImageBuffer(mrUInt32* pImage)
{
	/*Set the image buffer*/
	if (m_piImageBUffer != NULL)
	{
		delete[] m_piImageBUffer;
	}
	m_piImageBUffer = new mrUInt32[m_iWidth * m_iHeight];

	memcpy(m_piImageBUffer, pImage, sizeof(mrUInt32) * m_iWidth * m_iHeight);
}

mrUInt32 mrRGBAImage::GetWidth(void)
{
	return m_iWidth;
}

mrUInt32 mrRGBAImage::GetHeight(void)
{
	return m_iHeight;
}

mrUInt32 mrRGBAImage::GetColor(mrUInt32 iX, mrUInt32 iY)
{
	return m_piImageBUffer[iX + iY * m_iWidth];
}

mrUInt32* mrRGBAImage::GetImageBuffer(void)
{
	return m_piImageBUffer;
}
