package com.jni.netutil;


class ResultData_Channel {
	public int sectionid;							//��id
	public String sectionname;//char sectionname[50];					//������
	public int lobbyid;							//����id
	public String lobbyname;//char lobbyname[50];						//��������
	public String lobbyip_1;//char lobbyip_1[MAXLEN_IP];				//����ip��ַ
	public short lobbyport;						//�����˿�
	public String lobbyip_2;//char lobbyip_2[MAXLEN_IP];				//����ip��ַ
}

public class ResultData_GetChannellist {
	public int count;
	public ResultData_Channel[] Ch;
}
