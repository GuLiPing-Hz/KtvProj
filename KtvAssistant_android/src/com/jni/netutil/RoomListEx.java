package com.jni.netutil;

public class RoomListEx
{
	public int playerid;
	public int roomid; //房间id
	public String ip_1;//char ip_1[MAXLEN_IP];  //ip
	public short port;  //端口
	public String ip_2;//char ip_2[MAXLEN_IP];  //ip
	public String m_name;//char m_name[MAXLEN_TMPROOM_NAME];   //名称
	public byte haspasswd;     //是否有密码
	public byte scoreflag;
	public byte voteflag;
	public byte nmaxplayernum;  //最大人数
	public byte playernum;  //当前人数
};

