#ifndef XHHEAD__H__
#define XHHEAD__H__

namespace XHH
{
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

	typedef struct _DoubleStringB
	{
		int cmd;
		char str1[260];//����'\0'
		char str2[260];//����'\0'
		bool needstart;
	}DoubleStringB;

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

	typedef struct _PMessage
	{
		int cmd;
		int type;//0�� ��ͨ��1����Ļ
		unsigned int mode;//�Ե�Ļ��Ч����Ļģʽ: 0:��ͨ��Ļ;1:���ϵ�Ļ;2:���µ�Ļ
		unsigned int fsize;//�Ե�Ļ��Ч�������С1-10
		unsigned int argb;//�Ե�Ļ��Ч��������ɫ
		char str[1024];//����'\0'
	}PMessage;

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

	typedef struct _OtherGrade
	{
		int cmd;
		unsigned int		cur_tm;						// ��ǰʱ��
		float						cur_pitch;					// ��ǰ�ݳ��߳�������
		int						sentence_index;			// ����ţ���һ�����Ϊ0��
		float						realtime_score;			// ��ǰ���ʵʱ�÷�
		float						accumulate_score;		// ��ǰ����ۼӵ÷�
		double					cur_db;

		int						sentence_level;			 //�����ֵȼ�
		float						sentence_score;			 // ��÷�
		float						sentence_lyric_score; //��ʵ÷�
		bool						sentence_switch;		 // �Ƿ��л��˾���
	}OtherGrade;


	//*****************************HOST to XINGGUANG***********************//

	enum eCMD_C2P
	{
		CMD_XINGGUANG_FIRST=0,
		//�����˳�
		CMD_XINGGUANG_CLOSE
	};
	//*****************************XINGGUANG to HOST***********************//
	enum e_CMD_P2C
	{	
		CDM_HOST_FIRST=100,
		//ʵʱ��������
		CMD_HOST_GRADE,//OtherGrade
		//��Ƶ����
		CMD_HOST_VIDEO,//VideoInfo
		//��ǰ�ݳ�����
		CMD_HOST_SONG,//DoubleString
		//��Ϣ
		CMD_HOST_MSG,//PMessage
		//�뿪����������
		CMD_HOST_LEAVE,//NULL
		//֪ͨ����Ĭ�ϸ���
		CMD_HOST_DEFAULT,//NULL
		//����ر�
		CMD_HOST_REQUESTC,//NULL
		//���뷿��
		//CMD_HOST_ENTERROOM,//NULL  deprecated
		//������û���˳���
		CMD_HOST_NOSING,//NULL
	};
	//center ָ���ƶˣ�player ָdx���Ŷ� �������������ڴ������
	const char C2P[] = "CENTER_TO_XINGGUANG";//center ֻд��XINGGUANGֻ��
	const char P2C[] = "XINGGUANG_TO_CENTER";//center ֻ����XINGGUANGֻд
	const char VIDEO[] = "XINGGUANG_TO_PLAYER";
}


#endif// XHHEAD__H__

