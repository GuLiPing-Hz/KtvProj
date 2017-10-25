#include "StdAfx.h"
#include "TestResponse.h"
#include "NetUtil/RequestBase.h"

extern RequestBase request;
CTestResponse::CTestResponse(void)
{
}


CTestResponse::~CTestResponse(void)
{
}

/*			add by lixin 2011/10/13 <begin> ���졢���ȹ���			*/
void CTestResponse::SendChatFromUser(int ret, int to_idx, const char *content ,int contentlen, int time ){}
//����˽��
void CTestResponse::RecvChatFromUser(int ret, int from_idx, const char *content ,int contentlen, int time ){}
//���չ��ģ����ֲ��ڲ���
void CTestResponse::RecvChatFromClub(int ret, int from_idx, const char* content,int contentlen){}
//���չ��ģ������ڣ�
//ret:1--���� 0--������ -1--��������
void CTestResponse::RecvChatFromRoom(int ret, int errcode,int from_idx, const char* content,int contentlen, int time ){}	
//���մ����ȣ�ȫ����
void CTestResponse::RecvSpeakerFromAll(int ret, int type, int from_id, const char* msg, int msglen){}
//����С���ȣ�������
void CTestResponse::RecvSpeakerFromLobby(int ret, int from_idx, int room_id,const char* room_name,const char* msg, int msglen, int time,int type,bool isSend){}
//��������㲥
void CTestResponse::RecvPresentBroad(int ret, ResultData_GiftInfo* guestGiftInfo, ResultData_GiftInfo* commGiftInfo){}
//����ϵͳ���ֹ㲥
void CTestResponse::RecvSystemBroad( char type, const char * msg, int len ){}
//����ϵͳ���ֹ㲥�������н���Ϣ��
void CTestResponse::RecvSystemBroad2(const char* nickname, int len, int idx, int roomid, int giftid, int luckmultiple){}
//����ϵͳ���ֹ���
void CTestResponse::RecvSystemNotice( const char * msg, int len, bool once, int min ){}
//����ϵͳ���ֹ��棨�����н���Ϣ��
void CTestResponse::RecvSystemNotice2(const char* nickname, int len, int idx, int roomid, int giftid, int luckmultiple){}
//���շ�����ʱ����
void CTestResponse::RecvRoomNotice(int ret, const char* content,int contentlen){}
/************************************************************************/
/*			add by jinguanfu 2009/12/28 <begin> ���Ѳ���				*/
//ȡ�ú���λ��
void CTestResponse::RecvFriendLocation(int ret, int errcode,
							int friend_idx,
							int lobbyid, 
							int roomid,
							int cseq){}
