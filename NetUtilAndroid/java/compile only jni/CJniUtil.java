/*
 * 
 *   by glp
 *   2014-4-9   
 *   ��װjava native interface��
 *   �Լ�ʵ��JniUtil�ķ���.
 * 
 */

package com.jni.netutil;


public class CJniUtil implements JniNetUtil
{
	//ResultDate_Channel tmp;
	//���ڹ���DLL�ļ���������Ͱ汾��Eclipse����֧��д������������C++��
	//����v22.6.2-1085508��Ҫд��Launcher Activity �У��������д����Activity����,
	//���ܺܺõĵ��ԣ�����F6����û�õ�ʱ��������Ӹ��ϵ㣬�����������Ȼ���Լ�������
//	static {
//		System.loadLibrary("stlport_shared");
//		System.loadLibrary("JniNetUtil");
//	}
//	public ClassLoader getClassLoader(){
//		return this.getClassLoader();
//	}
	//��ʼ��
	/*
	 * @int return �� 
	 * 0 ���ɹ�
	 * -1��ʧ��
	 */
	//�����ͻ���
    public native int StartClient();
    //ֹͣ�ͻ���
    public native void StopClient();
    //ȡ��Token
    public native String GetToken();
    //ȡ��Token����
    public native int GetTokenlen();

    //��֤�������Ƿ�����
    public native boolean IsConnectAuth();
    //�����Ƿ�����
    public native boolean IsConnectLobby();
    //�����Ƿ�����
    public native boolean IsConnectRoom();
    //�����Ƿ�token
    public native boolean IsTokenLoginLobby();
    //�����Ƿ�token
    public native boolean IsTokenLoginRoom();

    public native int ConnectAuth(String host,short port,int timeout);
    public native int DisConnectAuth();
    public native int ConnectLobby(String host,short port,int timeout);
    public native int DisConnectLobby();
    public native int ConnectRoom(String host,short port,int type,int timeout);
    public native int DisConnectRoom();

    public native int IdPassAuth(String id,String passwd,int again);
    public native int TouristAuth(int again);
    //Room API
    //��֤token
    public native int ConfirmToken();
    //���뷿��
    public native int EnterRoom(int cseq,int roomid, String passwd, short type);
    //ȡ�÷��䵱ǰ������Ϣ
    public native int GetRoomAllInfomation(int cseq);
    //ȡ�����б�
    public native int GetWaitMicList();
    //����
    public native int RoomWaitMic( int musicid, short bk, char level, String  name, char speed );
    //ȡ������
    public native int RoomCancleWaitMic( int idx );
    //��������
    public native int RoomUpWaitMic(char tag, int idx, char flag);
    //flag==>MIC_UP_DOWN_FLAG
    //�½�����
    public native int RoomDownWaitMic(char tag, int idx, char flag);
    //flag==>MIC_UP_DOWN_FLAG
    //����׼��ok
    //int RoomOnMicReadyOK( short music_len );
    //׼������
    //int RoomOffMicReady();
    //��ָ���û��������
    public native int RoomKickSomeOne(int idx);
    //����ĳ�ˣ���ȡ�����ԣ�
    public native int RoomForbidenSomeOne(int idx, char flag);
    //����������
    public native int RoomOnvjMic();
    //����������
    public native int RoomOffvjMic();
    //�����������ĳ��
    public native int RoomGivOnevjMic(int idx);
    // �Ƿ�ͬ������������ add by cx 10-6-2
    public native int ApplyGetVJmic( int idx, char flag );
    //������������
    public native int RoomGiveOffvjMic(int idx);
    //����ͨ�������û�����
    public native int RoomGiveOffMic(int idx);
    //flag==>ADD_SUB_FLAG
    //���·���������б�(������Ӻ�ɾ��)
    public native int RoomUpdateBlackList(int cseq,int idx, char flag);
    //flag==>ADD_SUB_FLAG
    //���¹���Ա�б�(������Ӻ�ɾ��)
    public native int RoomUpdateMgrList(int idx, char flag, char identity);

    //��¼����
    /*
     @param mac:һ��Ϊ�豸��MAC��ַ����������IOS��ƻ����ֹ��ȡMAC��ַ��������IOS�豸��ʹ�õ���UIDevice Unicode���ַ���
     @param loginway:��¼�ķ�ʽ��0��PC�ˣ�1��ANDROID��2��IOS
     
     @return:1���ɹ���0��ʧ�ܡ�
     */
    public native int LoginLobby( int lobbyId,String  mac,int loginway);
    //��¼����(����)
    public native int LoginLobbyAuto( String  mac, int roomid );
    //��ȡ�û����� ����һ�û�idx�б�,����������
    public native int GetUserInfo_AII(int idxl[],int count);
    //��ȡ�����б�
    public native int GetFriendListWithoutUserInfo();
    //��ȡ�������б�
    public native int GetBlackListWithoutUserInfo();
    //��Ӻ���
    public native int AddFriend(int idx);
    //��Ӻ�����
    public native int AddBlack(int cseq,int idx);
    //ɾ������
    public native int DelFriend(int cseq,int idx);
    //ɾ��������
    public native int DelBlack(int cseq,int idx);
    //��ȡͨ���б�
    public native int GetChannellist( int version, String  mac );
    //*********************************room manage api*********************************
    //��ȡ��ǰ�����ķ����б�
    public native int GetRoomList(int cseq);
    public native int GetRoomListEx(int cseq, int type);
    //��ȡ������Ϣ,������һ�鷿��
    public native int GetRoomInfo(int cseq,int roomcount, int[] roomlist);
    public native int GetTmpRoomInfo( int roomId );
    //*********************************************************************************
    //�û�����ͷ����Ϣ 
    @Deprecated
    public native int CreateAvatar( String nickname, ResultData_Avatar_Updata avarat );
    //avatarname��������20,partlen,decoration��С����Ϊ10
    @Deprecated
    public native int ChangeAvatar( ResultData_Avatar_Updata avarat );
    //�޸��ǳ�
    public native int ChangeNickName( int idx, String  name, int len );
    //�������ͻ�
    public native int SendFlower(int s_idx,int r_idx,int cate_id,int count);
    //���ʹ�����
    public native int BroadAll(String content);
    //����С����
    public native int BroadLobby(int lobbyidx,String content);
    //���ͷ�����ʱ����
    public native int SendRoomNotice(String content);
    //ȡ��ָ��������Ϣ
    public native int GetLobbyInfo(int cseq,int lobbyid);
    //�����û��������������
    public native int InviteMic(int useridx);
    /*			add by jinguanfu 2010/3/2 <end> �ص�API����ɽӿ�			*/
    /*			add by jinguanfu 2009/12/18 <begin> ���졢���ȹ���			*/
    //�����û�˽��
    public native int SendChatToUser(int to_idx, String content);
    //���͹��ģ����ֲ��ڲ���
    public native int SendChatToClub(String content);
    //���͹��ģ������ڣ�
    public native int SendChatToRoom(String content);
    //���ʹ����ȣ�ȫ����
    public native int SendSpeakerToAll(String msg);
    //����С���ȣ�������
    public native int SendSpeakerToLobby(int lobbyid, String msg);
    /*			add by jinguanfu 2009/12/28 <begin> ���Ѳ���				*/
    //��ѯ����λ��
    public native int QueryFriendLocation(int cseq,int friend_idx);
    /*			add by jinguanfu 2009/12/28 <begin> �������ӿ�			*/
    //����ȡ�ô���˷����Ա�б�
    public native int GetRoomApplyList (int cseq);
    //�û����뷿���Ա
    public native int ApplyRoomMember(int cseq);
    //��˻�Ա��������
    public native int AuditAddApply (int cseq,int idx,char type);
    //ȡ�����Ա�б�
    public native int GetRoomMemberList (int cseq);
    //��������Ա�ʸ�
    public native int UnchainMember (int cseq,int idx);
    //�����û�������������Ȩ��
    public native int SetRoomPesideAssistant(int cseq,int idx);
    //�����û�����������Ȩ��
    public native int SetRoomPeside (int cseq, int idx);
    //�����û����丱����Ȩ��
    public native int SetRoomSubHost(int cseq, int idx);
    //����Ϊ�����Ա���ɹ���Ա��Ϊ��Ա��	//add by jinguanfu 2010/5/13
    public native int SetRoomMember(int cseq, int idx);
    //ȡ�÷��������
    public native int GetBlackList (int cseq);
    //�������û����/ɾ��
    public native int AddDelBlack (int idx ,char type);
    //�鿴�����������
    public native int GetRoomPanelInfo (int cseq);
    //���÷�������
    public native int SetRoomPassword(int cseq,String password);
    //���÷��俪��/�ر�(����)
    public native int SetRoomSwitch(int cseq,char roomflag);
    //���÷�����Ի�Ա����
    public native int SetRoomPrivate(int cseq,char accessflag);
    //�û�������Ϣ����/�ر�
    public native int SetUserInOutSwitch(int cseq,char inoutflag);
    //��������������/�ر�
    public native int SetGetMicSwitch(int cseq,char getmicflag);
    //��������������/�ر�
    public native int SetOnMicSwitch(int cseq,char onmicflag);
    //���������޸�
    public native int ModifyRoomName(int cseq,String roomname);
    //���÷��乫��
    public native int SetRoomAffiche (int cseq,String content);
    //���������乫��
    public native int AllowRoomAllChat (int cseq , char chatflag);
    //���÷��件ӭ��
    public native int SetRoomSalutatory (int cseq,String content);
    //���÷���logo
    public native int SetRoomLogo(int cseq, String path);
    /*			add by jinguanfu 2009/12/28 <begin> ��ɫ��Ϣ				*/
    //����ȡ�ý�ɫ��Ϣ����
    public native int GetPartInfo(int cseq,int idx);
    //ȡ��ָ�������ķ����б�
    public native int GetRoomList_II(int cseq,int lobbyid);
    /************************************************************************/
    /*			add by jinguanfu 2010/1/14 <begin> 							*/
    //�뿪���䣨�л�����ʱʹ�ã�
    public native int LeaveRoom();
    /*			add by jinguanfu 2010/1/20 <begin> 							*/
    //�Ƿ�ͬ����Ӻ���
    //����˵����int		idx		������Ӻ��ѵ��û�
    //			char	flag	0--ͬ�� 1--��ͬ��
    public native int ApplyAddFriend(int idx, char flag);
    /*add by jinguanfu 2010/1/27 ɾ����ɫ��Ϣ ********/
    /*����˵����int		idx		�û�idx	*/
    //int DeleteAvarta(int idx);
    //���ûص������
    //public native void SetResponseHandle(ResponseBase* pRb);

