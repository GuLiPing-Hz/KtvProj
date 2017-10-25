package com.jni.netutil;

class ResultData_RoomIPPort
{
	public int roomid;				//房间号
	public String ip_1;//char ip_1[MAXLEN_IP];	//房间IP
	public short port_1;			//房间端口
	public String ip_2;//char ip_2[MAXLEN_IP];	//房间IP
	public int status;				//0--UNRUNING,1--RUNING,2--LOCKED
};

public class ResultData_LobbyLogin extends ResultData_UserFullInfo
{
	public int roomcount;						//房间数量
	public ResultData_RoomIPPort[] roomlist;	//房间列表指针
	public int group_versionid;				//分组版本号
	public int m_roomright_versionid; //房间权限版本号
};