/*			add by jinguanfu 2009/12/28 <begin> �������ӿ�			*/
//����ȡ�ô���˷����Ա�б�
void CTestResponse::RecvRoomApplyList (int ret ,int errcode,ResultDate_RoomApplyList *applylist,int cseq){}
//�������뷿���Ա��Ӧ
void CTestResponse::RecvApplyRoomMember(int ret,int errcode,int cseq){}
//������˻�Ա����������
void CTestResponse::RecvAuditAddApply (int ret, int errcode,int idx,int cseq){}
//���վܾ���������ϵͳ��ʾ
void CTestResponse::RecvRoomApplyRefuseMsg (int ret, int roomid){}
//���շ����Ա�б�
void CTestResponse::RecvRoomMemberList (int ret ,int errcode, ResultDate_RoomMemList* memlist,int cseq){}
//���ս�������Ա�ʸ��Ӧ
void CTestResponse::RecvUnchainMember (int ret, int errcode, int idx,int cseq){}
//���ո����û�������������Ȩ�޻�Ӧ
void CTestResponse::RecvRoomPesideAssistant (int ret , int errcode, int idx,int cseq){}
//���ո�����������֪ͨ
void CTestResponse::RecvRoomNotice4PesideAssistant (int ret , int opidx,int bopidx){}
//���ո����û�����������Ȩ�޻�Ӧ
void CTestResponse::RecvRoomPeside (int ret ,int errcode, int idx,int cseq){}
//��������������Ȩ�޷���֪ͨ
void CTestResponse::RecvRoomNotice4Peside (int ret , int opidx,int bopidx){}
//���ո����û����丱����Ȩ�޻�Ӧ
void CTestResponse::RecvRoomSubHost (int ret ,int errcode, int idx,int cseq){}
//�������ø�����Ȩ�޷���֪ͨ
void CTestResponse::RecvRoomNotice4SubHost(int ret , int opidx,int bopidx){}
//���շ��������
void CTestResponse::RecvBlackList(int ret,	int errcode,ResultDate_RoomBlackList* blacklist,int cseq){}
//���պ������û����/ɾ����Ӧ
void CTestResponse::RecvAddDelBlack(int ret , int errcode,int idx, int type){}
//���շ������������Ϣ
void CTestResponse::RecvRoomPanelInfo(int ret ,int errcode, ResultDate_RoomPanel* panelinfo){}
//�������÷��������Ӧ
void CTestResponse::RecvRoomPassword(int ret,int errcode,int cseq){}
//�������÷���������Ϣ��ʾ
void CTestResponse::RecvRoomNotice4Password(int ret, int idx, char pwdflag){}
//�������÷��俪��/�ر�(����)��Ӧ
void CTestResponse::RecvRoomSwitch(int ret,int errcode,char roomflag,int cseq){}
//���շ��俪��/�ر�(����)��Ϣ��ʾ
void CTestResponse::RecvRoomNotice4RoomSwitch(int ret , int idx , char roomflag){}
//�������÷������Ȩ�޻�Ӧ
void CTestResponse::RecvRoomPrivate(int ret , int errcode,char accessflag,int cseq){}
//���շ������Ȩ����Ϣ��ʾ
void CTestResponse::RecvRoomNotice4Private(int ret , int idx , char accessflag){}
//���������û�������Ϣ����/�رջ�Ӧ
void CTestResponse::RecvUserInOutSwitch(int ret , int errcode, char inoutflag,int cseq){}
//�����û�������Ϣ����/�ر���Ϣ��ʾ
void CTestResponse::RecvRoomNotice4UserSwitch (int ret , int idx , char inoutflag){}
//������������������/�رջ�Ӧ
void CTestResponse::RecvGetMicSwitch (int ret, int errcode,char onmicflag,int cseq){}
//������������������/�رջ�Ӧ
void CTestResponse::RecvOnMicSwitch (int ret, int errcode,char onmicflag,int cseq){}
//��������������/�ر���Ϣ��ʾ
void CTestResponse::RecvRoomNotice4GetMicSwitch(int ret , int idx , char getmicflag){}
//��������������/�ر���Ϣ��ʾ
void CTestResponse::RecvRoomNotice4OnMicSwitch(int ret , int idx , char onmicflag){}
//���������޸Ļ�Ӧ
void CTestResponse::RecvModifyRoomName(int ret,int errcode,int cseq){}
//���շ�����
void CTestResponse::RecvRoomName (int ret , int roomid , const char* roomname , int rnlen){}
//���÷��乫����Ӧ
void CTestResponse::RecvRoomAffiche(int ret,int errcode,int cseq){}
//���շ��乫������¼�
void CTestResponse::RecvRoomAfficheEvent(int ret){}
//���������乫����Ӧ
void CTestResponse::RecvAllowRoomAllChat (int ret, int errcode,char chatflag,int cseq){}
//���շ��乫��������Ϣ��ʾ
void CTestResponse::RecvRoomNotice4AllChat(int ret , int idx , char chatflag){}
//���÷��件ӭ����Ӧ
void CTestResponse::RecvRoomSalutatory(int ret, int errcode,int cseq){}
//���շ��件ӭ�ʸ���֪ͨ
void CTestResponse::RecvRoomNotice4Salutatory(int ret,int idx){}
//���÷���logo��Ӧ
void CTestResponse::RecvRoomLogo (int ret,int errcode ,int cseq){}
//���շ���LOGO����֪ͨ
void CTestResponse::RecvRoomNotice4Logo(int ret,int idx){}
//���շ��䶳����Ϣ��ʾ
void CTestResponse::RecvRoomNotice4Lock(int idx){}
//���ܴ��������Ӧ
void CTestResponse::RecvCreateRoom( int ret, int errcode, RoomListEx * info ){}
//�������跿��ȫ��Ϣ
void CTestResponse::RecvTmpRoomInfo( int ret, int errcode, RoomListEx * info ){}
// ��Ӹ跿�㲥
void CTestResponse::RecvRoomNotice4Add( RoomListEx * info ){}
void CTestResponse::RecvRoomNotice4Add( int idx ){}
// ɾ���跿�㲥
void CTestResponse::RecvRoomNotice4Del( int idx ){}
// ���շ�������֪ͨ
void CTestResponse::RecvRoomNotice4ChangeMaster(int idx){}
// ���շ�����Ϣ����֪ͨ
void CTestResponse::RecvRoomNotice4ChangeInfo( int idx ){}
/*			add by jinguanfu 2009/12/28 <begin> ��ɫ��Ϣ				*/
//ȡ�����˽�ɫ��Ϣ
void CTestResponse::RecvPartInfo(int ret, int errcode, ResultData_PartInfo * part_info, int cseq){
	//m_pDataMgr->savePartInfo(part_info);
}
//ȡ���Լ���ɫ��Ϣ,
void CTestResponse::RecvPartInfoBySelf(int ret, int errcode, ResultData_PartInfo * part_info, int cseq){
	
}
/*			add by jinguanfu 2010/1/5 <begin> ȡ�÷����б�				*/
void CTestResponse::RecvRoomList(int ret,ResultDate_RoomList* roomlist){}
void CTestResponse::RecvRoomListEx(int ret,ResultDate_RoomListEx * roomlist){}
//���֪ͨ
//void CTestResponse::RecvNotice4Score(int onmic_idx,int onmic_score,int pk_idx,int pk_score){}
//void CTestResponse::RecvNotice4ScoreAdd(int epchange, int silverchange){}
//����֪ͨ
void CTestResponse::RecvNotice4OffMic( int onmic_idx, int pk_idx, int exp, const char * scoreinfo, int len ){}
// add by cx 10-6-2
//��Ӧ�Ƿ���������ɹ�
void CTestResponse::RecvRoomGivOnevjMic( int idx, char flag ){}
//�Ƿ�ͬ����������ɹ�
void CTestResponse::RecvRoomGivOnevjMicApply( int formidx ){}
//add by jinguanfu 2010/1/20 <begin> 
//�Ƿ�ͬ����ӶԷ�Ϊ����
//����˵����int idx ��Ӻ�����idx
void CTestResponse::RecvAddFriendApply(int idx){}
//add by jinguanfu 2010/1/20 <end> 
//void CTestResponse::RecvDeleteAvarta(int ret){}
//add by jinguanfu 2010/3/4 ԭ�лص�API�ĳɽӿ�<begin>
//���ӳɹ�
//�ɹ�������֤������
void CTestResponse::OnAuthTunnelConnectSuccess(){
	printf("CTestResponse::OnAuthTunnelConnectSuccess\n");
}
//�ɹ����Ӵ���������
void CTestResponse::OnLobbyTunnelConnectSuccess(){
	printf("CTestResponse::OnLobbyTunnelConnectSuccess\n");
}	
//�ɹ����ӷ��������
void CTestResponse::OnRoomTunnelConnectSuccess(){
}	
//�ɹ�����Ƶ��������
void CTestResponse::OnChannellistTunnelConnectSuccess(){
}	
//���ӳ�ʱ
//������֤��������ʱ
void CTestResponse::OnAuthTunnelConnectTimeout(){
	printf("CTestResponse::OnAuthTunnelConnectTimeout\n");
}		
//���Ӵ�����������ʱ
void CTestResponse::OnLobbyTunnelConnectTimeout(){
}		
//���ӷ����������ʱ
void CTestResponse::OnRoomTunnelConnectTimeout(){
}	
//����Ƶ����������ʱ
void CTestResponse::OnChannellistTunnelConnectTimeout(){
}
//���Ӵ���
void CTestResponse::OnAuthTunnelConnectError( int code ){
	printf("CTestResponse::OnAuthTunnelConnectError : %d\n",code);
}	
void CTestResponse::OnLobbyTunnelConnectError( int code ){
}
void CTestResponse::OnRoomTunnelConnectError( int code ){
}
void CTestResponse::OnChannellistTunnelConnectError( int code ){
}
//�����������Ͽ�������,�ͻ���recv == 0��ʱ��,�ص������µĽӿ�
//�Ͽ���֤������
void CTestResponse::OnAuthTunnelClose(){
	printf("CTestResponse::OnAuthTunnelClose\n");
}	
//�Ͽ�����������
void CTestResponse::OnLobbyTunnelClose(){
}	
//�Ͽ����������
void CTestResponse::OnRoomTunnelClose(){
}	
//�Ͽ�Ƶ��������
void CTestResponse::OnChannellistTunnelClose(){
}
//�ͻ���recv�쳣,send�쳣,�����buf���,select��������,����ص���������½ӿ�
void CTestResponse::OnAuthTunnelError( int code ){
	printf("CTestResponse::OnAuthTunnelError %d\n",code);
}	
void CTestResponse::OnLobbyTunnelError( int code ){

}
void CTestResponse::OnRoomTunnelError( int code ){

}
void CTestResponse::OnChannellistTunnelError( int code ){

}

