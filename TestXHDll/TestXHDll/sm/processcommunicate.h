#ifndef PROCESSCOMMUNICATE__H__
#define PROCESSCOMMUNICATE__H__

typedef struct _DoubleInt
{
	int cmd;
	int wparam;
	int lparam;
}DoubleInt;

typedef struct _SingleInt
{
	int cmd;
	int wparam;
}SingleInt;

typedef struct _DoubleString
{
	int cmd;
	char str1[260];//����'\0'
	char str2[260];//����'\0'
}DoubleString;

typedef struct _SingleString
{
	int cmd;
	char str[260];//����'\0'
}SingleString;

typedef struct _DStringInt
{
	int cmd;
	char str[260];//����'\0'
	char mdm[260];//����'\0',mdm�ļ��������ڲ��ŷ�MV��Ƶ�ļ���ʱ��һ�㲻��
	int wparam;
}DStringInt;

typedef struct _VideoInfo
{
	int cmd;
	int operation;//0���رգ�1��������2����ͣ����
	int type;//0�������ˣ�1������
 	bool bname;//��һ�η������ֵ�ʱ����Ҫ��Ϊtrue��������Ϊfalse
 	char name[260];
	unsigned int width;//��Ƶ���
	unsigned int height;//��Ƶ�߶�
	unsigned int bytes_pixel;//��λ���ص��ֽ���
	unsigned int size;//��Ƶ���������ֽ�Ϊ��λ,��������Ϊ0
	unsigned char* buffer;//��Ƶ RGB ����
}VideoInfo;

typedef struct _PMessage
{
	int cmd;
	int type;//0�� ��ͨ��1����Ļ
	unsigned int mode;//�Ե�Ļ��Ч����Ļģʽ: 0:��ͨ��Ļ;1:���ϵ�Ļ;2:���µ�Ļ
	unsigned int fsize;//�Ե�Ļ��Ч�������С1-10
	unsigned int argb;//�Ե�Ļ��Ч��������ɫ
	char str[260];//����'\0'
}PMessage;

typedef struct _PlayerScore
{
	bool valid;//��Ч��
	int noInSong;//��������ʷ����
	int noInHistory;//��ͬ��,�õ���յ�����(������)
	int percentHistory;//���ܶ��ٱ����ݳ���
	int noInCountry;//��ͬ��,ȫ�����յ�����
	int percentCountry;//���ܶ���ȫ���ݳ���
	_PlayerScore(){memset(this,0,sizeof(_PlayerScore));}
}PlayerScore;

typedef struct _RankingInfo
{
	int cmd;
	PlayerScore ps[2];//0 �ǵ�һλ�ݳ��ߵ�����;1�ǵڶ�λ�ݳ��ߵ�����,�����ݳ���ʱ����Ҫ
}RankingInfo;

typedef struct _OtherGrade
{
	int cmd;
	unsigned int		cur_tm;						// ��ǰʱ��
	float						cur_pitch;					// ��ǰ�ݳ��߳�������
	int						sentence_index;			// ����ţ���һ�����Ϊ0��
	float						realtime_score;			// ��ǰ���ʵʱ�÷�
	float						accumulate_score;		// ��ǰ����ۼӵ÷�
	double					cur_db;

	float						sentence_score;			 // ��÷�
	bool						sentence_switch;		 // �Ƿ��л��˾���
}OtherGrade;

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
	//wparam����ͼƬ(char*����·��,��׺)
	CMD_DXPLAYER_SINGERPIC,
	//���ø�������ʼ���������
	//wparam��ʼ(int)lparam����(int)
	CMD_DXPLAYER_SONGSTARTEND,
	//�л�����״̬��
	//wparam(int)���ͣ�0,�رգ�1����ģʽ��2˫��pkģʽ��3�Գ�ģʽ
	CMD_DXPLAYER_GRADETYPE,
	//�л���������һ�ο��Ե��ɲ���
	//wparam����������������·���ͺ�׺��mpg/avi(char*)
	//lparam(int):���׸߶�1(��һ���ֽ�*10)����9λ:�Ƿ����ָ���(1�����֣�0�Ƿ�����)
	//10-13λ:�ǵ�ǰ����������0-10����14-17λ:�ǵ�ǰ��˷�������0-10����18-21λ:�ǵ�ǰ����ֵ��0-10��
	//22λ:�Ƿ�Ե�����ģ�23λ:�Ƿ��ǶԳ���24-31λ:(����*10�����׸߶�2)��32λ:����Ƿ���ʾ������(1����ʾ��0�ǲ���ʾ)
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
	//���ò��������� (0-10]
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
	//����,��,��ʱ���ѵ�һ����Ϣ,���е�Ļ��Ϣ
	CMD_DXPLAYER_MESSAGE,
	//����Ҫ����֪ͨ
	CMD_DXPLAYER_FIREWARNING,
	//ȡ����
	CMD_DXPLAYER_NOFIREWARNING,
	//����wparam(int) : 0 ���� ���ƣ�1����Ȳ�
	CMD_DXPLAYER_ATMOSPHERE,
	//�Ƿ�����wparam(int),1 �������ߣ�0��������
	CMD_DXPLAYER_ONLINE,
	//��Ƶ���ݴ���
	CMD_DXPLAYER_VIDEO_INFO,
	//����÷�����
	CMD_DXPLAYER_RANKINGLIST,//RankingInfo
	//ʵʱ���־��������ݣ������ǹ�
	CMD_DXPLAYER_OTHERGRADE,//OtherGrade
	//֪ͨ�Ƿ��ǲ����������û����ĸ��� 1:true 0:false
	CMD_DXPALYER_FROMOTHER,//DoubleInt
	//���ò��Ž���0-100
	CMD_DXPLAYER_PROGRESS,//SingleInt
	//ѡ�����
	CMD_DXPLAYER_CATEGORY,//SingleString
	//���ƹ���
	CMD_DXPLAYER_SELECTSTATE,//SingleInt
};
//*****************************Player to Center***********************//
enum e_CMD_P2C
{	
	CDM_PLAYER_FIRST=500,
	//��ǰ�����������
	CMD_DXPLAYER_ENDSONG,
	//wparam��һ���˵��ܷ�(int),lparam�ڶ����˵��ܷ�(int),��Ҫ���������Լ��ж�ʲôģʽ������ǵ���ģʽ���ڶ����˵�������Ҫ����
	CMD_DXPLAYER_SCORE_STARNUM,
	//wparam��������(int) 
	CMD_DXPLAYER_MUSICVOLUME_CURRENT,
	//wparam��˷�����(int)
	CMD_DXPLAYER_MICVOLUME_CURRENT,
	//wparam����(int)
	CMD_DXPLAYER_TONE_CURRENT,
	//����button��֪ͨ��������
	CMD_DXPLAYER_ENABLE_BUTTON,
	//����һЩ��ť��ֱ���и���س�
	CMD_DXPALYER_DISABLE_BUTTON,
	//��Ҫ����������Ϣ 
	//int
	CMD_RANKING_NUM,
};
//center ָ���ƶˣ�player ָdx���Ŷ� �������������ڴ������
const char C2P[] = "CENTER_TO_PLAYER";//center ֻд��playerֻ��
const char P2C[] = "PLAYER_TO_CENTER";//center ֻ����playerֻд
const char VIDEO[] = "XINGGUANG_TO_PLAYER";


#endif// PROCESSCOMMUNICATE__H__

