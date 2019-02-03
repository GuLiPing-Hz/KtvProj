#pragma once

#include <atlbase.h>
#include <atlcoll.h>
#include <streams.h>

// {DBD365F1-1AAE-4d5e-BF9F-AAE12BFF3AEE}
DEFINE_GUID(IID_IOS_ChangePitch, 0xdbd365f1, 0x1aae, 0x4d5e, 0xbf, 0x9f, 0xaa, 0xe1, 0x2b, 0xff, 0x3a, 0xee);
interface IOS_ChangePitch : public IUnknown
{
	STDMETHOD(ChangeCurPitch)(int nPitch) PURE;
};