/******************��֤�����������ӿ�*****************************/
//������֤���
void CTestResponse::RecvAuth(int ret, int errcode, const char * token, int len){

}
//������token
//void CTestResponse::RecvNewToken(int ret,ReserveDataS* pRD){}
void CTestResponse::RecvNewToken(int ret,int errcode){}
//����Ƶ���б�
void CTestResponse::RecvSectionList(int ret,int errcode,ResultData_GetChannellist* pRDc,int cseq){
	
}
//����Ƶ���б�(��ӷ�������Ϣ)
void CTestResponse::RecvSectionList2(int ret,int errcode,int leftlogintime,ResultData_GetChannellist* pRDc,int cseq){}
/******************���������������ӿ�*****************************/
//���մ���token��֤
void CTestResponse::RecvTokenAuth4Lobby( int ret, int errcode, ResultData_LobbyLogin * pRDl, ResultData_SelfData * pSD, int cseq ){
	
}
//�û���¼����֪ͨ
void CTestResponse::RecvUserLoginLobbyNotice(int ret,ResultData_LobbyLoginUser* pRlu){}
//�û��˳�����֪ͨ
void CTestResponse::RecvUserLogoutLobbyNotice(int ret,ResultData_LobbyLoginUser* pRlu){}
//�����û���Ϣ
void CTestResponse::RecvUserInfo(int ret,int errcode,ResultData_UserFullInfo* pRDu){}
//���պ���״̬
void CTestResponse::RecvFriendStatus(int ret,int errcode,ResultData_GetFriendlist* pRD){}
//���պ�����״̬
void CTestResponse::RecvBlackStatus(int ret,int errcode,ResultData_GetFriendlist* pRD){}
//���վ��ֲ�״̬
void CTestResponse::RecvClubStatus(int ret,int errcode,ResultData_GetClublist* pRD){}
//������Ӻ��ѽ��
void CTestResponse::RecvAddFriend(int ret,int errcode,int idx){}
//������Ӻ��������
void CTestResponse::RecvAddBlack(int ret,int errcode,int cseq){}
//����ɾ�����ѽ��
void CTestResponse::RecvDelFriend(int ret,int errcode,int cseq){}
//����ɾ�����������
void CTestResponse::RecvDelBlack(int ret,int errcode,int cseq){}
//�������������ֲ����
void CTestResponse::RecvApplyClub(int ret,int errcode,int cseq){}
//�����˳����ֲ����
void CTestResponse::RecvExitClub(int ret,int errcode,int cseq){}
//������������ڵļ�����ֲ���������
void CTestResponse::RecvApplyListCount(int ret,int errcode,int count,int cseq){}
//������������ڵļ�����ֲ������б�
void CTestResponse::RecvApplyList(int ret,int errcode,ResultData_ClubApplyList* pRD,int cseq){}
//ͨ��������ֲ�����
void CTestResponse::RecvAddClubOk(int ret,ResultData_Approvelresult* pRD){}
//ɾ�������б�
void CTestResponse::RecvDelApplyList(int ret,ResultData_Approvelresult* pRD){}
//���ռ�����ֲ��㲥��Ϣ(�����˼�����ֲ�)
//modify by jinguanfu 2010/4/11
//void CTestResponse::RecvAddClubNotice(int ret ,int idx){}
//opidx--������idx
//addidx--������idx
void CTestResponse::RecvAddClubNoticeByLobby(int ret ,int clubid,const char* clubname,int opidx,int addidx){}
void CTestResponse::RecvAddClubNoticeByRoom( int opidx, int addidx ){}
//���������Ա�ܾ���Ϣ
//opidx--�����idx
void CTestResponse::RecvDenyClubNoticeByLobby(int ret ,int clubid,const char* clubname,int opidx){}
//void CTestResponse::RecvDenyClubNoticeByRoom(int ret ,int clubid,char* clubname,int opidx){}
//���ջ�Ա�˳����ֲ���Ϣ
void CTestResponse::RecvExitClubNoticeByLobby(int ret,int clubid,const char* clubname,int exitidx){}
void CTestResponse::RecvExitClubNoticeByRoom( int exitidx ){}
//���ջ�Ա���߳����ֲ���Ϣ
//opidx--������idx
//kickidx--���߳���idx
void CTestResponse::RecvKickClubNoticeByLobby(int ret,int clubid,const char* clubname,int opidx,int kickidx){}
void CTestResponse::RecvKickClubNoticeByRoom( int opidx, int kickidx ){}
//�������ֲ�
void CTestResponse::RecvCreateClub(int ret,int errcode,int clubid,int cseq){}
//��ɢ���ֲ�
void CTestResponse::RecvDismissClub(int ret,int errcode,int cseq){}
//���վ��ֲ���Ϣ
void CTestResponse::RecvClubInfo(int ret,int errcode,ResultData_ClubFullInfo* pRD,int cseq){}
////���շ����б�
//void CTestResponse::RecvRoomList(int ret,ResultData_RoomList* pRD){}
//���շ�����Ϣ
void CTestResponse::RecvRoomInfo(int ret,ResultData_RoomInfo* pRD){}
//����ͷ��
void CTestResponse::RecvCreateAvatar(int ret,int errcode){}
//�޸�ͷ��
void CTestResponse::RecvChangeAvatar(int ret,int errcode){}
//�޸��ǳ�
void CTestResponse::RecvChangeNickName(int ret,int errcode,const char * name, int len){}
//���뷿��֪ͨ
void CTestResponse::RecvLobbyEnterRoom(int ret,ResultData_LobbyRoomEnter* pRD){}
//�뿪����֪ͨ
void CTestResponse::RecvLobbyLeaveRoom(int ret,ResultData_LobbyRoomEnter* pRD){}
//��Ӻ���֪ͨ
void CTestResponse::RecvAddFriendNotice(int ret,int idx){}
//ȡ�ô�����Ϣ
void CTestResponse::RecvLobbyInfo(int ret,int errcode,ResultData_LobbyName* pRD,int cseq){}
//�������Դ�����Ȩ�޸�����Ϣ add by jinguanfu 2010/6/5
void CTestResponse::RecvChangePower(int optionidx/*������ID*/,char identity/*���ĺ�Ȩ��*/){}
//ȡ������(CMD_SYSTEMMSG_L2C)
//void CTestResponse::RecvLobbyName(){}
/******************��������ӿ�*****************************/
//���շ���token��֤ //
void CTestResponse::RecvTokenAuth4Room(int ret,int errcode){

}
//���뷿��
void CTestResponse::RecvEnterRoom(int ret,int errcode,ResultData_LoginInfo* pRDlf,int cseq){
	
}
//�û����뷿��֪ͨ
void CTestResponse::RecvEnterRoomNotice(int ret,ResultData_UserInfo* pRDu){}
//�û��뿪����֪ͨ
void CTestResponse::RecvLeaveRoomNotice(int ret,int idx){}
//���շ�����ϸ��Ϣ
void CTestResponse::RecvRoomAllInfo(int ret,int errcode,ResultData_RoomAllInfo *pRD,int cseq){}
//��������ɹ�֪ͨ
void CTestResponse::RecvWaitMicNotice(int ret,ResultData_MicInfo *pRD){}
////��������׼��֪ͨ
//void CTestResponse::RecvWaitMicReadyNotice(int ret,ResultData_MicInfo *pRD){}
//��������׼�����֪ͨ
//void CTestResponse::RecvOnMicReadyOkNotice(int ret,ResultData_MicInfo *pRD){}
//��������֪ͨ
void CTestResponse::RecvOnMicNotice(int ret,ResultData_MicInfo *pRD){}
//ȡ������ɹ�֪ͨ
void CTestResponse::RecvCancelWaitMicNotice(int ret,int opidx,int bopidx){}
//���ռ���PK��֪ͨ
void CTestResponse::RecvWaitPKMicNotice(int ret,ResultData_MicInfo *pRD){}
//����ȡ��PK��֪ͨ
void CTestResponse::RecvCancelWaitPKMicNotice(int ret,int opidx,int bopidx){}
//��������ɹ�֪ͨ
void CTestResponse::RecvUpWaitMicNotice(int ret,ResultData_UDMicInfo *pRD){}
//���ս���ɹ�֪ͨ
void CTestResponse::RecvDownWaitMicNotice(int ret,ResultData_UDMicInfo *pRD){}
//�����߳�����֪ͨ
void CTestResponse::RecvKickOutNotice(int idx,int bidx){}
//������VJ��������֪ͨ
void CTestResponse::RecvOnVJMicNotice(int ret,int idx){}
//������VJ��������֪ͨ
void CTestResponse::RecvOffVJMicNotice(int ret,int idx){}
//���ձ�����֪ͨ
void CTestResponse::RecvGiveOffMicNotice(int ret,int idx){}
//���ձ�����֪ͨ
void CTestResponse::RecvForbidenNotice(int ret,ResultData_ForbidenInfo* pRD){}
//���պ���������֪ͨ
void CTestResponse::RecvUpdateBlackListNotice(int ret,ResultData_UpBlackList* pRD){}
//���չ���Ա����֪ͨ
void CTestResponse::RecvUpdateMGRListNotice(int ret,ResultData_UpMgrList* pRD){}
//������������֪ͨ
void CTestResponse::RecvSendFlowerNotice(int ret,ResultData_SendFlowerInfo* pRD){
	
}
//add by jinguanfu 2010/3/4 ԭ�лص�API�ĳɽӿ�<end>
//���շ����ظ���¼��Ϣ
void CTestResponse::RecvRoomKickUser(){}
//���մ����ظ���¼��Ϣ
void CTestResponse::RecvLobbyKickUser(){}
//�Ӵ������շ��������Ϣ
void CTestResponse::RecvRoomSimpleInfo(int ret,int errcode,ResultData_RoomSimpleInfoList* pRD){}
void CTestResponse::RecvRoomSimpleInfoEx(int ret,int errcode,ResultData_RoomSimpleInfoList* pRD, int type){}
//�����û��˳������Ա��Ӧ
void CTestResponse::RecvExitRoomMember(int ret,int errcode){}
//����WEB���������ʼ���Ϣ
void CTestResponse::RecvMailInfo(int ret,char* context,int length){}
//���մ�����������
void CTestResponse::RecvLobbyOnlineNum(int ret,int errcode,ResultData_LobbyOnlineList* pRD){}
//���������Ӧ��Ϣ	//add by jinguanfu 2010/4/27
void CTestResponse::RecvSendFlower(int ret,int errcode,int recevie_idx,int item_id,int count,int gold,int silver){

}
//���շ��ʹ����Ȼ�Ӧ //add by jinguanfu 2010/5/13
void CTestResponse::RecvSpeakerToAll(int ret,int errcode,int gold,int silver){}
//���շ���С���Ȼ�Ӧ //add by jinguanfu 2010/5/13
void CTestResponse::RecvSpeakerToLobby(int ret,int errcode,int gold,int silver){

}
//�����ɹ���Ա��Ϊһ���Ա�Ļ�Ӧ //add by jinguanfu 2010/5/13
void CTestResponse::RecvRoomMember (int ret , int errcode, int idx,int cseq){}
//�����ɹ���Ա��Ϊһ���Ա��֪ͨ //add by jinguanfu 2010/5/13
void CTestResponse::RecvRoomNotice4Member (int ret , int opidx,int bopidx){}
//���շ��������˸ı���avarat�Ĺ㲥 //add by cx 2010-5-20
void CTestResponse::RecvRoomNotice4ChangeAvarat (int ret, int idx){}
//���շ����ڴ��������ʾ //add by cx 2010-6-6
void CTestResponse::RecvOneError( short cmd ){}
//add by cx 2010-7-6 ���ܹ�����Ʒ��Ӧ
void CTestResponse::RecvBuyPack( int ret, int errcode, int gold, int silver, short * goods_id, int * global_id, int count, const char * day ){}
// ���ܹ�����Ʒ��Ӧ
void CTestResponse::RecvBuyForSend( int ret, int errcode, int idx, int gold, int silver ){}
//���ܻ�ȡ������Ӧ
void CTestResponse::RecvGetPack( int ret, int errcode, ResultData_Pack * pPack, int count, int all_count ){}
//����ɾ����Ʒ��Ӧ
void CTestResponse::RecvDelPack( int ret, int errcode, int cseq ){}
//������Ʒ����֪ͨ
void CTestResponse::RecvTimeoutPack( int count, int * global_id ){}
//������Ʒ����
void CTestResponse::RecvSendPack( int idx, int global_id, short goods_id, const char * day, const char * msg ){}
//����ʹ����Ʒ��Ӧ
void CTestResponse::RecvUsePack( int ret, int errcode, int global_id ){}
// �����㲥
void CTestResponse::RecvNotice4LevelUp( int idx, char level ){}
// ��������
void CTestResponse::RecvExpChange( int nexp, int level ){}
// vip�ı�
void CTestResponse::RecvVipChange( int idx, int level, char is_vip ){}
// Ǯ�Ҹı�
void CTestResponse::UpdataMoney( int r_gold, int r_silver ){
	
}
// �����Լ�����Ϣ
void CTestResponse::RecvUpdataMyInfo(){}
// �����̻��㲥
void CTestResponse::RecvFireworks( int ret, int errcode, int cate_idx, int count, int gold, int silver ){}
// �����̻��㲥
void CTestResponse::RecvFireworksNotice( int player_idx, int cate_idx, int count ){}
// �н���
void CTestResponse::RecvLucky( int player_idx, int cate_idx, int lotte ){}
// �н���
void CTestResponse::RecvLucky( int room_idx, int player_idx, int cate_idx, int lotte ){}
// �б�
void CTestResponse::RecvGiftTopList( ResultData_GiftList * gift_list ){}
// ����ʱ��
void CTestResponse::RecvOnlineTime( int idx, int day_time, int online ){}
// ��ȡ��������
void CTestResponse::RecvGetNewUserTask( short step ){}
// �������������ύ���
void CTestResponse::RecvTaskStep( int ret, int err_code, int gold, int silver, int total_gold, int total_silver ){}
void CTestResponse::RecvTaskGift( int ret, int err_code, int item_id, int object_id, const char * time_buf ){}
// ��Ʒ��Ϣ����֪ͨ
void CTestResponse::RecvNotice4ChangeGoods(){}
// ����������ˢ�±���
void CTestResponse::RecvGetBagAgain(){}
// �����û�ӡ��
void CTestResponse::RecvUserSeel( int user_idx, int seel_idx ){}
// �����û�������״̬
void CTestResponse::EnterAllMentState( int idx, short state ){}
// room loge
void CTestResponse::RecvRoomLoge( int room_idx, const char * loge, int len ){}
// �·���汾��
void CTestResponse::RecvNewVersionId( int version_id ){
	
}
// ������Ϣ
void CTestResponse::RecvGroupInfo( int count, ResultData_GroupInfo * pInfo ){
	
}
// ��ȡ������ķ����б�
void CTestResponse::RecvRoomListInGroup( short group_idx, int count, int * list ){
	
}
void CTestResponse::RecvRoomListInGroup(short count, ResultData_GroupRoomInfo* pInfo)
{
	//��
}
// �ϴ���־֪ͨ
void CTestResponse::RecvNotice4CollectData( const char * server_ip, short server_port ){}
// ��ȡ������Ϣ
void CTestResponse::RecvSongInfoList( short totalnum, short curnum, ResultData_SongInfo * info ){}
// ��ȡ����Ƶ�������б�
void CTestResponse::RecvAvServerList( ResultData_AvServer * list, int count ){}
// ��������Ƶ�������б�
void CTestResponse::UpdataAvServerList(){}
// �������Ϸ������
void CTestResponse::RecvCheckOutGame( int result ){}
// ��ȡ������Ϣ
void CTestResponse::RecvOffLineMsg( int count, ResultData_OffLineMsg * msg ){}
// ���ط����ö���Ϣ
void CTestResponse::RecvRoomSortInGroup( int idx, int count, ResultData_RoomSort * list ){}
// ֪ͨ�����ö���Ϣ�ı�
void CTestResponse::RecvRoomSortChange( int idx ){}
// ���ֵȼ�
void CTestResponse::RecvSingerLevel(int idx, short curlevel, int expvalue){}
// �Ƹ��ȼ�
void CTestResponse::RecvWealthLevel(int idx, short curlevel, int expvalue){}