    //add by jinguanfu 2010/4/8 ȡ�ü򵥷�����Ϣ
    public native int GetRoomSimpleInfo();
    public native int GetRoomSimpleInfoEx( int type );
    //add by jinguanfu 2010/4/11 �û��˳������Ա
    public native int ExitRoomMember();
    //add by jinguanfu 2010/4/15 ȡ�ô�����������
    public native int GetLobbyOnlineNum();
    //add by cx 10-7-6 ����
    @Deprecated
    public native int BuyPack( ResultData_Buy [] pPack, int count, int type, int idx );
    @Deprecated
    public native int GetPack();
    @Deprecated
    public native int DelPack( int cseq, int [] idx, int count );
    @Deprecated
    public native int UsePack( int idx );
    @Deprecated
    public native int CreateRoom( ResultData_CreateRoom room );
    //�̻�
    public native int SendFireworks( int cate_idx, int count );

    //////////////////////////////////////////////////////////////////////////

    //ȡ�ô����������а���Ϣ
    public native int GetGiftTopList();
    //ȡ������ʱ��
    public native int GetOnlineTime( int idx );
    //ȡ����������
    @Deprecated
    public native int GetNewUserTask();
    //�ύ��������
    @Deprecated
    public native int TaskStep( short step );
    @Deprecated
    public native int TaskGift( short step );
    //��ȡ��������㲥
    public native int GetGiftInfo();
    //��ȡϵͳ���ֹ���
    public native int GetSystemBroad();
    //��ȡӡ����Ϣ
    public native int GetUserSeel( int [] idx );
    //�����û�pc����
    public native int SendSysteInfo( String  data );
    //��ȡ������Ϣ
    public native int GetGroupInfo();
    //��ȡ������ķ����б�
    public native int GetRoomListInGroup( int group_idx );
    public native int GetRoomListInGroupBatch(short[] groupId);
    //��ȡ�����б�
    public native int GetSongInfoList();
    //��ȡ����Ƶ�������б�
    public native int GetAvServerList( short net_type );
    //��ȡ������Ϣ
    public native int GetOffLineMsg();
    //��ȡ�����ö���Ϣ
    public native int GetRoomSortInGroup( short group_id );
    //��ȡС������Ϣ
    public native int GetUserSpeakerInfo();
    // ��ȡ������Ϣ
    public native int GetFamilyInfo();

    //////////////////////////////////////////////////////////////////////////

    //�ͻ��˷�����
    //String scoreinfo -- �����Ϣ�ṹ��ָ��
    //int length--�����Ϣ�ṹ�峤��
    public native int RoomScore(String scoreinfo);
    //��ʾ�������ȡ����
    @Deprecated
    public native int ShowRoomIncome();
    //��ʾ��������ȡ��¼
    @Deprecated
    public native int ShowRoomGetIncome();
    //������ȡ����������
    @Deprecated
    public native int GetRoomIncome();
    //�����û�pc�ſ�
    public native int SendPcData( String  data );
    //�����û����뷿���Ա
    public native int InviteJoinRoom( int idx, int wseq );
    //�����û����뷿���Ա
    public native int ReInviteJoinRoom( char type );
    //ȡ�õ����������ͼ�¼
    public native int GetGiftSend();
    //ȡ�õ���������ռ�¼
    public native int GetGiftRecv();
    //��mac
    public native int KickUserByMac( int idx );
    //��ip
    public native int KickUserByIp( int idx );
    //��������ʣ��ʱ��
    public native int SetOnMicTemainTime( int idx, int time );
    //�����ڸ���ʣ��ʱ��
    public native int SendSongRemnantsTime( short time );

    /* ˽��ӿ�                                                             */
    //��˽��, mictypeΪ˽�������(vip���Ͽɹۿ�����Ҫ�Է�ȷ�Ͽɹۿ�)
    public native int OpenPrivateMic(short mictype);
    //��˽��
    public native int ClosePrivateMic();
    //������˽��
    public native int ClosedPrivateMic(int bidx); //bidx=˽�������
    //����ۿ�idx��˽��
    public native int RequestWatchPrivateMic(int idx); //idx=˽�������
    //�ظ��ۿ�˽��,ֻ��ͬ��ŵ�������ӿ�
    public native int ResponseWatchPrivateMic(int allow, int idx);//idx=�ۿ���,0->ͬ�⣬-1->�ܾ�
    //���������˿�˽��
    public native int InviteWatchPrivateMic(int idxs[]);
    //�ظ����뿴˽��
    public native int ResponseInviteWatchPrivateMic(int allow, int idx);
    //����˽�����
    public native int OpenRoomPrivateMicFlag(char flag);//0�������� 1������

