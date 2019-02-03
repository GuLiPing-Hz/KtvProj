/*
	ע��ʱ��:2014-4-25
	author: glp
*/
#ifndef GLDEF__H__
#define GLDEF__H__

//#include <objbase.h>
//#include "progress/processcommunicate.h"
//glp

static char LOGPATH[260] = {0}; //��־path

#define WSTR_MONEYEXCHANGE_NOTIFY L"������������һ�����!"

enum eVideoType
{
	vtHost,
	vtSing,
};

enum eShowWave
{
	SHOW_NONE,//������
	SHOW_ONE,//����ģʽ
	SHOW_TWO,//PKģʽ
	SHOW_THREE,//�Գ�ģʽ
};

enum eNotify
{
	NOTIFY_NONE=-1,
	//NOTIFY_MESSAGE
	NOTIFY_MUTE_OPEN,//Notify0
	NOTIFY_MUTE_CLOSE,//Notify1
	NOTIFY_PLAY_STOPPING,//Notify2
	NOTIFY_PLAY_RUNNING,//Notify3
	NOTIFY_ORIGIN_OPEN,//Notify4
	NOTIFY_ORIGIN_CLOSE,//Notify5
	//����ģʽ
	NOTIFY_CLOSE_GRADE,//Notify6
	NOTIFY_SINGLE_GRADE,//Notify7
	NOTIFY_DOUBLE_GRADE,//Notify8
	NOTIFY_THREE_GRADE,//Notify9

	//��Ч
	NOTIFY_MAGICAL_FAST,//Notify 10
	NOTIFY_MAGICAL_TOP,//Notify 11
	NOTIFY_MAGICAL_SINGER,//Notify 12
	NOTIFY_MAGICAL_KTV,//Notify 13
	NOTIFY_PROFESSINAL_FAST,//Notify 14
	NOTIFY_PROFESSINAL_TOP,//Notify 15
	NOTIFY_PROFESSINAL_SINGER,//Notify 16
	NOTIFY_PROFESSINAL_KTV,//Notify 17

	NOTIFY_APPLAUD,//png���� applaud
	NOTIFY_CHEER,//png���� cheer

	NOTIFY_MICVOLUME_SET,//���ͼ
	NOTIFY_MUSICVOLUME_SET,//���ͼ
	NOTIFY_TONE_SET,//���ͼ
	//HEAD_MESSAGE
	MESSAGE_NOTIFY,
	MESSAGE_NEXTSONG,
};

typedef struct _SingInfo{
	char mdmFile[260];
	char songName[260];
	char singerName[260];
	char singerSingle[260];//���Ϊ�Գ������������ﱣ��ĳһλ������
	char nextSongName[260];
}SingInfo;

typedef struct _StructSound{
	int cur_mic;
	int min_mic;
	int max_mic;
	int cur_music;
	int min_music;
	int max_music;
	int cur_tone;
	int min_tone;
	int max_tone;
}StructSound;

