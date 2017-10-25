/*
 * 
 *   by glp
 *   2012-4-9   
 *   ����c�е���Ӧ�ṹ���ݣ��Ա���java��ʹ��
 * 
 */

package com.jni.netutil;


class ResultData_GiftInfo
{
	public int from_idx;
	public int to_id;	//�̻�--Ϊ����id�����̻�--Ϊ�û�idx
	public int number;
	public int pid;
	public int type; //0--���̻���1--�̻�����
	public int time;
	public int runway; //1--��ͨ�����2--������
	public int roomid; //���ĸ����������
	public String roomname;//char roomname[128]; //������
};

class RoomApply    //��������ṹ
{
	public int idx;				//������idx
	public int roomid;				//���뷿���
	public int state;				//����״̬
	public String time;//char time[32];			//����ʱ��
};
class ResultDate_RoomApplyList
{
	public int count;          	//�������
	public RoomApply[] ra;			//��������
};

class RoomUser
{
	public int  idx;     //��Աidx
	public boolean online;   //����״̬
};
class ResultDate_RoomMemList
{
	public int count;   //��Ա����
	public RoomUser[] userlist;  //��Աidx�б�
};

class ResultDate_RoomBlackList
{
	public int count;   //��������Ա����
	public int[] list;    //��������Աidx�б�
};

class ResultDate_RoomPanel
{
	public byte pwdflag; 				//�Ƿ����÷������� 0:�� 1:��
	public byte roomflag; 				//����״̬ 
	public byte accessflag; 			//�������Ȩ��
	public byte inoutflag; 			//�Ƿ���ʾ�û�������Ϣ
	public byte onmicflag; 			//��������
	public byte getmicflag;			//��������
	public String name;//char name[50]; 				//������
	public String notice;//char notice[512]; 			//���乫��
	public byte chatflag; 				//�����乫��
	public String Salutatory;//char Salutatory[256]; 		//��ӭ��
	public String logopath;//char logopath[256]; 		//logo·��
	public String pwd;//char pwd[MAXLEN_ROOM_PWD];	//pwd����
	public byte privatemicflag;		//0:�ر�˽��  1:����˽��
};

class ResultDate_RoomList
{
	public int lobbyid;	//����ID
	public int count;		//�������
	public RoomList[] RL;	//������Ϣ
};

//class RoomListEx
//{
//	public int playerid;
//	public int roomid; //����id
//	public String ip_1;//char ip_1[MAXLEN_IP];  //ip
//	public short port;  //�˿�
//	public String ip_2;//char ip_2[MAXLEN_IP];  //ip
//	public String m_name;//char m_name[MAXLEN_TMPROOM_NAME];   //����
//	public byte haspasswd;     //�Ƿ�������
//	public byte scoreflag;
//	public byte voteflag;
//	public byte nmaxplayernum;  //�������
//	public byte playernum;  //��ǰ����
//};

class ResultDate_RoomListEx
{
	public int count;			//�������
	public RoomListEx[]  RL;	//������Ϣ
};

//class ResultDate_Channel {
//	public int sectionid;							//��id
//	public String sectionname;//char sectionname[50];					//������
//	public int lobbyid;							//����id
//	public String lobbyname;//char lobbyname[50];						//��������
//	public String lobbyip_1;//char lobbyip_1[MAXLEN_IP];				//����ip��ַ
//	public short lobbyport;						//�����˿�
//	public String lobbyip_2;//char lobbyip_2[MAXLEN_IP];				//����ip��ַ
//}
//
//class ResultData_GetChannellist
//{
//	public int count;
//	public ResultDate_Channel[] Ch;
//};

class ResultData_LobbyLoginUser
{
	public int idx;
	public char relation;						//USER_RELATION�Է���ϵ
	public int lobbyid;
};

//class ResultData_RoomIPPort
//{
//	public int roomid;				//�����
//	public String ip_1;//char ip_1[MAXLEN_IP];	//����IP
//	public short port_1;			//����˿�
//	public String ip_2;//char ip_2[MAXLEN_IP];	//����IP
//	public int status;				//0--UNRUNING,1--RUNING,2--LOCKED
//};