    // �ʹ�����
    public native int CrownKickoutSomeOne(int idx);
    // �������û���IP���н���
    public native int ForbidUserIp(int idx);

    // ���;�����
    public native int SingerScores( int idx, short scores[] );
    // Ҫ���ϴ�����
    public native int UpdateSong( int idx );
    
    //2014-09-26 by glp
    /*
     ��ȡ�����ȵ��û���Ϣ
     */
    public native int GetUserInfoForSpeaker(int idx[]);
    /*
     ��ȡ�û���Ϣ�д��汾ID
     */
    public native int GetUserInfoWithVersion(RequestData_IdxWithVersion data[]);
    //����ӿڶ���0�ɹ���-1ʧ��
    
    //�������ݿ� by glp 2014-6-18
    /*
     ��ʼ������URL��ַ(����һЩ��Դ��ַ�ȵ�)�������ԭPC�ǹ�ͻ���
     @param url[in]: URL·��
     @param forUse[in]: ʹ�õ���;,����һһ˵��,
        0 : ������µ�ַ
     
     @return��1���ɹ���0��ʧ��
     
     @��:InitDownLoadUrl("http://res2.17xg.com/res/giftxml.html",0);
     
     @ע�������ȵ���InitDownLoadUrl�ٵ���InitLoaclDatabaseDir.
     */
    public native int InitDownLoadUrl(String url,int forUse);
    //��ʼ����������Ŀ¼,����ͼƬ��Դ·��
    /*
    @param path[in]��ָ�����������ļ���Ŀ¼��ע����Ŀ¼
     
    @return��1���ɹ���0��ʧ��
    */
    public native int InitLocalDatabaseDir(String path);
    //�������ﲿ����Ϣ��
    /*
     @param partInfo[in]���������Ϣ�ṹ
     
     @return��1���ɹ���0��ʧ��
     */
    //public native int SavePartInfo(ResultData_PartInfo partInfo);
    //�����Լ��Ĳ�����Ϣ
    /*
     @param partInfo[in]���������Ϣ�ṹ
     
     @return��1���ɹ���0��ʧ��
     */
    //public native int SaveMyPartInfo(ResultData_PartInfo partInfo);
    //��ȡ�Լ�����Ϣ
    public native LocalData_UserInfo GetMyInfo();
    //��ȡ���Լ�������
    public native ResultData_SelfData GetSelfData();
    //�����Լ�������
    //public native int SaveSelfData(ResultData_SelfData pSD);
    //��ȡ���˵�ID
    /*
     @return������ID
     */
    public native int GetMyIdx();
    //���ñ��˵�ID
    //public native int SaveMyIdx(int idx);
    //�����û���Ϣ��
    /*
    @param userInfo[in]���û�������Ϣ
         
    @return��1���ɹ���0��ʧ��
    */
    //public native int SaveUserInfo(USER_DATA_BASE userInfo);
    //�ṩ���ֻ�ֻ�Ǳ��浱ǰ�������û�����Ϣ
    /*
     @param idx:IDX
     @param nickName:�û��ǳ�
     
     @return��1���ɹ���0��ʧ��
     */
    //public native int SaveUserInfoSample(int idx,String nickName);
    //���淿����Ϣ
    //public native int SaveRoomInfo(ROOM_DATA roomInfo);
    //��ȡ�������汾��
    /*
     @param lobbyId[in]������ID
     
     @return������汾��
     */
    //public native int GetServerGroupVersion(int lobbyId);
    //���淿�����汾��
    //public native int SaveServerGroupVersion(int lobbyID,int groupVersionId);
    //����������з��仺����Ϣ
    //public native int ClearRoom();
    //����������з��黺����Ϣ
    //public native int ClearGroup();
    //����һ��������Ϣ
    /*
     @param groupId[in]������ID
     @param groupName[in]����������
     @param disable[in]���Ƿ����
     @param lobbyId[in]������ID
     
     @return��1���ɹ���0��ʧ��
     ע��֮��������AddGroupDataEnd����
     */
    //public native int AddGroupData(int groupId,String groupName,boolean disable,int lobbyId);
    //������Ϣ��ӽ�����
    //public native int AddGroupDataEnd();
    //����һ��������Ϣ
    /*
     @param roomId[in]������ID
     @param groupId[in]������ID
     @param lobbyId[in]������ID
     
     @return��1���ɹ���0��ʧ��
     ע��֮��������AddRoomDataEnd����
     */
    //public native int AddRoomData(int roomId,int groupId,int lobbyId);
    //������Ϣ��ӽ�����
   // public native int AddRoomDataEnd();
    /*
     �ж��Ƿ�����ͨ�û�
     @param host[in]��Ҫ���ӵ�����
     @param strWangtong[in]��ָ������ͨIP��ַ�����԰Ѽ���IP��ַ���������Ž�ȥ��ͨ��";"�ָ
     
     @return��0��������ͨ�û���1������ͨ�û���
     */
    public native int IsWangtong(String host,String strWangtong);
    /*
     �ӱ������ݿ��л�ȡ��ָ������ID�ķ�����Ϣ
     @param roomId[in]��ָ���ķ���ID�����߰���ID
     @parma roomInfo[out]��ָ��������Ϣ�Ĵ��λ��
     
     @return��1���ɹ���0��ʧ�ܡ�
     */
    public native ResultData_RoomInfo GetRoomInfoFromLocal(int roomId);
    /*
     �ӱ������ݿ��л�ȡָ��IDX���û���Ϣ
     @param idx[in]:ָ��IDX
     @param userInfo[out]:��ȡ���ĸ�����Ϣ��
     
     @return��1���ɹ���0��ʧ�ܡ�
     */
    public native LocalData_UserInfo GetUserInfoFromLocal(int idx);
    /*
     �ӱ������ݿ��ȡ�ƶ�������Ϣ
     @param giftId:ָ������ID
     @param pGI:������Ϣ���λ��
     
     @return��1���ɹ���0��ʧ�ܡ�
     */
    public native GiftInfo GetGiftInfoFromLocal(int giftId);
    //��ȡ����������Ϣ��ID
    /*
     @param pId[out]������ID��ŵ�λ��
     @param nNum[in out]�������С�����ش���˶��ٸ�ID
     @return ��������ţ�true������������ţ�false
     
     @ע��һ��Ҫ�ϸ��жϷ���ֵ
     */
    public native int[] GetAllGiftId();
    /*
     ר�������ǹ�����ĸ��½ӿڣ����������
     
     @param taskInfo[in]������������Ϣ������μ��ṹ˵��
     @param priority[in]���Ƿ���������
     
     @ע1������TaskDownload�ṹ�е�DownloadInfo�ṹ�е�saveBuf�������Ѿ��������˹����û������Լ���������ڴ棬ֻҪ�ڻص��ĺ�����ʹ�ü���
     @ע2�����ڽṹ��url��download�������ʵ����ƥ��
     
     @��:
     TaskDownload task;
     strcpy(task.url , "http://xg.91555.com/res/giftxml.html");
     task.onProgress = &OnProgress;//��ѡ��ָ�����Ⱥ�����������ʾ���أ����ʽ�������
     task.onFinish = &OnFinish;//ָ����ɺ�����
     task.info.download = false;
     strcpy(task.info.fileName , "C:\\Users\\Administrator\\Desktop\\gift\\gift_items.zip");//ָ���ļ�����λ��
     task.info.mandatory = false;
     strcpy(task.info.unzipDir , "C:\\Users\\Administrator\\Desktop\\testunzip");//ָ����ѹ·��
     b = g_gMgr.addTask(task);
     //������������У��㻹��Ҫ��OnFinish�н�����Http���󵽵����ݣ�Ȼ���һ�������������������zip�ļ�����������ѯ@guliping��
     
     @return��0���������ɹ���-1���������ʧ�ܣ�-2���ļ��Ѿ�������������
     */
    //public native int AddHttpDownLoadTask(TaskDownload taskInfo,boolean priority);
    /*
     �ֻ��û�����ӿ�
     @param toIdx[in]:���͸�˭�������0���ǹ��ġ�
     @param content[in]:���������.
     
     @return��0���ɹ���-1��ʧ�ܡ�
     */
    public native int SendChatKTV(int toIdx,String content);
    /*
     �ֻ��û�����ӿ�
     @param toIdx[in]:���͸�˭
     @param giftId[in]:����ID
     @param number[in]:��������
     @param roomId[in]:����ID������ڴ�����0
     
     @return��0���ɹ���-1��ʧ�ܡ�
     */
    public native int SendGiftInLobby(int toIdx,int giftId,int number,int roomId);
    