typedef struct _WaveConfig{
	float			SING_START_B;//����ݳ���
	int 			WAVEWND_WIDTH_TIMEMS;// = 10000;//2222;//5000;		// һ��5000����
	int 			WAVEWND_WIDTH_PIXELS;// = 1280;//320;//720;		// һ�����720����
	int 			WAVEWND_HEIGHT_PIXELS;// = 87;//79;		// ���׵ĸ߶�40����
	int 			WAVEWND_HEIGHT_PIXELS_KTV;// = 87;//105;	// ���׵ĸ߶�40����
	int 			WAVEWND_LEFT_PIXELS;// = 200;//170;		// ��ߵ�����
	//int CURSOR_WIDTH_PIXELS;// = 36;			// �����
	int			WAVEWND_LEFT_TIMEMS;// = 1210;		// WAVEWND_WIDTH_TIMEMS * WAVEWND_LEFT_PIXELS / WAVEWND_WIDTH_PIXELS + 30;

	//int WAVEWND_WIDTH_PIXELS_KTV;// = 320;//460;	// һ�����632����(KTVģʽ)
	float			WAVEWND_WIDTH_PIXELS_KTV_TM_CUR;//ָ������ ����/ʱ�䣨���룩
	float			WAVEWND_WIDTH_PIXELS_KTV_TM_NEXT;

	int			SHOW_TIP_IMAGE_MSEC;// = 4000;		// 4000����~2000����	��ʾ��ʾͼ�꣨ͼ����˸��
	int			SHOW_MOVE_IMAGE_MSEC;// = 2000;		// 2000����~0����		��ʾ��껬������
	int			NEED_SHOW_MOVE_IMAGE_MSEC;// = 500;	// >= 1000����		��Ҫ��ʾ��껬������

	// ����16�����������Ӣ���ַ����
	int			CHAR_WIDTH_CHS;// = 21;				// ���������ȸ߶�
	int			CHAR_WIDTH_EN;// = 12;				// Ӣ�������ȸ߶�

	int			MAX_LYRIC_WIDTH;//  700// ��������
	float			VERTICAL_LINE_HEIGHT;//����λ�õĸ߶�
	int			CONFIG_SPEED_PARTICLE;
	int			CONFIG_BARRAGEF_SIZE;//��Ļ�����С
}WaveConfig;

typedef struct _PlayInfo{
	float			POS_WAVE_HEIGHTSINGLE;//���ݿ���ȡ�����׸߶�
	float			POS_WAVE_HEIGHTDOUBLE;
	//float height_second;//����2�߶�
	eShowWave eType;//����ģʽ
	int			start_ms;//֧�ּ������Ŀ�ʼ������
	int			end_ms;//֧�ּ������Ľ���������
	bool			ktv_mode;//�Ƿ�ΪKTV ģʽ
}PlayInfo;

#define SECONDWAVEHEIGHT 280
//�������ݾ��ĵ��䣬�ڵ�ǰ��͸��ͶӰ�¿���ʱ��Ļ�����LEFTUPPER��0,0����RIGHTBOTTOM��1280,720��
//͸��ͶӰʹ��ֵ ֧�������λС�������˾�û����
#define GL3D_Z_VIDEO 10.0f
//10.0f
//base z
//������SceneObject��:����������Ⱦ��ܣ���һ�ж���,����������ϵ�һ������һ��SceneObject�����ٵ����ṩĳ��render��
//�����ֽ���SceneObject��:��ԭ����object�е�render���������
#define GL3D_Z_BASE 9.999f
//������->������SceneObject��
#define GL3D_Z_LYRIC 9.998f
//�����ǹ����Ƶ���ṩ���ǹ���ƵSceneObject��
#define GL3D_Z_XGVIDEO 9.997f
//��Ļ����������SceneObject
#define GL3D_Z_BARRAGE 9.996f
//���ѣ����棬���յȣ���Ϊһ������������SceneObject����ʹ�ù������ѹ���
#define GL3D_Z_NOTIFY 9.995f
//9.995f
#define GL3D_Z_SONGINFO 9.994f
//Ĭ������ϵͳz���꣬��������Ҫ����ϵͳչʾ��Object��Ҳ���ṩ������ϵͳSceneObject������һ������������ϵͳ
#define GL3D_Z_PARTICLE 9.993f
//��Ҫչʾ������ϵͳ֮ǰ�����
#define GL3D_Z_BEFOREPARTICLE 9.992f
//�𾯣���Ļ���������ϵͳ��ʹ��Ĭ��ֵ������Ҫֹͣ�����µ����ӣ���Stop.�ṩ����SceneObject��
#define GL3D_Z_FIRALARM 9.5f
//����zֵҲ�����鿼��ʹ�ã�
#define CAN_BEFORE_FIRALARM(z) \
{ \
	(-0.0000001f<((z)-9.0f)&&((z)-9.0f)<0.0000001f) ? true:false; \
}

