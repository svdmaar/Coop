#pragma once

#include "GuiElement.h"
#include "Objects2D.h"

#include <d3d9.h>

class CPictureBackground : public CGuiElement
{
	IDirect3DVertexBuffer9 *m_pVertexBuffer;
	IDirect3DTexture9 *m_pTexture;
	

public:
	CPictureBackground();
	virtual ~CPictureBackground();

	virtual bool Init(std::string _bmpFileName);
	virtual bool CleanUp();

	virtual bool Render() const;
};
