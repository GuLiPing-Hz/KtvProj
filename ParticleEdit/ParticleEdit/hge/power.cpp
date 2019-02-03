/*
** Haaf's Game Engine 1.8
** Copyright (C) 2003-2007, Relish Games
** hge.relishgames.com
**
** Core functions implementation: power status
*/


#include "hge_impl.h"


void HGE_Impl::_InitPowerStatus()//初始化电源状态获取函数
{
	hKrnl32 = LoadLibrary("kernel32.dll");

	if(hKrnl32 != NULL)
		lpfnGetSystemPowerStatus = (GetSystemPowerStatusFunc)GetProcAddress(hKrnl32, "GetSystemPowerStatus");

	_UpdatePowerStatus();
}


void HGE_Impl::_UpdatePowerStatus()//更新当前系统的电源状态
{
	SYSTEM_POWER_STATUS ps;

	if(lpfnGetSystemPowerStatus != NULL && lpfnGetSystemPowerStatus(&ps))
	{
		if(ps.ACLineStatus == 1)//使用电线
		{
			nPowerStatus = HGEPWR_AC;
		}
		else if(ps.BatteryFlag < 128)//使用电池
		{
			nPowerStatus = ps.BatteryLifePercent;//当前电量所占总量百分比
		}
		else
		{
			nPowerStatus = HGEPWR_UNSUPPORTED;
		}
	}
	else
	{
		nPowerStatus = HGEPWR_UNSUPPORTED;
	}
}


void HGE_Impl::_DonePowerStatus()//释放库
{
	if(hKrnl32 != NULL) FreeLibrary(hKrnl32);
}
