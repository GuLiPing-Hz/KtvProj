package com.jni.netutil;

class ResultData_RoomIPPort
{
	public int roomid;				//�����
	public String ip_1;//char ip_1[MAXLEN_IP];	//����IP
	public short port_1;			//����˿�
	public String ip_2;//char ip_2[MAXLEN_IP];	//����IP
	public int status;				//0--UNRUNING,1--RUNING,2--LOCKED
};

public class ResultData_LobbyLogin extends ResultData_UserFullInfo
{
	public int roomcount;						//��������
	public ResultData_RoomIPPort[] roomlist;	//�����б�ָ��
	public int group_versionid;				//����汾��
	public int m_roomright_versionid; //����Ȩ�ް汾��
};

