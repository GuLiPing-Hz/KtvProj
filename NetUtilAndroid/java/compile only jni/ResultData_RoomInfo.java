package com.jni.netutil;

public class ResultData_RoomInfo
{
	public int idx;						//����idx
	public int hallid;						//�������ڴ�����id
	public String name;//char name[50];					//������
	//char pwd[20];					//���뷿������		del by jinguanfu 2010/4/8
	public byte pwdflag;					//�����Ƿ������� 0-�� 1-�� add by jinguanfu 2010/4/8
	public int type;						//��������	0--���˷�,1--���˷�
	public int state;						//����״̬,����/����/ֹͣ
	public int masterid;					//����id
	public String createdate;//char createdate[30];			//���䴴������
	public String sortflag;//char sortflag[20];				//�����־(�Ƿ�������������)
	public int maxusernum;					//���������������
	public String deadline;//char deadline[30];				//���䵽��ʱ��
	public String ispassaudit;//char ispassaudit[2];			//�����Ա�Ƿ���Ҫ����
	public String introduce;//char introduce[512];			//���件ӭ��
	public String ip_1;//char ip_1[MAXLEN_IP];			//����IP
	public short port;						//�������Ӷ˿�
	public String ip_2;//char ip_2[MAXLEN_IP];			//����IP
	public String content;//char content[1024];				//���乫��
	public String updatedate;//char updatedate[30];			//����ʱ��
	public String logo;//char logo[MAXLEN_ROOM_LOGO];
	public int chatflag;					//�����乫�ı�ʶ
	public int inoutflag;					//��ʾ���������Ϣ��ʶ
	public int useronlyflag;				//����˽�б�־λ��������//add by jinguanfu 2010/4/22
	public int closeflag;					//����رձ�־λ		//add by jinguanfu 2010/4/22
};