typedef struct _CGPoint
{
	float x;
	float y;
	float z;
	_CGPoint& operator+=(const _CGPoint& a)
	{
		this->x += a.x;
		this->y += a.y;
		this->z += a.z;
		return *this;
	}
	_CGPoint& operator=(const _CGPoint& a)
	{
		this->x = a.x;
		this->y = a.y;
		this->z = a.z;
		return *this;
	}
	bool operator==(const _CGPoint& a)
	{
		if ((this->x-a.x<0.01)&&(this->y-a.y)<0.01&&(this->z-a.z)<0.01)
		{
			return true;
		}
		return false;
	}
	_CGPoint(const float a=0,const float b=0,const float c=GL3D_Z_BASE)
	{
		this->x = a;
		this->y = b;
		this->z = c;
	}
	_CGPoint(const _CGPoint& point)
	{
		this->x = point.x;
		this->y = point.y;
		this->z = point.z;
	}
}GPoint;

typedef struct _CGSize 
{
	int width;
	int height;
	_CGSize& operator+=(const _CGSize& a)
	{
		this->width += a.width;
		this->height += a.height;
		return *this;
	}
	_CGSize& operator=(const _CGSize& a)
	{
		if ( this == &a )
			return *this;
		this->width = a.width;
		this->height = a.height;
		return *this;
	}
	bool operator==(const _CGSize& a)
	{
		if (this->width==a.width&&this->height==a.height)
		{
			return true;
		}
		return false;
	}
	_CGSize(const int w=0,const int h=0)
	{
		this->width = w;
		this->height = h;
	}
	_CGSize(const _CGSize& size)
	{
		this->width = size.width;
		this->height = size.height;
	}
}GSize;

static GSize CGSIZEZERO(0,0);

typedef struct _CGRect
{
	GPoint origin;
	GSize size;
	_CGRect& operator=(const _CGRect& a)
	{
		this->origin = a.origin;
		this->size = a.size;
		return *this;
	}
	bool operator==(const _CGRect& a)
	{
		if (this->origin==a.origin&&this->size==a.size)
		{
			return true;
		}
		return false;
	}
	_CGRect(const GPoint& a=GPoint(0.0f,0.0f),const GSize& b=GSize(0,0))
	{
		this->origin = a;
		this->size = b;
	}
	_CGRect(const float x,const float y,const int w,const int h)
	{
		this->origin = GPoint(x,y);
		this->size = GSize(w,h);
	}
	_CGRect(const _CGRect& rect)
	{
		this->origin = rect.origin;
		this->size = rect.size;
	}
}CGRect;

typedef struct _LINEVERTEX
{
	GPoint point;
	unsigned long colour;
}LINEVERTEX;


typedef struct _CUSTOMVERTEX
{
	GPoint point;
	unsigned long colour;
	float u;
	float v;
}CUSTOMVERTEX;

static CUSTOMVERTEX s_initVertex = { GPoint(0.0f,0.0f,GL3D_Z_BASE),
																0xFFFFFFFF,
																0.0f,0.0f};

typedef struct _hgeInputEvent 
{
	int		type;			// event type
	int		key;			// key code
	int		flags;			// event flags
	int		chr;			// character code
	int		wheel;			// wheel shift
	float		x;				// mouse cursor x-coordinate
	float		y;				// mouse cursor y-coordinate
}hgeInputEvent;

typedef struct _CInputEventList
{
	hgeInputEvent		event;
	_CInputEventList*	next;
}CInputEventList;

typedef struct _BarrageInfo
{
	int type;//0�� ��ͨ��1����Ļ
	unsigned int mode;//�Ե�Ļ��Ч����Ļģʽ: 0:��ͨ��Ļ;1:���ϵ�Ļ;2:���µ�Ļ
	/*
	��Բ�Ʒ�����ƶ����ַ�����ʽ
	<txt></txt>��ǩ�У� ����spaceָ�����ּ�࣬����sizeָ�����ִ�С,����colorָ��������ɫ������txtָ����Ҫչʾ����������
	<space></space>��ǩ�У�����widthָ���ո����ش�С
	<img></img>��ǩ�У�����srcָ������������ͼƬ������spaceָ��ͼƬǰ��ļ�࣬ǰ���space/2
	*/
	char str[1024];//����'\0'   ��Ҫ����CONFIG_MAX_LEN���ַ� ����ǵ�Ļ�Ļ�����˵��������
	int sayid;//�Ե�Ļ��Ч ˵����ID
	char sayname[100];//�Ե�Ļ��Ч ˵��������
	char location[260];//˵����λ��
}BarrageInfo;