    /*
     ��ص��ô˷�������������MFǰ׺�ĺ�����������false
     */
	public native boolean MFInitFilter();
	/*
     ���������ַ���
     @param utf8[in out]����Ҫ�Աȵ�UTF8�ַ���
     @param size[in] : �ַ����ĳ���
     
     @return :
     false��û�г�ʼ������utf8ΪNULL
     true���ɹ��������
     
     @ע��utf8�е��ַ���������UTF8�ַ���
     */
    public native String MFFilter( String  utf8 );
	/*
     �����Ƿ��������ַ���
     
     @param utf8[in]����Ҫ�Աȵ�UTF8�ַ���
     
     @ע��utf8�е��ַ���������UTF8�ַ���
     
     @return ��true���У�false��û��
     */
	public native boolean MFFindSensWord( String utf8 );
	/*
     �����Ƿ����������
     
     @param utf8[in]����Ҫ�Աȵ��ַ���
     
     @return ��true���У�false��û��
     */
	public native boolean MFFindSensWord2( String utf8 );
	
	//------------------------------------------------------JniUtil ʵ��-glp---------------------------------------------//
  //���졢���ȹ���
	public void SendChatFromUser(int ret, int to_idx, String content, int time ){}
	//����˽��
	public void RecvChatFromUser(int ret, int from_idx, String content, int time ){}
	//���չ��ģ����ֲ��ڲ���
	public void RecvChatFromClub(int ret, int from_idx, String content){}
	//���չ��ģ������ڣ�
	//ret:1--���� 0--������ -1--��������
	public void RecvChatFromRoom(int ret, int errcode,int from_idx, String content, int time ){}	
	//���մ����ȣ�ȫ����
	public void RecvSpeakerFromAll(int ret, int type, int from_id, String msg){}
	//����С���ȣ�������
	public void RecvSpeakerFromLobby(int ret, int from_idx,String room_name, String msg, int time,int type,boolean isSend ){
		
	}
	//��������㲥
	public void RecvPresentBroad(int ret, ResultData_GiftInfo guestGiftInfo, ResultData_GiftInfo commGiftInfo){}
	//����ϵͳ���ֹ㲥
	public void RecvSystemBroad( int type, String msg){}
	//����ϵͳ���ֹ㲥�������н���Ϣ��
	public void RecvSystemBroad2(String nickname, int idx, int roomid, int giftid, int luckmultiple){
		
	}
	//����ϵͳ���ֹ���
	public void RecvSystemNotice( String msg, boolean once, int min ){}
	//����ϵͳ���ֹ��棨�����н���Ϣ��
	public void RecvSystemNotice2(String nickname, int idx, int roomid, int giftid, int luckmultiple){}
	//���շ�����ʱ����
	public void RecvRoomNotice(int ret, String content){}
	//ȡ�ú���λ��
	public void RecvFriendLocation(int ret, int errcode,int friend_idx,int lobbyid, int roomid,int cseq){}
	/*			 ���Ѳ���					*/
	/*			�������ӿ�			*/
	//����ȡ�ô���˷����Ա�б�
	public void RecvRoomApplyList (int ret ,int errcode,ResultDate_RoomApplyList applylist,int cseq){}
	//�������뷿���Ա��Ӧ
	public void RecvApplyRoomMember(int ret,int errcode,int cseq){}
	//������˻�Ա����������
	public void RecvAuditAddApply (int ret, int errcode,int idx,int cseq){}
	//���վܾ���������ϵͳ��ʾ
	public void RecvRoomApplyRefuseMsg (int ret, int roomid){}
	//���շ����Ա�б�
	public void RecvRoomMemberList (int ret ,int errcode, ResultDate_RoomMemList memlist,int cseq){}
	//���ս�������Ա�ʸ��Ӧ
	public void RecvUnchainMember (int ret, int errcode, int idx,int cseq){}
	//���ո����û�������������Ȩ�޻�Ӧ
	public void RecvRoomPesideAssistant (int ret , int errcode, int idx,int cseq){}
	//���ո�����������֪ͨ
	public void RecvRoomNotice4PesideAssistant (int ret , int opidx,int bopidx){}
	//���ո����û�����������Ȩ�޻�Ӧ
	public void RecvRoomPeside (int ret ,int errcode, int idx,int cseq){}
	//��������������Ȩ�޷���֪ͨ
	public void RecvRoomNotice4Peside (int ret , int opidx,int bopidx){}
	//���ո����û����丱����Ȩ�޻�Ӧ
	public void RecvRoomSubHost (int ret ,int errcode, int idx,int cseq){}
	//�������ø�����Ȩ�޷���֪ͨ
	public void RecvRoomNotice4SubHost(int ret , int opidx,int bopidx){}
	//���շ��������
	public void RecvBlackList(int ret,	int errcode,ResultDate_RoomBlackList blacklist,int cseq){}
	//���պ������û����/ɾ����Ӧ
	public void RecvAddDelBlack(int ret , int errcode,int idx, int type){}
	//���շ������������Ϣ
	public void RecvRoomPanelInfo(int ret ,int errcode, ResultDate_RoomPanel panelinfo){}
	//�������÷��������Ӧ
	public void RecvRoomPassword(int ret,int errcode,int cseq){}
	//�������÷���������Ϣ��ʾ
	public void RecvRoomNotice4Password(int ret, int idx, int pwdflag){}
	//�������÷��俪��/�ر�(����)��Ӧ
	public void RecvRoomSwitch(int ret,int errcode,int roomflag,int cseq){}
	//���շ��俪��/�ر�(����)��Ϣ��ʾ
	public void RecvRoomNotice4RoomSwitch(int ret , int idx , int roomflag){}
	//�������÷������Ȩ�޻�Ӧ
	public void RecvRoomPrivate(int ret , int errcode,int accessflag,int cseq){}
	//���շ������Ȩ����Ϣ��ʾ
	public void RecvRoomNotice4Private(int ret , int idx , int accessflag){}
	//���������û�������Ϣ����/�رջ�Ӧ
	public void RecvUserInOutSwitch(int ret , int errcode, int inoutflag,int cseq){}
	//�����û�������Ϣ����/�ر���Ϣ��ʾ
	public void RecvRoomNotice4UserSwitch (int ret , int idx , int inoutflag){}
	//������������������/�رջ�Ӧ
	public void RecvGetMicSwitch (int ret, int errcode,int onmicflag,int cseq){}
	//������������������/�رջ�Ӧ
	public void RecvOnMicSwitch (int ret, int errcode,int onmicflag,int cseq){}
	//��������������/�ر���Ϣ��ʾ
	public void RecvRoomNotice4GetMicSwitch(int ret , int idx , int getmicflag){}
	//��������������/�ر���Ϣ��ʾ
	public void RecvRoomNotice4OnMicSwitch(int ret , int idx , int onmicflag){}
	//���������޸Ļ�Ӧ
	public void RecvModifyRoomName(int ret,int errcode,int cseq){}
	//���շ�����
	public void RecvRoomName (int ret , int roomid , String roomname){}
	//���÷��乫����Ӧ
	public void RecvRoomAffiche(int ret,int errcode,int cseq){}
	//���շ��乫������¼�
	public void RecvRoomAfficheEvent(int ret){}
	//���������乫����Ӧ
	public void RecvAllowRoomAllChat (int ret, int errcode,int chatflag,int cseq){}
	//���շ��乫��������Ϣ��ʾ
	public void RecvRoomNotice4AllChat(int ret , int idx , int chatflag){}
	//���÷��件ӭ����Ӧ
	public void RecvRoomSalutatory(int ret, int errcode,int cseq){}
	//���շ��件ӭ�ʸ���֪ͨ
	public void RecvRoomNotice4Salutatory(int ret,int idx){}
	//���÷���logo��Ӧ
	public void RecvRoomLogo (int ret,int errcode ,int cseq){}
	//���շ���LOGO����֪ͨ
	public void RecvRoomNotice4Logo(int ret,int idx){}
	//���շ��䶳����Ϣ��ʾ
	public void RecvRoomNotice4Lock(int idx){}
	//���ܴ��������Ӧ
	public void RecvCreateRoom( int ret, int errcode, RoomListEx info ){}
	//�������跿��ȫ��Ϣ
	public void RecvTmpRoomInfo( int ret, int errcode, RoomListEx info ){}
	// ��Ӹ跿�㲥
	public void RecvRoomNotice4Add( RoomListEx info ){}
	public void RecvRoomNotice4Add( int idx ){}
	// ɾ���跿�㲥
	public void RecvRoomNotice4Del( int idx ){}
	// ���շ�������֪ͨ
	public void RecvRoomNotice4ChangeMaster(int idx){}
	// ���շ�����Ϣ����֪ͨ
	public void RecvRoomNotice4ChangeInfo( int idx ){}
	/*			�������ӿ�				*/
	/*			 ��ɫ��Ϣ				*/
	//ȡ�����˽�ɫ��Ϣ
	public void RecvPartInfo(int ret, int errcode, ResultData_PartInfo part_info, int cseq){
		
	}
	  
