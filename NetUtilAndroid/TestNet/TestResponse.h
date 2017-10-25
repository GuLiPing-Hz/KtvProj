#pragma once

#include "netutil\responsebase.h"

class CTestResponse :
	public ResponseBase
{
public:
	CTestResponse(void);
	virtual ~CTestResponse(void);

	/*			add by lixin 2011/10/13 <begin> ���졢���ȹ���			*/
	virtual void SendChatFromUser(int ret, int to_idx, const char *content ,int contentlen, int time );
	/*			add by jinguanfu 2009/12/18 <begin> ���졢���ȹ���			*/
	//����˽��
	virtual void RecvChatFromUser(int ret, int from_idx, const char *content ,int contentlen, int time );
	//���չ��ģ����ֲ��ڲ���
	virtual void RecvChatFromClub(int ret, int from_idx, const char* content,int contentlen);
	//���չ��ģ������ڣ�
	//ret:1--���� 0--������ -1--��������
	virtual void RecvChatFromRoom(int ret, int errcode,int from_idx, const char* content,int contentlen, int time );	
	//���մ����ȣ�ȫ����
	virtual void RecvSpeakerFromAll(int ret, int type, int from_id, const char* msg, int msglen);
	//����С���ȣ�������
	virtual void RecvSpeakerFromLobby(int ret, int from_idx, int room_id,const char* room_name, const char* msg, int msglen, int time,int type,bool isSend);
	//��������㲥
	virtual void RecvPresentBroad(int ret, ResultData_GiftInfo* guestGiftInfo, ResultData_GiftInfo* commGiftInfo);
	//����ϵͳ���ֹ㲥
	virtual void RecvSystemBroad( char type, const char * msg, int len );
	//����ϵͳ���ֹ㲥�������н���Ϣ��
	virtual void RecvSystemBroad2(const char* nickname, int len, int idx, int roomid, int giftid, int luckmultiple);
	//����ϵͳ���ֹ���
	virtual void RecvSystemNotice( const char * msg, int len, bool once, int min );
	//����ϵͳ���ֹ��棨�����н���Ϣ��
	virtual void RecvSystemNotice2(const char* nickname, int len, int idx, int roomid, int giftid, int luckmultiple);
	//���շ�����ʱ����
	virtual void RecvRoomNotice(int ret, const char* content,int contentlen);
	/************************************************************************/
	/*			add by jinguanfu 2009/12/28 <begin> ���Ѳ���				*/
	//ȡ�ú���λ��
	virtual void RecvFriendLocation(int ret, int errcode,
								int friend_idx,
								int lobbyid, 
								int roomid,
								int cseq);
	/*			add by jinguanfu 2009/12/28 <begin> �������ӿ�			*/
	//����ȡ�ô���˷����Ա�б�
	virtual void RecvRoomApplyList (int ret ,int errcode,ResultDate_RoomApplyList *applylist,int cseq);
	//�������뷿���Ա��Ӧ
	virtual void RecvApplyRoomMember(int ret,int errcode,int cseq);
	//������˻�Ա����������
	virtual void RecvAuditAddApply (int ret, int errcode,int idx,int cseq);
	//���վܾ���������ϵͳ��ʾ
	virtual void RecvRoomApplyRefuseMsg (int ret, int roomid);
	//���շ����Ա�б�
	virtual void RecvRoomMemberList (int ret ,int errcode, ResultDate_RoomMemList* memlist,int cseq);
	//���ս�������Ա�ʸ��Ӧ
	virtual void RecvUnchainMember (int ret, int errcode, int idx,int cseq);
	//���ո����û�������������Ȩ�޻�Ӧ
	virtual void RecvRoomPesideAssistant (int ret , int errcode, int idx,int cseq);
	//���ո�����������֪ͨ
	virtual void RecvRoomNotice4PesideAssistant (int ret , int opidx,int bopidx);
	//���ո����û�����������Ȩ�޻�Ӧ
	virtual void RecvRoomPeside (int ret ,int errcode, int idx,int cseq);
	//��������������Ȩ�޷���֪ͨ
	virtual void RecvRoomNotice4Peside (int ret , int opidx,int bopidx);
	//���ո����û����丱����Ȩ�޻�Ӧ
	virtual void RecvRoomSubHost (int ret ,int errcode, int idx,int cseq);
	//�������ø�����Ȩ�޷���֪ͨ
	virtual void RecvRoomNotice4SubHost(int ret , int opidx,int bopidx);
	//���շ��������
	virtual void RecvBlackList(int ret,	int errcode,ResultDate_RoomBlackList* blacklist,int cseq);
	//���պ������û����/ɾ����Ӧ
	virtual void RecvAddDelBlack(int ret , int errcode,int idx, int type);
	//���շ������������Ϣ
	virtual void RecvRoomPanelInfo(int ret ,int errcode, ResultDate_RoomPanel* panelinfo);
	//�������÷��������Ӧ
	virtual void RecvRoomPassword(int ret,int errcode,int cseq);
	//�������÷���������Ϣ��ʾ
	virtual void RecvRoomNotice4Password(int ret, int idx, char pwdflag);
	//�������÷��俪��/�ر�(����)��Ӧ
	virtual void RecvRoomSwitch(int ret,int errcode,char roomflag,int cseq);
	//���շ��俪��/�ر�(����)��Ϣ��ʾ
	virtual void RecvRoomNotice4RoomSwitch(int ret , int idx , char roomflag);
	//�������÷������Ȩ�޻�Ӧ
	virtual void RecvRoomPrivate(int ret , int errcode,char accessflag,int cseq);
	//���շ������Ȩ����Ϣ��ʾ
	virtual void RecvRoomNotice4Private(int ret , int idx , char accessflag);
	//���������û�������Ϣ����/�رջ�Ӧ
	virtual void RecvUserInOutSwitch(int ret , int errcode, char inoutflag,int cseq);
	//�����û�������Ϣ����/�ر���Ϣ��ʾ
	virtual void RecvRoomNotice4UserSwitch (int ret , int idx , char inoutflag);
	//������������������/�رջ�Ӧ
	virtual void RecvGetMicSwitch (int ret, int errcode,char onmicflag,int cseq);
	//������������������/�رջ�Ӧ
	virtual void RecvOnMicSwitch (int ret, int errcode,char onmicflag,int cseq);
	//��������������/�ر���Ϣ��ʾ
	virtual void RecvRoomNotice4GetMicSwitch(int ret , int idx , char getmicflag);
	//��������������/�ر���Ϣ��ʾ
	virtual void RecvRoomNotice4OnMicSwitch(int ret , int idx , char onmicflag);
	//���������޸Ļ�Ӧ
	virtual void RecvModifyRoomName(int ret,int errcode,int cseq);
	//���շ�����
	virtual void RecvRoomName (int ret , int roomid , const char* roomname , int rnlen);
	//���÷��乫����Ӧ
	virtual void RecvRoomAffiche(int ret,int errcode,int cseq);
	//���շ��乫������¼�
	virtual void RecvRoomAfficheEvent(int ret);
	//���������乫����Ӧ
	virtual void RecvAllowRoomAllChat (int ret, int errcode,char chatflag,int cseq);
	//���շ��乫��������Ϣ��ʾ
	virtual void RecvRoomNotice4AllChat(int ret , int idx , char chatflag);
	//���÷��件ӭ����Ӧ
	virtual void RecvRoomSalutatory(int ret, int errcode,int cseq);
	//���շ��件ӭ�ʸ���֪ͨ
	virtual void RecvRoomNotice4Salutatory(int ret,int idx);
	//���÷���logo��Ӧ
	virtual void RecvRoomLogo (int ret,int errcode ,int cseq);
	//���շ���LOGO����֪ͨ
	virtual void RecvRoomNotice4Logo(int ret,int idx);
	//���շ��䶳����Ϣ��ʾ
	virtual void RecvRoomNotice4Lock(int idx);
	//���ܴ��������Ӧ
	virtual void RecvCreateRoom( int ret, int errcode, RoomListEx * info );
	//�������跿��ȫ��Ϣ
	virtual void RecvTmpRoomInfo( int ret, int errcode, RoomListEx * info );
	// ��Ӹ跿�㲥
	virtual void RecvRoomNotice4Add( RoomListEx * info );
	virtual void RecvRoomNotice4Add( int idx );
	// ɾ���跿�㲥
	virtual void RecvRoomNotice4Del( int idx );
	// ���շ�������֪ͨ
	virtual void RecvRoomNotice4ChangeMaster(int idx);
	// ���շ�����Ϣ����֪ͨ
	virtual void RecvRoomNotice4ChangeInfo( int idx );
	/*			add by jinguanfu 2009/12/28 <begin> ��ɫ��Ϣ				*/
	//ȡ�����˽�ɫ��Ϣ
	virtual void RecvPartInfo(int ret, int errcode, ResultData_PartInfo * part_info, int cseq);
	//ȡ���Լ���ɫ��Ϣ
	virtual void RecvPartInfoBySelf(int ret, int errcode, ResultData_PartInfo * part_info, int cseq);
	/*			add by jinguanfu 2010/1/5 <begin> ȡ�÷����б�				*/
	virtual void RecvRoomList(int ret,ResultDate_RoomList* roomlist);
	virtual void RecvRoomListEx(int ret,ResultDate_RoomListEx * roomlist);
	//���֪ͨ
	//virtual void RecvNotice4Score(int onmic_idx,int onmic_score,int pk_idx,int pk_score);
	//virtual void RecvNotice4ScoreAdd(int epchange, int silverchange);
	//����֪ͨ
	virtual void RecvNotice4OffMic( int onmic_idx, int pk_idx, int exp, const char * scoreinfo, int len );
	// add by cx 10-6-2
	//��Ӧ�Ƿ���������ɹ�
	virtual void RecvRoomGivOnevjMic( int idx, char flag );
	//�Ƿ�ͬ����������ɹ�
	virtual void RecvRoomGivOnevjMicApply( int formidx );
	//add by jinguanfu 2010/1/20 <begin> 
	//�Ƿ�ͬ����ӶԷ�Ϊ����
	//����˵����int idx ��Ӻ�����idx
	virtual void RecvAddFriendApply(int idx);
	//add by jinguanfu 2010/1/20 <end> 
	//virtual void RecvDeleteAvarta(int ret);
	//add by jinguanfu 2010/3/4 ԭ�лص�API�ĳɽӿ�<begin>
	//���ӳɹ�
	virtual void OnAuthTunnelConnectSuccess();			//�ɹ�������֤������
	virtual void OnLobbyTunnelConnectSuccess();		//�ɹ����Ӵ���������
	virtual void OnRoomTunnelConnectSuccess();			//�ɹ����ӷ��������
	virtual void OnChannellistTunnelConnectSuccess();	//�ɹ�����Ƶ��������
	//���ӳ�ʱ
	virtual void OnAuthTunnelConnectTimeout();		//������֤��������ʱ
	virtual void OnLobbyTunnelConnectTimeout();		//���Ӵ�����������ʱ
	virtual void OnRoomTunnelConnectTimeout();		//���ӷ����������ʱ
	virtual void OnChannellistTunnelConnectTimeout();//����Ƶ����������ʱ
	//���Ӵ���
	virtual void OnAuthTunnelConnectError( int code );	
	virtual void OnLobbyTunnelConnectError( int code );
	virtual void OnRoomTunnelConnectError( int code );
	virtual void OnChannellistTunnelConnectError( int code );
	//�����������Ͽ�������,�ͻ���recv == 0��ʱ��,�ص������µĽӿ�
	virtual void OnAuthTunnelClose();	//�Ͽ���֤������
	virtual void OnLobbyTunnelClose();	//�Ͽ�����������
	virtual void OnRoomTunnelClose();	//�Ͽ����������
	virtual void OnChannellistTunnelClose();//�Ͽ�Ƶ��������
	//�ͻ���recv�쳣,send�쳣,�����buf���,select��������,����ص���������½ӿ�
	virtual void OnAuthTunnelError( int code );	
	virtual void OnLobbyTunnelError( int code );
	virtual void OnRoomTunnelError( int code );
	virtual void OnChannellistTunnelError( int code );

	/******************��֤�����������ӿ�*****************************/
	//������֤���
	virtual void RecvAuth(int ret, int errcode, const char * token, int len);
	//������token
	//virtual void RecvNewToken(int ret,ReserveDataS* pRD);
	virtual void RecvNewToken(int ret,int errcode);
	//����Ƶ���б�
	virtual void RecvSectionList(int ret,int errcode,ResultData_GetChannellist* pRDc,int cseq);
	//����Ƶ���б�(��ӷ�������Ϣ)
	virtual void RecvSectionList2(int ret,int errcode,int leftlogintime,ResultData_GetChannellist* pRDc,int cseq);
	/******************���������������ӿ�*****************************/
	//���մ���token��֤
	virtual void RecvTokenAuth4Lobby( int ret, int errcode, ResultData_LobbyLogin * pRDl, ResultData_SelfData * pSD, int cseq );
	//�û���¼����֪ͨ
	virtual void RecvUserLoginLobbyNotice(int ret,ResultData_LobbyLoginUser* pRlu);
	//�û��˳�����֪ͨ
	virtual void RecvUserLogoutLobbyNotice(int ret,ResultData_LobbyLoginUser* pRlu);
	//�����û���Ϣ
	virtual void RecvUserInfo(int ret,int errcode,ResultData_UserFullInfo* pRDu);
	//���պ���״̬
	virtual void RecvFriendStatus(int ret,int errcode,ResultData_GetFriendlist* pRD);
	//���պ�����״̬
	virtual void RecvBlackStatus(int ret,int errcode,ResultData_GetFriendlist* pRD);
	//���վ��ֲ�״̬
	virtual void RecvClubStatus(int ret,int errcode,ResultData_GetClublist* pRD);
	//������Ӻ��ѽ��
	virtual void RecvAddFriend(int ret,int errcode,int idx);
	//������Ӻ��������
	virtual void RecvAddBlack(int ret,int errcode,int cseq);
	//����ɾ�����ѽ��
	virtual void RecvDelFriend(int ret,int errcode,int cseq);
	//����ɾ�����������
	virtual void RecvDelBlack(int ret,int errcode,int cseq);
	//�������������ֲ����
	virtual void RecvApplyClub(int ret,int errcode,int cseq);
	//�����˳����ֲ����
	virtual void RecvExitClub(int ret,int errcode,int cseq);
	//������������ڵļ�����ֲ���������
	virtual void RecvApplyListCount(int ret,int errcode,int count,int cseq);
	//������������ڵļ�����ֲ������б�
	virtual void RecvApplyList(int ret,int errcode,ResultData_ClubApplyList* pRD,int cseq);
	//ͨ��������ֲ�����
	virtual void RecvAddClubOk(int ret,ResultData_Approvelresult* pRD);
	//ɾ�������б�
	virtual void RecvDelApplyList(int ret,ResultData_Approvelresult* pRD);
	//���ռ�����ֲ��㲥��Ϣ(�����˼�����ֲ�)
	//modify by jinguanfu 2010/4/11
	//virtual void RecvAddClubNotice(int ret ,int idx);
	//opidx--������idx
	//addidx--������idx
	virtual void RecvAddClubNoticeByLobby(int ret ,int clubid,const char* clubname,int opidx,int addidx);
	virtual void RecvAddClubNoticeByRoom( int opidx, int addidx );
	//���������Ա�ܾ���Ϣ
	//opidx--�����idx
	virtual void RecvDenyClubNoticeByLobby(int ret ,int clubid,const char* clubname,int opidx);
	//virtual void RecvDenyClubNoticeByRoom(int ret ,int clubid,char* clubname,int opidx);
	//���ջ�Ա�˳����ֲ���Ϣ
	virtual void RecvExitClubNoticeByLobby(int ret,int clubid,const char* clubname,int exitidx);
	virtual void RecvExitClubNoticeByRoom( int exitidx );
	//���ջ�Ա���߳����ֲ���Ϣ
	//opidx--������idx
	//kickidx--���߳���idx
	virtual void RecvKickClubNoticeByLobby(int ret,int clubid,const char* clubname,int opidx,int kickidx);
	virtual void RecvKickClubNoticeByRoom( int opidx, int kickidx );
	//�������ֲ�
	virtual void RecvCreateClub(int ret,int errcode,int clubid,int cseq);
	//��ɢ���ֲ�
	virtual void RecvDismissClub(int ret,int errcode,int cseq);
	//���վ��ֲ���Ϣ
	virtual void RecvClubInfo(int ret,int errcode,ResultData_ClubFullInfo* pRD,int cseq);
	////���շ����б�
	//virtual void RecvRoomList(int ret,ResultData_RoomList* pRD);
	//���շ�����Ϣ
	virtual void RecvRoomInfo(int ret,ResultData_RoomInfo* pRD);
	//����ͷ��
	virtual void RecvCreateAvatar(int ret,int errcode);
	//�޸�ͷ��
	virtual void RecvChangeAvatar(int ret,int errcode);
	//�޸��ǳ�
	virtual void RecvChangeNickName(int ret,int errcode,const char * name, int len);
	//���뷿��֪ͨ
	virtual void RecvLobbyEnterRoom(int ret,ResultData_LobbyRoomEnter* pRD);
	//�뿪����֪ͨ
	virtual void RecvLobbyLeaveRoom(int ret,ResultData_LobbyRoomEnter* pRD);
	//��Ӻ���֪ͨ
	virtual void RecvAddFriendNotice(int ret,int idx);
	//ȡ�ô�����Ϣ
	virtual void RecvLobbyInfo(int ret,int errcode,ResultData_LobbyName* pRD,int cseq);
	//�������Դ�����Ȩ�޸�����Ϣ add by jinguanfu 2010/6/5
	virtual void RecvChangePower(int optionidx/*������ID*/,char identity/*���ĺ�Ȩ��*/);
	//ȡ������(CMD_SYSTEMMSG_L2C)
	//virtual void RecvLobbyName();
	/******************��������ӿ�*****************************/
	//���շ���token��֤ //
	virtual void RecvTokenAuth4Room(int ret,int errcode);
	//���뷿��
	virtual void RecvEnterRoom(int ret,int errcode,ResultData_LoginInfo* pRDlf,int cseq);
	//�û����뷿��֪ͨ
	virtual void RecvEnterRoomNotice(int ret,ResultData_UserInfo* pRDu);
	//�û��뿪����֪ͨ
	virtual void RecvLeaveRoomNotice(int ret,int idx);
	//���շ�����ϸ��Ϣ
	virtual void RecvRoomAllInfo(int ret,int errcode,ResultData_RoomAllInfo *pRD,int cseq);
	//��������ɹ�֪ͨ
	virtual void RecvWaitMicNotice(int ret,ResultData_MicInfo *pRD);
	////��������׼��֪ͨ
	//virtual void RecvWaitMicReadyNotice(int ret,ResultData_MicInfo *pRD);
	//��������׼�����֪ͨ
	//virtual void RecvOnMicReadyOkNotice(int ret,ResultData_MicInfo *pRD);
	//��������֪ͨ
	virtual void RecvOnMicNotice(int ret,ResultData_MicInfo *pRD);
	//ȡ������ɹ�֪ͨ
	virtual void RecvCancelWaitMicNotice(int ret,int opidx,int bopidx);
	//���ռ���PK��֪ͨ
	virtual void RecvWaitPKMicNotice(int ret,ResultData_MicInfo *pRD);
	//����ȡ��PK��֪ͨ
	virtual void RecvCancelWaitPKMicNotice(int ret,int opidx,int bopidx);
	//��������ɹ�֪ͨ
	virtual void RecvUpWaitMicNotice(int ret,ResultData_UDMicInfo *pRD);
	//���ս���ɹ�֪ͨ
	virtual void RecvDownWaitMicNotice(int ret,ResultData_UDMicInfo *pRD);
	//�����߳�����֪ͨ
	virtual void RecvKickOutNotice(int idx,int bidx);
	//������VJ��������֪ͨ
	virtual void RecvOnVJMicNotice(int ret,int idx);
	//������VJ��������֪ͨ
	virtual void RecvOffVJMicNotice(int ret,int idx);
	//���ձ�����֪ͨ
	virtual void RecvGiveOffMicNotice(int ret,int idx);
	//���ձ�����֪ͨ
	virtual void RecvForbidenNotice(int ret,ResultData_ForbidenInfo* pRD);
	//���պ���������֪ͨ
	virtual void RecvUpdateBlackListNotice(int ret,ResultData_UpBlackList* pRD);
	//���չ���Ա����֪ͨ
	virtual void RecvUpdateMGRListNotice(int ret,ResultData_UpMgrList* pRD);
	//������������֪ͨ
	virtual void RecvSendFlowerNotice(int ret,ResultData_SendFlowerInfo* pRD);
	//add by jinguanfu 2010/3/4 ԭ�лص�API�ĳɽӿ�<end>
	//���շ����ظ���¼��Ϣ
	virtual void RecvRoomKickUser();
	//���մ����ظ���¼��Ϣ
	virtual void RecvLobbyKickUser();
	//�Ӵ������շ��������Ϣ
	virtual void RecvRoomSimpleInfo(int ret,int errcode,ResultData_RoomSimpleInfoList* pRD);
	virtual void RecvRoomSimpleInfoEx(int ret,int errcode,ResultData_RoomSimpleInfoList* pRD, int type);
	//�����û��˳������Ա��Ӧ
	virtual void RecvExitRoomMember(int ret,int errcode);
	//����WEB���������ʼ���Ϣ
	virtual void RecvMailInfo(int ret,char* context,int length);
	//���մ�����������
	virtual void RecvLobbyOnlineNum(int ret,int errcode,ResultData_LobbyOnlineList* pRD);
	//���������Ӧ��Ϣ	//add by jinguanfu 2010/4/27
	virtual void RecvSendFlower(int ret,int errcode,int recevie_idx,int item_id,int count,int gold,int silver);
	//���շ��ʹ����Ȼ�Ӧ //add by jinguanfu 2010/5/13
	virtual void RecvSpeakerToAll(int ret,int errcode,int gold,int silver);
	//���շ���С���Ȼ�Ӧ //add by jinguanfu 2010/5/13
	virtual void RecvSpeakerToLobby(int ret,int errcode,int gold,int silver);
	//�����ɹ���Ա��Ϊһ���Ա�Ļ�Ӧ //add by jinguanfu 2010/5/13
	virtual void RecvRoomMember (int ret , int errcode, int idx,int cseq);
	//�����ɹ���Ա��Ϊһ���Ա��֪ͨ //add by jinguanfu 2010/5/13
	virtual void RecvRoomNotice4Member (int ret , int opidx,int bopidx);
	//���շ��������˸ı���avarat�Ĺ㲥 //add by cx 2010-5-20
	virtual void RecvRoomNotice4ChangeAvarat (int ret, int idx);
	//���շ����ڴ��������ʾ //add by cx 2010-6-6
	virtual void RecvOneError( short cmd );
	//add by cx 2010-7-6 ���ܹ�����Ʒ��Ӧ
	virtual void RecvBuyPack( int ret, int errcode, int gold, int silver, short * goods_id, int * global_id, int count, const char * day );
	// ���ܹ�����Ʒ��Ӧ
	virtual void RecvBuyForSend( int ret, int errcode, int idx, int gold, int silver );
	//���ܻ�ȡ������Ӧ
	virtual void RecvGetPack( int ret, int errcode, ResultData_Pack * pPack, int count, int all_count );
	//����ɾ����Ʒ��Ӧ
	virtual void RecvDelPack( int ret, int errcode, int cseq );
	//������Ʒ����֪ͨ
	virtual void RecvTimeoutPack( int count, int * global_id );
	//������Ʒ����
	virtual void RecvSendPack( int idx, int global_id, short goods_id, const char * day, const char * msg );
	//����ʹ����Ʒ��Ӧ
	virtual void RecvUsePack( int ret, int errcode, int global_id );
	// �����㲥
	virtual void RecvNotice4LevelUp( int idx, char level );
	// ��������
	virtual void RecvExpChange( int nexp, int level );
	// vip�ı�
	virtual void RecvVipChange( int idx, int level, char is_vip );
	// Ǯ�Ҹı�
	virtual void UpdataMoney( int r_gold, int r_silver );
	// �����Լ�����Ϣ
	virtual void RecvUpdataMyInfo();
	// �����̻��㲥
	virtual void RecvFireworks( int ret, int errcode, int cate_idx, int count, int gold, int silver );
	// �����̻��㲥
	virtual void RecvFireworksNotice( int player_idx, int cate_idx, int count );
	// �н���
	virtual void RecvLucky( int player_idx, int cate_idx, int lotte );
	// �н���
	virtual void RecvLucky( int room_idx, int player_idx, int cate_idx, int lotte );
	// �б�
	virtual void RecvGiftTopList( ResultData_GiftList * gift_list );
	// ����ʱ��
	virtual void RecvOnlineTime( int idx, int day_time, int online );
	// ��ȡ��������
	virtual void RecvGetNewUserTask( short step );
	// �������������ύ���
	virtual void RecvTaskStep( int ret, int err_code, int gold, int silver, int total_gold, int total_silver );
	virtual void RecvTaskGift( int ret, int err_code, int item_id, int object_id, const char * time_buf );
	// ��Ʒ��Ϣ����֪ͨ
	virtual void RecvNotice4ChangeGoods();
	// ����������ˢ�±���
	virtual void RecvGetBagAgain();
	// �����û�ӡ��
	virtual void RecvUserSeel( int user_idx, int seel_idx );
	// �����û�������״̬
	virtual void EnterAllMentState( int idx, short state );
	// room loge
	virtual void RecvRoomLoge( int room_idx, const char * loge, int len );
	// �·���汾��
	virtual void RecvNewVersionId( int version_id );
	// ������Ϣ
	virtual void RecvGroupInfo( int count, ResultData_GroupInfo * pInfo );
	// ��ȡ������ķ����б�
	virtual void RecvRoomListInGroup( short group_idx, int count, int * list );
	virtual void RecvRoomListInGroup(short count, ResultData_GroupRoomInfo* pInfo);
	// �ϴ���־֪ͨ
	virtual void RecvNotice4CollectData( const char * server_ip, short server_port );
	// ��ȡ������Ϣ
	virtual void RecvSongInfoList( short totalnum, short curnum, ResultData_SongInfo * info );
	// ��ȡ����Ƶ�������б�
	virtual void RecvAvServerList( ResultData_AvServer * list, int count );
	// ��������Ƶ�������б�
	virtual void UpdataAvServerList();
	// �������Ϸ������
	virtual void RecvCheckOutGame( int result );
	// ��ȡ������Ϣ
	virtual void RecvOffLineMsg( int count, ResultData_OffLineMsg * msg );
	// ���ط����ö���Ϣ
	virtual void RecvRoomSortInGroup( int idx, int count, ResultData_RoomSort * list );
	// ֪ͨ�����ö���Ϣ�ı�
	virtual void RecvRoomSortChange( int idx );
	// ���ֵȼ�
	virtual void RecvSingerLevel(int idx, short curlevel, int expvalue);
	// �Ƹ��ȼ�
	virtual void RecvWealthLevel(int idx, short curlevel, int expvalue);

	//////////////////////////////////////////////////////////////////////////

	// ����ʧЧ��ӡ�£���Э��
	virtual void RecvGiftInvalid( int idx, int GiftID );
	// ������Ч��ӡ�£���Э��
	virtual void RecvGiftValid( int idx, int GiftID );
	// ��ʾ�������ȡ����
	virtual void RecvShowRoomIncom( int income );
	// ��ʾ��������ȡ��¼
	virtual void RecvShowRoomGetIncom( ResultData_IncomeDate * data, int count );
	// ������ȡ����������
	virtual void RecvGetRoomIncom( int ret );
	// ������Ϣ����֪ͨ
	virtual void RecvNotice4ChangeFlower();
	// Ȩ�����ø���֪ͨ
	virtual void RecvNotice4ChangePower();
	// ��������Ƶ�������㲥
	virtual void RecvNotice4AvserverChange( ResultData_AvChange * info );
	// ����gm
	virtual void RecvChangeGM( int idx, char add );
	// ������˵�pc�ſ�
	virtual void RecvMicPcInfo( int idx, const char * buf, int len );
	// ��Ӧ�Ƿ������뷿��ɹ�
	virtual void RecvSendJoinRoomInvite( int wseq, int ret, int err_code );
	// ��Ӧ�Ƿ������뷿��ɹ�
	virtual void RecvInviteJoinRoom( int idx );
	//ȡ�õ����������ͼ�¼
	virtual void RecvGiftSend( int count, ResultData_GiftCount * pInfo );
	//ȡ�õ���������ռ�¼
	virtual void RecvGiftRecv( int count, ResultData_GiftCount * pInfo );
	//���ط�mac���
	virtual void RecvKickUserByMac( int ret, int idx );
	//���ط�ip���
	virtual void RecvKickUserByIp( int ret, int idx );
	//������������ʱ����
	virtual void RecvSetMicTime( int ret, int idx );
	//�㲥����ʱ���ӳ�
	virtual void RecvNotice4SetMicTime( int idx, int time );
	//ȡ�����б�
	virtual void RecvWaitMicList( short count, int* idxs );
	//������ʱ�䵽,�Ͽ�����
	virtual void RecvIndulgeTimeUp();
	/*������Ϣ�ص�
	������
	flag��
	1:ɾ������
	2:��ӻ���¼���
	familyNum��
	��������(N)
	familys��
	������Ϣ����һ������familys[N-1];
	ע�⣺��flagΪ1ʱ��familyNum�ض�Ϊ1��ͬʱfamilys[0]��ֻ��familyid�ǿ��õ�(��Ϊɾ����ʱ��һ��ֻ��ɾ��һ����ֻ�м���id��Ϣ)
	*/
	virtual void RecvFamilyInfo(int flag, int familyNum, ResultData_Family* familys);

	/*
	av������Ϣ
	*/
	virtual void RecvAVConfigInfo(int infolen, const char* info);

	/* ˽��                                                                 */
	/************************************************************************/
	/*��˽��*/
	virtual void RecvOpenPrivateMicSuccess(int idx, short mictype, int posindex, int roomid);
	virtual void RecvOpenPrivateMicFailed(int errorcode, int idx, int bidx);
	virtual void RecvOpenPrivateMicNotice(int idx, int mictype); /*�������Լ�*/
	
	/*��˽��*/
	virtual void RecvClosePrivateMicFailed(int errorcode, int idx, int bidx);
	virtual void RecvClosePriveteMicNotice(int idx); /*�����Լ�*/
	
	/*������˽��*/
	virtual void RecvClosedPrivateMicFailed(int errorcode, int idx, int bidx);
	virtual void RecvClosedPriveteMicNotice(int idx, int bidx); /*�����Լ�*/
	
	/*�ۿ�˽��*/
	virtual void RecvWatchPrivateMicSuccess(int idx, short mictype, int posindex, int roomid);
	virtual void RecvWatchPrivateMicFailed(int errorcode, int idx, int bidx);
	virtual void RecvRequestWatchedPrivateMic(int idx); /*idx=������, �Ƿ�ͬ���idx�ۿ����˽��*/
	virtual void RecvWatchedPrivateMicFailed(int errorcode, int idx, int bidx);

	virtual void RecvWatchedPrivateMicReject(int idx); //�Է��ܾ���idx=˭�ܾ���

	/*����˽��*/
	virtual void RecvInviteWatchPrivateMicSuccess();
	virtual void RecvInviteWatchPrivateMicFailed(int errorcode, int idx, int bidx);
	virtual void RecvRequestInvitedWatchPrivateMic(int idx); /*idx=������,�Ƿ�ͬ��ۿ�idx��˽��*/
	virtual void RecvInvitedWatchPrivateMicSuccess(int idx, short mictype, int posindex, int roomid);
	virtual void RecvInvitedWatchPrivateMicFailed(int errorcode, int idx, int bidx);

	virtual void RecvInvitedWatchPrivateMicReject(int idx);//�Է��ܾ���idx=˭�ܾ���

	/*������˽��*/
	virtual void RecvOpenRoomPrivateMicFlag(int ret, char flag);
	virtual void RecvOpenRoomPrivateMicFlagNotice(int idx, char flag); /*�����Լ�*/

	/*�û���¼����ʱ���ص������˽����Ϣ*/
	virtual void RecvPrivateMicWithLoginRoom(int idx, short mictype, int posindex, int roomid);
	/*	���ػʹ����˽��
		int ret
		 0: �ɹ� 
		-1: ʧ�ܣ��Ƿ�����
		-2: ʧ�ܣ�ʣ���˴�������
		int kickuse
		���β��������˴�
		int totalkick
		ʣ�������˴���
		int idx;	
		������idx
	*/
	virtual void RecvCrownKickoutSomeOne(int ret, int kickuse, int totalkick, int idx);
	/*������ֹ�û�ip
	int ret;
	 0���ɹ�
	 -1����IP�ѱ�����
	 -2�����û����뿪����
	
	int idx;
	��������idx
	*/
	virtual void RecvForbidUserIp(int ret, int idx);

	//Ҫ���ϴ�����
	virtual void RecvUpdateSong();
	//��Ӧ�������ķ����û��質��Ϣ
	//virtual void RecvUserSingInfo(int ret,int idx,const ResultData_UserSingInfo* pData);
	//��ȡ�û������������
	virtual void RecvGiftCount(int recvcount);

	//��ȡ�û���߷ָ�����Ϣ���ݳ���������
	virtual void RecvTopSongInfo(int songcount, int topsongscore, const char* topsongname, int len);
	//http������ɵķ��ؽӿ�
	//static void OnFinish(_TaskDownload* downloadInfo,bool success);
	//�ֻ�ktv������Ϣ
    /*
     @param ret:����ֵ��0,OK;����ֵ,something error
     @param fromIdx:����˭,�����������Ҳ�п���ָ���͵�IDX
     @param roomName:˵�����ڵķ��䣬���δ�ڷ�����Ϊ��0��
     @param content:˵������
     @param chatTime:˵��ʱ��
     @param isPublic:�Ƿ�Ϊ����
     @param isSend:�Ƿ����ҷ��͵ģ����Ϊfalse�����Ǳ��˷����ҵ���Ϣ
     
     @ע:chattime
     */
    virtual void RecvKtvChatInfo(int ret,int fromIdx,int roomid,const char* roomName,const char* content,
                                 int chatTime,bool isPublic=true,bool isSend=true);
    //�ֻ����յ���ǰ�Ĳ����б�����ǰktv�ĵ���б����ڸ���
    /*
     @param pSonglist:��Ϣ�ṹ��
     @param listcount:��������
     */
    virtual void RecvKtvSongList(const ResultData_SongList* pSonglist,int listCount);
    //�ֻ���ȡ��ǰ����Ŀ���״̬
    /*
     @param isOpen:true,���Ὺ����false������رգ��û��˳�����
     */
    virtual void RecvKtvRoomState(bool isOpen);
    //�ֻ��յ������ϰ�֪ͨ,��ʾ�û���������Ϣ����ȥ����
    virtual void RecvKtvSongRanking();
	/*
	�յ��������ֻ��û�IDX�б��������
	ֻ��IDX
	*/
	virtual void RecvKtvIdxList(int* pIdx,int number);
	/*
	֪ͨ�û�ͷ�����
	@param [in] idx��ָ�����û�IDX
	@param [in] picture���û�����ͷ��
	*/
	virtual void RecvUserPicChange(int idx,const char* picture);
    //��������ͼƬ�������֪ͨ
    //����������ȥ��֮ǰû�л�ȡ��ͼƬ�ĵط�������ȥ��ȡһ��,�����Ѿ�����������
    /*
     @param pictureName[in]:ͼƬ����(��·��)
     
     @ע:����ص����̸߳�����Ļص��̶߳���һ����������HTTP�����̵߳Ļص�
     */
    virtual void RecvImageDownladFinished(const char* pictureName);
	/*
     ���̻߳���HTTP�̻߳ص����ڳ�ʼ�����ص�ַ֮��ɹ��Ļ��ͻ�ص���֪ͨ�������ݿ�������Ϣ�Ѿ��������
     */
    virtual void RecvLocalGiftDatabaseFinished();
};

