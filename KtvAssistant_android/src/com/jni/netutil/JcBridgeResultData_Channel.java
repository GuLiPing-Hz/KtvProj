package com.jni.netutil;

public class JcBridgeResultData_Channel {
	public int sectionid;							//区id
	public String sectionname;//char sectionname[50];					//区名称
	public int lobbyid;							//大厅id
	public String lobbyname;//char lobbyname[50];						//大厅名称
	public String lobbyip_1;//char lobbyip_1[MAXLEN_IP];				//大厅ip地址
	public short lobbyport;						//大厅端口
	public String lobbyip_2;//char lobbyip_2[MAXLEN_IP];				//大厅ip地址
	
	public int iCountOL;			// 在线人数
	public short m_iGroupVersion;	// 分组版本号
	public int s_iRightVersionid;
	
	public void jConverFromC(ResultData_Channel c)
	{
		this.sectionid = c.sectionid;
		this.sectionname = c.sectionname;
		this.lobbyid = c.lobbyid;
		this.lobbyname = c.lobbyname;
		this.lobbyip_1 = c.lobbyip_1;
		this.lobbyport = c.lobbyport;
		this.lobbyip_2 = c.lobbyip_2;
		
		this.iCountOL = 0;
		this.m_iGroupVersion = 0;
		this.s_iRightVersionid = 0;
	}
}