//////////////////////////////////////////////////////////////////////////

// ����ʧЧ��ӡ�£���Э��
void CTestResponse::RecvGiftInvalid( int idx, int GiftID ){}
// ������Ч��ӡ�£���Э��
void CTestResponse::RecvGiftValid( int idx, int GiftID ){}
// ��ʾ�������ȡ����
void CTestResponse::RecvShowRoomIncom( int income ){}
// ��ʾ��������ȡ��¼
void CTestResponse::RecvShowRoomGetIncom( ResultData_IncomeDate * data, int count ){}
// ������ȡ����������
void CTestResponse::RecvGetRoomIncom( int ret ){}
// ������Ϣ����֪ͨ
void CTestResponse::RecvNotice4ChangeFlower(){}
// Ȩ�����ø���֪ͨ
void CTestResponse::RecvNotice4ChangePower(){}
// ��������Ƶ�������㲥
void CTestResponse::RecvNotice4AvserverChange( ResultData_AvChange * info ){}
// ����gm
void CTestResponse::RecvChangeGM( int idx, char add ){}
// ������˵�pc�ſ�
void CTestResponse::RecvMicPcInfo( int idx, const char * buf, int len ){}
// ��Ӧ�Ƿ������뷿��ɹ�
void CTestResponse::RecvSendJoinRoomInvite( int wseq, int ret, int err_code ){}
// ��Ӧ�Ƿ������뷿��ɹ�
void CTestResponse::RecvInviteJoinRoom( int idx ){}
//ȡ�õ����������ͼ�¼
void CTestResponse::RecvGiftSend( int count, ResultData_GiftCount * pInfo ){}
//ȡ�õ���������ռ�¼
void CTestResponse::RecvGiftRecv( int count, ResultData_GiftCount * pInfo ){}
//���ط�mac���
void CTestResponse::RecvKickUserByMac( int ret, int idx ){}
//���ط�ip���
void CTestResponse::RecvKickUserByIp( int ret, int idx ){}
//������������ʱ����
void CTestResponse::RecvSetMicTime( int ret, int idx ){}
//�㲥����ʱ���ӳ�
void CTestResponse::RecvNotice4SetMicTime( int idx, int time ){}
//ȡ�����б�
void CTestResponse::RecvWaitMicList( short count, int* idxs ){}
//������ʱ�䵽,�Ͽ�����
void CTestResponse::RecvIndulgeTimeUp(){}
/*������Ϣ�ص�
������
flag��
1:ɾ������
2:��ӻ���¼���
familyNum��
��������(N)
familys��
������Ϣ����һ������familys[N-1]{}
ע�⣺��flagΪ1ʱ��familyNum�ض�Ϊ1��ͬʱfamilys[0]��ֻ��familyid�ǿ��õ�(��Ϊɾ����ʱ��һ��ֻ��ɾ��һ����ֻ�м���id��Ϣ)
*/
void CTestResponse::RecvFamilyInfo(int flag, int familyNum, ResultData_Family* familys){}

