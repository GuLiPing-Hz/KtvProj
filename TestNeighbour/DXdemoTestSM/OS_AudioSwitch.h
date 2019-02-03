#pragma once

#include <atlbase.h>
#include <atlcoll.h>
#include <streams.h>

// {1A803F61-158F-49ed-871F-037FCE71A339}
DEFINE_GUID(IID_IOS_AudioSwitch, 0x1a803f61, 0x158f, 0x49ed, 0x87, 0x1f, 0x3, 0x7f, 0xce, 0x71, 0xa3, 0x39);
interface IOS_AudioSwitch : public IUnknown
{
	STDMETHOD(SwitchAudio)(bool bFirstAudio) PURE;
};

