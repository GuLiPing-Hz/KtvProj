/*
 * 
 *   by glp
 *   2014-4-9   
 *   �����ṩ��c netutil��java�ص��ӿڣ�c netutil�ڲ������
 *   ����ӿ������뿴��Ӧ����˵������Ҫ�Լ�ȥʵ����Ӧҵ��
 * 
 */

package com.jni.netutil;

public interface JniNetUtil{
	//���졢���ȹ���
	  void SendChatFromUser(int ret, int to_idx, String content, int time );
	//����˽��
	  void RecvChatFromUser(int ret, int from_idx, String content, int time );
	//���չ��ģ����ֲ��ڲ���
	  void RecvChatFromClub(int ret, int from_idx, String content);
	//���չ��ģ������ڣ�
	//ret:1--���� 0--������ -1--��������
	  void RecvChatFromRoom(int ret, int errcode,int from_idx, String content, int time );	
	//���մ����ȣ�ȫ����
	  void RecvSpeakerFromAll(int ret, int type, int from_id, String msg);
	//����С���ȣ�������
	  //int ret, int from_idx, int room_id,const char* room_name,const char* msg, int time,int type,bool isSend
	  void RecvSpeakerFromLobby(int ret, int from_idx,String room_name, String msg, int time,int type,boolean isSend );
	//��������㲥
	  void RecvPresentBroad(int ret, ResultData_GiftInfo guestGiftInfo, ResultData_GiftInfo commGiftInfo);
	//����ϵͳ���ֹ㲥
	  void RecvSystemBroad( int type, String msg);
	//����ϵͳ���ֹ㲥�������н���Ϣ��
	  void RecvSystemBroad2(String nickname, int idx, int roomid, int giftid, int luckmultiple);
	//����ϵͳ���ֹ���
	  void RecvSystemNotice( String msg, boolean once, int min );
	//����ϵͳ���ֹ��棨�����н���Ϣ��
	  void RecvSystemNotice2(String nickname, int idx, int roomid, int giftid, int luckmultiple);
	//���շ�����ʱ����
	  void RecvRoomNotice(int ret, String content);
	//ȡ�ú���λ��
	  void RecvFriendLocation(int ret, int errcode,int friend_idx,int lobbyid, int roomid,int cseq);
	/*			 ���Ѳ���					*/
	/*			�������ӿ�			*/
	//����ȡ�ô���˷����Ա�б�
	  void RecvRoomApplyList (int ret ,int errcode,ResultDate_RoomApplyList applylist,int cseq);
	//�������뷿���Ա��Ӧ
	  void RecvApplyRoomMember(int ret,int errcode,int cseq);
	//������˻�Ա����������
	  void RecvAuditAddApply (int ret, int errcode,int idx,int cseq);
	//���վܾ���������ϵͳ��ʾ
	  void RecvRoomApplyRefuseMsg (int ret, int roomid);
	//���շ����Ա�б�
	  void RecvRoomMemberList (int ret ,int errcode, ResultDate_RoomMemList memlist,int cseq);
	//���ս�������Ա�ʸ��Ӧ
	  void RecvUnchainMember (int ret, int errcode, int idx,int cseq);
	//���ո����û�������������Ȩ�޻�Ӧ
	  void RecvRoomPesideAssistant (int ret , int errcode, int idx,int cseq);
	//���ո�����������֪ͨ
	  void RecvRoomNotice4PesideAssistant (int ret , int opidx,int bopidx);
	//���ո����û�����������Ȩ�޻�Ӧ
	  void RecvRoomPeside (int ret ,int errcode, int idx,int cseq);
	//��������������Ȩ�޷���֪ͨ
	  void RecvRoomNotice4Peside (int ret , int opidx,int bopidx);
	//���ո����û����丱����Ȩ�޻�Ӧ
	  void RecvRoomSubHost (int ret ,int errcode, int idx,int cseq);
	//�������ø�����Ȩ�޷���֪ͨ
	  void RecvRoomNotice4SubHost(int ret , int opidx,int bopidx);
	//���շ��������
	  void RecvBlackList(int ret,	int errcode,ResultDate_RoomBlackList blacklist,int cseq);
	//���պ������û����/ɾ����Ӧ
	  void RecvAddDelBlack(int ret , int errcode,int idx, int type);
	//���շ������������Ϣ
	  void RecvRoomPanelInfo(int ret ,int errcode, ResultDate_RoomPanel panelinfo);
	//�������÷��������Ӧ
	  void RecvRoomPassword(int ret,int errcode,int cseq);
	//�������÷���������Ϣ��ʾ
	  void RecvRoomNotice4Password(int ret, int idx, int pwdflag);
	//�������÷��俪��/�ر�(����)��Ӧ
	  void RecvRoomSwitch(int ret,int errcode,int roomflag,int cseq);
	//���շ��俪��/�ر�(����)��Ϣ��ʾ
	  void RecvRoomNotice4RoomSwitch(int ret , int idx , int roomflag);
	//�������÷������Ȩ�޻�Ӧ
	  void RecvRoomPrivate(int ret , int errcode,int accessflag,int cseq);
	//���շ������Ȩ����Ϣ��ʾ
	  void RecvRoomNotice4Private(int ret , int idx , int accessflag);
	//���������û�������Ϣ����/�رջ�Ӧ
	  void RecvUserInOutSwitch(int ret , int errcode, int inoutflag,int cseq);
	//�����û�������Ϣ����/�ر���Ϣ��ʾ
	  void RecvRoomNotice4UserSwitch (int ret , int idx , int inoutflag);
	//������������������/�رջ�Ӧ
	  void RecvGetMicSwitch (int ret, int errcode,int onmicflag,int cseq);
	//������������������/�رջ�Ӧ
	  void RecvOnMicSwitch (int ret, int errcode,int onmicflag,int cseq);
	//��������������/�ر���Ϣ��ʾ
	  void RecvRoomNotice4GetMicSwitch(int ret , int idx , int getmicflag);
	//��������������/�ر���Ϣ��ʾ
	  void RecvRoomNotice4OnMicSwitch(int ret , int idx , int onmicflag);
	//���������޸Ļ�Ӧ
	  void RecvModifyRoomName(int ret,int errcode,int cseq);
	//���շ�����
	  void RecvRoomName (int ret , int roomid , String roomname);
	//���÷��乫����Ӧ
	  void RecvRoomAffiche(int ret,int errcode,int cseq);
	//���շ��乫������¼�
	  void RecvRoomAfficheEvent(int ret);
	//���������乫����Ӧ
	  void RecvAllowRoomAllChat (int ret, int errcode,int chatflag,int cseq);
	//���շ��乫��������Ϣ��ʾ
	  void RecvRoomNotice4AllChat(int ret , int idx , int chatflag);
	//���÷��件ӭ����Ӧ
	  void RecvRoomSalutatory(int ret, int errcode,int cseq);
	//���շ��件ӭ�ʸ���֪ͨ
	  void RecvRoomNotice4Salutatory(int ret,int idx);
	//���÷���logo��Ӧ
	  void RecvRoomLogo (int ret,int errcode ,int cseq);
	//���շ���LOGO����֪ͨ
	  void RecvRoomNotice4Logo(int ret,int idx);
	//���շ��䶳����Ϣ��ʾ
	  void RecvRoomNotice4Lock(int idx);
	//���ܴ��������Ӧ
	  void RecvCreateRoom( int ret, int errcode, RoomListEx info );
	//�������跿��ȫ��Ϣ
	  void RecvTmpRoomInfo( int ret, int errcode, RoomListEx info );
	// ��Ӹ跿�㲥
	  void RecvRoomNotice4Add( RoomListEx info );
	  void RecvRoomNotice4Add( int idx );
	// ɾ���跿�㲥
	  void RecvRoomNotice4Del( int idx );
	// ���շ�������֪ͨ
	  void RecvRoomNotice4ChangeMaster(int idx);
	// ���շ�����Ϣ����֪ͨ
	  void RecvRoomNotice4ChangeInfo( int idx );
	/*			�������ӿ�				*/
	/*			 ��ɫ��Ϣ				*/
	//ȡ�����˽�ɫ��Ϣ
	  void RecvPartInfo(int ret, int errcode, ResultData_PartInfo part_info, int cseq);
	  