//����������Ϣ
//class RoomList
//{
//	public int roomid;				//������
//	public String roomname;//char roomname[50];	    //������  
//};

//class ResultData_LobbyLogin extends ResultData_UserFullInfo
//{
//	public int roomcount;						//��������
//	public ResultData_RoomIPPort[] roomlist;	//�����б�ָ��
//	public int group_versionid;				//����汾��
//	public int m_roomright_versionid; //����Ȩ�ް汾��
//};

class UserStatus
{
	public int idx;	//�����ʺ�
	public int lobbyid;//����id
	public int roomid; //����id
};

class ResultData_GetFriendlist
{
	public int count;												//�б���
	public UserStatus[] US;
};

class ClubUserStatus
{
	public int idx;	//�����ʺ�
	public byte pro;	//���ֲ�ְҵ
	public int lobbyid;//����id
	public int roomid;	//����id
};

class ResultData_GetClublist
{
	public int count;
	public ClubUserStatus[] CUS;
};

class ClubApply
{
	public int id;								//������
	public int idx;							//������idx
	public String time;//char time[32];						//����ʱ��
};

class ResultData_ClubApplyList
{
	public int count;
	public ClubApply[] CA;
};

class ResultData_Approvelresult
{
	public int ret;							//��˽��
	public int id;								//������
	public int idx;							//������idx
};

class ResultData_ClubFullInfo
{
	public String name;//char name[20];						//���ֲ�����
	public int maxnumber;						//���ֲ��û�����
	public String slogan;//char slogan[50];					//���ֲ��ں�
	public String notice;//char notice[50];					//���ֲ�֪ͨ
	public String picture;//char picture[100];					//���ֲ�ͼƬ����
	public int adminidx;						//���ֲ�����Աidx
	public String admintele;//char admintele[20];					//���ֲ�����Ա�绰
	public String adminQQ;//char adminQQ[20];					//���ֲ�����ԱQQ
};

class ResultData_LobbyRoomEnter
{
	public int idx;
	public byte relation;						//USER_RELATION�Է���ϵ
	public int lobbyid;
	public int roomid;
};

class ResultData_LobbyName
{
	public int lobbyid;
	public String lobbyname;//char lobbyname[50];
};

class ResultData_LoginInfo
{
	public byte ret;							//LOGIN_RET��¼���
	public byte identity;						//USER_IDENTITY�û�����
};

class ResultData_UserInfo
{
	public int idx;
	public byte identity;						//USER_IDENTITY�û�����
};

class ResultData_MicInfo
{
	public int idx;							//�����˵�idx
	public int musicid;						//����id
	public short bk;							//����
	public byte state;							//״̬
	public int pkidx;							//pk�˵�idx
	public int pkmusicid;					//pk������id
	public short pkbk;							//pk�ı���
	public byte pkstate;						//pk״̬
	public byte level;							//�Ѷ�
	//char change;						//�Ƿ�Ե�
	public byte speed;							//�ٶ�
	public String name;//char name[128];						//��������
};
class ResultData_RoomAllInfo
{
	public int usercount;						//�����û�����
	//ResultData_UserInfo *pUI;			//�����û��б�
	public int[] userlist;						//�����û��б�
	public int miccount;						//���������б����
	public ResultData_MicInfo[] MLI;			//���������б�
	public int vjonmic;						//������idx
	public ResultData_MicInfo[] onmic;			//������Ϣ
	public int managercount;					//����Ա����
	public ResultData_UserInfo[] MI;			//�������Ա�б�
	public String roomcontent;//char *roomcontent;					//���乫��
	public String[] ip_telcom;//char ip_telcom[ MAXLEN_ROOM_AVIPCOUNT ][ MAXLEN_IP ];
	public String[] ip_netcom;//char ip_netcom[ MAXLEN_ROOM_AVIPCOUNT ][ MAXLEN_IP ];
	public byte ip_telcom_count;
	public byte ip_netcom_count;
	public short port_telcom;;
	public short port_netcom;
};

class ResultData_UDMicInfo
{
	public byte tag;							//MIC_UP_DOWN_TAG	idxָʾ��־
	public int idx;
	public byte flag;							//MIC_UP_DOWN_FLAG�����������
};

