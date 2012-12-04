#include "ErrorParser.h"


#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <XAudio2.h>
#include <sstream>
#include <d3d9.h>

using namespace std;

static void AddString(stringstream& _out, string _text)
{
	if(_out.str().length() > 0)
	{
		_out << ", ";
	}

	_out << _text;
}

string CErrorParser::ParseError(HRESULT _hr)
{
	stringstream out;

	if(_hr == S_OK) AddString(out, "S_OK");
	else if(_hr == S_FALSE) AddString(out, "S_FALSE");
	else if(_hr == E_UNEXPECTED) AddString(out, "E_UNEXPECTED");
	else if(_hr == D3D_OK) AddString(out, "D3D_OK");
	else if(_hr == D3DERR_WRONGTEXTUREFORMAT) AddString(out, "D3DERR_WRONGTEXTUREFORMAT");
	else if(_hr == D3DERR_UNSUPPORTEDCOLOROPERATION) AddString(out, "D3DERR_UNSUPPORTEDCOLOROPERATION");
	else if(_hr == D3DERR_UNSUPPORTEDCOLORARG) AddString(out, "D3DERR_UNSUPPORTEDCOLORARG");
	else if(_hr == D3DERR_UNSUPPORTEDALPHAOPERATION) AddString(out, "D3DERR_UNSUPPORTEDALPHAOPERATION");
	else if(_hr == D3DERR_UNSUPPORTEDALPHAARG) AddString(out, "D3DERR_UNSUPPORTEDALPHAARG");
	else if(_hr == D3DERR_TOOMANYOPERATIONS) AddString(out, "D3DERR_TOOMANYOPERATIONS");
	else if(_hr == D3DERR_CONFLICTINGTEXTUREFILTER) AddString(out, "D3DERR_CONFLICTINGTEXTUREFILTER");
	else if(_hr == D3DERR_UNSUPPORTEDFACTORVALUE) AddString(out, "D3DERR_UNSUPPORTEDFACTORVALUE");
	else if(_hr == D3DERR_CONFLICTINGRENDERSTATE) AddString(out, "D3DERR_CONFLICTINGRENDERSTATE");
	else if(_hr == D3DERR_UNSUPPORTEDTEXTUREFILTER) AddString(out, "D3DERR_UNSUPPORTEDTEXTUREFILTER");
	else if(_hr == D3DERR_CONFLICTINGTEXTUREPALETTE) AddString(out, "D3DERR_CONFLICTINGTEXTUREPALETTE");
	else if(_hr == D3DERR_DRIVERINTERNALERROR) AddString(out, "D3DERR_DRIVERINTERNALERROR");
	else if(_hr == D3DERR_NOTFOUND) AddString(out, "D3DERR_NOTFOUND");
	else if(_hr == D3DERR_MOREDATA) AddString(out, "D3DERR_MOREDATA");
	else if(_hr == D3DERR_DEVICELOST) AddString(out, "D3DERR_DEVICELOST");
	else if(_hr == D3DERR_DEVICENOTRESET) AddString(out, "D3DERR_DEVICENOTRESET");
	else if(_hr == D3DERR_NOTAVAILABLE) AddString(out, "D3DERR_NOTAVAILABLE");
	else if(_hr == D3DERR_OUTOFVIDEOMEMORY) AddString(out, "D3DERR_OUTOFVIDEOMEMORY");
	else if(_hr == D3DERR_INVALIDDEVICE) AddString(out, "D3DERR_INVALIDDEVICE");
	else if(_hr == D3DERR_INVALIDCALL) AddString(out, "D3DERR_INVALIDCALL");
	else if(_hr == D3DERR_DRIVERINVALIDCALL) AddString(out, "D3DERR_DRIVERINVALIDCALL");
	else if(_hr == D3DERR_WASSTILLDRAWING) AddString(out, "D3DERR_WASSTILLDRAWING");
	else if(_hr == D3DOK_NOAUTOGEN) AddString(out, "D3DOK_NOAUTOGEN");
	else if(_hr == D3DERR_DEVICEREMOVED) AddString(out, "D3DERR_DEVICEREMOVED");
	else if(_hr == S_NOT_RESIDENT) AddString(out, "S_NOT_RESIDENT");
	else if(_hr == S_RESIDENT_IN_SHARED_MEMORY) AddString(out, "S_RESIDENT_IN_SHARED_MEMORY");
	else if(_hr == S_PRESENT_MODE_CHANGED) AddString(out, "S_PRESENT_MODE_CHANGED");
	else if(_hr == S_PRESENT_OCCLUDED) AddString(out, "S_PRESENT_OCCLUDED");
	else if(_hr == D3DERR_DEVICEHUNG) AddString(out, "D3DERR_DEVICEHUNG");
	else if(_hr == D3DERR_UNSUPPORTEDOVERLAY) AddString(out, "D3DERR_UNSUPPORTEDOVERLAY");
	else if(_hr == D3DERR_UNSUPPORTEDOVERLAYFORMAT) AddString(out, "D3DERR_UNSUPPORTEDOVERLAYFORMAT");
	else if(_hr == D3DERR_CANNOTPROTECTCONTENT) AddString(out, "D3DERR_CANNOTPROTECTCONTENT");
	else if(_hr == D3DERR_UNSUPPORTEDCRYPTO) AddString(out, "D3DERR_UNSUPPORTEDCRYPTO");
	else if(_hr == D3DERR_PRESENT_STATISTICS_DISJOINT) AddString(out, "D3DERR_PRESENT_STATISTICS_DISJOINT");
	else if(_hr == DI_OK) AddString(out, "DI_OK");
	else if(_hr == DI_NOTATTACHED) AddString(out, "DI_NOTATTACHED");
	else if(_hr == DI_BUFFEROVERFLOW) AddString(out, "DI_BUFFEROVERFLOW");
	else if(_hr == DI_PROPNOEFFECT) AddString(out, "DI_PROPNOEFFECT");
	else if(_hr == DI_NOEFFECT) AddString(out, "DI_NOEFFECT");
	else if(_hr == DI_POLLEDDEVICE) AddString(out, "DI_POLLEDDEVICE");
	else if(_hr == DI_DOWNLOADSKIPPED) AddString(out, "DI_DOWNLOADSKIPPED");
	else if(_hr == DI_EFFECTRESTARTED) AddString(out, "DI_EFFECTRESTARTED");
	else if(_hr == DI_TRUNCATED) AddString(out, "DI_TRUNCATED");
	else if(_hr == DI_SETTINGSNOTSAVED) AddString(out, "DI_SETTINGSNOTSAVED");
	else if(_hr == DI_TRUNCATEDANDRESTARTED) AddString(out, "DI_TRUNCATEDANDRESTARTED");
	else if(_hr == DI_WRITEPROTECT) AddString(out, "DI_WRITEPROTECT");
	else if(_hr == DIERR_OLDDIRECTINPUTVERSION) AddString(out, "DIERR_OLDDIRECTINPUTVERSION");
	else if(_hr == DIERR_BETADIRECTINPUTVERSION) AddString(out, "DIERR_BETADIRECTINPUTVERSION");
	else if(_hr == DIERR_BADDRIVERVER) AddString(out, "DIERR_BADDRIVERVER");
	else if(_hr == DIERR_DEVICENOTREG) AddString(out, "DIERR_DEVICENOTREG");
	else if(_hr == DIERR_NOTFOUND) AddString(out, "DIERR_NOTFOUND");
	else if(_hr == DIERR_OBJECTNOTFOUND) AddString(out, "DIERR_OBJECTNOTFOUND");
	else if(_hr == DIERR_INVALIDPARAM) AddString(out, "DIERR_INVALIDPARAM");
	else if(_hr == DIERR_NOINTERFACE) AddString(out, "DIERR_NOINTERFACE");
	else if(_hr == DIERR_GENERIC) AddString(out, "DIERR_GENERIC");
	else if(_hr == DIERR_OUTOFMEMORY) AddString(out, "DIERR_OUTOFMEMORY");
	else if(_hr == DIERR_UNSUPPORTED) AddString(out, "DIERR_UNSUPPORTED");
	else if(_hr == DIERR_NOTINITIALIZED) AddString(out, "DIERR_NOTINITIALIZED");
	else if(_hr == DIERR_ALREADYINITIALIZED) AddString(out, "DIERR_ALREADYINITIALIZED");
	else if(_hr == DIERR_NOAGGREGATION) AddString(out, "DIERR_NOAGGREGATION");
	else if(_hr == DIERR_OTHERAPPHASPRIO) AddString(out, "DIERR_OTHERAPPHASPRIO");
	else if(_hr == DIERR_INPUTLOST) AddString(out, "DIERR_INPUTLOST");
	else if(_hr == DIERR_ACQUIRED) AddString(out, "DIERR_ACQUIRED");
	else if(_hr == DIERR_NOTACQUIRED) AddString(out, "DIERR_NOTACQUIRED");
	else if(_hr == DIERR_READONLY) AddString(out, "DIERR_READONLY");
	else if(_hr == DIERR_HANDLEEXISTS) AddString(out, "DIERR_HANDLEEXISTS");
	else if(_hr == E_PENDING) AddString(out, "E_PENDING");
	else if(_hr == DIERR_INSUFFICIENTPRIVS) AddString(out, "DIERR_INSUFFICIENTPRIVS");
	else if(_hr == DIERR_DEVICEFULL) AddString(out, "DIERR_DEVICEFULL");
	else if(_hr == DIERR_MOREDATA) AddString(out, "DIERR_MOREDATA");
	else if(_hr == DIERR_NOTDOWNLOADED) AddString(out, "DIERR_NOTDOWNLOADED");
	else if(_hr == DIERR_HASEFFECTS) AddString(out, "DIERR_HASEFFECTS");
	else if(_hr == DIERR_NOTEXCLUSIVEACQUIRED) AddString(out, "DIERR_NOTEXCLUSIVEACQUIRED");
	else if(_hr == DIERR_INCOMPLETEEFFECT) AddString(out, "DIERR_INCOMPLETEEFFECT");
	else if(_hr == DIERR_NOTBUFFERED) AddString(out, "DIERR_NOTBUFFERED");
	else if(_hr == DIERR_EFFECTPLAYING) AddString(out, "DIERR_EFFECTPLAYING");
	else if(_hr == DIERR_UNPLUGGED) AddString(out, "DIERR_UNPLUGGED");
	else if(_hr == DIERR_REPORTFULL) AddString(out, "DIERR_REPORTFULL");
	else if(_hr == DIERR_MAPFILEFAIL) AddString(out, "DIERR_MAPFILEFAIL");
	else if(_hr == XAUDIO2_E_INVALID_CALL) AddString(out, "XAUDIO2_E_INVALID_CALL");
	else if(_hr == XAUDIO2_E_XMA_DECODER_ERROR) AddString(out, "XAUDIO2_E_XMA_DECODER_ERROR");
	else if(_hr == XAUDIO2_E_XAPO_CREATION_FAILED) AddString(out, "XAUDIO2_E_XAPO_CREATION_FAILED");
	else if(_hr == XAUDIO2_E_DEVICE_INVALIDATED) AddString(out, "XAUDIO2_E_DEVICE_INVALIDATED");

	return out.str();
}
