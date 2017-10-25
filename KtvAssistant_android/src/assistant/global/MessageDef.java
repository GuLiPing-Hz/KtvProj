package assistant.global;
/**
 * @author 宋静
 * @version 创建时间：2013-7-11 下午5:35:14
 * 类说明 全局消息变量定义
 */
public interface MessageDef {
	int START = 110000;
	
	int PHONE_GOING 		= START + 1;
	int PHONE_ENDING 		= START + 2;
	int PHONE_COMING 		= START + 3;
	
	int NET_ON 				= START + 4;
	int NET_OFF 			= START + 5;
	
	String INTENT_ACTION 	= "action";
	
	//api command
	int WM_PARAMATER_ERROR						= START + 1000; //参数传递错误
	int WM_GET_SONG_TOPICS						= START + 2000; //根据点歌模块获取歌曲专题列表
	int WM_GET_SONG_TOPICS_BY_ARTIST			= START + 2001; //根据点歌模块获取歌曲专题列表，歌星为三级列表
	int WM_GET_SONG_LIST_BY_TOPIC				= START + 2002; //根据模块和歌曲专题获取歌曲列表
	int WM_GET_SONG_LIST_BY_ARTIST				= START + 2003; //根据原唱歌手获取歌曲列表
	int WM_LOGIN_ROOM							= START + 2004; //登录房间
	int WM_LOGOUT_ROOM							= START + 2005; //退出房间
	int WM_SONG_SEARCH							= START + 2006; //歌曲搜索
	int WM_GET_SONG_LIST_BY_LANGUAGE			= START + 2007; //根据歌曲语音种类获取歌曲列表
	int WM_ORDER_SONG							= START + 2008; //点歌
	int WM_GET_ORDERED_SONG_LIST				= START + 2009; //获取已点歌曲列表
	int WM_SET_SONG_TO_TOP						= START + 2010; //置顶歌曲
	int WM_DELETE_SONG_FROM_ORDERED				= START + 2011; //删除已点歌曲
	int WM_UPDATE_ORDERED						= START + 2012; //更新已点歌曲列表
	int WM_ADD_AND_SET_SONG_TO_TOP				= START + 2013; //点歌，并把歌曲置顶
	int WM_GET_CURRENT_SONG						= START + 2014; //获取当前正在播放的歌曲
	int WM_UPDATE_SONGS							= START + 2015; //更新已点歌曲列表
	int WM_SONG_SEARCH_BY_ARTIST				= START + 2016; //歌手下的歌曲搜索
	
	//1.1版本新增
	int WM_CHECK_POINT_RESULT					= START + 3001; //是否有未领积分
	int WM_GET_POINTS_INFO						= START + 3002; //获取用户的积分信息
	int WM_RECEIVE_POINTS						= START + 3003; //用户领取积分
	int WM_THIRD_LOGIN_RESULT 					= START + 3004;	//第三方账户登录
	int WM_THIRD_LOGOUT 						= START + 3005;	//第三方账户退出
	int WM_UPLOAD_PHOTO 						= START + 3006;	//用户头像上传
	int WM_GET_COLLECT_LIST 					= START + 3007;	//获取收藏列表
	int WM_COLLECT_SONG 						= START + 3008;	//收藏歌曲
	int WM_DELETE_COLLECT_SONG 			= START + 3009;	//取消收藏歌曲
	int WM_GET_TOP_SCORELIST 					= START + 3010;	//获取高分榜歌曲列表
	int WM_GET_BANNERLIST 						= START + 3011;	//获取banner列表
	int WM_POSSESS_RANKWORK 				= START + 3012;	//高分榜上榜歌曲抢榜
	int WM_CHANGE_SONG 							= START + 3013;	//切歌
	int WM_GET_MESSAGE_LIST 					= START + 3014;	//获取消息列表
	int WM_GET_MESSAGE_DETAIL 			    = START + 3015;	//获取消息详细
	int WM_MODIFT_NICKNAME 				    = START + 3016;	//修改昵称
	int WM_AUTOLOGIN_SUCCESS				= START+3017;//自动登陆
	int WM_GET_ROOM_GOLD						= START+3018;//扫码包厢金币
	int WM_REGISTER_SUCCESS						= START+3019;//注册
	int WM_LOGIN_RESULT							=START+3020;//界面输入账号登陆成功
	
	//账户
	int WM_LOGIN 						            = START + 4001; //登录
	int WM_LOGIN_FAIL 			                = START + 4002; //登录失败
	int WM_LOGIN_CANCEL 			        = START + 4003; //登录取消
	int WM_BIND							            = START + 4004; //绑定账号
	int WM_BIND_FAIL 				            = START + 4005; //绑定失败
	int WM_BIND_CANCEL 			            = START + 4006; //绑定取消
	int WM_BIND_ERROR 			            = START + 4007; //服务器返回绑定失败
	int WM_UNBIND						        = START + 4008; //解除绑定
	int WM_UNBIND_ERROR 			        = START + 4009; //服务器返回解绑失败
	int WM_UPDATE_USER_INFO 			= START + 4010; //更新用户信息
	int WM_GETUSER_SONGRECORD    = START+4011;//获取到用户的演唱记录
	
	int WM_GETHONGBAO_LIST = START+4012;//请求获取红包列表
	int WM_GETHONGBAO = START+4013;//请求领取某个红包
	
	int HTTP_GRADE_TOPSONG = START+5000;//http 取得各个排行榜的前几名
	
	int HTTP_VOICE_PAD 			= START+5001;//http 设置调音面板
	int HTTP_PAUSE_OO				= START+5002;//http 设置暂停
	int HTTP_MUTE_OO				= START+5003;//http 设置静音
	int HTTP_SING_MODE			= START+5004;//http 设置原伴唱
	int HTTP_SCORE_STAT			= START+5005;//http 设置评分模式
	int HTTP_ATMOSPHERE		= START+5006;//http 设置气氛
	int HTTP_RESING					= START+5007;//http 设置重唱
	
	int HTTP_GETKTVSHOPLIST	= START+5008;//http 获取ktv门店列表	
	int HTTP_GETGIFTLIST			= START+5009;//http 获取指定门店礼物列表
	int HTTP_GETGIFTRECORD	= START+5010;//http 获取礼物兑换记录
	int HTTP_GIFTEXCHANGE		= START+5011;//http 兑换礼物
	int HTTP_GOLDFROM			= START+5012;//http 获得金币来源信息
}