/*
av������Ϣ
*/
void CTestResponse::RecvAVConfigInfo(int infolen, const char* info){}

/* ˽��                                                                 */
/************************************************************************/
/*��˽��*/
void CTestResponse::RecvOpenPrivateMicSuccess(int idx, short mictype, int posindex, int roomid){}
void CTestResponse::RecvOpenPrivateMicFailed(int errorcode, int idx, int bidx){}
void CTestResponse::RecvOpenPrivateMicNotice(int idx, int mictype){} /*�������Լ�*/

/*��˽��*/
void CTestResponse::RecvClosePrivateMicFailed(int errorcode, int idx, int bidx){}
void CTestResponse::RecvClosePriveteMicNotice(int idx){} /*�����Լ�*/

/*������˽��*/
void CTestResponse::RecvClosedPrivateMicFailed(int errorcode, int idx, int bidx){}
void CTestResponse::RecvClosedPriveteMicNotice(int idx, int bidx){} /*�����Լ�*/

/*�ۿ�˽��*/
void CTestResponse::RecvWatchPrivateMicSuccess(int idx, short mictype, int posindex, int roomid){}
void CTestResponse::RecvWatchPrivateMicFailed(int errorcode, int idx, int bidx){}
void CTestResponse::RecvRequestWatchedPrivateMic(int idx){} /*idx=������, �Ƿ�ͬ���idx�ۿ����˽��*/
void CTestResponse::RecvWatchedPrivateMicFailed(int errorcode, int idx, int bidx){}

