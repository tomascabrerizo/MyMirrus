#include "mrTemplateSet.h"

mrTemplateSet::mrTemplateSet(void)
{
	m_pkTexture = NULL;
	m_iTextureWidth = 0;
	m_iTextureHeight = 0;
	m_iCellWidth = 0;
	m_iCellHeight = 0;
}

mrTemplateSet::~mrTemplateSet(void)
{
	m_iTextureWidth = 0;
	m_iTextureHeight = 0;
	m_iCellWidth = 0;
	m_iCellHeight = 0;
	m_pkTexture = NULL;
}

void mrTemplateSet::Create(mrTexture* pkTexture, mrUInt32 iTextureWidth, mrUInt32 iTextureHegiht, 
	mrUInt32 iCellWidth, mrUInt32 m_iCellHeight)
{
	m_iTextureWidth = iTextureWidth;
	m_iTextureHeight = iTextureHegiht;
	m_iCellWidth = iCellWidth;
	m_iCellHeight = m_iCellHeight;

	m_pkTexture = pkTexture;
}

void mrTemplateSet::GetUV(mrCellID kPosition, mrRectText* pkUVRect)
{
	/*Returns the texture UV rect for a given cell*/
	pkUVRect->m_fLeft = (mrReal32)(1 + ((1 + m_iCellWidth) * kPosition.m_iX)) / m_iTextureWidth;
	pkUVRect->m_fRight = (mrReal32)(1 + ((1 + m_iCellWidth) * kPosition.m_iX) + m_iCellWidth) / m_iTextureWidth;
	pkUVRect->m_fTop = (mrReal32)(1 + ((1 + m_iCellHeight) * kPosition.m_iY)) / m_iTextureHeight;
	pkUVRect->m_fBottom = (mrReal32)(1 + ((1 + m_iCellHeight) * kPosition.m_iY) + m_iCellHeight) / m_iTextureHeight;
}

void mrTemplateSet::SetActiveTextures(void)
{
	m_pkTexture->SetActiveTexture();
}

mrUInt32 mrTemplateSet::GetTextureWidth(void)
{
	return m_iTextureWidth;
}

mrUInt32 mrTemplateSet::GetTextureHeight(void)
{
	return m_iTextureHeight;
}

mrUInt32 mrTemplateSet::GetCellWidth(void)
{
	return m_iCellWidth;
}

mrUInt32 mrTemplateSet::GetCellHeight(void)
{
	return m_iCellHeight;
}