	  //ȡ���Լ���ɫ��Ϣ ר����java�Ľӿ� ��½�������ķ��ؽ�� @glp
	  //@ע��Android���еĻص����������ڴ�����½���һ���Ļص�
	  void OnLoginLobby(int ret, int errcode, LocalData_UserInfo userInfo);
	//ȡ���Լ���ɫ��Ϣ
	  void RecvPartInfoBySelf(int ret, int errcode, ResultData_PartInfo part_info, int cseq);
	//��ɫ��Ϣ
	//ȡ�÷����б�
	  void RecvRoomList(int ret,ResultDate_RoomList roomlist);
	  void RecvRoomListEx(int ret,ResultDate_RoomListEx roomlist);
	//����֪ͨ
	  void RecvNotice4OffMic( int onmic_idx, int pk_idx, int exp, String scoreinfo);
	// add by cx 10-6-2
	//��Ӧ�Ƿ���������ɹ�
	  void RecvRoomGivOnevjMic( int idx, int flag );
	//�Ƿ�ͬ����������ɹ�
	  void RecvRoomGivOnevjMicApply( int formidx );
	//�Ƿ�ͬ����ӶԷ�Ϊ����
	//����˵����int idx ��Ӻ�����idx
	  void RecvAddFriendApply(int idx);
	//  void RecvDeleteAvarta(int ret);
	//add by jinguanfu 2010/3/4 ԭ�лص�API�ĳɽӿ�<begin>
	//���ӳɹ�
	  void OnAuthTunnelConnectSuccess();			//�ɹ�������֤������
	  void OnLobbyTunnelConnectSuccess();		//�ɹ����Ӵ���������
	  void OnRoomTunnelConnectSuccess();			//�ɹ����ӷ��������
	  void OnChannellistTunnelConnectSuccess();	//�ɹ�����Ƶ��������
	//���ӳ�ʱ
	  void OnAuthTunnelConnectTimeout();		//������֤��������ʱ
	  void OnLobbyTunnelConnectTimeout();		//���Ӵ�����������ʱ
	  void OnRoomTunnelConnectTimeout();		//���ӷ����������ʱ
	  void OnChannellistTunnelConnectTimeout();//����Ƶ����������ʱ
	//���Ӵ���
	  void OnAuthTunnelConnectError( int code );	
	  void OnLobbyTunnelConnectError( int code );
	  void OnRoomTunnelConnectError( int code );
	  void OnChannellistTunnelConnectError( int code );
	//�����������Ͽ�������,�ͻ���recv == 0��ʱ��,�ص������µĽӿ�
	  void OnAuthTunnelClose();	//�Ͽ���֤������
	  void OnLobbyTunnelClose();	//�Ͽ�����������
	  void OnRoomTunnelClose();	//�Ͽ����������
	  void OnChannellistTunnelClose();//�Ͽ�Ƶ��������
	//�ͻ���recv�쳣,send�쳣,�����buf���,select��������,����ص���������½ӿ�
	  void OnAuthTunnelError( int code );	
	  void OnLobbyTunnelError( int code );
	  void OnRoomTunnelError( int code );
	  void OnChannellistTunnelError( int code );