void CTestResponse::RecvWatchedPrivateMicReject(int idx){} //�Է��ܾ���idx=˭�ܾ���

/*����˽��*/
void CTestResponse::RecvInviteWatchPrivateMicSuccess(){}
void CTestResponse::RecvInviteWatchPrivateMicFailed(int errorcode, int idx, int bidx){}
void CTestResponse::RecvRequestInvitedWatchPrivateMic(int idx){} /*idx=������,�Ƿ�ͬ��ۿ�idx��˽��*/
void CTestResponse::RecvInvitedWatchPrivateMicSuccess(int idx, short mictype, int posindex, int roomid){}
void CTestResponse::RecvInvitedWatchPrivateMicFailed(int errorcode, int idx, int bidx){}

void CTestResponse::RecvInvitedWatchPrivateMicReject(int idx){}//�Է��ܾ���idx=˭�ܾ���

/*������˽��*/
void CTestResponse::RecvOpenRoomPrivateMicFlag(int ret, char flag){}
void CTestResponse::RecvOpenRoomPrivateMicFlagNotice(int idx, char flag){} /*�����Լ�*/

/*�û���¼����ʱ���ص������˽����Ϣ*/
void CTestResponse::RecvPrivateMicWithLoginRoom(int idx, short mictype, int posindex, int roomid){}
/*	���ػʹ����˽��
	int ret
	 0: �ɹ� 
	-1: ʧ�ܣ��Ƿ�����
	-2: ʧ�ܣ�ʣ���˴�������
	int kickuse
	���β��������˴�
	int totalkick
	ʣ�������˴���
	int idx{}	
	������idx
*/
void CTestResponse::RecvCrownKickoutSomeOne(int ret, int kickuse, int totalkick, int idx){}
/*������ֹ�û�ip
int ret{}
 0���ɹ�
 -1����IP�ѱ�����
 -2�����û����뿪����

int idx{}
��������idx
*/
void CTestResponse::RecvForbidUserIp(int ret, int idx){}