class ResultData_ForbidenInfo
{
	public int idx;
	public byte flag;							//FORBIDEN_FLAG�����������
};

class ResultData_UpBlackList
{
	public int idx;
	public byte flag;							//ADD_SUB_FLAG���ӻ�ɾ����־
};

class ResultData_UpMgrList
{
	public int idx;
	public byte flag;							//ADD_SUB_FLAG���ӻ�ɾ����־
	public byte identity;						//USER_IDENTITY�û�����
};

class ResultData_RoomSimpleInfo
{
	public int roomid;			//����ID
	public String roomname;//char roomname[50];	//������
	public byte pwdflag;		//������������ 0-�� 1-��
	public short clientnum;	//��������
	public short boy;
	public short girl;
	public int isvip;
	public int	isclose;
	public int	state;
	public int	maxplayer;
	public short re_player_num;
};
class ResultData_RoomSimpleInfoList
{
	public short roomcount;	//�������
	public ResultData_RoomSimpleInfo[] RoomSimpleInfoList;
};

class ResultData_Pack
{
	public int global_id;
	public short goods_id;
	public byte status;
	public int day_num;
	public String buy_day;//char buy_day[ MAXLEN_TIMESTR ];
	public byte is_use;
	public int count;
};

class ResultData_Gift
{
	public int gift_type; //��������
	public int recv_idx;
	public String recv_id;//char recv_id[ MAXLEN_NICKNAME ];
	public int recv_num; //���ܵ�������
};
class ResultData_GiftList
{
	public short gift_count;
	public String curtime;//char curtime[MAXLEN_TIMESTR];
	public ResultData_Gift[] list ;//list[50];
};

class ResultData_SongInfo
{
	public int m_musicid;
	public short m_topscores; //��߷�
	public int m_topidx; //��߷�idx
	public String m_topname;//char m_topname[ MAXLEN_NICKNAME ];
	public short m_dayclicks; //�յ����
	public short m_weekclicks; //�ܵ����
	public short m_monthclicks; //�µ����
	public int m_totalclicks; //�ܵĵ����
};

class ResultData_AvServer 
{
	public String ip;//char ip[ MAXLEN_IP ];
	public short port;
};

class ResultData_OffLineMsg
{
	public int idx;
	public String buy_day;//char buy_day[ MAXLEN_TIMESTR ];
	public String buy_msg;//char buy_msg[ MAXLEN_BUY_MSG ];
};

class ResultData_RoomSort
{
	public int idx;
	public int sort;
};

class ResultData_IncomeDate
{
	public int income;
	public String date;//char date[MAXLEN_TIMESTR];
};

class ResultData_AvChange
{
	public short port_telcom;
	public short port_netcom;

	public byte telcom_count;
	public byte netcom_count;

	public String[] telcom;//char telcom[ MAXLEN_ROOM_AVIPCOUNT ][ MAXLEN_IP ];
	public String[] netcom;//char netcom[ MAXLEN_ROOM_AVIPCOUNT ][ MAXLEN_IP ];
};

class ResultData_GiftCount
{
	public int   idx;
	public int   giftid;
	public int   giftcount;
};

/*
 ������Ϣ                                                             

#define FAMILY_NAME_MAXLEN 128
#define FAMILY_URL_MAXLEN 128
#define FAMILY_INTRUDUCE 256
#define FAMILY_MAX_ROOMS 128 //ÿ���������󷿼���
*/
class ResultData_Family
{

	public int familyId; //����id
	public int familyManageIdx; //�峤idx
	public short familyState; //����״̬��0������1���ᣬ9ɾ����
	public short familySort; //����˳��
	public String familyName;//char familyName[FAMILY_NAME_MAXLEN]; //������
	public String familyIntroduce;//char familyIntroduce[FAMILY_INTRUDUCE]; //�������
	public String familyImage;//char familyImage[FAMILY_URL_MAXLEN]; // ���庣��
	public String familyLogo;//char familyLogo[FAMILY_URL_MAXLEN];//����logo

	public int roomsize; //������ķ�������
	public int[] roomids;// roomids[128];//������ķ����б�
};

