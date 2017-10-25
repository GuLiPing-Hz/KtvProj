package com.jni.netutil;

public class ResultData_RoomInfo
{
	public int idx;						//房间idx
	public int hallid;						//房间所在大厅的id
	public String name;//char name[50];					//房间名
	//char pwd[20];					//进入房间密码		del by jinguanfu 2010/4/8
	public byte pwdflag;					//房间是否有密码 0-无 1-有 add by jinguanfu 2010/4/8
	public int type;						//房间类型	0--百人房,1--六人房
	public int state;						//房间状态,运行/冻结/停止
	public int masterid;					//室主id
	public String createdate;//char createdate[30];			//房间创建日期
	public String sortflag;//char sortflag[20];				//排序标志(是否允许自由排麦)
	public int maxusernum;					//房间最大在线人数
	public String deadline;//char deadline[30];				//房间到期时间
	public String ispassaudit;//char ispassaudit[2];			//申请会员是否需要审批
	public String introduce;//char introduce[512];			//房间欢迎词
	public String ip_1;//char ip_1[MAXLEN_IP];			//房间IP
	public short port;						//房间连接端口
	public String ip_2;//char ip_2[MAXLEN_IP];			//房间IP
	public String content;//char content[1024];				//房间公告
	public String updatedate;//char updatedate[30];			//更新时间
	public String logo;//char logo[MAXLEN_ROOM_LOGO];
	public int chatflag;					//允许房间公聊标识
	public int inoutflag;					//显示房间进出信息标识
	public int useronlyflag;				//房间私有标志位（锁定）//add by jinguanfu 2010/4/22
	public int closeflag;					//房间关闭标志位		//add by jinguanfu 2010/4/22
};