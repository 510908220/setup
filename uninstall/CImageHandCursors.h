#include "ui/image_button.h"
class CImageHandCursor: public CImageButton
{
public:
	CImageHandCursor(DWORD dwImgNormal, DWORD dwImgHover, DWORD dwImgDown, DWORD dwImgDisable);
	~CImageHandCursor();

	BEGIN_MSG_MAP_EX(CImageHandCursor)
		MSG_WM_SETCURSOR(OnSetCursor)
		CHAIN_MSG_MAP(CImageButton)
		END_MSG_MAP()
public:
	BOOL OnSetCursor(CWindow /*wnd*/, UINT /*nHitTest*/, UINT /*message*/);
private:
	HCURSOR					m_hCursor;
};

