/*
** Haaf's Game Engine 1.8
** Copyright (C) 2003-2007, Relish Games
** hge.relishgames.com
**
** Core functions implementation: power status
*/


#include "hge_impl.h"


void HGE_Impl::_InitPowerStatus()//��ʼ����Դ״̬��ȡ����
{
	hKrnl32 = LoadLibrary("kernel32.dll");

	if(hKrnl32 != NULL)
		lpfnGetSystemPowerStatus = (GetSystemPowerStatusFunc)GetProcAddress(hKrnl32, "GetSystemPowerStatus");

	_UpdatePowerStatus();
}


void HGE_Impl::_UpdatePowerStatus()//���µ�ǰϵͳ�ĵ�Դ״̬
{
	SYSTEM_POWER_STATUS ps;

	if(lpfnGetSystemPowerStatus != NULL && lpfnGetSystemPowerStatus(&ps))
	{
		if(ps.ACLineStatus == 1)//ʹ�õ���
		{
			nPowerStatus = HGEPWR_AC;
		}
		else if(ps.BatteryFlag < 128)//ʹ�õ��
		{
			nPowerStatus = ps.BatteryLifePercent;//��ǰ������ռ�����ٷֱ�
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


void HGE_Impl::_DonePowerStatus()//�ͷſ�
{
	if(hKrnl32 != NULL) FreeLibrary(hKrnl32);
}