	  //ȡ���Լ���ɫ��Ϣ ר����java�Ľӿ� ��½�������ķ��ؽ�� @glp
	  //@ע��Android���еĻص����������ڴ�����½���һ���Ļص�
	public void OnLoginLobby(int ret, int errcode, LocalData_UserInfo userInfo){
		
	}
	//ȡ���Լ���ɫ��Ϣ
	public void RecvPartInfoBySelf(int ret, int errcode, ResultData_PartInfo part_info, int cseq){}
	//��ɫ��Ϣ
	//ȡ�÷����б�
	public void RecvRoomList(int ret,ResultDate_RoomList roomlist){}
	public void RecvRoomListEx(int ret,ResultDate_RoomListEx roomlist){}
	//����֪ͨ
	public void RecvNotice4OffMic( int onmic_idx, int pk_idx, int exp, String scoreinfo){}
	// add by cx 10-6-2
	//��Ӧ�Ƿ���������ɹ�
	public void RecvRoomGivOnevjMic( int idx, int flag ){}
	//�Ƿ�ͬ����������ɹ�
	public void RecvRoomGivOnevjMicApply( int formidx ){}
	//�Ƿ�ͬ����ӶԷ�Ϊ����
	//����˵����int idx ��Ӻ�����idx
	public void RecvAddFriendApply(int idx){}
	//public void RecvDeleteAvarta(int ret){}
	//�ɹ�������֤������
	public void OnAuthTunnelConnectSuccess(){
	}			
	//�ɹ����Ӵ���������
	public void OnLobbyTunnelConnectSuccess(){
	}
	//�ɹ����ӷ��������		
	public void OnRoomTunnelConnectSuccess(){
	}			
	public void OnChannellistTunnelConnectSuccess(){}	//�ɹ�����Ƶ��������
	////������֤��������ʱ
	public void OnAuthTunnelConnectTimeout(){
	}		
	//���Ӵ�����������ʱ
	public void OnLobbyTunnelConnectTimeout(){
	}		
	//���ӷ����������ʱ
	public void OnRoomTunnelConnectTimeout(){
	}	
	public void OnChannellistTunnelConnectTimeout(){}//����Ƶ����������ʱ
	//���Ӵ���
	public void OnAuthTunnelConnectError( int code ){
	}	
	public void OnLobbyTunnelConnectError( int code ){
	}
	public void OnRoomTunnelConnectError( int code ){
	}
	public void OnChannellistTunnelConnectError( int code ){}
	//�����������Ͽ�������,�ͻ���recv == 0��ʱ��,�ص������µĽӿ�
	//�Ͽ���֤������
	public void OnAuthTunnelClose(){
	}	
	//�Ͽ�����������
	public void OnLobbyTunnelClose(){
	}
	//�Ͽ����������
	public void OnRoomTunnelClose(){
	}	
	//�Ͽ�Ƶ��������
	public void OnChannellistTunnelClose(){}
	//�ͻ���recv�쳣,send�쳣,�����buf���,select��������,����ص���������½ӿ�
	public void OnAuthTunnelError( int code ){
	}	
	public void OnLobbyTunnelError( int code ){
	}
	public void OnRoomTunnelError( int code ){
	}
	public void OnChannellistTunnelError( int code ){}

