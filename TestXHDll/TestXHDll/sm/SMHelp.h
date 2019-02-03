#ifndef SMHELP__H__
#define SMHELP__H__

#ifdef SMDLL_EXPORTS
#define DLL_CLASS __declspec(dllexport)
#else
#define DLL_CLASS __declspec(dllimport)
#endif

typedef void (*DEALMESSAGE) (int cmd,int wParam,int lParam);

class DLL_CLASS CSMHelp
{
public:
	//注册回调函数
	static void InitSMHelp(DEALMESSAGE pDealMessage=NULL);
	//(player.exe 所在的目录,"player.exe")
	CSMHelp(const char* exeDir,const char* exeName);
	~CSMHelp();
	//关闭播放器
	bool PostClosePlayer();
	//切换歌曲,第二个参数用于互联网用户的mdm文件，一般只要设为NULL
	bool PostSwitchSong(const char* file,const char* mdm,int info);
	//歌手图片
	bool PostSingerPic(const char* file);
	//继续播放
	bool PostControlResume();
	//暂停播放
	bool PostControlPause();
	//切换评分模式
	bool PostPlayGrade(unsigned int eType);
	//原伴唱切换
	bool PostPlayAccompany();
	//歌曲信息
	bool PostSongInfo(const char* singer,const char* song);
	//支持简版歌曲
	bool PostSongStartEnd(int start,int end);
	//设置麦克风音量
	bool PostMicValue(int info);
	//设置音量
	bool PostSetVolume(int info);
	//bool PostGetVolume();
	//增加音调
	bool PostAddTone();
	//减音调
	bool PostSubTone();
	//获取音调
	bool PostGetTone();
	//静音
	bool PostMuteOpen();
	//取消静音
	bool PostMuteClose();
	//发送公告,以及弹幕信息，具体参数参见processcommunicate.h PMessage结构
	bool PostMessage(int type,unsigned int mode,unsigned int fsize,unsigned int argb,const char* message);
	//火警
	bool PostFireWarning();
	//取消火警
	bool PostNoFireWarning();
	//下首歌的名字
	bool PostNextSong(const char* name);
	//气氛
	bool PostSound(unsigned int eInt);
	//是否联网
	bool PostOnline(int eInt);
	//发送视频信息或者携带数据,具体参数参见processcommunicate.h VideoInfo结构
	bool PostVideoInfo(int operation,int type,unsigned int width,unsigned int height
		,unsigned int bytes_pixel,unsigned int size,bool first,const char* name);
	//发送演唱者排名信息，具体参数参见processcommunicate.h RankingInfo结构
	//pPlayerScore:PlayerScore* 指针;nPlayer: 含几个分数
	bool PostRankingList(void* pPlayerScore,int nPlayer);
	//通知播放器播放的是否是外网用户演唱的歌曲,是否是广告宣传片
	bool PostFromOther(bool bOther,bool bAdvertisement);
	//来自星光的实时评测，包含句评测,具体参数参见processcommunicate.h OtherGrade结构
	bool PostOhterGrade(unsigned int cur_time,float cur_pitch,int sentence_index,float realtime_score
		,float accumulate_score,double cur_db,float sentence_score,bool sentence_switch);
	//调整播放进度，0-100
	bool PostProgress(int progress);
	//选择歌曲分类，播放器默认会在当前目录读取randomlist.csv文件，获取分类歌曲信息，请保证歌曲名不同
	bool PostCategory(const char* category);
	//正如你所见，这个的意思true是暂停滚动，如果为false可以继续重新滚动
	bool PostSelectState(bool bStop);
private:
	bool KillProcessFromName(const char* pProcessName);
public:
	static DEALMESSAGE m_pDealMessage;
};

#endif//SMHELP__H__