	/*----------------------------��֤�����������ӿ�--------------------------------------*/
	//������֤���
	  void RecvAuth(int ret, int errcode,int again);
	//������token
	//  void RecvNewToken(int ret,ReserveDataS pRD);
	  void RecvNewToken(int ret,int errcode);
	//����Ƶ���б�
	  void RecvSectionList(int ret,int errcode,ResultData_GetChannellist RDc,int cseq);
	//����Ƶ���б�(��ӷ�������Ϣ)
	  void RecvSectionList2(int ret,int errcode,int leftlogintime,ResultData_GetChannellist RDc,int cseq);
	/*-------------------------���������������ӿ�------------------*/
	//���մ���token��֤
	  void RecvTokenAuth4Lobby( int ret, int errcode, ResultData_LobbyLogin pRDl, ResultData_SelfData pSD, int cseq );
	//�û���¼����֪ͨ
	  void RecvUserLoginLobbyNotice(int ret,/*ResultData_LobbyLoginUser pRlu*/
				int idx,char relation,int lobbyid);
	//�û��˳�����֪ͨ
	  void RecvUserLogoutLobbyNotice(int ret,/*ResultData_LobbyLoginUser pRlu*/
				int idx,char relation,int lobbyid);
	//�����û���Ϣ
	  void RecvUserInfo(int ret,int idx,ResultData_UserFullInfo RDu);
	//���պ���״̬
	  void RecvFriendStatus(int ret,int errcode,ResultData_GetFriendlist RD);
	//���պ�����״̬
	  void RecvBlackStatus(int ret,int errcode,ResultData_GetFriendlist RD);
	//���վ��ֲ�״̬
	  void RecvClubStatus(int ret,int errcode,ResultData_GetClublist RD);
	//������Ӻ��ѽ��
	  void RecvAddFriend(int ret,int errcode,int idx);
	//������Ӻ��������
	  void RecvAddBlack(int ret,int errcode,int cseq);
	//����ɾ�����ѽ��
	  void RecvDelFriend(int ret,int errcode,int cseq);
	//����ɾ�����������
	  void RecvDelBlack(int ret,int errcode,int cseq);
	//�������������ֲ����
	  void RecvApplyClub(int ret,int errcode,int cseq);
	//�����˳����ֲ����
	  void RecvExitClub(int ret,int errcode,int cseq);
	//������������ڵļ�����ֲ���������
	  void RecvApplyListCount(int ret,int errcode,int count,int cseq);
	//������������ڵļ�����ֲ������б�
	  void RecvApplyList(int ret,int errcode,ResultData_ClubApplyList RD,int cseq);
	//ͨ��������ֲ�����
	  void RecvAddClubOk(int ret,ResultData_Approvelresult RD);
	//ɾ�������б�
	  void RecvDelApplyList(int ret,ResultData_Approvelresult RD);
	//���ռ�����ֲ��㲥��Ϣ(�����˼�����ֲ�)
	//modify by jinguanfu 2010/4/11
	//  void RecvAddClubNotice(int ret ,int idx);
	//opidx--������idx
	//addidx--������idx
	  void RecvAddClubNoticeByLobby(int ret ,int clubid,String clubname,int opidx,int addidx);
	  void RecvAddClubNoticeByRoom( int opidx, int addidx );
	//���������Ա�ܾ���Ϣ
	//opidx--�����idx
	  void RecvDenyClubNoticeByLobby(int ret ,int clubid,String clubname,int opidx);
	//  void RecvDenyClubNoticeByRoom(int ret ,int clubid,String clubname,int opidx);
	//���ջ�Ա�˳����ֲ���Ϣ
	  void RecvExitClubNoticeByLobby(int ret,int clubid,String clubname,int exitidx);
	  void RecvExitClubNoticeByRoom( int exitidx );
	//���ջ�Ա���߳����ֲ���Ϣ
	//opidx--������idx
	//kickidx--���߳���idx
	  void RecvKickClubNoticeByLobby(int ret,int clubid,String clubname,int opidx,int kickidx);
	  void RecvKickClubNoticeByRoom( int opidx, int kickidx );
	//�������ֲ�
	  void RecvCreateClub(int ret,int errcode,int clubid,int cseq);
	//��ɢ���ֲ�
	  void RecvDismissClub(int ret,int errcode,int cseq);
	//���վ��ֲ���Ϣ
	  void RecvClubInfo(int ret,int errcode,ResultData_ClubFullInfo RD,int cseq);
	////���շ����б�
	//  void RecvRoomList(int ret,ResultData_RoomList pRD);
	//���շ�����Ϣ
	  void RecvRoomInfo(int ret,ResultData_RoomInfo RD);
	//����ͷ��
	  void RecvCreateAvatar(int ret,int errcode);
	//�޸�ͷ��
	  void RecvChangeAvatar(int ret,int errcode);
	//�޸��ǳ�
	  void RecvChangeNickName(int ret,int errcode, String name);
	//���뷿��֪ͨ
	  void RecvLobbyEnterRoom(int ret,ResultData_LobbyRoomEnter RD);
	//�뿪����֪ͨ
	  void RecvLobbyLeaveRoom(int ret,ResultData_LobbyRoomEnter RD);
	//��Ӻ���֪ͨ
	  void RecvAddFriendNotice(int ret,int idx);
	//ȡ�ô�����Ϣ
	  void RecvLobbyInfo(int ret,int errcode,ResultData_LobbyName RD,int cseq);
	//�������Դ�����Ȩ�޸�����Ϣ add by jinguanfu 2010/6/5
	  void RecvChangePower(int optionidx/*������ID*/,int identity/*���ĺ�Ȩ��*/);
	//ȡ������(CMD_SYSTEMMSG_L2C)
	//  void RecvLobbyName();
	/*------------------------��������ӿ�------------------------------*/
	//���շ���token��֤ //
	  void RecvTokenAuth4Room(int ret,int errcode);
	//���뷿��
	  void RecvEnterRoom(int ret,int errcode,/*ResultData_LoginInfo RDlf*/byte retlogin,byte identity,int cseq);
	//�û����뷿��֪ͨ
	  void RecvEnterRoomNotice(int ret,ResultData_UserInfo RDu);
	//�û��뿪����֪ͨ
	  void RecvLeaveRoomNotice(int ret,int idx);
	//���շ�����ϸ��Ϣ
	  void RecvRoomAllInfo(int ret,int errcode,ResultData_RoomAllInfo RD,int cseq);
	//��������ɹ�֪ͨ
	  void RecvWaitMicNotice(int ret,ResultData_MicInfo RD);
	////��������׼��֪ͨ
	//  void RecvWaitMicReadyNotice(int ret,ResultData_MicInfo pRD);
	//��������׼�����֪ͨ
	//  void RecvOnMicReadyOkNotice(int ret,ResultData_MicInfo pRD);
	//��������֪ͨ
	  void RecvOnMicNotice(int ret,ResultData_MicInfo RD);
	//ȡ������ɹ�֪ͨ
	  void RecvCancelWaitMicNotice(int ret,int opidx,int bopidx);
	//���ռ���PK��֪ͨ
	  void RecvWaitPKMicNotice(int ret,ResultData_MicInfo RD);
	//����ȡ��PK��֪ͨ
	  void RecvCancelWaitPKMicNotice(int ret,int opidx,int bopidx);
	//��������ɹ�֪ͨ
	  void RecvUpWaitMicNotice(int ret,ResultData_UDMicInfo RD);
	//���ս���ɹ�֪ͨ
	  void RecvDownWaitMicNotice(int ret,ResultData_UDMicInfo RD);
	//�����߳�����֪ͨ
	  void RecvKickOutNotice(int idx,int bidx);
	//������VJ��������֪ͨ
	  void RecvOnVJMicNotice(int ret,int idx);
	//������VJ��������֪ͨ
	  void RecvOffVJMicNotice(int ret,int idx);
	//���ձ�����֪ͨ
	  void RecvGiveOffMicNotice(int ret,int idx);
	//���ձ�����֪ͨ
	  void RecvForbidenNotice(int ret,ResultData_ForbidenInfo RD);
	//���պ���������֪ͨ
	  void RecvUpdateBlackListNotice(int ret,ResultData_UpBlackList RD);
	//���չ���Ա����֪ͨ
	  void RecvUpdateMGRListNotice(int ret,ResultData_UpMgrList RD);
	//������������֪ͨ
	  void RecvSendFlowerNotice(int ret,ResultData_SendFlowerInfo RD);
	//add by jinguanfu 2010/3/4 ԭ�лص�API�ĳɽӿ�<end>
	//���շ����ظ���¼��Ϣ
	  void RecvRoomKickUser();
	//���մ����ظ���¼��Ϣ
	  void RecvLobbyKickUser();
	//�Ӵ������շ��������Ϣ
	  void RecvRoomSimpleInfo(int ret,int errcode,ResultData_RoomSimpleInfoList RD);
	  void RecvRoomSimpleInfoEx(int ret,int errcode,ResultData_RoomSimpleInfoList RD, int type);
	//�����û��˳������Ա��Ӧ
	  void RecvExitRoomMember(int ret,int errcode);
	//����WEB���������ʼ���Ϣ
	  void RecvMailInfo(int ret,String context);
	//���մ�����������
	  void RecvLobbyOnlineNum(int ret,int errcode,ResultData_LobbyOnlineList RD);
	//���������Ӧ��Ϣ	//add by jinguanfu 2010/4/27
	  void RecvSendFlower(int ret,int errcode,int recevie_idx,int item_id,int count,int gold,int silver);
	//���շ��ʹ����Ȼ�Ӧ //add by jinguanfu 2010/5/13
	  void RecvSpeakerToAll(int ret,int errcode,int gold,int silver);
	//���շ���С���Ȼ�Ӧ //add by jinguanfu 2010/5/13
	  void RecvSpeakerToLobby(int ret,int errcode,int gold,int silver);
	//�����ɹ���Ա��Ϊһ���Ա�Ļ�Ӧ //add by jinguanfu 2010/5/13
	  void RecvRoomMember (int ret , int errcode, int idx,int cseq);
	//�����ɹ���Ա��Ϊһ���Ա��֪ͨ //add by jinguanfu 2010/5/13
	  void RecvRoomNotice4Member (int ret , int opidx,int bopidx);
	//���շ��������˸ı���avarat�Ĺ㲥 //add by cx 2010-5-20
	  void RecvRoomNotice4ChangeAvarat (int ret, int idx);
	//���շ����ڴ��������ʾ //add by cx 2010-6-6
	  void RecvOneError( short cmd );
	//add by cx 2010-7-6 ���ܹ�����Ʒ��Ӧ
	  void RecvBuyPack( int ret, int errcode, int gold, int silver, short [] goods_id, int [] global_id, int count, String day );
	// ���ܹ�����Ʒ��Ӧ
	  void RecvBuyForSend( int ret, int errcode, int idx, int gold, int silver );
	//���ܻ�ȡ������Ӧ
	  void RecvGetPack( int ret, int errcode, ResultData_Pack [] pPack, int count, int all_count );
	//����ɾ����Ʒ��Ӧ
	  void RecvDelPack( int ret, int errcode, int cseq );
	//������Ʒ����֪ͨ
	  void RecvTimeoutPack( int count, int [] global_id );
	//������Ʒ����
	  void RecvSendPack( int idx, int global_id, short goods_id, String day, String msg );
	//����ʹ����Ʒ��Ӧ
	  void RecvUsePack( int ret, int errcode, int global_id );
	// �����㲥
	  void RecvNotice4LevelUp( int idx, int level );
	// ��������
	  void RecvExpChange( int nexp, int level );
	// vip�ı�
	  void RecvVipChange( int idx, int level, int is_vip );
	// Ǯ�Ҹı�
	  void UpdataMoney( int r_gold, int r_silver );
	// �����Լ�����Ϣ
	  void RecvUpdataMyInfo();
	// �����̻��㲥
	  void RecvFireworks( int ret, int errcode, int cate_idx, int count, int gold, int silver );
	// �����̻��㲥
	  void RecvFireworksNotice( int player_idx, int cate_idx, int count );
	// �н���
	  void RecvLucky( int player_idx, int cate_idx, int lotte );
	// �н���
	  void RecvLucky( int room_idx, int player_idx, int cate_idx, int lotte );
	// �б�
	  void RecvGiftTopList( ResultData_GiftList gift_list );
	// ����ʱ��
	  void RecvOnlineTime( int idx, int day_time, int online );
	// ��ȡ��������
	  void RecvGetNewUserTask( short step );
	// �������������ύ���
	  void RecvTaskStep( int ret, int err_code, int gold, int silver, int total_gold, int total_silver );
	  void RecvTaskGift( int ret, int err_code, int item_id, int object_id, String time_buf );
	// ��Ʒ��Ϣ����֪ͨ
	  void RecvNotice4ChangeGoods();
	// ����������ˢ�±���
	  void RecvGetBagAgain();
	// �����û�ӡ��
	  void RecvUserSeel( int user_idx, int seel_idx );
	// �����û�������״̬
	  void EnterAllMentState( int idx, short state );
	// room loge
	  void RecvRoomLoge( int room_idx, String loge, int len );
	// �·���汾��
	  void RecvNewVersionId( int version_id );
	// ������Ϣ
	  void RecvGroupInfo( int count, ResultData_GroupInfo Info );
	// ��ȡ������ķ����б�
	  void RecvRoomListInGroup( short group_idx, int count, int [] list );
	  void RecvRoomListInGroup(short count, ResultData_GroupRoomInfo[] pInfo);
	// �ϴ���־֪ͨ
	  void RecvNotice4CollectData( String server_ip, short server_port );
	// ��ȡ������Ϣ
	  void RecvSongInfoList( short totalnum, short curnum, ResultData_SongInfo info );
	// ��ȡ����Ƶ�������б�
	  void RecvAvServerList( ResultData_AvServer [] list, int count );
	// ��������Ƶ�������б�
	  void UpdataAvServerList();
	// �������Ϸ������
	  void RecvCheckOutGame( int result );
	// ��ȡ������Ϣ
	  void RecvOffLineMsg( int count, ResultData_OffLineMsg [] msg );
	// ���ط����ö���Ϣ
	  void RecvRoomSortInGroup( int idx, int count, ResultData_RoomSort [] list );
	// ֪ͨ�����ö���Ϣ�ı�
	  void RecvRoomSortChange( int idx );
	// ���ֵȼ�
	  void RecvSingerLevel(int idx, short curlevel, int expvalue);
	// �Ƹ��ȼ�
	  void RecvWealthLevel(int idx, short curlevel, int expvalue);

