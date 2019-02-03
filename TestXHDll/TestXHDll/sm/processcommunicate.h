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
	char str1[260];//包含'\0'
	char str2[260];//包含'\0'
}DoubleString;

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

typedef struct _PMessage
{
	int cmd;
	int type;//0： 普通，1：弹幕
	unsigned int mode;//对弹幕有效，字幕模式: 0:普通弹幕;1:顶上弹幕;2:底下弹幕
	unsigned int fsize;//对弹幕有效，字体大小1-10
	unsigned int argb;//对弹幕有效，字体颜色
	char str[260];//包含'\0'
}PMessage;

typedef struct _PlayerScore
{
	bool valid;//有效？
	int noInSong;//这个歌的历史排名
	int noInHistory;//不同歌,该店今日的排名(店排名)
	int percentHistory;//击败多少本店演唱者
	int noInCountry;//不同歌,全国今日的排名
	int percentCountry;//击败多少全国演唱者
	_PlayerScore(){memset(this,0,sizeof(_PlayerScore));}
}PlayerScore;

typedef struct _RankingInfo
{
	int cmd;
	PlayerScore ps[2];//0 是第一位演唱者的数据;1是第二位演唱者的数据,单人演唱的时候不需要
}RankingInfo;

typedef struct _OtherGrade
{
	int cmd;
	unsigned int		cur_tm;						// 当前时间
	float						cur_pitch;					// 当前演唱者唱的音高
	int						sentence_index;			// 句序号（第一句序号为0）
	float						realtime_score;			// 当前句的实时得分
	float						accumulate_score;		// 当前句的累加得分
	double					cur_db;

	float						sentence_score;			 // 句得分
	bool						sentence_switch;		 // 是否切换了句子
}OtherGrade;