	/*----------------------------��֤�����������ӿ�--------------------------------------*/
	//������֤���
	public void RecvAuth(int ret, int errcode,int again){
	}
	//������token
	//public void RecvNewToken(int ret,ReserveDataS pRD){}
	public void RecvNewToken(int ret,int errcode){}
	//����Ƶ���б�
	public void RecvSectionList(int ret,int errcode,ResultData_GetChannellist RDc,int cseq){
	}
	//����Ƶ���б�(��ӷ�������Ϣ)   ----���Է�������Ϣ
	public void RecvSectionList2(int ret,int errcode,int leftlogintime,ResultData_GetChannellist RDc,int cseq){}
	/*-------------------------���������������ӿ�------------------*/
	//���մ���token��֤
	public void RecvTokenAuth4Lobby( int ret, int errcode, ResultData_LobbyLogin pRDl, ResultData_SelfData pSD, int cseq ){
	}
	//�û���¼����֪ͨ
	public void RecvUserLoginLobbyNotice(int ret,int idx,char relation,int lobbyid){
	}
	//�û��˳�����֪ͨ
	public void RecvUserLogoutLobbyNotice(int ret,int idx,char relation,int lobbyid){
	}
	//�����û���Ϣ
	//���ﲢû���û���Ϣ��������ʹ��GetUserInfoFromLocal();
	public void RecvUserInfo(int ret,int idx,ResultData_UserFullInfo RDu){
	}
	//���պ���״̬
	public void RecvFriendStatus(int ret,int errcode,ResultData_GetFriendlist RD){}
	//���պ�����״̬
	public void RecvBlackStatus(int ret,int errcode,ResultData_GetFriendlist RD){}
	//���վ��ֲ�״̬
	public void RecvClubStatus(int ret,int errcode,ResultData_GetClublist RD){}
	//������Ӻ��ѽ��
	public void RecvAddFriend(int ret,int errcode,int idx){}
	//������Ӻ��������
	public void RecvAddBlack(int ret,int errcode,int cseq){}
	//����ɾ�����ѽ��
	public void RecvDelFriend(int ret,int errcode,int cseq){}
	//����ɾ�����������
	public void RecvDelBlack(int ret,int errcode,int cseq){}
	//�������������ֲ����
	public void RecvApplyClub(int ret,int errcode,int cseq){}
	//�����˳����ֲ����
	public void RecvExitClub(int ret,int errcode,int cseq){}
	//������������ڵļ�����ֲ���������
	public void RecvApplyListCount(int ret,int errcode,int count,int cseq){}
	//������������ڵļ�����ֲ������б�
	public void RecvApplyList(int ret,int errcode,ResultData_ClubApplyList RD,int cseq){}
	//ͨ��������ֲ�����
	public void RecvAddClubOk(int ret,ResultData_Approvelresult RD){}
	//ɾ�������б�
	public void RecvDelApplyList(int ret,ResultData_Approvelresult RD){}
	//���ռ�����ֲ��㲥��Ϣ(�����˼�����ֲ�)
	//modify by jinguanfu 2010/4/11
	//public void RecvAddClubNotice(int ret ,int idx){}
	//opidx--������idx
	//addidx--������idx
	public void RecvAddClubNoticeByLobby(int ret ,int clubid,String clubname,int opidx,int addidx){}
	public void RecvAddClubNoticeByRoom( int opidx, int addidx ){}
	//���������Ա�ܾ���Ϣ
	//opidx--�����idx
	public void RecvDenyClubNoticeByLobby(int ret ,int clubid,String clubname,int opidx){}
	//public void RecvDenyClubNoticeByRoom(int ret ,int clubid,String clubname,int opidx){}
	//���ջ�Ա�˳����ֲ���Ϣ
	public void RecvExitClubNoticeByLobby(int ret,int clubid,String clubname,int exitidx){}
	public void RecvExitClubNoticeByRoom( int exitidx ){}
	//���ջ�Ա���߳����ֲ���Ϣ
	//opidx--������idx
	//kickidx--���߳���idx
	public void RecvKickClubNoticeByLobby(int ret,int clubid,String clubname,int opidx,int kickidx){}
	public void RecvKickClubNoticeByRoom( int opidx, int kickidx ){}
	//�������ֲ�
	public void RecvCreateClub(int ret,int errcode,int clubid,int cseq){}
	//��ɢ���ֲ�
	public void RecvDismissClub(int ret,int errcode,int cseq){}
	//���վ��ֲ���Ϣ
	public void RecvClubInfo(int ret,int errcode,ResultData_ClubFullInfo RD,int cseq){}
	////���շ����б�
	//public void RecvRoomList(int ret,ResultData_RoomList pRD){}
	//���շ�����Ϣ
	public void RecvRoomInfo(int ret,ResultData_RoomInfo RD){}
	//����ͷ��
	public void RecvCreateAvatar(int ret,int errcode){}
	//�޸�ͷ��
	public void RecvChangeAvatar(int ret,int errcode){}
	//�޸��ǳ�
	public void RecvChangeNickName(int ret,int errcode, String name){}
	//���뷿��֪ͨ
	public void RecvLobbyEnterRoom(int ret,ResultData_LobbyRoomEnter RD){}
	//�뿪����֪ͨ
	public void RecvLobbyLeaveRoom(int ret,ResultData_LobbyRoomEnter RD){}
	//��Ӻ���֪ͨ
	public void RecvAddFriendNotice(int ret,int idx){}
	//ȡ�ô�����Ϣ
	public void RecvLobbyInfo(int ret,int errcode,ResultData_LobbyName RD,int cseq){}
	//�������Դ�����Ȩ�޸�����Ϣ add by jinguanfu 2010/6/5
	public void RecvChangePower(int optionidx/*������ID*/,int identity/*���ĺ�Ȩ��*/){}
	//ȡ������(CMD_SYSTEMMSG_L2C)
	//public void RecvLobbyName(){}
	/*------------------------��������ӿ�------------------------------*/
	//���շ���token��֤ //
	public void RecvTokenAuth4Room(int ret,int errcode){
	}
	//���뷿��
	public void RecvEnterRoom(int ret,int errcode,/*ResultData_LoginInfo RDlf*/byte retlogin,byte identity,int cseq){
	}
	//�û����뷿��֪ͨ
	public void RecvEnterRoomNotice(int ret,ResultData_UserInfo RDu){}
	//�û��뿪����֪ͨ
	public void RecvLeaveRoomNotice(int ret,int idx){}
	//���շ�����ϸ��Ϣ
	public void RecvRoomAllInfo(int ret,int errcode,ResultData_RoomAllInfo RD,int cseq){}
	//��������ɹ�֪ͨ
	public void RecvWaitMicNotice(int ret,ResultData_MicInfo RD){}
	////��������׼��֪ͨ
	//public void RecvWaitMicReadyNotice(int ret,ResultData_MicInfo pRD){}
	//��������׼�����֪ͨ
	//public void RecvOnMicReadyOkNotice(int ret,ResultData_MicInfo pRD){}
	//��������֪ͨ
	public void RecvOnMicNotice(int ret,ResultData_MicInfo RD){}
	//ȡ������ɹ�֪ͨ
	public void RecvCancelWaitMicNotice(int ret,int opidx,int bopidx){}
	//���ռ���PK��֪ͨ
	public void RecvWaitPKMicNotice(int ret,ResultData_MicInfo RD){}
	//����ȡ��PK��֪ͨ
	public void RecvCancelWaitPKMicNotice(int ret,int opidx,int bopidx){}
	//��������ɹ�֪ͨ
	public void RecvUpWaitMicNotice(int ret,ResultData_UDMicInfo RD){}
	//���ս���ɹ�֪ͨ
	public void RecvDownWaitMicNotice(int ret,ResultData_UDMicInfo RD){}
	//�����߳�����֪ͨ
	public void RecvKickOutNotice(int idx,int bidx){}
	//������VJ��������֪ͨ
	public void RecvOnVJMicNotice(int ret,int idx){}
	//������VJ��������֪ͨ
	public void RecvOffVJMicNotice(int ret,int idx){}
	//���ձ�����֪ͨ
	public void RecvGiveOffMicNotice(int ret,int idx){}
	//���ձ�����֪ͨ
	public void RecvForbidenNotice(int ret,ResultData_ForbidenInfo RD){}
	//���պ���������֪ͨ
	public void RecvUpdateBlackListNotice(int ret,ResultData_UpBlackList RD){}
	//���չ���Ա����֪ͨ
	public void RecvUpdateMGRListNotice(int ret,ResultData_UpMgrList RD){}
	//������������֪ͨ
	public void RecvSendFlowerNotice(int ret,ResultData_SendFlowerInfo RD){
	}
	//add by jinguanfu 2010/3/4 ԭ�лص�API�ĳɽӿ�<end>
	//���շ����ظ���¼��Ϣ
	public void RecvRoomKickUser(){}
	//���մ����ظ���¼��Ϣ
	public void RecvLobbyKickUser(){}
	//�Ӵ������շ��������Ϣ
	public void RecvRoomSimpleInfo(int ret,int errcode,ResultData_RoomSimpleInfoList RD){}
	public void RecvRoomSimpleInfoEx(int ret,int errcode,ResultData_RoomSimpleInfoList RD, int type){}
	//�����û��˳������Ա��Ӧ
	public void RecvExitRoomMember(int ret,int errcode){}
	//����WEB���������ʼ���Ϣ
	public void RecvMailInfo(int ret,String context){}
	//���մ�����������
	public void RecvLobbyOnlineNum(int ret,int errcode,ResultData_LobbyOnlineList RD){}
	//���������Ӧ��Ϣ	//add by jinguanfu 2010/4/27
	public void RecvSendFlower(int ret,int errcode,int recevie_idx,int item_id,int count,int gold,int silver){
	}
	//���շ��ʹ����Ȼ�Ӧ //add by jinguanfu 2010/5/13
	public void RecvSpeakerToAll(int ret,int errcode,int gold,int silver){}
	//���շ���С���Ȼ�Ӧ //add by jinguanfu 2010/5/13
	public void RecvSpeakerToLobby(int ret,int errcode,int gold,int silver){
	}
	//�����ɹ���Ա��Ϊһ���Ա�Ļ�Ӧ //add by jinguanfu 2010/5/13
	public void RecvRoomMember (int ret , int errcode, int idx,int cseq){}
	//�����ɹ���Ա��Ϊһ���Ա��֪ͨ //add by jinguanfu 2010/5/13
	public void RecvRoomNotice4Member (int ret , int opidx,int bopidx){}
	//���շ��������˸ı���avarat�Ĺ㲥 //add by cx 2010-5-20
	public void RecvRoomNotice4ChangeAvarat (int ret, int idx){}
	//���շ����ڴ��������ʾ //add by cx 2010-6-6
	public void RecvOneError( short cmd ){}
	//add by cx 2010-7-6 ���ܹ�����Ʒ��Ӧ
	public void RecvBuyPack( int ret, int errcode, int gold, int silver, short [] goods_id, int [] global_id, int count, String day ){}
	// ���ܹ�����Ʒ��Ӧ
	public void RecvBuyForSend( int ret, int errcode, int idx, int gold, int silver ){}
	//���ܻ�ȡ������Ӧ
	public void RecvGetPack( int ret, int errcode, ResultData_Pack [] pPack, int count, int all_count ){}
	//����ɾ����Ʒ��Ӧ
	public void RecvDelPack( int ret, int errcode, int cseq ){}
	//������Ʒ����֪ͨ
	public void RecvTimeoutPack( int count, int [] global_id ){}
	//������Ʒ����
	public void RecvSendPack( int idx, int global_id, short goods_id, String day, String msg ){}
	//����ʹ����Ʒ��Ӧ
	public void RecvUsePack( int ret, int errcode, int global_id ){}
	// �����㲥
	public void RecvNotice4LevelUp( int idx, int level ){}
	// ��������
	public void RecvExpChange( int nexp, int level ){}
	// vip�ı�
	public void RecvVipChange( int idx, int level, int is_vip ){}
	// Ǯ�Ҹı�
	public void UpdataMoney( int r_gold, int r_silver ){
	}
	// �����Լ�����Ϣ
	public void RecvUpdataMyInfo(){}
	// �����̻��㲥
	public void RecvFireworks( int ret, int errcode, int cate_idx, int count, int gold, int silver ){}
	// �����̻��㲥
	public void RecvFireworksNotice( int player_idx, int cate_idx, int count ){}
	// �н���
	public void RecvLucky( int player_idx, int cate_idx, int lotte ){}
	// �н���
	public void RecvLucky( int room_idx, int player_idx, int cate_idx, int lotte ){}
	// �б�
	public void RecvGiftTopList( ResultData_GiftList gift_list ){}
	// ����ʱ��
	public void RecvOnlineTime( int idx, int day_time, int online ){}
	// ��ȡ��������
	public void RecvGetNewUserTask( short step ){}
	// �������������ύ���
	public void RecvTaskStep( int ret, int err_code, int gold, int silver, int total_gold, int total_silver ){}
	public void RecvTaskGift( int ret, int err_code, int item_id, int object_id, String time_buf ){}
	// ��Ʒ��Ϣ����֪ͨ
	public void RecvNotice4ChangeGoods(){}
	// ����������ˢ�±���
	public void RecvGetBagAgain(){}
	// �����û�ӡ��
	public void RecvUserSeel( int user_idx, int seel_idx ){}
	// �����û�������״̬
	public void EnterAllMentState( int idx, short state ){}
	// room loge
	public void RecvRoomLoge( int room_idx, String loge, int len ){}
	// �·���汾��
	public void RecvNewVersionId( int version_id ){}
	// ������Ϣ
	public void RecvGroupInfo( int count, ResultData_GroupInfo Info ){}
	// ��ȡ������ķ����б�
	public void RecvRoomListInGroup( short group_idx, int count, int [] list ){}
	public void RecvRoomListInGroup(short count, ResultData_GroupRoomInfo[] pInfo){}
	// �ϴ���־֪ͨ
	public void RecvNotice4CollectData( String server_ip, short server_port ){}
	// ��ȡ������Ϣ
	public void RecvSongInfoList( short totalnum, short curnum, ResultData_SongInfo info ){}
	// ��ȡ����Ƶ�������б�
	public void RecvAvServerList( ResultData_AvServer [] list, int count ){}
	// ��������Ƶ�������б�
	public void UpdataAvServerList(){}
	// �������Ϸ������
	public void RecvCheckOutGame( int result ){}
	// ��ȡ������Ϣ
	public void RecvOffLineMsg( int count, ResultData_OffLineMsg [] msg ){}
	// ���ط����ö���Ϣ
	public void RecvRoomSortInGroup( int idx, int count, ResultData_RoomSort [] list ){}
	// ֪ͨ�����ö���Ϣ�ı�
	public void RecvRoomSortChange( int idx ){}
	// ���ֵȼ�
	public void RecvSingerLevel(int idx, short curlevel, int expvalue){}
	// �Ƹ��ȼ�
	public void RecvWealthLevel(int idx, short curlevel, int expvalue){}