	//////////////////////////////////////////////////////////////////////////

	// ����ʧЧ��ӡ�£���Э��
	  void RecvGiftInvalid( int idx, int GiftID );
	// ������Ч��ӡ�£���Э��
	  void RecvGiftValid( int idx, int GiftID );
	// ��ʾ�������ȡ����
	  void RecvShowRoomIncom( int income );
	// ��ʾ��������ȡ��¼
	  void RecvShowRoomGetIncom( ResultData_IncomeDate [] data, int count );
	// ������ȡ����������
	  void RecvGetRoomIncom( int ret );
	// ������Ϣ����֪ͨ
	  void RecvNotice4ChangeFlower();
	// Ȩ�����ø���֪ͨ
	  void RecvNotice4ChangePower();
	// ��������Ƶ�������㲥
	  void RecvNotice4AvserverChange( ResultData_AvChange info );
	// ����gm
	  void RecvChangeGM( int idx, int add );
	// ������˵�pc�ſ�
	  void RecvMicPcInfo( int idx, String buf );
	// ��Ӧ�Ƿ������뷿��ɹ�
	  void RecvSendJoinRoomInvite( int wseq, int ret, int err_code );
	// ��Ӧ�Ƿ������뷿��ɹ�
	  void RecvInviteJoinRoom( int idx );
	//ȡ�õ����������ͼ�¼
	  void RecvGiftSend( int count, ResultData_GiftCount [] pInfo );
	//ȡ�õ���������ռ�¼
	  void RecvGiftRecv( int count, ResultData_GiftCount [] pInfo );
	//���ط�mac���
	  void RecvKickUserByMac( int ret, int idx );
	//���ط�ip���
	  void RecvKickUserByIp( int ret, int idx );
	//������������ʱ����
	  void RecvSetMicTime( int ret, int idx );
	//�㲥����ʱ���ӳ�
	  void RecvNotice4SetMicTime( int idx, int time );
	//ȡ�����б�
	  void RecvWaitMicList( short count, int[] idxs );
	//������ʱ�䵽,�Ͽ�����
	  void RecvIndulgeTimeUp();
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
	  void RecvFamilyInfo(int flag, int familyNum, ResultData_Family[] familys);

