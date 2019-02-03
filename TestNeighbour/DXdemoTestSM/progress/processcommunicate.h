#pragma  once

#ifdef __cplusplus
extern "C" {
#endif
	//*****************************Center to Player***********************//
	enum eCMD_C2P
	{
		CMD_CENTER_FIRST=0,
		//�����˳�
		CMD_DXPLAYER_CLOSE,
		//��������ֵ�ķ�Χ����Сwparam(int),���lparam(int)
		CMD_DXPLAYER_MUSIC_RANGE,
		//��˷�����ֵ�ķ�Χ����Сwparam(int),���lparam(int)
		CMD_DXPLAYER_MIC_RANGE,
		//����ֵ�ķ�Χ����Сwparam(int),���lparam(int)
		CMD_DXPLAYER_TONE_RANGE,
		//��һ�׸�������ֻ��Ҫ����
		//wparam(char*),��ע����������б�Ϊ������"ktv1",���û����һ������"ktv2"��
		CMD_DXPLAYER_NEXT_SONGNAME,
		//������Ϣ
		//wparam������(char*),lparam����(char*)
		CMD_DXPLAYER_SONGINFO,
		//����ͼƬ ����������������Ҫ��CMD_DXPLAYER_SONGINFO֮ǰ���ͣ�
		//wparam����ͼƬ(����·��,��׺)
		CMD_DXPLAYER_SINGERPIC,
		//���ø�������ʼ���������
		//wparam��ʼ(int)lparam����(int)
		CMD_DXPLAYER_SONGSTARTEND,
		//�л�����״̬��
		//wparam(int)���ͣ�0,�رգ�1����ģʽ��2˫��pkģʽ
		CMD_DXPLAYER_GRADE,
		//�л���������һ�ο��Ե��ɲ���
		//wparam����������������·���ͺ�׺��mpg/avi(char*)
		//lparam(int),���׸߶�1(��һ���ֽ�*10),�Ƿ����ָ���(��9λ��1�����֣�0�Ƿ�����),
		//(10-13λ���ǵ�ǰ����������0-10����14-17λ���ǵ�ǰ��˷�������0-10����18-21λ���ǵ�ǰ����ֵ��0-10��)
		//,22λ�����Ƿ�Ե������,23λ�����Ƿ��Ǻϳ�,(24-31λ,����*10�����׸߶�2)��
		CMD_DXPLAYER_SWITCHSONG,
		//��������
		CMD_DXPLAYER_CONTROL_RESUME,
		//��ͣ
		CMD_DXPLAYER_CONTROL_PAUSE,
		//�л�����״̬������/�ر�
		CMD_DXPLAYER_ACCOMPANY,
		//��ȡ��ǰ����������
		CMD_DXPLAYER_GETMUSICVOLUME,
		//�򿪾���
		CMD_DXPLAYER_MUTE_OPEN,
		//�رվ���
		CMD_DXPLAYER_MUTE_CLOSE,
		//���ò��������� -10000��0: -10000��С��0���.(������-10000)
		//wparam����(int)
		CMD_DXPLAYER_SETMUSICVOLUME,
		//��ȡ��˷�����
		CMD_DXPLAYER_GETMICVOLUME,
		//������˷�����wparam(int)
		CMD_DXPLAYER_SETMICVOLUME,
		//��ȡ����
		CMD_DXPLAYER_GETTONEVALUE,
		//��1����
		CMD_DXPLAYER_ADDTONEVALUE,
		//��1����
		CMD_DXPLAYER_SUBTONEVALUE,
		//�򿪻�Ӱ
		CMD_DXPLAYER_OPENHY,
		//�رջ�Ӱ
		CMD_DXPLAYER_CLOSEHY,
		//����,��,��ʱ���ѵ�һ����Ϣ
		//������Ϣwparam(char*)
		CMD_DXPLAYER_MESSAGE,
		//����Ҫ����֪ͨ
		CMD_DXPLAYER_FIREWARNING,
		//ȡ����
		CMD_DXPLAYER_NOFIREWARNING,
		//����wparam(int) : 0 ���� ���ƣ�1����Ȳ�
		CMD_DXPLAYER_ATMOSPHERE
	};
	//*****************************Player to Center***********************//
	enum e_CMD_P2C
	{	
		CDM_PLAYER_FIRST=100,
		//��ǰ�����������
		CMD_DXPLAYER_ENDSONG,
		//wparam�ܷ�(int),lparam����(int)
		CMD_DXPLAYER_SCORE_STARNUM,
		//wparam��������(int) 
		CMD_DXPLAYER_MUSICVOLUME_CURRENT,
		//wparam��˷�����(int)
		CMD_DXPLAYER_MICVOLUME_CURRENT,
		//wparam����(int)
		CMD_DXPLAYER_TONE_CURRENT,
		CMD_DXPLAYER_ENABLE_BUTTON
	};
	//center ָ���ƶˣ�player ָdx���Ŷ� �������������ڴ������
	const char C2P[] = "CENTER_TO_PLAYER";//center ֻд��playerֻ��
	const char P2C[] = "PLAYER_TO_CENTER";//center ֻ����playerֻд

#ifdef  __cplusplus
}
#endif