	//////////////////////////////////////////////////////////////////////////

	// ����ʧЧ��ӡ�£���Э��
	public void RecvGiftInvalid( int idx, int GiftID ){}
	// ������Ч��ӡ�£���Э��
	public void RecvGiftValid( int idx, int GiftID ){}
	// ��ʾ�������ȡ����
	public void RecvShowRoomIncom( int income ){}
	// ��ʾ��������ȡ��¼
	public void RecvShowRoomGetIncom( ResultData_IncomeDate [] data, int count ){}
	// ������ȡ����������
	public void RecvGetRoomIncom( int ret ){}
	// ������Ϣ����֪ͨ
	public void RecvNotice4ChangeFlower(){
		//m_handler.sendEmptyMessage(XGMessage.XGMSG_GIFT_UPDATE);
	}
	// Ȩ�����ø���֪ͨ
	public void RecvNotice4ChangePower(){}
	// ��������Ƶ�������㲥
	public void RecvNotice4AvserverChange( ResultData_AvChange info ){}
	// ����gm
	public void RecvChangeGM( int idx, int add ){}
	// ������˵�pc�ſ�
	public void RecvMicPcInfo( int idx, String buf ){}
	// ��Ӧ�Ƿ������뷿��ɹ�
	public void RecvSendJoinRoomInvite( int wseq, int ret, int err_code ){}
	// ��Ӧ�Ƿ������뷿��ɹ�
	public void RecvInviteJoinRoom( int idx ){}
	//ȡ�õ����������ͼ�¼
	public void RecvGiftSend( int count, ResultData_GiftCount [] pInfo ){}
	//ȡ�õ���������ռ�¼
	public void RecvGiftRecv( int count, ResultData_GiftCount [] pInfo ){}
	//���ط�mac���
	public void RecvKickUserByMac( int ret, int idx ){}
	//���ط�ip���
	public void RecvKickUserByIp( int ret, int idx ){}
	//������������ʱ����
	public void RecvSetMicTime( int ret, int idx ){}
	//�㲥����ʱ���ӳ�
	public void RecvNotice4SetMicTime( int idx, int time ){}
	//ȡ�����б�
	public void RecvWaitMicList( short count, int[] idxs ){}
	//������ʱ�䵽,�Ͽ�����
	public void RecvIndulgeTimeUp(){}
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
	public void RecvFamilyInfo(int flag, int familyNum, ResultData_Family[] familys){}