	/*
	av������Ϣ
	*/
	  void RecvAVConfigInfo(int infolen, String info);

	//
	/*------------------------------˽��---------------------------------------*/
	/*��˽��*/
	  void RecvOpenPrivateMicSuccess(int idx, short mictype, int posindex, int roomid);
	  void RecvOpenPrivateMicFailed(int errorcode, int idx, int bidx);
	  void RecvOpenPrivateMicNotice(int idx, int mictype); //�������Լ�
	
	/*��˽��*/
	  void RecvClosePrivateMicFailed(int errorcode, int idx, int bidx);
	  void RecvClosePriveteMicNotice(int idx); //�����Լ�
	
	/*������˽��*/
	  void RecvClosedPrivateMicFailed(int errorcode, int idx, int bidx);
	  void RecvClosedPriveteMicNotice(int idx, int bidx); //�����Լ�
	
	/*�ۿ�˽��*/
	  void RecvWatchPrivateMicSuccess(int idx, short mictype, int posindex, int roomid);
	  void RecvWatchPrivateMicFailed(int errorcode, int idx, int bidx);
	  void RecvRequestWatchedPrivateMic(int idx); /*idx=������, �Ƿ�ͬ���idx�ۿ����˽��*/
	  void RecvWatchedPrivateMicFailed(int errorcode, int idx, int bidx);

