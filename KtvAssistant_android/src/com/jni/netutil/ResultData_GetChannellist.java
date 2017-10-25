package com.jni.netutil;


class ResultData_Channel {
	public int sectionid;							//区id
	public String sectionname;//char sectionname[50];					//区名称
	public int lobbyid;							//大厅id
	public String lobbyname;//char lobbyname[50];						//大厅名称
	public String lobbyip_1;//char lobbyip_1[MAXLEN_IP];				//大厅ip地址
	public short lobbyport;						//大厅端口
	public String lobbyip_2;//char lobbyip_2[MAXLEN_IP];				//大厅ip地址
}

public class ResultData_GetChannellist {
	public int count;
	public ResultData_Channel[] Ch;
}