//*****************************Center to Player***********************//
enum eCMD_C2P
{
	CMD_CENTER_FIRST=0,
	//程序退出
	CMD_DXPLAYER_CLOSE,
	//音乐音量值的范围：最小wparam(int),最大lparam(int)
	CMD_DXPLAYER_MUSIC_RANGE,
	//麦克风音量值的范围：最小wparam(int),最大lparam(int)
	CMD_DXPLAYER_MIC_RANGE,
	//音调值的范围：最小wparam(int),最大lparam(int)
	CMD_DXPLAYER_TONE_RANGE,
	//下一首歌曲名，只需要名字
	//wparam(char*),（注：如果歌曲列表为空则发送"ktv1",如果没有下一首则发送"ktv2"）
	CMD_DXPLAYER_NEXT_SONGNAME,
	//歌曲信息
	//wparam歌手名(char*),lparam歌名(char*)
	CMD_DXPLAYER_SONGINFO,
	//歌手图片 ！！！！！！（需要在CMD_DXPLAYER_SONGINFO之前发送）
	//wparam歌手图片(char*包含路径,后缀)
	CMD_DXPLAYER_SINGERPIC,
	//设置歌曲的起始秒跟结束秒
	//wparam起始(int)lparam结束(int)
	CMD_DXPLAYER_SONGSTARTEND,
	//切换评分状态，
	//wparam(int)类型，0,关闭，1单人模式，2双人pk模式，3对唱模式
	CMD_DXPLAYER_GRADETYPE,
	//切换歌曲，第一次可以当成播放
	//wparam歌曲名，包含完整路径和后缀名mpg/avi(char*)
	//lparam(int):曲谱高度1(第一个字节*10)；第9位:是否评分歌曲(1是评分，0是非评分)
	//10-13位:是当前音乐音量（0-10）；14-17位:是当前麦克风音量（0-10）；18-21位:是当前音调值（0-10）
	//22位:是否对调音轨的；23位:是否是对唱；24-31位:(数据*10，曲谱高度2)；32位:最后是否显示评测结果(1是显示，0是不显示)
	CMD_DXPLAYER_SWITCHSONG,
	//继续播放
	CMD_DXPLAYER_CONTROL_RESUME,
	//暂停
	CMD_DXPLAYER_CONTROL_PAUSE,
	//切换伴奏状态，开启/关闭
	CMD_DXPLAYER_ACCOMPANY,
	//获取当前播放器音量
	CMD_DXPLAYER_GETMUSICVOLUME,
	//打开静音
	CMD_DXPLAYER_MUTE_OPEN,
	//关闭静音
	CMD_DXPLAYER_MUTE_CLOSE,
	//设置播放器声音 (0-10]
	//wparam音量(int)
	CMD_DXPLAYER_SETMUSICVOLUME,
	//获取麦克风音量
	CMD_DXPLAYER_GETMICVOLUME,
	//设置麦克风音量wparam(int)
	CMD_DXPLAYER_SETMICVOLUME,
	//获取音调
	CMD_DXPLAYER_GETTONEVALUE,
	//加1音调
	CMD_DXPLAYER_ADDTONEVALUE,
	//减1音调
	CMD_DXPLAYER_SUBTONEVALUE,
	//打开幻影
	CMD_DXPLAYER_OPENHY,
	//关闭幻影
	CMD_DXPLAYER_CLOSEHY,
	//公告,火警,到时提醒等一切信息,还有弹幕信息
	CMD_DXPLAYER_MESSAGE,
	//火警需要额外通知
	CMD_DXPLAYER_FIREWARNING,
	//取消火警
	CMD_DXPLAYER_NOFIREWARNING,
	//气氛wparam(int) : 0 代表 鼓掌，1代表喝彩
	CMD_DXPLAYER_ATMOSPHERE,
	//是否联网wparam(int),1 代表在线，0代表离线
	CMD_DXPLAYER_ONLINE,
	//视频数据传输
	CMD_DXPLAYER_VIDEO_INFO,
	//唱歌得分排名
	CMD_DXPLAYER_RANKINGLIST,//RankingInfo
	//实时评分句评分数据，来自星光
	CMD_DXPLAYER_OTHERGRADE,//OtherGrade
	//通知是否是播放外网的用户唱的歌曲 1:true 0:false
	CMD_DXPALYER_FROMOTHER,//DoubleInt
	//设置播放进度0-100
	CMD_DXPLAYER_PROGRESS,//SingleInt
	//选歌分类
	CMD_DXPLAYER_CATEGORY,//SingleString
	//控制滚动
	CMD_DXPLAYER_SELECTSTATE,//SingleInt
};
//*****************************Player to Center***********************//
enum e_CMD_P2C
{	
	CDM_PLAYER_FIRST=500,
	//当前歌曲播放完毕
	CMD_DXPLAYER_ENDSONG,
	//wparam第一个人的总分(int),lparam第二个人的总分(int),需要操作界面自己判断什么模式，如果是单人模式，第二个人的评分需要丢弃
	CMD_DXPLAYER_SCORE_STARNUM,
	//wparam音乐音量(int) 
	CMD_DXPLAYER_MUSICVOLUME_CURRENT,
	//wparam麦克风音量(int)
	CMD_DXPLAYER_MICVOLUME_CURRENT,
	//wparam音调(int)
	CMD_DXPLAYER_TONE_CURRENT,
	//启用button，通知操作界面
	CMD_DXPLAYER_ENABLE_BUTTON,
	//禁掉一些按钮，直到切歌或重唱
	CMD_DXPALYER_DISABLE_BUTTON,
	//需要几条排名信息 
	//int
	CMD_RANKING_NUM,
};
//center 指控制端，player 指dx播放端 定义两个共享内存的名字
const char C2P[] = "CENTER_TO_PLAYER";//center 只写，player只读
const char P2C[] = "PLAYER_TO_CENTER";//center 只读，player只写
const char VIDEO[] = "XINGGUANG_TO_PLAYER";


#endif// PROCESSCOMMUNICATE__H__