	  void RecvWatchedPrivateMicReject(int idx); //�Է��ܾ���idx=˭�ܾ���

	/*����˽��*/
	  void RecvInviteWatchPrivateMicSuccess();
	  void RecvInviteWatchPrivateMicFailed(int errorcode, int idx, int bidx);
	  void RecvRequestInvitedWatchPrivateMic(int idx); /*idx=������,�Ƿ�ͬ��ۿ�idx��˽��*/
	  void RecvInvitedWatchPrivateMicSuccess(int idx, short mictype, int posindex, int roomid);
	  void RecvInvitedWatchPrivateMicFailed(int errorcode, int idx, int bidx);

	  void RecvInvitedWatchPrivateMicReject(int idx);//�Է��ܾ���idx=˭�ܾ���

	/*������˽��*/
	  void RecvOpenRoomPrivateMicFlag(int ret, int flag);
	  void RecvOpenRoomPrivateMicFlagNotice(int idx, int flag); //�����Լ�

	/*�û���¼����ʱ���ص������˽����Ϣ*/
	  void RecvPrivateMicWithLoginRoom(int idx, short mictype, int posindex, int roomid);
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
	  void RecvCrownKickoutSomeOne(int ret, int kickuse, int totalkick, int idx);
	/*������ֹ�û�ip
	int ret;
	 0���ɹ�
	 -1����IP�ѱ�����
	 -2�����û����뿪����
	
	int idx;
	��������idx
	*/
	  void RecvForbidUserIp(int ret, int idx);