	/*
	av������Ϣ
	*/
	public void RecvAVConfigInfo(int infolen, String info){}

	//
	/*------------------------------˽��---------------------------------------*/
	/*��˽��*/
	public void RecvOpenPrivateMicSuccess(int idx, short mictype, int posindex, int roomid){}
	public void RecvOpenPrivateMicFailed(int errorcode, int idx, int bidx){}
	public void RecvOpenPrivateMicNotice(int idx, int mictype){} //�������Լ�
	
	/*��˽��*/
	public void RecvClosePrivateMicFailed(int errorcode, int idx, int bidx){}
	public void RecvClosePriveteMicNotice(int idx){} //�����Լ�
	
	/*������˽��*/
	public void RecvClosedPrivateMicFailed(int errorcode, int idx, int bidx){}
	public void RecvClosedPriveteMicNotice(int idx, int bidx){} //�����Լ�
	
	/*�ۿ�˽��*/
	public void RecvWatchPrivateMicSuccess(int idx, short mictype, int posindex, int roomid){}
	public void RecvWatchPrivateMicFailed(int errorcode, int idx, int bidx){}
	public void RecvRequestWatchedPrivateMic(int idx){} /*idx=������, �Ƿ�ͬ���idx�ۿ����˽��*/
	public void RecvWatchedPrivateMicFailed(int errorcode, int idx, int bidx){}

	public void RecvWatchedPrivateMicReject(int idx){} //�Է��ܾ���idx=˭�ܾ���

	/*����˽��*/
	public void RecvInviteWatchPrivateMicSuccess(){}
	public void RecvInviteWatchPrivateMicFailed(int errorcode, int idx, int bidx){}
	public void RecvRequestInvitedWatchPrivateMic(int idx){} /*idx=������,�Ƿ�ͬ��ۿ�idx��˽��*/
	public void RecvInvitedWatchPrivateMicSuccess(int idx, short mictype, int posindex, int roomid){}
	public void RecvInvitedWatchPrivateMicFailed(int errorcode, int idx, int bidx){}

	public void RecvInvitedWatchPrivateMicReject(int idx){}//�Է��ܾ���idx=˭�ܾ���

	/*������˽��*/
	public void RecvOpenRoomPrivateMicFlag(int ret, int flag){}
	public void RecvOpenRoomPrivateMicFlagNotice(int idx, int flag){} //�����Լ�

	/*�û���¼����ʱ���ص������˽����Ϣ*/
	public void RecvPrivateMicWithLoginRoom(int idx, short mictype, int posindex, int roomid){}
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
	public void RecvCrownKickoutSomeOne(int ret, int kickuse, int totalkick, int idx){}
	/*������ֹ�û�ip
	int ret{}
	 0���ɹ�
	 -1����IP�ѱ�����
	 -2�����û����뿪����
	
	int idx{}
	��������idx
	*/
	public void RecvForbidUserIp(int ret, int idx){}

	//Ҫ���ϴ�����
	public void RecvUpdateSong(){}
	//��Ӧ�������ķ����û��質��Ϣ
	//public void RecvUserSingInfo(int ret,int idx,const ResultData_UserSingInfo pData){}
	//��ȡ�û������������
	public void RecvGiftCount(int recvcount){}

	//��ȡ�û���߷ָ�����Ϣ���ݳ���������
	public void RecvTopSongInfo(int songcount, int topsongscore, String topsongname){}
	  
	  ///////////////////////////////////////////////////�ֻ������ص�////////////////////////////////////////////////////////////
	//�ֻ�ktv������Ϣ
	    /*
	     @param ret:����ֵ��0,OK{}����ֵ,something error
	     @param fromIdx:����˭,�����������Ҳ�п���ָ���͵�IDX
	     @param roomName:˵�����ڵķ��䣬���δ�ڷ�����Ϊ��0��
	     @param content:˵������
	     @param chatTime:˵��ʱ��
	     @param isPublic:�Ƿ�Ϊ����
	     
	     @ע:chattime
	     */
	public void RecvKtvChatInfo(int ret,int fromIdx,int roomid,String roomName,String content,
	                                 int chatTime,boolean isPublic,boolean isSend){
	}
	    //�ֻ����յ���ǰ�Ĳ����б�����ǰktv�ĵ���б����ڸ���
	    /*
	     @param pSonglist:��Ϣ�ṹ��
	     @param listcount:��������
	     */
	public void RecvKtvSongList(ResultData_SongList[] pSonglist,int listCount){
	}
	    //�ֻ���ȡ��ǰ����Ŀ���״̬
	    /*
	     @param isOpen:true,���Ὺ����false������رգ��û��˳�����
	     */
	public void RecvKtvRoomState(boolean isOpen){
	}
	    //�ֻ��յ������ϰ�֪ͨ,��ʾ�û���������Ϣ����ȥ����
	@Deprecated
	public void RecvKtvSongRanking(){}
    /*
	֪ͨ�����ݳ�����
	*/
	public void RecvKtvSongNormalEnd(int nMusicId){
	}
	    /*
	     �յ��������ֻ��û�IDX�б��������
	     ֻ��IDX
	     */
	public void RecvKtvIdxList(int[] pIdx,int number){
	}
		/*
		֪ͨ�û�ͷ�����
		@param [in] idx��ָ�����û�IDX
		@param [in] picture���û�����ͷ��
		*/
	public void RecvUserPicChange(int idx,String picture){
	}
	public void RecvUserNameChange(int idx,String name){
	}
	/*
	֪ͨ��ȡ���������û���Ϣ
	*/
	public void RecvUserInfoForSpeaker(int errcode,ResultData_UserSampleInfo info){
	}
    /*
     ���ܷ���������Ϣ��ʱ
     ��Ҫ�ж�idx�����еľ���idx
     @param [in] idx:idx���飻 ������ĳ��IDX���Ϊ0����ԭ����IDX�Ѿ���ȡ����Ϣ��ֻ�ǵײ������Ϊ0
     @param [in] count:idx�����С
    */
	public void RecvUserInfoForSpeakerTimeOut(int idx[],int count){}
    /*
     �����û���Ϣ��ʱ
     ��Ҫ�ж�idx�����еľ���idx
     @param [in] idx:idx���飻 ������ĳ��IDX���Ϊ0����ԭ����IDX�Ѿ���ȡ����Ϣ��ֻ�ǵײ������Ϊ0
     @param [in] count:idx�����С
     */
	public void RecvUserInfoTimeOut(int idx[],int count){}
    //��������ͼƬ�������֪ͨ
    //����������ȥ��֮ǰû�л�ȡ��ͼƬ�ĵط�������ȥ��ȡһ��,�����Ѿ�����������
    /*
     @param pictureName[in]:ͼƬ����(��·��)
     */
	    
	public void RecvImageDownladFinished(String pictureName){
	}
	/*
	     ���̻߳�ص����ڳ�ʼ�����ص�ַ֮��ɹ��Ļ��ͻ�ص���֪ͨ�������ݿ�������Ϣ�Ѿ��������
	*/
	public void RecvLocalGiftDatabaseFinished(){
	}
}
