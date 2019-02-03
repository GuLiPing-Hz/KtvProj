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
		char str1[260];//包含'\0'
		char str2[260];//包含'\0'
	}DoubleString;

	typedef struct _DoubleStringB
	{
		int cmd;
		char str1[260];//包含'\0'
		char str2[260];//包含'\0'
		bool needstart;
	}DoubleStringB;

	typedef struct _SingleString
	{
		int cmd;
		char str[260];//包含'\0'
	}SingleString;

	typedef struct _DStringInt
	{
		int cmd;
		char str[260];//包含'\0'
		char mdm[260];//包含'\0',mdm文件名，用于播放非MV视频文件的时候。一般不用
		int wparam;
	}DStringInt;

	typedef struct _PMessage
	{
		int cmd;
		int type;//0： 普通，1：弹幕
		unsigned int mode;//对弹幕有效，字幕模式: 0:普通弹幕;1:顶上弹幕;2:底下弹幕
		unsigned int fsize;//对弹幕有效，字体大小1-10
		unsigned int argb;//对弹幕有效，字体颜色
		char str[1024];//包含'\0'
	}PMessage;

	typedef struct _VideoInfo
	{
		int cmd;
		int operation;//0：关闭，1：开启，2：暂停排麦
		int type;//0：主持人，1：本地
		bool bname;//第一次发送名字的时候需要置为true，其他置为false
		char name[260];
		unsigned int width;//视频宽度
		unsigned int height;//视频高度
		unsigned int bytes_pixel;//单位像素的字节数
		unsigned int size;//视频数据量，字节为单位,无数据设为0
		unsigned char* buffer;//视频 RGB 数据
	}VideoInfo;

	typedef struct _OtherGrade
	{
		int cmd;
		unsigned int		cur_tm;						// 当前时间
		float						cur_pitch;					// 当前演唱者唱的音高
		int						sentence_index;			// 句序号（第一句序号为0）
		float						realtime_score;			// 当前句的实时得分
		float						accumulate_score;		// 当前句的累加得分
		double					cur_db;

		int						sentence_level;			 //句评分等级
		float						sentence_score;			 // 句得分
		float						sentence_lyric_score; //歌词得分
		bool						sentence_switch;		 // 是否切换了句子
	}OtherGrade;


	//*****************************HOST to XINGGUANG***********************//

	enum eCMD_C2P
	{
		CMD_XINGGUANG_FIRST=0,
		//程序退出
		CMD_XINGGUANG_CLOSE
	};
	//*****************************XINGGUANG to HOST***********************//
	enum e_CMD_P2C
	{	
		CDM_HOST_FIRST=100,
		//实时评测数据
		CMD_HOST_GRADE,//OtherGrade
		//视频数据
		CMD_HOST_VIDEO,//VideoInfo
		//当前演唱歌曲
		CMD_HOST_SONG,//DoubleString
		//消息
		CMD_HOST_MSG,//PMessage
		//离开房间进入大厅
		CMD_HOST_LEAVE,//NULL
		//通知播放默认歌曲
		CMD_HOST_DEFAULT,//NULL
		//请求关闭
		CMD_HOST_REQUESTC,//NULL
		//进入房间
		//CMD_HOST_ENTERROOM,//NULL  deprecated
		//房间内没有人唱歌
		CMD_HOST_NOSING,//NULL
	};
	//center 指控制端，player 指dx播放端 定义两个共享内存的名字
	const char C2P[] = "CENTER_TO_XINGGUANG";//center 只写，XINGGUANG只读
	const char P2C[] = "XINGGUANG_TO_CENTER";//center 只读，XINGGUANG只写
	const char VIDEO[] = "XINGGUANG_TO_PLAYER";
}


#endif// XHHEAD__H__