//Ҫ���ϴ�����
void CTestResponse::RecvUpdateSong(){}
//��Ӧ�������ķ����û��質��Ϣ
//void CTestResponse::RecvUserSingInfo(int ret,int idx,const ResultData_UserSingInfo* pData){}
//��ȡ�û������������
void CTestResponse::RecvGiftCount(int recvcount){}

//��ȡ�û���߷ָ�����Ϣ���ݳ���������
void CTestResponse::RecvTopSongInfo(int songcount, int topsongscore, const char* topsongname, int len){}

//http������ɵķ��ؽӿ�
// void CTestResponse::OnFinish(_TaskDownload* downloadInfo,bool success)
// {
// 	//�ص���java
// 	CTestResponse* pJniResponse = getSingleton();
// 	//����URL,���ҷ���saveBuf��������ݣ����downloadΪture�����ǿ��ַ���
// }
//�ֻ�ktv������Ϣ
void CTestResponse::RecvKtvChatInfo(int ret,const int fromIdx,int roomid,const char* roomName,const char* content
	,int chatTime,bool isPublic,bool isSend){
	
}
//�ֻ����յ���ǰ�Ĳ����б�����ǰktv�ĵ���б����ڸ���
/*
 @param pSonglist:��Ϣ�ṹ��
 @param listcount:��������
 */
void CTestResponse::RecvKtvSongList(const ResultData_SongList* pSonglist,int listCount){
	
}
//�ֻ���ȡ��ǰ����Ŀ���״̬
/*
 @param isOpen:true,���Ὺ����false������رգ��û��˳�����
 */
void CTestResponse::RecvKtvRoomState(bool isOpen)
{

}
//�ֻ��յ������ϰ�֪ͨ,��ʾ�û���������Ϣ����ȥ����
void CTestResponse::RecvKtvSongRanking(){
}
/*
	�յ��������ֻ��û�IDX�б��������
	ֻ��IDX
*/
void CTestResponse::RecvKtvIdxList(int* pIdx,int number)
{

}
/*
֪ͨ�û�ͷ�����
@param [in] idx��ָ�����û�IDX
@param [in] picture���û�����ͷ��
*/
void CTestResponse::RecvUserPicChange(int idx,const char* picture)
{

}
//��������ͼƬ�������֪ͨ
void CTestResponse::RecvImageDownladFinished(const char* pictureName){
	printf("%s is downloaded!\n",pictureName?pictureName:"");
}

void CTestResponse::RecvLocalGiftDatabaseFinished()
{

}