class ResultData_LobbyOnline
{
	public int lobbyid;		//����ID
	public int onlinenum;		//������������
};

class ResultData_LobbyOnlineList
{
	public short lobbycount;	//��������
	public ResultData_LobbyOnline[] Lobbyonline;
};

class ResultData_Avatar
{
	public byte avaratid;
	public byte len;
	public short[] goods_id;//short goods_id[PART_LEN+LINK_LEN*10];
};
class ResultData_Avatar_Updata extends ResultData_Avatar
{
	public String[] count;//char count[PART_LEN+LINK_LEN][COUNT_MAX];
	public int[] global_id;//int global_id[PART_LEN+LINK_LEN*10];
};

class ResultData_Buy
{
	public short goods_id;
	public byte count;
	public String msg;//char msg[ MAXLEN_BUY_MSG ];
};

class ResultData_CreateRoom
{
	public String name;//char name[MAXLEN_TMPROOM_NAME];
	public byte is_pwd;
	public String pwd;//char pwd[MAXLEN_TMPROOM_PWD];
	public byte is_score;
	public byte is_ballot;
	public byte count;
};

//class ResultData_GroupInfo
//{
//	public short group_id;
//	public String group_name;//char group_name[ MAXLEN_ROOM_TYPENAME ];
//	public short disable; //0������ 1������
//};

class ResultData_PartInfo
{
	public int idx; 
	public byte sex; 
	public short age;
	public String province;//char province[MAXLEN_PROVINCE];
	public int plen;
	public String city;//char city[MAXLEN_CITY];
	public int clen;
	public String nick;//char nick[MAXLEN_NICKNAME];
	public int nicklen; 
	public byte singlevel;
	public int gold; 
	public int silver;
	public int nexp;
};

//class ResultData_RoomInfo
//{
//	public int idx;						//����idx
//	public int hallid;						//�������ڴ�����id
//	public String name;//char name[50];					//������
//	//char pwd[20];					//���뷿������		del by jinguanfu 2010/4/8
//	public byte pwdflag;					//�����Ƿ������� 0-�� 1-�� add by jinguanfu 2010/4/8
//	public int type;						//��������	0--���˷�,1--���˷�
//	public int state;						//����״̬,����/����/ֹͣ
//	public int masterid;					//����id
//	public String createdate;//char createdate[30];			//���䴴������
//	public String sortflag;//char sortflag[20];				//�����־(�Ƿ�������������)
//	public int maxusernum;					//���������������
//	public String deadline;//char deadline[30];				//���䵽��ʱ��
//	public String ispassaudit;//char ispassaudit[2];			//�����Ա�Ƿ���Ҫ����
//	public String introduce;//char introduce[512];			//���件ӭ��
//	public String ip_1;//char ip_1[MAXLEN_IP];			//����IP
//	public short port;						//�������Ӷ˿�
//	public String ip_2;//char ip_2[MAXLEN_IP];			//����IP
//	public String content;//char content[1024];				//���乫��
//	public String updatedate;//char updatedate[30];			//����ʱ��
//	public String logo;//char logo[MAXLEN_ROOM_LOGO];
//	public int chatflag;					//�����乫�ı�ʶ
//	public int inoutflag;					//��ʾ���������Ϣ��ʶ
//	public int useronlyflag;				//����˽�б�־λ��������//add by jinguanfu 2010/4/22
//	public int closeflag;					//����رձ�־λ		//add by jinguanfu 2010/4/22
//};



//class ResultData_SendFlowerInfo
//{
//	public int s_idx;						//������IDX
//	public int r_idx;						//������IDX
//	public int cate_idx;				    //�������
//	public int number;			            //��������
//	public int leavings;					//����ʣ����
//	public int time;
//	public int once_num;					//ÿ�����������
//};

class ResultData_UserSpeakerInfo
{
	int time;			// ����ʱ��
	int fromidx;		//������idx;
	String chat;	// С��������
    int roomId;//����ID
    String roomName;//�������� ktv����+������ ������С��ɽ���͵�A31С��
};

class ResultData_GroupRoomInfo{
    short grpup_id;//����ID
    short roomnum;//��������
    int[] roomlist;//����ID�б�
};