	//Ҫ���ϴ�����
	  void RecvUpdateSong();
	//��Ӧ�������ķ����û��質��Ϣ
	//  void RecvUserSingInfo(int ret,int idx,const ResultData_UserSingInfo pData);
	//��ȡ�û������������
	  void RecvGiftCount(int recvcount);

	//��ȡ�û���߷ָ�����Ϣ���ݳ���������
	  void RecvTopSongInfo(int songcount, int topsongscore, String topsongname);
	  
	  ///////////////////////////////////////////////////�ֻ������ص�////////////////////////////////////////////////////////////
	//�ֻ�ktv������Ϣ
	    /*
	     @param ret:����ֵ��0,OK;����ֵ,something error
	     @param fromIdx:����˭,�����������Ҳ�п���ָ���͵�IDX
	     @param roomName:˵�����ڵķ��䣬���δ�ڷ�����Ϊ��0��
	     @param content:˵������
	     @param chatTime:˵��ʱ��
	     @param isPublic:�Ƿ�Ϊ����
	     
	     @ע:chattime
	     */
	    void RecvKtvChatInfo(int ret,int fromIdx,int roomid,String roomName,String content,
	                                 int chatTime,boolean isPublic,boolean isSend);
	    //�ֻ����յ���ǰ�Ĳ����б�����ǰktv�ĵ���б����ڸ���
	    /*
	     @param pSonglist:��Ϣ�ṹ��
	     @param listcount:��������
	     */
	    void RecvKtvSongList(ResultData_SongList[] pSonglist,int listCount);
	    //�ֻ���ȡ��ǰ����Ŀ���״̬
	    /*
	     @param isOpen:true,���Ὺ����false������رգ��û��˳�����
	     */
	    void RecvKtvRoomState(boolean isOpen);
	    //�ֻ��յ������ϰ�֪ͨ,��ʾ�û���������Ϣ����ȥ����
	    void RecvKtvSongRanking();
	    /*
		֪ͨ�����ݳ�����
		*/
	    void RecvKtvSongNormalEnd(int nMusicId);
	    /*
	     �յ��������ֻ��û�IDX�б��������
	     ֻ��IDX
	     */
		void RecvKtvIdxList(int[] pIdx,int number);
		/*
		֪ͨ�û�ͷ�����
		@param [in] idx��ָ�����û�IDX
		@param [in] picture���û�����ͷ��
		*/
		void RecvUserPicChange(int idx,String picture);
		//֪ͨ�û������޸�
		void RecvUserNameChange(int idx,String name);
	    //��������ͼƬ�������֪ͨ
	    //����������ȥ��֮ǰû�л�ȡ��ͼƬ�ĵط�������ȥ��ȡһ��,�����Ѿ�����������
	    /*
	     @param pictureName[in]:ͼƬ����(��·��)
	     
	     @ע:����ص����̸߳�����Ļص��̶߳���һ����������HTTP�����̵߳Ļص�
	     */
	    
	    void RecvImageDownladFinished(String pictureName);
	    /*
	     ���̻߳���HTTP�̻߳ص����ڳ�ʼ�����ص�ַ֮��ɹ��Ļ��ͻ�ص���֪ͨ�������ݿ�������Ϣ�Ѿ��������
	     */
	    void RecvLocalGiftDatabaseFinished();
}