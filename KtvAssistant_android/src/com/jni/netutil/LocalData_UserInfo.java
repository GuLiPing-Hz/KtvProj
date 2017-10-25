package com.jni.netutil;
import java.io.Serializable;
class ResultData_UserFullInfo implements Serializable
{
	/**
	 * 
	 */
	private static final long serialVersionUID = 3145927808602561953L;
	public int ret;											//登录结果
	public int m_idx;											//idx编号

	public String m_id;//char m_id[21];										//用户id,为字符串
	public String m_name;//char m_name[MAXLEN_NICKNAME];						//用户名字
	public String m_province;//char m_province[21];								//省
	public String m_city;//char m_city[11];									//城市
	public String m_sex;//char m_sex[2];										//性别 1代表男
	public String m_age;//char m_age[4];										//年龄
	public int m_level;										//用户等级可转int
	public String m_LevelTime;//char m_LevelTime[MAXLEN_TIMESTR];					//vip到期时间
	public byte m_isVip;										//是否vip账户
	public String m_stage;//char m_stage[6];
	public String m_stageName;//char m_stageName[51];

	public String m_ClubId;//char m_ClubId[10];									//所在俱乐部id
	public String m_ClubProfession;//char m_ClubProfession[4];							//俱乐部权限：1管理员,0非管理员
	public String m_ClubName;//char m_ClubName[50];								//俱乐部名称	add by jinguanfu 2010/4/20
	public byte m_ExpLevel;

	public byte m_AvatarId;									//
	public byte m_AvaratCount;
	public short[] m_AvaratData;

	// add by wpf 2012/5/24
	public short m_singerlevel; //歌手等级
	public int m_singer_expvalue;
	public short m_wealthlevel; //财富等级
	public int m_wealth_expvalue;
	// end by wpf

	public String headurl;//char		headurl[260];    //头像URL
	public int		recvcount;        //礼物接收数
	public int		songcount;        //演唱歌曲总数
	public String topsongname;//char		topsongname[260];   //拿手歌曲
	public int		topsongscore;   //演唱最高分
	
	public String songname;  // 歌曲名
	public String singername;  //歌手名
	public int score;       //分数
};

public class LocalData_UserInfo extends ResultData_UserFullInfo 
{
	/**
	 * 
	 */
	private static final long serialVersionUID = -8471902076513087476L;
	// 0: 表示没有认证
	// 1: 表示暂定未防沉迷 （认证后，成年用户，未向公安部验证）
	// 2: 防沉迷 (认证后，未成年用户)
	// 3: 非防沉迷（向公安部验证后的成年用户）
	public short							is_adult;	// 防沉迷认证标志
	public boolean						is_remind;// 是否提示消费通知
	public int								consume;// 皇冠用户当月消费金币数
	//是否美女
	public boolean						peri;
	//位置信息
	public int lobbyid;					// 大厅id
	public int roomid;						// 房间id
	public int notify;	
	
//	enum eRelationShip{
//		RELATIONSHIP_NONE,//陌生人 - 0
//		RELATIONSHIP_FRIEND,//好友 - 1
//		RELATIONSHIP_BLACK,//黑名单 - 2
//		RELATIONSHIP_SELF,//自己 - 3
//		RELATIONSHIP_SYSTEM,//系统 - 4
//		RELATIONSHIP_MAX,//无意义 - 5
//	}
	public int relationship;				// 与本人的关系

	////////////////////////////////////////////////////////////////////////////////////
	//异步获取，所以信息状态需要判断一下
//	enum eINFO_STATE
//	{
//		INFO_STATE_NULL,//无效 - 0
//		INFO_STATE_GETING,//正在获取 - 1
//		IF_STATE_OK//有效信息 - 2
//	};
	public int name_state;//名字信息的状态值 
	public int info_state;//信息状态值
	public int location_state;//位置状态值
	public int part_state;//房间归属状态值
	
	public int version;//用户信息版本号
	
	//在线类型，底层不保存,需要上层应用自己保存用户的在线方式
	//0-主播，1-本包厢，2-其他包厢主播，3-大厅
	public int online_type;
}
