#pragma once

#include "GuiElement.h"
#include "GraphicsManager.h"

#include <string>
#include <vector>

typedef std::vector<std::string> StringVector;

// todo: think about renaming enums, for example: Selector_State { stInactive, stActive, stLocked }
enum ESSELECTOR_STATE
{
	SELECTOR_INACTIVE,
	SELECTOR_ACTIVE,
	SELECTOR_LOCKED,
};

class CStringSelector : public CGuiElement
{
	// Graphics.
	IDirect3DVertexBuffer9 *m_pActiveVB;
	IDirect3DVertexBuffer9 *m_pInactiveVB;
	IDirect3DVertexBuffer9 *m_pLockedVB;
	SFloatRect m_rInner;

	// Logic.
	const StringVector *m_pStrings;
	int m_iStringIndex;
	ESSELECTOR_STATE m_eState;

public:
	virtual bool Init(SFloatRect _rPos, const StringVector *_pStrings);
	virtual bool Render() const;
	virtual bool CleanUp();
	virtual bool OnButtonPressed(int _iIndex);

	void OnDPadInput(int _iUpDown);
	std::string GetSelectedString();
	ESSELECTOR_STATE GetState();
};
