#include "stdafx.h"
#include "CImageHandCursors.h"
CImageHandCursor::CImageHandCursor(DWORD dwImgNormal, DWORD dwImgHover, DWORD dwImgDown, DWORD dwImgDisable):
CImageButton(dwImgNormal, dwImgHover, dwImgDown, dwImgDisable),
m_hCursor(NULL){

}
CImageHandCursor::~CImageHandCursor(){

}
BOOL CImageHandCursor::OnSetCursor(CWindow /*wnd*/, UINT /*nHitTest*/, UINT /*message*/)
{
	if (m_hCursor == NULL) m_hCursor = ::LoadCursor(NULL, IDC_HAND);
	SetCursor(m_hCursor);

	return TRUE;
}
