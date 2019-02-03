#pragma  once

#ifdef __cplusplus
extern "C" {
#endif
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
		//wparam歌手图片(包含路径,后缀)
		CMD_DXPLAYER_SINGERPIC,
		//设置歌曲的起始秒跟结束秒
		//wparam起始(int)lparam结束(int)
		CMD_DXPLAYER_SONGSTARTEND,
		//切换评分状态，
		//wparam(int)类型，0,关闭，1单人模式，2双人pk模式
		CMD_DXPLAYER_GRADE,
		//切换歌曲，第一次可以当成播放
		//wparam歌曲名，包含完整路径和后缀名mpg/avi(char*)
		//lparam(int),曲谱高度1(第一个字节*10),是否评分歌曲(第9位，1是评分，0是非评分),
		//(10-13位，是当前音乐音量（0-10），14-17位，是当前麦克风音量（0-10），18-21位，是当前音调值（0-10）)
		//,22位，是是否对调音轨的,23位，是是否是合唱,(24-31位,数据*10，曲谱高度2)，
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
		//设置播放器声音 -10000到0: -10000最小，0最大.(静音传-10000)
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
		//公告,火警,到时提醒等一切信息
		//具体信息wparam(char*)
		CMD_DXPLAYER_MESSAGE,
		//火警需要额外通知
		CMD_DXPLAYER_FIREWARNING,
		//取消火警
		CMD_DXPLAYER_NOFIREWARNING,
		//气氛wparam(int) : 0 代表 鼓掌，1代表喝彩
		CMD_DXPLAYER_ATMOSPHERE
	};
	//*****************************Player to Center***********************//
	enum e_CMD_P2C
	{	
		CDM_PLAYER_FIRST=100,
		//当前歌曲播放完毕
		CMD_DXPLAYER_ENDSONG,
		//wparam总分(int),lparam星星(int)
		CMD_DXPLAYER_SCORE_STARNUM,
		//wparam音乐音量(int) 
		CMD_DXPLAYER_MUSICVOLUME_CURRENT,
		//wparam麦克风音量(int)
		CMD_DXPLAYER_MICVOLUME_CURRENT,
		//wparam音调(int)
		CMD_DXPLAYER_TONE_CURRENT,
		CMD_DXPLAYER_ENABLE_BUTTON
	};
	//center 指控制端，player 指dx播放端 定义两个共享内存的名字
	const char C2P[] = "CENTER_TO_PLAYER";//center 只写，player只读
	const char P2C[] = "PLAYER_TO_CENTER";//center 只读，player只写

#ifdef  __cplusplus
}
#endif

