package com.jni.netutil;
class ResultData_UserFullInfo
{
	public int ret;											//��¼���
	public int m_idx;											//idx���

	public String m_id;//char m_id[21];										//�û�id,Ϊ�ַ���
	public String m_name;//char m_name[MAXLEN_NICKNAME];						//�û�����
	public String m_province;//char m_province[21];								//ʡ
	public String m_city;//char m_city[11];									//����
	public String m_sex;//char m_sex[2];										//�Ա� 1������
	public String m_age;//char m_age[4];										//����
	public int m_level;										//�û��ȼ���תint
	public String m_LevelTime;//char m_LevelTime[MAXLEN_TIMESTR];					//vip����ʱ��
	public byte m_isVip;										//�Ƿ�vip�˻�
	public String m_stage;//char m_stage[6];
	public String m_stageName;//char m_stageName[51];

	public String m_ClubId;//char m_ClubId[10];									//���ھ��ֲ�id
	public String m_ClubProfession;//char m_ClubProfession[4];							//���ֲ�Ȩ�ޣ�1����Ա,0�ǹ���Ա
	public String m_ClubName;//char m_ClubName[50];								//���ֲ�����	add by jinguanfu 2010/4/20
	public byte m_ExpLevel;

	public byte m_AvatarId;									//
	public byte m_AvaratCount;
	public short[] m_AvaratData;

	// add by wpf 2012/5/24
	public short m_singerlevel; //���ֵȼ�
	public int m_singer_expvalue;
	public short m_wealthlevel; //�Ƹ��ȼ�
	public int m_wealth_expvalue;
	// end by wpf

	public String headurl;//char		headurl[260];    //ͷ��URL
	public int		recvcount;        //���������
	public int		songcount;        //�ݳ���������
	public String topsongname;//char		topsongname[260];   //���ָ���
	public int		topsongscore;   //�ݳ���߷�
};

public class LocalData_UserInfo extends ResultData_UserFullInfo 
{
	// 0: ��ʾû����֤
	// 1: ��ʾ�ݶ�δ������ ����֤�󣬳����û���δ�򹫰�����֤��
	// 2: ������ (��֤��δ�����û�)
	// 3: �Ƿ����ԣ��򹫰�����֤��ĳ����û���
	public short							is_adult;	// ��������֤��־
	public boolean						is_remind;// �Ƿ���ʾ����֪ͨ
	public int								consume;// �ʹ��û��������ѽ����
	//�Ƿ���Ů
	public boolean						peri;
	//λ����Ϣ
	public int lobbyid;					// ����id
	public int roomid;						// ����id
	public int notify;	
	
//	enum eRelationShip{
//		RELATIONSHIP_NONE,//İ���� - 0
//		RELATIONSHIP_FRIEND,//���� - 1
//		RELATIONSHIP_BLACK,//������ - 2
//		RELATIONSHIP_SELF,//�Լ� - 3
//		RELATIONSHIP_SYSTEM,//ϵͳ - 4
//		RELATIONSHIP_MAX,//������ - 5
//	}
	public int relationship;				// �뱾�˵Ĺ�ϵ

	////////////////////////////////////////////////////////////////////////////////////
	//�첽��ȡ��������Ϣ״̬��Ҫ�ж�һ��
//	enum eINFO_STATE
//	{
//		INFO_STATE_NULL,//��Ч - 0
//		INFO_STATE_GETING,//���ڻ�ȡ - 1
//		IF_STATE_OK//��Ч��Ϣ - 2
//	};
	public int name_state;//������Ϣ��״ֵ̬ 
	public int info_state;//��Ϣ״ֵ̬
	public int location_state;//λ��״ֵ̬
	public int part_state;//�������״ֵ̬
	
	public int version;//�û���Ϣ�汾��
}
