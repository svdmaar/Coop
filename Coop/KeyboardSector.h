#pragma once

#include "InputDevice.h"
#include "Keyboard.h"

const int g_iKeyboardClusterKeyCount = 4;

class CKeyboardSector : public CInputDevice
{
public:
	struct SKeySetup
	{
		// Set to -1 if not defined.
		int m_iUp;
		int m_iDown;
		int m_iLeft;
		int m_iRight;
		int m_iButtons[g_iKeyboardClusterKeyCount];
	};

private:
	const CKeyboard *m_pKeyboard;
	SKeySetup m_keys;

	bool _GetActualKeyState(int _iActualKeyIndex) const;

protected:
	virtual bool _ButtonIsDown(int _iButtonIndex) const;
	virtual bool _OnStateUpdateRequest();

public:
	CKeyboardSector();
	virtual ~CKeyboardSector();

	virtual bool Init(const CKeyboard *_pKeyboard, SKeySetup _keySetup);
	virtual bool CleanUp();

	virtual E_INPUTTYPE GetType() const;
	virtual bool HasJoystick1() const;
	virtual bool HasJoystick2() const;
	virtual bool HasDPad() const;
	virtual int GetButtonCount() const;
	virtual bool HasForceFeedback() const;

	virtual float GetXAxis1() const;
	virtual float GetYAxis1() const;
	virtual float GetXAxis2() const;
	virtual float GetYAxis2() const;
	virtual float GetDPadXAxis() const;
	virtual float GetDPadYAxis() const;
	
	static SKeySetup GetDefaultCluster1();
	static SKeySetup GetDefaultCluster2();
	static SKeySetup GetDefaultCluster(int _iIndex);
};