typedef unsigned char uchar;
typedef float CGFloat;
typedef unsigned int  uint;
typedef unsigned long ulong;

static CGRect CGRECTZERO(0.0f,0.0f,0,0);

typedef struct _reVertex
{
	CUSTOMVERTEX* v;
	unsigned int count;
	void* tex;
	int blend;
}reVertex;
/*
** Triple structure
*/
typedef struct _reTriangle
{
	CUSTOMVERTEX		v[3];
	void*			tex;
	int				blend;
}reTriangle;
/*
** Quad structure
*/
typedef struct _reQuad
{
	CUSTOMVERTEX		v[4];
	void*		tex;
	int			blend;
}reQuad;

typedef enum _eDrawType
{
	REPRIM_LINESTRIP,
	REPRIM_TRIANGLESTRIP,
	REPRIM_LINES=2,
	REPRIM_TRIANGLES,
	REPRIM_QUADS,
}eDrawType;
/*
** Common math constants
*/
#ifndef M_PI
#define M_PI	3.14159265358979323846f
#endif//M_PI
#define M_PI_2	1.57079632679489661923f
#define M_PI_4	0.785398163397448309616f
#define M_1_PI	0.318309886183790671538f
#define M_2_PI	0.636619772367581343076f
#define M_PI_180 0.01745329252f
#define M_180_PI 57.29577951f


#define	BLEND_COLORMUL		0
#define	BLEND_COLORADD		1
#define	BLEND_COLORSELECT2 2
#define	BLEND_COLORMASK		0xF

#define	BLEND_ALPHAADD		0x00
#define	BLEND_ALPHABLEND	0x10
#define	BLEND_ALPHAMASK		0x10

#define   BLEND_NOZWRITE 0x00
#define   BLEND_ZWRITE		0x20
#define   BLEND_ZMASK		0x20

#define   BLEND_DEFAULT		(BLEND_NOZWRITE | BLEND_ALPHABLEND | BLEND_COLORMUL)


#define D3DFVF_CUSTOMVERTEX ( D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX0 )
#define D3DFVF_LINEVERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE)

#define BATCH_BUFFER_SIZE 6000
#define BATCH_LINE_BUFFERSIZE 2000
#define WM_PLAYER_MESSAGE (WM_USER+200)
#define WM_GRAPHNOTIFY				(WM_PLAYER_MESSAGE+1)
#define WM_SWITCHSONG				(WM_PLAYER_MESSAGE+2)
#define WM_PLAY_CONTROL			(WM_PLAYER_MESSAGE+3)
#define WM_PLAY_GRADE				(WM_PLAYER_MESSAGE+4)
#define WM_PLAY_ACOMMPANY	(WM_PLAYER_MESSAGE+5)
#define WM_PLAY_END					    (WM_PLAYER_MESSAGE+6)
#define WM_DEBUG							(WM_PLAYER_MESSAGE+7)
#define WM_PLAY_SONGINFO			(WM_PLAYER_MESSAGE+8)
#define WM_PLAY_SINGERPIC			(WM_PLAYER_MESSAGE+9)
#define WM_PLAY_SONGSTARTEND	(WM_PLAYER_MESSAGE+10)
#define WM_PLAY_FINALSCORE (WM_PLAYER_MESSAGE+11)

#define WM_PLAY_VOLUME_PUT		(WM_PLAYER_MESSAGE+11)
#define WM_PLAY_VOLUME_GET		(WM_PLAYER_MESSAGE+12)
//#define MSE_PITCHTRACK_ALGTYPE_YinACF	2

#endif//GLDEF__H__

