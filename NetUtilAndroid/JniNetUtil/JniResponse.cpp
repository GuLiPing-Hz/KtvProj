/***
 * @author GLP
 */

#include "JniResponse.h"
#include "NetUtil/NetApp.h"
#include "NetUtil/localstorage/ServerData.h"
#include "com_jni_netutil_CJniUtil.h"
#include <assert.h>
#include "NetUtil/ChatCompatibility.h"

#undef SAVEINMAPCLASS
#define SAVEINMAPCLASS(className) \
{ \
	jclass cls = env->FindClass(className); \
	if(cls) \
	{ \
		m_mapClass[className] = (jclass)env->NewGlobalRef(cls); \
		env->DeleteLocalRef(cls); \
	} \
}

#undef CALLBACKJAVABEGIN
#define CALLBACKJAVABEGIN(method,signature) \
JNIEnv* env; \
if(getJniByThreadID(g_NetApp.m_nThreadId,&env)) \
{ \
	jclass cls = env->GetObjectClass(m_obj); \
	if(!cls) \
		return ; \
	static jmethodID jm##method = env->GetMethodID(cls,#method,signature)

//env->CallVoidMethod(m_obj,jm##method);自己实现

#undef CALLBACKJAVAEND
#define CALLBACKJAVAEND \
}

#undef CALLBACKJAVAVOID
#define CALLBACKJAVAVOID(method,signature) \
CALLBACKJAVABEGIN(method,signature); \
env->CallVoidMethod(m_obj,jm##method);\
CALLBACKJAVAEND

#undef GETJAVAFIELDID
#define GETJAVAFIELDID(name,clsname,signature) \
static jfieldID jfid_##name = env->GetFieldID(clsname,#name,signature)

//需要事先定义一个int len变量保存长度
#undef  GETUNICODEFROMGBK
#define GETUNICODEFROMGBK(name,from) \
unsigned short* wstr##name = g_NetApp.m_pCharsetCodecGBK->convertToUnicode(from); \
len = CCharsetCodec::caculateLength(wstr##name); \
jstring name = env->NewString(wstr##name?((const jchar*)wstr##name):((const jchar*)L""),len);\
if(wstr##name) delete wstr##name;

extern CDataMgr g_DM;

jobject GetUserInfoCom(JNIEnv* env,jclass cls_localdata_userinfo,USER_DATA_BASE* pUserInfo,bool bMy = false)
{
	if(!env || !cls_localdata_userinfo || !pUserInfo)
		return NULL;

	jobject objUser = env->AllocObject(cls_localdata_userinfo);
	if(!objUser)
		return NULL;
	GETJAVAFIELDID(ret,cls_localdata_userinfo,"I");
	GETJAVAFIELDID(m_idx,cls_localdata_userinfo,"I");
	GETJAVAFIELDID(m_id,cls_localdata_userinfo,"Ljava/lang/String;");
	GETJAVAFIELDID(m_name,cls_localdata_userinfo,"Ljava/lang/String;");
	GETJAVAFIELDID(m_province,cls_localdata_userinfo,"Ljava/lang/String;");
	GETJAVAFIELDID(m_city,cls_localdata_userinfo,"Ljava/lang/String;");
	GETJAVAFIELDID(m_sex,cls_localdata_userinfo,"Ljava/lang/String;");
	GETJAVAFIELDID(m_age,cls_localdata_userinfo,"Ljava/lang/String;");
	GETJAVAFIELDID(m_level,cls_localdata_userinfo,"I");
	GETJAVAFIELDID(m_LevelTime,cls_localdata_userinfo,"Ljava/lang/String;");
	GETJAVAFIELDID(m_isVip,cls_localdata_userinfo,"B");
	GETJAVAFIELDID(m_stage,cls_localdata_userinfo,"Ljava/lang/String;");
	GETJAVAFIELDID(m_stageName,cls_localdata_userinfo,"Ljava/lang/String;");
	GETJAVAFIELDID(m_ClubId,cls_localdata_userinfo,"Ljava/lang/String;");
	GETJAVAFIELDID(m_ClubProfession,cls_localdata_userinfo,"Ljava/lang/String;");
	GETJAVAFIELDID(m_ClubName,cls_localdata_userinfo,"Ljava/lang/String;");
	GETJAVAFIELDID(m_ExpLevel,cls_localdata_userinfo,"B");
	GETJAVAFIELDID(m_singerlevel,cls_localdata_userinfo,"S");
	GETJAVAFIELDID(m_singer_expvalue,cls_localdata_userinfo,"I");
	GETJAVAFIELDID(m_wealthlevel,cls_localdata_userinfo,"S");
	GETJAVAFIELDID(m_wealth_expvalue,cls_localdata_userinfo,"I");
	GETJAVAFIELDID(headurl,cls_localdata_userinfo,"Ljava/lang/String;");
	GETJAVAFIELDID(recvcount,cls_localdata_userinfo,"I");
	GETJAVAFIELDID(songcount,cls_localdata_userinfo,"I");
	GETJAVAFIELDID(topsongname,cls_localdata_userinfo,"Ljava/lang/String;");
	GETJAVAFIELDID(topsongscore,cls_localdata_userinfo,"I");
	GETJAVAFIELDID(is_adult,cls_localdata_userinfo,"S");
	GETJAVAFIELDID(is_remind,cls_localdata_userinfo,"Z");
	GETJAVAFIELDID(consume,cls_localdata_userinfo,"I");
	GETJAVAFIELDID(peri,cls_localdata_userinfo,"Z");
	GETJAVAFIELDID(name_state,cls_localdata_userinfo,"I");
	GETJAVAFIELDID(info_state,cls_localdata_userinfo,"I");
	GETJAVAFIELDID(location_state,cls_localdata_userinfo,"I");
	GETJAVAFIELDID(part_state,cls_localdata_userinfo,"I");
	GETJAVAFIELDID(lobbyid,cls_localdata_userinfo,"I");
	GETJAVAFIELDID(roomid,cls_localdata_userinfo,"I");
	GETJAVAFIELDID(notify,cls_localdata_userinfo,"I");
	GETJAVAFIELDID(relationship,cls_localdata_userinfo,"I");
	GETJAVAFIELDID(version,cls_localdata_userinfo,"I");
	/*
	char songname[256];  // 歌曲名
	char singername[51];  //歌手名
	int score;       //分数*/
	GETJAVAFIELDID(songname,cls_localdata_userinfo,"Ljava/lang/String;");
	GETJAVAFIELDID(singername,cls_localdata_userinfo,"Ljava/lang/String;");
	GETJAVAFIELDID(score,cls_localdata_userinfo,"I");

	int len = 0;
	GETUNICODEFROMGBK(m_id,pUserInfo->user_info.base.m_id);
	GETUNICODEFROMGBK(m_name,pUserInfo->user_info.base.m_name);
	GETUNICODEFROMGBK(m_province,pUserInfo->user_info.base.m_province);
	GETUNICODEFROMGBK(m_city,pUserInfo->user_info.base.m_city);
	GETUNICODEFROMGBK(m_sex,pUserInfo->user_info.base.m_sex);
	GETUNICODEFROMGBK(m_age,pUserInfo->user_info.base.m_age);
	GETUNICODEFROMGBK(m_LevelTime,pUserInfo->user_info.base.m_LevelTime);
	GETUNICODEFROMGBK(m_stage,pUserInfo->user_info.base.m_stage);
	GETUNICODEFROMGBK(m_stageName,pUserInfo->user_info.base.m_stageName);
	GETUNICODEFROMGBK(m_ClubId,pUserInfo->user_info.base.m_ClubId);
	GETUNICODEFROMGBK(m_ClubProfession,pUserInfo->user_info.base.m_ClubProfession);
	GETUNICODEFROMGBK(m_ClubName,pUserInfo->user_info.base.m_ClubName);
	GETUNICODEFROMGBK(headurl,pUserInfo->user_info.base.headurl);
	GETUNICODEFROMGBK(topsongname,pUserInfo->user_info.base.topsongname);
	GETUNICODEFROMGBK(songname,pUserInfo->user_info.base.songname);
	GETUNICODEFROMGBK(singername,pUserInfo->user_info.base.singername);

	env->SetIntField(objUser,jfid_ret,pUserInfo->user_info.base.ret);
	env->SetIntField(objUser,jfid_m_idx,pUserInfo->user_info.base.m_idx);											//idx编号
	env->SetObjectField(objUser,jfid_m_id,m_id);//用户id,为字符串
	env->SetObjectField(objUser,jfid_m_name,m_name);						//用户名字
	env->SetObjectField(objUser,jfid_m_province,m_province);								//省
	env->SetObjectField(objUser,jfid_m_city,m_city);									//城市
	env->SetObjectField(objUser,jfid_m_sex,m_sex);										//性别
	env->SetObjectField(objUser,jfid_m_age,m_age);										//年龄
	env->SetIntField(objUser,jfid_m_level,pUserInfo->user_info.base.m_level);										//用户等级可转int
	env->SetObjectField(objUser,jfid_m_LevelTime,m_LevelTime);					//vip到期时间
	env->SetByteField(objUser,jfid_m_isVip,pUserInfo->user_info.base.m_isVip);//是否vip账户
	env->SetObjectField(objUser,jfid_m_stage,m_stage);
	env->SetObjectField(objUser,jfid_m_stageName,m_stageName);

	env->SetObjectField(objUser,jfid_m_ClubId,m_ClubId);									//所在俱乐部id
	env->SetObjectField(objUser,jfid_m_ClubProfession,m_ClubProfession);							//俱乐部权限：1管理员,0非管理员
	env->SetObjectField(objUser,jfid_m_ClubName,m_ClubName);								//俱乐部名称	add by jinguanfu 2010/4/20
	env->SetByteField(objUser,jfid_m_ExpLevel,pUserInfo->user_info.base.m_ExpLevel);
	//avarata deprecated

	env->SetShortField(objUser,jfid_m_singerlevel,pUserInfo->user_info.base.m_singerlevel);//歌手等级
	env->SetIntField(objUser,jfid_m_singer_expvalue,pUserInfo->user_info.base.m_singer_expvalue);
	env->SetShortField(objUser,jfid_m_wealthlevel,pUserInfo->user_info.base.m_wealthlevel); //财富等级
	env->SetIntField(objUser,jfid_m_wealth_expvalue,pUserInfo->user_info.base.m_wealth_expvalue);

	env->SetObjectField(objUser,jfid_headurl,headurl);    //头像URL
	env->SetIntField(objUser,jfid_recvcount,pUserInfo->user_info.base.recvcount);        //礼物接收数
	env->SetIntField(objUser,jfid_songcount,pUserInfo->user_info.base.songcount);        //演唱歌曲总数
	env->SetObjectField(objUser,jfid_topsongname,topsongname);   //拿手歌曲
	env->SetIntField(objUser,jfid_topsongscore,pUserInfo->user_info.base.topsongscore);   //演唱最高分

	env->SetObjectField(objUser,jfid_songname,songname);//最近演唱的歌曲歌名
	env->SetObjectField(objUser,jfid_singername,singername);//最近演唱的歌曲歌手名
	env->SetIntField(objUser,jfid_score,pUserInfo->user_info.base.score);//最近演唱的歌曲分数
	if(bMy)
	{
		ResultData_SelfData* pSD = g_DM.getSelfData();
		env->SetShortField(objUser,jfid_is_adult,pSD?pSD->is_adult:0);
		env->SetBooleanField(objUser,jfid_is_remind,pSD?pSD->m_remind:0);
		env->SetIntField(objUser,jfid_consume,pSD?pSD->m_consume:0);
	}
	else
	{
		env->SetShortField(objUser,jfid_is_adult,0);
		env->SetBooleanField(objUser,jfid_is_remind,0);
		env->SetIntField(objUser,jfid_consume,0);
	}

	//是否美女用户,暂时无用
	env->SetBooleanField(objUser,jfid_peri,0);

	env->SetIntField(objUser,jfid_name_state,(int)pUserInfo->name_state);
	env->SetIntField(objUser,jfid_info_state,(int)pUserInfo->info_state);
	env->SetIntField(objUser,jfid_location_state,(int)pUserInfo->location_state);
	env->SetIntField(objUser,jfid_part_state,(int)pUserInfo->part_state);
	env->SetIntField(objUser,jfid_lobbyid,pUserInfo->user_info.lobbyid);
	env->SetIntField(objUser,jfid_roomid,pUserInfo->user_info.roomid);
	env->SetIntField(objUser,jfid_notify,pUserInfo->user_info.notify);
	env->SetIntField(objUser,jfid_relationship,(int)pUserInfo->user_info.relationship);
	env->SetIntField(objUser,jfid_version,pUserInfo->version);

	env->DeleteLocalRef(singername);
	env->DeleteLocalRef(songname);
	env->DeleteLocalRef(m_id);
	env->DeleteLocalRef(m_name);
	env->DeleteLocalRef(m_province);
	env->DeleteLocalRef(m_city);
	env->DeleteLocalRef(m_sex);
	env->DeleteLocalRef(m_age);
	env->DeleteLocalRef(m_LevelTime);
	env->DeleteLocalRef(m_stage);
	env->DeleteLocalRef(m_stageName);
	env->DeleteLocalRef(m_ClubId);
	env->DeleteLocalRef(m_ClubProfession);
	env->DeleteLocalRef(m_ClubName);
	env->DeleteLocalRef(headurl);
	env->DeleteLocalRef(topsongname);

	return objUser;
}

jobject GetMyInfo(JNIEnv* env,USER_DATA_BASE* pMy)
{
	if(!env)
		return NULL;
	jclass cls = env->FindClass("com/jni/netutil/LocalData_UserInfo");
	return GetUserInfoCom(env,cls,pMy,true);
}

jobject GetSelfData(JNIEnv* env)
{
	if(!env)
		return NULL;
	ResultData_SelfData* pSelf = g_DM.getSelfData();
	if(!pSelf)
		return NULL;

	jclass cls_selfdata = env->FindClass("com/jni/netutil/ResultData_SelfData");
	jobject objSelf = env->AllocObject(cls_selfdata);
	if(!objSelf)
		return NULL;

	GETJAVAFIELDID(is_adult,cls_selfdata,"S");
	GETJAVAFIELDID(gold,cls_selfdata,"I");
	GETJAVAFIELDID(silver,cls_selfdata,"I");
	GETJAVAFIELDID(nexp,cls_selfdata,"I");
	GETJAVAFIELDID(m_remind,cls_selfdata,"I");
	GETJAVAFIELDID(m_consume,cls_selfdata,"I");
	
	env->SetShortField(objSelf,jfid_is_adult,pSelf->is_adult);
	env->SetIntField(objSelf,jfid_gold,pSelf->gold);
	env->SetIntField(objSelf,jfid_silver,pSelf->silver);
	env->SetIntField(objSelf,jfid_nexp,pSelf->nexp);
	env->SetIntField(objSelf,jfid_m_remind,pSelf->m_remind);
	env->SetIntField(objSelf,jfid_m_consume,pSelf->m_consume);

	return objSelf;
}

jobject GetUserInfo(JNIEnv* env,int idx)
{
	if(!env)
		return NULL;
	jclass cls = env->FindClass("com/jni/netutil/LocalData_UserInfo");
	USER_DATA_BASE* pUserInfo = g_DM.getUserInfo(idx);
	return GetUserInfoCom(env,cls,pUserInfo);
}
jobject GetGiftInfo(JNIEnv* env,int giftId)
{
	if(!env)
		return NULL;

	GiftInfo gi = {0};
	if(g_DM.getGiftInfoForId(giftId,&gi))
	{
		jclass cls_giftinfo = env->FindClass("com/jni/netutil/GiftInfo");
		
		jobject objGift = env->AllocObject(cls_giftinfo);
		if(!objGift)
			return NULL;

		GETJAVAFIELDID(id,cls_giftinfo,"I");
		GETJAVAFIELDID(order,cls_giftinfo,"I");
		GETJAVAFIELDID(name,cls_giftinfo,"Ljava/lang/String;");
		GETJAVAFIELDID(image_name,cls_giftinfo,"Ljava/lang/String;");
		GETJAVAFIELDID(image_url,cls_giftinfo,"Ljava/lang/String;");
		GETJAVAFIELDID(unit_name,cls_giftinfo,"Ljava/lang/String;");
		GETJAVAFIELDID(cash,cls_giftinfo,"I");
		GETJAVAFIELDID(coin,cls_giftinfo,"I");
		GETJAVAFIELDID(effectId,cls_giftinfo,"I");
		GETJAVAFIELDID(gift_type,cls_giftinfo,"I");

		int len = 0;
		jstring name = env->NewStringUTF(gi.name);
		jstring image_name = env->NewStringUTF(gi.image_name);
		jstring image_url = env->NewStringUTF(gi.image_url);
		jstring unit_name = env->NewStringUTF(gi.unit_name);

		env->SetIntField(objGift,jfid_id,gi.id);
		env->SetIntField(objGift,jfid_order,gi.order);
		env->SetObjectField(objGift,jfid_name,name);
		env->SetObjectField(objGift,jfid_image_name,image_name);
		env->SetObjectField(objGift,jfid_image_url,image_url);
		env->SetIntField(objGift,jfid_cash,gi.cash);
		env->SetIntField(objGift,jfid_coin,gi.coin);
		env->SetObjectField(objGift,jfid_unit_name,unit_name);
		env->SetIntField(objGift,jfid_effectId,gi.effectId);
		env->SetIntField(objGift,jfid_gift_type,gi.gift_type);

		env->DeleteLocalRef(name);
		env->DeleteLocalRef(image_name);
		env->DeleteLocalRef(image_url);
		env->DeleteLocalRef(unit_name);

		return objGift;
	}
	else
		return NULL;
}

CJniResponse* CJniResponse::getSingleton()
{
	static CJniResponse s_instance;
	return &s_instance;
}

CJniResponse::CJniResponse(void)
	:m_jvm(NULL)
	,m_obj(NULL)
	,m_nLobbyId(0)
	,m_pDataMgr(NULL)
	,m_nTempCount(0)
	,m_nTempCur(0)
	,m_nGroupVersionID(0)
	,m_bRequestUpdateGroupRoomlist(false)
{
	m_pDataMgr = &g_DM;
}

CJniResponse::~CJniResponse(void)
{
	m_jvm = NULL;
}

//初始化Java环境变量，用于线程的回调，
bool CJniResponse::initJniEnv(JNIEnv* env, jobject obj)
{
	if(env->GetJavaVM(&m_jvm) != 0)
		return false;
	m_obj = env->NewGlobalRef(obj);

	initMapClass(env);

	return !!m_obj;
}
//回收资源
void CJniResponse::uninitJniEnv(JNIEnv* env){
	//回收字典资源
	MAPSTRCLASS::iterator it = m_mapClass.begin();
	for(;it!=m_mapClass.end();it++)
	{
		if(it->second)
			env->DeleteGlobalRef(it->second);
	}
	m_mapClass.clear();
	//回收对象资源
	if(m_obj)
	{
		env->DeleteGlobalRef(m_obj);
		m_obj = NULL;
	}
}

//只能在java线程中使用
jobject CJniResponse::getRoomInfo(JNIEnv* env,int roomid)
{
	 jclass cls_rdri = env->FindClass("com/jni/netutil/ResultData_RoomInfo");

	 GETJAVAFIELDID(idx,cls_rdri,"I");
	 GETJAVAFIELDID(hallid,cls_rdri,"I");
	 GETJAVAFIELDID(name,cls_rdri,"Ljava/lang/String;");
	 GETJAVAFIELDID(pwdflag,cls_rdri,"B");
	 GETJAVAFIELDID(type,cls_rdri,"I");
	 GETJAVAFIELDID(state,cls_rdri,"I");
	 GETJAVAFIELDID(masterid,cls_rdri,"I");
	 GETJAVAFIELDID(createdate,cls_rdri,"Ljava/lang/String;");
	 GETJAVAFIELDID(sortflag,cls_rdri,"Ljava/lang/String;");
	 GETJAVAFIELDID(maxusernum,cls_rdri,"I");
	 GETJAVAFIELDID(deadline,cls_rdri,"Ljava/lang/String;");
	 GETJAVAFIELDID(ispassaudit,cls_rdri,"Ljava/lang/String;");
	 GETJAVAFIELDID(introduce,cls_rdri,"Ljava/lang/String;");
	 GETJAVAFIELDID(ip_1,cls_rdri,"Ljava/lang/String;");
	 GETJAVAFIELDID(port,cls_rdri,"S");
	 GETJAVAFIELDID(ip_2,cls_rdri,"Ljava/lang/String;");
	 GETJAVAFIELDID(content,cls_rdri,"Ljava/lang/String;");
	 GETJAVAFIELDID(updatedate,cls_rdri,"Ljava/lang/String;");
	 GETJAVAFIELDID(logo,cls_rdri,"Ljava/lang/String;");
	 GETJAVAFIELDID(chatflag,cls_rdri,"I");
	 GETJAVAFIELDID(inoutflag,cls_rdri,"I");
	 GETJAVAFIELDID(useronlyflag,cls_rdri,"I");
	 GETJAVAFIELDID(closeflag,cls_rdri,"I");

	 jobject obj = NULL;
	 ROOM_DATA* pRoomData = m_pDataMgr->getRoomInfo(roomid);
	 if(pRoomData)
	 {
		 obj = env->AllocObject(cls_rdri);
		 if(!obj)
			 return obj;

		 int len = 0;
		 GETUNICODEFROMGBK(name,pRoomData->base.name);
		 GETUNICODEFROMGBK(createdate,pRoomData->base.createdate);
		 GETUNICODEFROMGBK(sortflag,pRoomData->base.sortflag);
		 GETUNICODEFROMGBK(deadline,pRoomData->base.deadline);
		 GETUNICODEFROMGBK(ispassaudit,pRoomData->base.ispassaudit);
		 GETUNICODEFROMGBK(introduce,pRoomData->base.introduce);
		 GETUNICODEFROMGBK(ip_1,pRoomData->base.ip_1);
		 GETUNICODEFROMGBK(ip_2,pRoomData->base.ip_2);
		 GETUNICODEFROMGBK(content,pRoomData->base.content);
		 GETUNICODEFROMGBK(updatedate,pRoomData->base.updatedate);
		 GETUNICODEFROMGBK(logo,pRoomData->base.logo);

		 env->SetIntField(obj,jfid_idx,pRoomData->base.idx);
		 env->SetIntField(obj,jfid_hallid,pRoomData->base.hallid);
		 env->SetObjectField(obj,jfid_name,name);
		 env->SetByteField(obj,jfid_pwdflag,pRoomData->base.pwdflag);
		 env->SetIntField(obj,jfid_type,pRoomData->base.type);
		 env->SetIntField(obj,jfid_state,pRoomData->base.state);
		 env->SetIntField(obj,jfid_masterid,pRoomData->base.masterid);
		 env->SetObjectField(obj,jfid_createdate,createdate);
		 env->SetObjectField(obj,jfid_sortflag,sortflag);
		 env->SetIntField(obj,jfid_maxusernum,pRoomData->base.maxusernum);
		 env->SetObjectField(obj,jfid_deadline,deadline);
		 env->SetObjectField(obj,jfid_ispassaudit,ispassaudit);
		 env->SetObjectField(obj,jfid_introduce,introduce);
		 env->SetObjectField(obj,jfid_ip_1,ip_1);
		 env->SetShortField(obj,jfid_port,pRoomData->base.port);
		 env->SetObjectField(obj,jfid_ip_2,ip_2);
		 env->SetObjectField(obj,jfid_content,content);
		 env->SetObjectField(obj,jfid_updatedate,updatedate);
		 env->SetObjectField(obj,jfid_logo,logo);
		 env->SetIntField(obj,jfid_chatflag,pRoomData->base.chatflag);
		 env->SetIntField(obj,jfid_inoutflag,pRoomData->base.inoutflag);
		 env->SetIntField(obj,jfid_useronlyflag,pRoomData->base.useronlyflag);
		 env->SetIntField(obj,jfid_closeflag,pRoomData->base.closeflag);

		 env->DeleteLocalRef(name);
		 env->DeleteLocalRef(createdate);
		 env->DeleteLocalRef(sortflag);
		 env->DeleteLocalRef(deadline);
		 env->DeleteLocalRef(ispassaudit);
		 env->DeleteLocalRef(introduce);
		 env->DeleteLocalRef(ip_1);
		 env->DeleteLocalRef(ip_2);
		 env->DeleteLocalRef(content);
		 env->DeleteLocalRef(updatedate);
		 env->DeleteLocalRef(logo);
	 }

	 return obj;
}

//存入class 字典，用于多线程使用
void CJniResponse::initMapClass(JNIEnv* env)
{
	//对回调线程需要使用的类，需要提前在这里获取该类的类域，以便使用,使用宏函数，参数是所需类的类Signature
	SAVEINMAPCLASS("com/jni/netutil/ResultData_Channel");
	SAVEINMAPCLASS("com/jni/netutil/ResultData_GetChannellist");
	//SAVEINMAPCLASS("com/jni/netutil/ResultData_RoomIPPort");
	//SAVEINMAPCLASS("com/jni/netutil/ResultData_LobbyLogin");
	SAVEINMAPCLASS("com/jni/netutil/LocalData_UserInfo");
	SAVEINMAPCLASS("com/jni/netutil/ResultData_SendFlowerInfo");
	SAVEINMAPCLASS("com/jni/netutil/ResultData_SongList");
	SAVEINMAPCLASS("com/jni/netutil/ResultData_UserSampleInfo");
	SAVEINMAPCLASS("com/jni/netutil/ResultData_SelfData");
	SAVEINMAPCLASS("com/jni/netutil/ResultData_UIdxWithType");
	SAVEINMAPCLASS("com/jni/netutil/ResultData_HBDetail");
	SAVEINMAPCLASS("com/jni/netutil/ResultData_RoomHB");
}

//在native代码中，如果线程需要使用jnienv环境变量，需要先附加到当前线程，注：jnienv只在本线程使用
bool CJniResponse::attachCurThread(unsigned int threadid){
	JNIEnv* env = NULL;
#ifdef _WIN32
	void** pEnv = (void**)&env;
	if(JNI_OK != m_jvm->AttachCurrentThread(pEnv,NULL))
#else
	if(JNI_OK != m_jvm->AttachCurrentThread(&env,NULL))
#endif
		return false;

	m_mapEnv[threadid] = env;

	return true;
}
//线程退出时务必分离
bool CJniResponse::detachCurThread(){
	return (JNI_OK == m_jvm->DetachCurrentThread());
}
//获取线程的java环境变量
bool CJniResponse::getJniByThreadID(unsigned int threadid,JNIEnv** penv){
	MAPINTJNIENV::iterator it = m_mapEnv.find(threadid);
	if(it == m_mapEnv.end())
		return false;
	*penv = it->second;
	return true;
}

/*			add by lixin 2011/10/13 <begin> 聊天、喇叭功能			*/
void CJniResponse::SendChatFromUser(int ret, int to_idx, const char *content ,int contentlen, int time ){}
//接收私聊
void CJniResponse::RecvChatFromUser(int ret, int from_idx, const char *content ,int contentlen, int time ){}
//接收公聊（俱乐部内部）
void CJniResponse::RecvChatFromClub(int ret, int from_idx, const char* content,int contentlen){}
//接收公聊（房间内）
//ret:1--正常 0--被禁言 -1--不允许公聊
void CJniResponse::RecvChatFromRoom(int ret, int errcode,int from_idx, const char* content,int contentlen, int time ){}	
//接收大喇叭（全区）
void CJniResponse::RecvSpeakerFromAll(int ret, int type, int from_id, const char* msg, int msglen){}
//接收小喇叭（大厅）
void CJniResponse::RecvSpeakerFromLobby(int ret, ResultData_UserSpeakerInfo* spkInfo,int type,bool isSend){
	if(!spkInfo)
		return ;

	CALLBACKJAVABEGIN(RecvSpeakerFromLobby,"(IILjava/lang/String;Ljava/lang/String;IIZ)V");

	char roomname[260] = {0};
	if(spkInfo->roomId != 0)
		strcpy(roomname,spkInfo->roomName);
	int len = 0;
	GETUNICODEFROMGBK(jstrRoomname,roomname);
	//LOGI("%s\n",spkInfo->chat);
	jstring jstrMsg = NULL;
	if(type == BT_BROAD)
	{
		jstrMsg = env->NewStringUTF(spkInfo->chat);
	}
	else
	{
		GETUNICODEFROMGBK(tjstrMsg,spkInfo->chat);
		jstrMsg = tjstrMsg;
	}
	jboolean jb = isSend?1:0;

	env->CallVoidMethod(m_obj,jmRecvSpeakerFromLobby,ret,spkInfo->fromidx,jstrRoomname
			,jstrMsg,spkInfo->time,type,jb);

	env->DeleteLocalRef(jstrMsg);
	env->DeleteLocalRef(jstrRoomname);

	CALLBACKJAVAEND;
}
//接收礼物广播
void CJniResponse::RecvPresentBroad(int ret, ResultData_GiftInfo* guestGiftInfo, ResultData_GiftInfo* commGiftInfo){}
//接受系统文字广播
void CJniResponse::RecvSystemBroad( char type, const char * msg, int len ){}
//接受系统文字广播（幸运中奖信息）
void CJniResponse::RecvSystemBroad2(const char* nickname, int len, int idx, int roomid, int giftid, int luckmultiple){
	CALLBACKJAVABEGIN(RecvSystemBroad2,"(Ljava/lang/String;IIII)V");

	int len = 0;
	GETUNICODEFROMGBK(jstrName,nickname);
	env->CallVoidMethod(m_obj,jmRecvSystemBroad2,jstrName,idx,roomid,giftid,luckmultiple);
	env->DeleteLocalRef(jstrName);

	CALLBACKJAVAEND;
}
//接受系统文字公告
void CJniResponse::RecvSystemNotice( const char * msg, int len, bool once, int min ){}
//接受系统文字公告（幸运中奖信息）
void CJniResponse::RecvSystemNotice2(const char* nickname, int len, int idx, int roomid, int giftid, int luckmultiple){}
//接收房间临时公告
void CJniResponse::RecvRoomNotice(int ret, const char* content,int contentlen){}
/************************************************************************/
/*			add by jinguanfu 2009/12/28 <begin> 好友操作				*/
//取得好友位置
void CJniResponse::RecvFriendLocation(int ret, int errcode,
							int friend_idx,
							int lobbyid, 
							int roomid,
							int cseq){}
/*			add by jinguanfu 2009/12/28 <begin> 房间管理接口			*/
//接收取得待审核房间会员列表
void CJniResponse::RecvRoomApplyList (int ret ,int errcode,ResultDate_RoomApplyList *applylist,int cseq){}
//接收申请房间会员响应
void CJniResponse::RecvApplyRoomMember(int ret,int errcode,int cseq){}
//接收审核会员加入申请结果
void CJniResponse::RecvAuditAddApply (int ret, int errcode,int idx,int cseq){}
//接收拒绝房间申请系统提示
void CJniResponse::RecvRoomApplyRefuseMsg (int ret, int roomid){}
//接收房间会员列表
void CJniResponse::RecvRoomMemberList (int ret ,int errcode, ResultDate_RoomMemList* memlist,int cseq){}
//接收解除房间会员资格回应
void CJniResponse::RecvUnchainMember (int ret, int errcode, int idx,int cseq){}
//接收赋予用户房间主持助理权限回应
void CJniResponse::RecvRoomPesideAssistant (int ret , int errcode, int idx,int cseq){}
//接收赋予主持助理通知
void CJniResponse::RecvRoomNotice4PesideAssistant (int ret , int opidx,int bopidx){}
//接收赋予用户房间主持人权限回应
void CJniResponse::RecvRoomPeside (int ret ,int errcode, int idx,int cseq){}
//接收设置主持人权限房间通知
void CJniResponse::RecvRoomNotice4Peside (int ret , int opidx,int bopidx){}
//接收赋予用户房间副室主权限回应
void CJniResponse::RecvRoomSubHost (int ret ,int errcode, int idx,int cseq){}
//接收设置副室主权限房间通知
void CJniResponse::RecvRoomNotice4SubHost(int ret , int opidx,int bopidx){}
//接收房间黑名单
void CJniResponse::RecvBlackList(int ret,	int errcode,ResultDate_RoomBlackList* blacklist,int cseq){}
//接收黑名单用户添加/删除回应
void CJniResponse::RecvAddDelBlack(int ret , int errcode,int idx, int type){}
//接收房间设置面板信息
void CJniResponse::RecvRoomPanelInfo(int ret ,int errcode, ResultDate_RoomPanel* panelinfo){}
//接收设置房间密码回应
void CJniResponse::RecvRoomPassword(int ret,int errcode,int cseq){}
//接收设置房间密码消息提示
void CJniResponse::RecvRoomNotice4Password(int ret, int idx, char pwdflag){}
//接收设置房间开启/关闭(上锁)回应
void CJniResponse::RecvRoomSwitch(int ret,int errcode,char roomflag,int cseq){}
//接收房间开启/关闭(上锁)消息提示
void CJniResponse::RecvRoomNotice4RoomSwitch(int ret , int idx , char roomflag){}
//接收设置房间访问权限回应
void CJniResponse::RecvRoomPrivate(int ret , int errcode,char accessflag,int cseq){}
//接收房间访问权限消息提示
void CJniResponse::RecvRoomNotice4Private(int ret , int idx , char accessflag){}
//接收设置用户进出信息开启/关闭回应
void CJniResponse::RecvUserInOutSwitch(int ret , int errcode, char inoutflag,int cseq){}
//接收用户进出信息开启/关闭消息提示
void CJniResponse::RecvRoomNotice4UserSwitch (int ret , int idx , char inoutflag){}
//接收设置自由排麦开启/关闭回应
void CJniResponse::RecvGetMicSwitch (int ret, int errcode,char onmicflag,int cseq){}
//接收设置自由上麦开启/关闭回应
void CJniResponse::RecvOnMicSwitch (int ret, int errcode,char onmicflag,int cseq){}
//接收自由排麦开启/关闭消息提示
void CJniResponse::RecvRoomNotice4GetMicSwitch(int ret , int idx , char getmicflag){}
//接收自由上麦开启/关闭消息提示
void CJniResponse::RecvRoomNotice4OnMicSwitch(int ret , int idx , char onmicflag){}
//房间名称修改回应
void CJniResponse::RecvModifyRoomName(int ret,int errcode,int cseq){}
//接收房间名
void CJniResponse::RecvRoomName (int ret , int roomid , const char* roomname , int rnlen){}
//设置房间公告响应
void CJniResponse::RecvRoomAffiche(int ret,int errcode,int cseq){}
//接收房间公告更新事件
void CJniResponse::RecvRoomAfficheEvent(int ret){}
//设置允许房间公聊响应
void CJniResponse::RecvAllowRoomAllChat (int ret, int errcode,char chatflag,int cseq){}
//接收房间公聊设置消息提示
void CJniResponse::RecvRoomNotice4AllChat(int ret , int idx , char chatflag){}
//设置房间欢迎词响应
void CJniResponse::RecvRoomSalutatory(int ret, int errcode,int cseq){}
//接收房间欢迎词更新通知
void CJniResponse::RecvRoomNotice4Salutatory(int ret,int idx){}
//设置房间logo响应
void CJniResponse::RecvRoomLogo (int ret,int errcode ,int cseq){}
//接收房间LOGO更新通知
void CJniResponse::RecvRoomNotice4Logo(int ret,int idx){}
//接收房间冻结消息提示
void CJniResponse::RecvRoomNotice4Lock(int idx){}
//接受创建房间回应
void CJniResponse::RecvCreateRoom( int ret, int errcode, RoomListEx * info ){}
//接受练歌房完全信息
void CJniResponse::RecvTmpRoomInfo( int ret, int errcode, RoomListEx * info ){}
// 添加歌房广播
void CJniResponse::RecvRoomNotice4Add( RoomListEx * info ){}
void CJniResponse::RecvRoomNotice4Add( int idx ){}
// 删除歌房广播
void CJniResponse::RecvRoomNotice4Del( int idx ){}
// 接收房间易主通知
void CJniResponse::RecvRoomNotice4ChangeMaster(int idx){}
// 接收房间信息更改通知
void CJniResponse::RecvRoomNotice4ChangeInfo( int idx ){}
/*			add by jinguanfu 2009/12/28 <begin> 角色信息				*/
//取得他人角色信息
void CJniResponse::RecvPartInfo(int ret, int errcode, ResultData_PartInfo * part_info, int cseq){
	//m_pDataMgr->savePartInfo(part_info);
}
//取得自己角色信息,
void CJniResponse::RecvPartInfoBySelf(int ret, int errcode, ResultData_PartInfo * part_info, int cseq){
	//登陆大厅最后一步，本来是还要获取背包信息，去掉
	CALLBACKJAVABEGIN(OnLoginLobby,"(IILcom/jni/netutil/LocalData_UserInfo;)V");
	jint jret = ret;
	jint jerrcode = errcode;
	jint jcseq = cseq;

	USER_DATA_BASE* pMy = m_pDataMgr->getMyInfo();
	jobject objUserInfo = GetUserInfoCom(env,m_mapClass["com/jni/netutil/LocalData_UserInfo"],pMy,true);

	env->CallVoidMethod(m_obj,jmOnLoginLobby,jret,jerrcode,objUserInfo,jcseq);

	env->DeleteLocalRef(objUserInfo);

	CALLBACKJAVAEND;
}
/*			add by jinguanfu 2010/1/5 <begin> 取得房间列表				*/
void CJniResponse::RecvRoomList(int ret,ResultDate_RoomList* roomlist){}
void CJniResponse::RecvRoomListEx(int ret,ResultDate_RoomListEx * roomlist){}
//打分通知
//void CJniResponse::RecvNotice4Score(int onmic_idx,int onmic_score,int pk_idx,int pk_score){}
//void CJniResponse::RecvNotice4ScoreAdd(int epchange, int silverchange){}
//下麦通知
void CJniResponse::RecvNotice4OffMic( int onmic_idx, int pk_idx, int exp, const char * scoreinfo, int len ){}
// add by cx 10-6-2
//回应是否邀请上麦成功
void CJniResponse::RecvRoomGivOnevjMic( int idx, char flag ){}
//是否同意邀请上麦成功
void CJniResponse::RecvRoomGivOnevjMicApply( int formidx ){}
//add by jinguanfu 2010/1/20 <begin> 
//是否同意添加对方为好友
//参数说明：int idx 添加好友者idx
void CJniResponse::RecvAddFriendApply(int idx){}
//add by jinguanfu 2010/1/20 <end> 
//void CJniResponse::RecvDeleteAvarta(int ret){}
//add by jinguanfu 2010/3/4 原有回调API改成接口<begin>
//连接成功
//成功连接认证服务器
void CJniResponse::OnAuthTunnelConnectSuccess(){
	CALLBACKJAVAVOID(OnAuthTunnelConnectSuccess,"()V");
}
//成功连接大厅服务器
void CJniResponse::OnLobbyTunnelConnectSuccess(){
	CALLBACKJAVAVOID(OnLobbyTunnelConnectSuccess,"()V");
}	
//成功连接房间服务器
void CJniResponse::OnRoomTunnelConnectSuccess(){
	CALLBACKJAVAVOID(OnRoomTunnelConnectSuccess,"()V");
}	
//成功连接频道服务器
void CJniResponse::OnChannellistTunnelConnectSuccess(){
	CALLBACKJAVAVOID(OnChannellistTunnelConnectSuccess,"()V");
}	
//连接超时
//连接认证服务器超时
void CJniResponse::OnAuthTunnelConnectTimeout(){
	CALLBACKJAVAVOID(OnAuthTunnelConnectTimeout,"()V");
}		
//连接大厅服务器超时
void CJniResponse::OnLobbyTunnelConnectTimeout(){
	CALLBACKJAVAVOID(OnLobbyTunnelConnectTimeout,"()V");
}		
//连接房间服务器超时
void CJniResponse::OnRoomTunnelConnectTimeout(){
	CALLBACKJAVAVOID(OnRoomTunnelConnectTimeout,"()V");
}	
//连接频道服务器超时
void CJniResponse::OnChannellistTunnelConnectTimeout(){
	CALLBACKJAVAVOID(OnChannellistTunnelConnectTimeout,"()V");
}
//连接错误
void CJniResponse::OnAuthTunnelConnectError( int code ){
	CALLBACKJAVABEGIN(OnAuthTunnelConnectError,"(I)V");
	jint jcode = code;
	env->CallVoidMethod(m_obj,jmOnAuthTunnelConnectError,jcode);
	CALLBACKJAVAEND;
}	
void CJniResponse::OnLobbyTunnelConnectError( int code ){
	CALLBACKJAVABEGIN(OnLobbyTunnelConnectError,"(I)V");
	jint jcode = code;
	env->CallVoidMethod(m_obj,jmOnLobbyTunnelConnectError,jcode);
	CALLBACKJAVAEND;
}
void CJniResponse::OnRoomTunnelConnectError( int code ){
	CALLBACKJAVABEGIN(OnRoomTunnelConnectError,"(I)V");
	jint jcode = code;
	env->CallVoidMethod(m_obj,jmOnRoomTunnelConnectError,jcode);
	CALLBACKJAVAEND;
}
void CJniResponse::OnChannellistTunnelConnectError( int code ){
	CALLBACKJAVABEGIN(OnChannellistTunnelConnectError,"(I)V");
	jint jcode = code;
	env->CallVoidMethod(m_obj,jmOnChannellistTunnelConnectError,jcode);
	CALLBACKJAVAEND;
}
//服务器主动断开的连接,客户端recv == 0的时候,回调到以下的接口
//断开认证服务器
void CJniResponse::OnAuthTunnelClose(){
	CALLBACKJAVAVOID(OnAuthTunnelClose,"()V");
}	
//断开大厅服务器
void CJniResponse::OnLobbyTunnelClose(){
	CALLBACKJAVAVOID(OnLobbyTunnelClose,"()V");
}	
//断开房间服务器
void CJniResponse::OnRoomTunnelClose(){
	CALLBACKJAVAVOID(OnRoomTunnelClose,"()V");
}	
//断开频道服务器
void CJniResponse::OnChannellistTunnelClose(){
	CALLBACKJAVAVOID(OnChannellistTunnelClose,"()V");
}
//客户端recv异常,send异常,网络层buf溢出,select出现问题,都会回调到这个以下接口
void CJniResponse::OnAuthTunnelError( int code ){
	CALLBACKJAVABEGIN(OnAuthTunnelError,"(I)V");
	jint jcode = code;
	env->CallVoidMethod(m_obj,jmOnAuthTunnelError,jcode);
	CALLBACKJAVAEND;
}	
void CJniResponse::OnLobbyTunnelError( int code ){
	CALLBACKJAVABEGIN(OnLobbyTunnelError,"(I)V");
	jint jcode = code;
	env->CallVoidMethod(m_obj,jmOnLobbyTunnelError,jcode);
	CALLBACKJAVAEND;
}
void CJniResponse::OnRoomTunnelError( int code ){
	CALLBACKJAVABEGIN(OnRoomTunnelError,"(I)V");
	jint jcode = code;
	env->CallVoidMethod(m_obj,jmOnRoomTunnelError,jcode);
	CALLBACKJAVAEND;
}
void CJniResponse::OnChannellistTunnelError( int code ){
	CALLBACKJAVABEGIN(OnChannellistTunnelError,"(I)V");
	jint jcode = code;
	env->CallVoidMethod(m_obj,jmOnChannellistTunnelError,jcode);
	CALLBACKJAVAEND;
}

/******************认证服务器操作接口*****************************/
//接收认证结果
void CJniResponse::RecvAuth(int ret, int errcode,int again){
	CALLBACKJAVABEGIN(RecvAuth,"(III)V");
	env->CallVoidMethod(m_obj,jmRecvAuth,ret,errcode,again);
	CALLBACKJAVAEND;
}
//接收新token
//void CJniResponse::RecvNewToken(int ret,ReserveDataS* pRD){}
void CJniResponse::RecvNewToken(int ret,int errcode){}
//接收频道列表
void CJniResponse::RecvSectionList(int ret,int errcode,ResultData_GetChannellist* pRDc,int cseq){
	CALLBACKJAVABEGIN(RecvSectionList,"(IILcom/jni/netutil/ResultData_GetChannellist;I)V");

	if(ret == -1)
	{
		//调用java中的方法
		env->CallVoidMethod(m_obj,jmRecvSectionList,ret,errcode,NULL,cseq);
	}
	else
	{
		jobjectArray aRd_Channel = NULL;
		jclass cls_Channel = m_mapClass["com/jni/netutil/ResultData_Channel"];

		//获得类 域ID
		//jfieldID jfid_sectionid = env->GetFieldID(cls_Channel,"sectionid","I");
		//统一前缀jfid_
		GETJAVAFIELDID(sectionid,cls_Channel,"I");
		GETJAVAFIELDID(sectionname,cls_Channel,"Ljava/lang/String;");
		GETJAVAFIELDID(lobbyid,cls_Channel,"I");
		GETJAVAFIELDID(lobbyname,cls_Channel,"Ljava/lang/String;");
		GETJAVAFIELDID(lobbyip_1,cls_Channel,"Ljava/lang/String;");
		GETJAVAFIELDID(lobbyport,cls_Channel,"S");
		GETJAVAFIELDID(lobbyip_2,cls_Channel,"Ljava/lang/String;");
		//放到数组中
		aRd_Channel = env->NewObjectArray(pRDc->count,cls_Channel,0);
		for (int i=0;i<pRDc->count;i++)
		{
			jobject tmpObj = env->AllocObject(cls_Channel);

			int len = 0;
			jint sectionid = pRDc->pCh[i].sectionid;
			GETUNICODEFROMGBK(sectionname,pRDc->pCh[i].sectionname);
			jint lobbyid = pRDc->pCh[i].lobbyid;
			GETUNICODEFROMGBK(lobbyname,pRDc->pCh[i].lobbyname);
			jstring lobbyip_1 = env->NewStringUTF(pRDc->pCh[i].lobbyip_1);
			jshort lobbyport = pRDc->pCh->lobbyport;
			jstring lobbyip_2 = env->NewStringUTF(pRDc->pCh[i].lobbyip_2);

			env->SetIntField(tmpObj,jfid_sectionid,sectionid);
			env->SetObjectField(tmpObj,jfid_sectionname,sectionname);
			env->SetIntField(tmpObj,jfid_lobbyid,lobbyid);
			env->SetObjectField(tmpObj,jfid_lobbyname,lobbyname);
			env->SetObjectField(tmpObj,jfid_lobbyip_1,lobbyip_1);
			env->SetShortField(tmpObj,jfid_lobbyport,lobbyport);
			env->SetObjectField(tmpObj,jfid_lobbyip_2,lobbyip_2);

			env->SetObjectArrayElement(aRd_Channel,i,tmpObj);//推入数组

			env->DeleteLocalRef(lobbyip_2);
			env->DeleteLocalRef(lobbyip_1);
			env->DeleteLocalRef(lobbyname);
			env->DeleteLocalRef(sectionname);

			env->DeleteLocalRef(tmpObj);
		}

		jclass cls_ResultData_GetChannellist = m_mapClass["com/jni/netutil/ResultData_GetChannellist"];
		GETJAVAFIELDID(count,cls_ResultData_GetChannellist,"I");
		GETJAVAFIELDID(Ch,cls_ResultData_GetChannellist,"[Lcom/jni/netutil/ResultData_Channel;");
		jobject rd_channellist = env->AllocObject(cls_ResultData_GetChannellist);
		env->SetIntField(rd_channellist,jfid_count,pRDc->count);
		env->SetObjectField(rd_channellist,jfid_Ch,aRd_Channel);

		//调用java中的方法
		env->CallVoidMethod(m_obj,jmRecvSectionList,ret,errcode,rd_channellist,cseq);
		
		env->DeleteLocalRef(rd_channellist);
		env->DeleteLocalRef(aRd_Channel);
	}
	CALLBACKJAVAEND;
}
//接收频道列表(添加防沉迷信息)
void CJniResponse::RecvSectionList2(int ret,int errcode,int leftlogintime,ResultData_GetChannellist* pRDc,int cseq){}
/******************大厅服务器操作接口*****************************/
//接收大厅token认证
void CJniResponse::RecvTokenAuth4Lobby( int ret, int errcode, ResultData_LobbyLogin * pRDl, ResultData_SelfData * pSD, int cseq ){
	CALLBACKJAVABEGIN(RecvTokenAuth4Lobby,"(IILcom/jni/netutil/ResultData_LobbyLogin;Lcom/jni/netutil/ResultData_SelfData;I)V");

	jint jret = ret;
	jint jerrcode = errcode;
	jint jcseq = cseq;

	//回调java返回结果，数据仍然后台处理,
	env->CallVoidMethod(m_obj,jmRecvTokenAuth4Lobby,jret,jerrcode,NULL,NULL,jcseq);

	//大厅认证成功
	if(ret == 0 && pRDl)
	{
		//check the group info if need to update
		m_nGroupVersionID = pRDl->group_versionid;
		assert(m_pDataMgr != NULL);
		CServerData* pSD = m_pDataMgr->m_pServerData;
		assert(pSD != NULL);
		if(pSD->getVersion(m_nLobbyId) < m_nGroupVersionID)
		{
			if(m_bRequestUpdateGroupRoomlist)//已经在更新中，等待完成
				return ;

			LOGI("%s : need to update the group info\n",__FUNCTION__);
			pSD->clearRoom();
			pSD->clearGroup();

			if(0 != Java_com_jni_netutil_CJniUtil_GetGroupInfo(NULL,NULL))
				LOGE("%s : Java_com_jni_netutil_CJniUtil_GetGroupInfo failed\n",__FUNCTION__);
		}
		else//don't update the group use local data
		{
			if(0 != Java_com_jni_netutil_CJniUtil_GetPartInfo(NULL,NULL,m_pDataMgr->getMyIdx(),m_pDataMgr->getMyIdx()))
				LOGE("%s : Java_com_jni_netutil_CJniUtil_GetPartInfo failed\n",__FUNCTION__);
		}
	}
	CALLBACKJAVAEND;
}
//用户登录大厅通知
void CJniResponse::RecvUserLoginLobbyNotice(int ret,ResultData_LobbyLoginUser* pRlu){
	if(!pRlu)
		return ;
	CALLBACKJAVABEGIN(RecvUserLoginLobbyNotice,"(IICI)V");
	env->CallVoidMethod(m_obj,jmRecvUserLoginLobbyNotice,ret,pRlu->idx,pRlu->relation,pRlu->lobbyid);
	CALLBACKJAVAEND;
}
//用户退出大厅通知
void CJniResponse::RecvUserLogoutLobbyNotice(int ret,ResultData_LobbyLoginUser* pRlu){
	if(!pRlu)
		return ;
	CALLBACKJAVABEGIN(RecvUserLogoutLobbyNotice,"(IICI)V");
	env->CallVoidMethod(m_obj,jmRecvUserLogoutLobbyNotice,ret,pRlu->idx,pRlu->relation,pRlu->lobbyid);
	CALLBACKJAVAEND;
}
//接收用户信息
void CJniResponse::RecvUserInfo(int ret,int errcode,ResultData_UserFullInfo* pRDu){
	if(!pRDu)
		return ;
	CALLBACKJAVABEGIN(RecvUserInfo,"(IILcom/jni/netutil/ResultData_UserFullInfo;)V");
	env->CallVoidMethod(m_obj,jmRecvUserInfo,ret,pRDu->m_idx,NULL);
	CALLBACKJAVAEND;
}
//接收好友状态
void CJniResponse::RecvFriendStatus(int ret,int errcode,ResultData_GetFriendlist* pRD){}
//接收黑名单状态
void CJniResponse::RecvBlackStatus(int ret,int errcode,ResultData_GetFriendlist* pRD){}
//接收俱乐部状态
void CJniResponse::RecvClubStatus(int ret,int errcode,ResultData_GetClublist* pRD){}
//接收添加好友结果
void CJniResponse::RecvAddFriend(int ret,int errcode,int idx){}
//接收添加黑名单结果
void CJniResponse::RecvAddBlack(int ret,int errcode,int cseq){}
//接收删除好友结果
void CJniResponse::RecvDelFriend(int ret,int errcode,int cseq){}
//接收删除黑名单结果
void CJniResponse::RecvDelBlack(int ret,int errcode,int cseq){}
//接收申请加入俱乐部结果
void CJniResponse::RecvApplyClub(int ret,int errcode,int cseq){}
//接收退出俱乐部结果
void CJniResponse::RecvExitClub(int ret,int errcode,int cseq){}
//接收最近几天内的加入俱乐部申请数量
void CJniResponse::RecvApplyListCount(int ret,int errcode,int count,int cseq){}
//接收最近几天内的加入俱乐部申请列表
void CJniResponse::RecvApplyList(int ret,int errcode,ResultData_ClubApplyList* pRD,int cseq){}
//通过加入俱乐部审批
void CJniResponse::RecvAddClubOk(int ret,ResultData_Approvelresult* pRD){}
//删除申请列表
void CJniResponse::RecvDelApplyList(int ret,ResultData_Approvelresult* pRD){}
//接收加入俱乐部广播消息(当有人加入俱乐部)
//modify by jinguanfu 2010/4/11
//void CJniResponse::RecvAddClubNotice(int ret ,int idx){}
//opidx--操作者idx
//addidx--加入者idx
void CJniResponse::RecvAddClubNoticeByLobby(int ret ,int clubid,const char* clubname,int opidx,int addidx){}
void CJniResponse::RecvAddClubNoticeByRoom( int opidx, int addidx ){}
//接收申请会员拒绝消息
//opidx--审核者idx
void CJniResponse::RecvDenyClubNoticeByLobby(int ret ,int clubid,const char* clubname,int opidx){}
//void CJniResponse::RecvDenyClubNoticeByRoom(int ret ,int clubid,char* clubname,int opidx){}
//接收会员退出俱乐部消息
void CJniResponse::RecvExitClubNoticeByLobby(int ret,int clubid,const char* clubname,int exitidx){}
void CJniResponse::RecvExitClubNoticeByRoom( int exitidx ){}
//接收会员被踢出俱乐部消息
//opidx--操作者idx
//kickidx--被踢出者idx
void CJniResponse::RecvKickClubNoticeByLobby(int ret,int clubid,const char* clubname,int opidx,int kickidx){}
void CJniResponse::RecvKickClubNoticeByRoom( int opidx, int kickidx ){}
//创建俱乐部
void CJniResponse::RecvCreateClub(int ret,int errcode,int clubid,int cseq){}
//解散俱乐部
void CJniResponse::RecvDismissClub(int ret,int errcode,int cseq){}
//接收俱乐部信息
void CJniResponse::RecvClubInfo(int ret,int errcode,ResultData_ClubFullInfo* pRD,int cseq){}
////接收房间列表
//void CJniResponse::RecvRoomList(int ret,ResultData_RoomList* pRD){}
//接收房间信息
void CJniResponse::RecvRoomInfo(int ret,ResultData_RoomInfo* pRD){}
//创建头像
void CJniResponse::RecvCreateAvatar(int ret,int errcode){}
//修改头像
void CJniResponse::RecvChangeAvatar(int ret,int errcode){}
//修改昵称
void CJniResponse::RecvChangeNickName(int ret,int errcode,const char * name, int len){}
//进入房间通知
void CJniResponse::RecvLobbyEnterRoom(int ret,ResultData_LobbyRoomEnter* pRD){}
//离开房间通知
void CJniResponse::RecvLobbyLeaveRoom(int ret,ResultData_LobbyRoomEnter* pRD){}
//添加好友通知
void CJniResponse::RecvAddFriendNotice(int ret,int idx){}
//取得大厅信息
void CJniResponse::RecvLobbyInfo(int ret,int errcode,ResultData_LobbyName* pRD,int cseq){}
//接收来自大厅的权限更改消息 add by jinguanfu 2010/6/5
void CJniResponse::RecvChangePower(int optionidx/*操作者ID*/,char identity/*更改后权限*/){}
//取大厅名(CMD_SYSTEMMSG_L2C)
//void CJniResponse::RecvLobbyName(){}
/******************房间操作接口*****************************/
//接收房间token验证 //
void CJniResponse::RecvTokenAuth4Room(int ret,int errcode){
	CALLBACKJAVABEGIN(RecvTokenAuth4Room,"(II)V");
	env->CallVoidMethod(m_obj,jmRecvTokenAuth4Room,ret,errcode);
	CALLBACKJAVAEND;
}
//进入房间
void CJniResponse::RecvEnterRoom(int ret,int errcode,ResultData_LoginInfo* pRDlf,int cseq){
	CALLBACKJAVABEGIN(RecvEnterRoom,"(IIBBI)V");
	if(ret == 0 && pRDlf)
		env->CallVoidMethod(m_obj,jmRecvEnterRoom,ret,errcode,pRDlf->ret,pRDlf->identity,cseq);
	else
		env->CallVoidMethod(m_obj,jmRecvEnterRoom,ret,errcode,(char)0,(char)0,cseq);
	CALLBACKJAVAEND;
}
//用户进入房间通知
void CJniResponse::RecvEnterRoomNotice(int ret,ResultData_UserInfo* pRDu){

}
//用户离开房间通知
void CJniResponse::RecvLeaveRoomNotice(int ret,int idx){

}
//接收房间详细信息
void CJniResponse::RecvRoomAllInfo(int ret,int errcode,ResultData_RoomAllInfo *pRD,int cseq){}
//接收排麦成功通知
void CJniResponse::RecvWaitMicNotice(int ret,ResultData_MicInfo *pRD){}
////接收上麦准备通知
//void CJniResponse::RecvWaitMicReadyNotice(int ret,ResultData_MicInfo *pRD){}
//接收上麦准备完成通知
//void CJniResponse::RecvOnMicReadyOkNotice(int ret,ResultData_MicInfo *pRD){}
//接收上麦通知
void CJniResponse::RecvOnMicNotice(int ret,ResultData_MicInfo *pRD){}
//取消排麦成功通知
void CJniResponse::RecvCancelWaitMicNotice(int ret,int opidx,int bopidx){}
//接收加入PK麦通知
void CJniResponse::RecvWaitPKMicNotice(int ret,ResultData_MicInfo *pRD){}
//接收取消PK麦通知
void CJniResponse::RecvCancelWaitPKMicNotice(int ret,int opidx,int bopidx){}
//接收升麦成功通知
void CJniResponse::RecvUpWaitMicNotice(int ret,ResultData_UDMicInfo *pRD){}
//接收降麦成功通知
void CJniResponse::RecvDownWaitMicNotice(int ret,ResultData_UDMicInfo *pRD){}
//接收踢出房间通知
void CJniResponse::RecvKickOutNotice(int idx,int bidx){}
//接收上VJ麦（主持麦）通知
void CJniResponse::RecvOnVJMicNotice(int ret,int idx){}
//接收下VJ麦（主持麦）通知
void CJniResponse::RecvOffVJMicNotice(int ret,int idx){}
//接收被下麦通知
void CJniResponse::RecvGiveOffMicNotice(int ret,int idx){}
//接收被禁言通知
void CJniResponse::RecvForbidenNotice(int ret,ResultData_ForbidenInfo* pRD){}
//接收黑名单更新通知
void CJniResponse::RecvUpdateBlackListNotice(int ret,ResultData_UpBlackList* pRD){}
//接收管理员更新通知
void CJniResponse::RecvUpdateMGRListNotice(int ret,ResultData_UpMgrList* pRD){}
//接收赠送礼物通知
void CJniResponse::RecvSendFlowerNotice(int ret,ResultData_SendFlowerInfo* pRD){
	//处理广播礼物消息
	if(!pRD)
		return ;

	CALLBACKJAVABEGIN(RecvSendFlowerNotice,"(ILcom/jni/netutil/ResultData_SendFlowerInfo;)V");

	jclass cls_rdsfi = m_mapClass["com/jni/netutil/ResultData_SendFlowerInfo"];
	jobject objSFI = env->AllocObject(cls_rdsfi);
	if(objSFI)
	{
		GETJAVAFIELDID(s_idx,cls_rdsfi,"I");
		GETJAVAFIELDID(s_roomid,cls_rdsfi,"I");
		GETJAVAFIELDID(s_room,cls_rdsfi,"Ljava/lang/String;");
		GETJAVAFIELDID(r_idx,cls_rdsfi,"I");
		GETJAVAFIELDID(r_roomid,cls_rdsfi,"I");
		GETJAVAFIELDID(r_room,cls_rdsfi,"Ljava/lang/String;");
		GETJAVAFIELDID(cate_idx,cls_rdsfi,"I");
		GETJAVAFIELDID(number,cls_rdsfi,"I");
		GETJAVAFIELDID(leavings,cls_rdsfi,"I");
		GETJAVAFIELDID(time,cls_rdsfi,"I");
		GETJAVAFIELDID(once_num,cls_rdsfi,"I");

		int len = 0;
		GETUNICODEFROMGBK(s_room,pRD->s_room);
		GETUNICODEFROMGBK(r_room,pRD->r_room);

		env->SetIntField(objSFI,jfid_s_idx,pRD->s_idx);
		env->SetIntField(objSFI,jfid_s_roomid,pRD->s_roomid);
		env->SetIntField(objSFI,jfid_r_idx,pRD->r_idx);
		env->SetIntField(objSFI,jfid_r_roomid,pRD->r_roomid);
		env->SetIntField(objSFI,jfid_cate_idx,pRD->cate_idx);
		env->SetIntField(objSFI,jfid_number,pRD->number);
		env->SetIntField(objSFI,jfid_leavings,pRD->leavings);
		env->SetIntField(objSFI,jfid_time,pRD->time);
		env->SetIntField(objSFI,jfid_once_num,pRD->once_num);
		env->SetObjectField(objSFI,jfid_s_room,s_room);
		env->SetObjectField(objSFI,jfid_r_room,r_room);

		env->CallVoidMethod(m_obj,jmRecvSendFlowerNotice,ret,objSFI);

		env->DeleteLocalRef(s_room);
		env->DeleteLocalRef(r_room);
	}
	else
		LOGE("%s : Objcet Alloc Failed\n",__FUNCTION__);

	CALLBACKJAVAEND;
}
//add by jinguanfu 2010/3/4 原有回调API改成接口<end>
//接收房间重复登录消息
void CJniResponse::RecvRoomKickUser(){}
//接收大厅重复登录消息
void CJniResponse::RecvLobbyKickUser(){}
//从大厅接收房间简易信息
void CJniResponse::RecvRoomSimpleInfo(int ret,int errcode,ResultData_RoomSimpleInfoList* pRD){}
void CJniResponse::RecvRoomSimpleInfoEx(int ret,int errcode,ResultData_RoomSimpleInfoList* pRD, int type){}
//接收用户退出房间会员回应
void CJniResponse::RecvExitRoomMember(int ret,int errcode){}
//接收WEB服务器上邮件信息
void CJniResponse::RecvMailInfo(int ret,char* context,int length){}
//接收大厅在线人数
void CJniResponse::RecvLobbyOnlineNum(int ret,int errcode,ResultData_LobbyOnlineList* pRD){}
//接收送礼回应消息	//add by jinguanfu 2010/4/27
void CJniResponse::RecvSendFlower(int ret,int errcode,int recevie_idx,int item_id,int count,int gold,int silver){
	CALLBACKJAVABEGIN(RecvSendFlower,"(IIIIIII)V");
	env->CallVoidMethod(m_obj,jmRecvSendFlower,ret,errcode,recevie_idx,item_id,count,gold,silver);
	CALLBACKJAVAEND;
}
//接收发送大喇叭回应 //add by jinguanfu 2010/5/13
void CJniResponse::RecvSpeakerToAll(int ret,int errcode,int gold,int silver){}
//接收发送小喇叭回应 //add by jinguanfu 2010/5/13
void CJniResponse::RecvSpeakerToLobby(int ret,int errcode,int gold,int silver){
	CALLBACKJAVABEGIN(RecvSpeakerToLobby,"(IIII)V");
	env->CallVoidMethod(m_obj,jmRecvSpeakerToLobby,ret,errcode,gold,silver);
	CALLBACKJAVAEND;
}
//接收由管理员设为一般会员的回应 //add by jinguanfu 2010/5/13
void CJniResponse::RecvRoomMember (int ret , int errcode, int idx,int cseq){}
//接收由管理员设为一般会员的通知 //add by jinguanfu 2010/5/13
void CJniResponse::RecvRoomNotice4Member (int ret , int opidx,int bopidx){}
//接收房间内有人改变了avarat的广播 //add by cx 2010-5-20
void CJniResponse::RecvRoomNotice4ChangeAvarat (int ret, int idx){}
//接收房间内错误操作提示 //add by cx 2010-6-6
void CJniResponse::RecvOneError( short cmd ){}
//add by cx 2010-7-6 接受购买物品回应
void CJniResponse::RecvBuyPack( int ret, int errcode, int gold, int silver, short * goods_id, int * global_id, int count, const char * day ){}
// 接受购买物品回应
void CJniResponse::RecvBuyForSend( int ret, int errcode, int idx, int gold, int silver ){}
//接受获取背包回应
void CJniResponse::RecvGetPack( int ret, int errcode, ResultData_Pack * pPack, int count, int all_count ){}
//接受删除物品回应
void CJniResponse::RecvDelPack( int ret, int errcode, int cseq ){}
//接受商品过期通知
void CJniResponse::RecvTimeoutPack( int count, int * global_id ){}
//接受商品赠送
void CJniResponse::RecvSendPack( int idx, int global_id, short goods_id, const char * day, const char * msg ){}
//接受使用物品回应
void CJniResponse::RecvUsePack( int ret, int errcode, int global_id ){}
// 升级广播
void CJniResponse::RecvNotice4LevelUp( int idx, char level ){}
// 经验提升
void CJniResponse::RecvExpChange( int nexp, int level ){}
// vip改变
void CJniResponse::RecvVipChange( int idx, int level, char is_vip ){}
// 钱币改变
void CJniResponse::UpdataMoney( int r_gold, int r_silver ){
	CALLBACKJAVABEGIN(UpdataMoney,"(II)V");
	env->CallVoidMethod(m_obj,jmUpdataMoney,r_gold,r_silver);
	CALLBACKJAVAEND;
}
// 更新自己的信息
void CJniResponse::RecvUpdataMyInfo(){

}
// 接受烟花广播
void CJniResponse::RecvFireworks( int ret, int errcode, int cate_idx, int count, int gold, int silver ){}
// 接受烟花广播
void CJniResponse::RecvFireworksNotice( int player_idx, int cate_idx, int count ){}
// 中奖？
void CJniResponse::RecvLucky( int player_idx, int cate_idx, int lotte ){}
// 中奖？
void CJniResponse::RecvLucky( int room_idx, int player_idx, int cate_idx, int lotte ){}
// 列表
void CJniResponse::RecvGiftTopList( ResultData_GiftList * gift_list ){}
// 在线时间
void CJniResponse::RecvOnlineTime( int idx, int day_time, int online ){}
// 获取新手任务
void CJniResponse::RecvGetNewUserTask( short step ){}
// 返回新手任务提交结果
void CJniResponse::RecvTaskStep( int ret, int err_code, int gold, int silver, int total_gold, int total_silver ){}
void CJniResponse::RecvTaskGift( int ret, int err_code, int item_id, int object_id, const char * time_buf ){}
// 商品信息更改通知
void CJniResponse::RecvNotice4ChangeGoods(){}
// 服务器请求刷新背包
void CJniResponse::RecvGetBagAgain(){}
// 返回用户印章
void CJniResponse::RecvUserSeel( int user_idx, int seel_idx ){}
// 返回用户防沉迷状态
void CJniResponse::EnterAllMentState( int idx, short state ){}
// room loge
void CJniResponse::RecvRoomLoge( int room_idx, const char * loge, int len ){}
// 新分组版本号
void CJniResponse::RecvNewVersionId( int version_id ){
	CServerData* pSD = m_pDataMgr->m_pServerData;
	if(!pSD)
		return ;

	if (version_id > pSD->getVersion(m_nLobbyId))
	{
		//更新本地分组信息
		if(m_bRequestUpdateGroupRoomlist)//已经在更新中，等待完成
			return ;

		LOGI("%s : need to update the group info\n",__FUNCTION__);
		pSD->clearRoom();
		pSD->clearGroup();

		if(0 != Java_com_jni_netutil_CJniUtil_GetGroupInfo(NULL,NULL))
			LOGE("%s : Java_com_jni_netutil_CJniUtil_GetGroupInfo failed\n",__FUNCTION__);
	}
}
// 分组信息
void CJniResponse::RecvGroupInfo( int count, ResultData_GroupInfo * pInfo ){
	CServerData* pSD = m_pDataMgr->m_pServerData;
	if(!pSD)
		return ;

	//对于可能以后出现的多个大厅信息，可以在返回的结构中要求加数据，目前只是默认登陆的大厅ID
	for(int i=0;i<count;i++)
	{
		//LOGI("AddGroupData groupID:%d,groupName:%s,%s\n",pInfo[i].group_id,pInfo[i].group_name,pInfo[i].disable?"DISABLE":"ENABLE");
		pSD->addGroupData(pInfo[i].group_id,pInfo[i].group_name,pInfo[i].disable,m_nLobbyId);
	}
	pSD->addEnd();

	//记录请求group的次数
	if(count != 0)
		m_bRequestUpdateGroupRoomlist = true;
	m_nTempCount = count;
	m_nTempCur = 0;//当前第几次回应

	//向服务器依次请求group数据
	for(int i=0;i<count;i++)
	{
		if(0 != Java_com_jni_netutil_CJniUtil_GetRoomListInGroup(NULL,NULL,pInfo[i].group_id))
		{
			LOGE("%s : Java_com_jni_netutil_CJniUtil_GetRoomListInGroup failed\n",__FUNCTION__);
			return ;
		}
	}
}
// 获取组里面的房间列表
void CJniResponse::RecvRoomListInGroup( short group_idx, int count, int * list ){
	CServerData* pSD = m_pDataMgr->m_pServerData;
	if(!pSD)
		return ;
	for(int i=0;i<count;i++)
	{
		//LOGI("AddRoomData roomid:%d,group_id:%d,lobbyID:%d\n",list[i],group_idx,m_nLobbyId);
		pSD->addRoomData(list[i],group_idx,m_nLobbyId);
	}
	pSD->addEnd();

	m_nTempCur ++;

	//回应已经全部接受
	if(m_nTempCur == m_nTempCount)
	{
		LOGI("%s : All Group Room data has been recevied \n",__FUNCTION__);
		pSD->setVersion(m_nLobbyId,m_nGroupVersionID);
		m_bRequestUpdateGroupRoomlist = false;
		//cseq保存idx，用于在服务器返回的时候可以处理该数据
		if(0 != Java_com_jni_netutil_CJniUtil_GetPartInfo(NULL,NULL,m_pDataMgr->getMyIdx(),m_pDataMgr->getMyIdx()))
			LOGE("%s : Java_com_jni_netutil_CJniUtil_GetPartInfo failed\n",__FUNCTION__);

		m_nTempCur = 0;
		m_nTempCount = 0;
	}
}
void CJniResponse::RecvRoomListInGroup(short count, ResultData_GroupRoomInfo* pInfo)
{
	//空
}
// 上传日志通知
void CJniResponse::RecvNotice4CollectData( const char * server_ip, short server_port ){}
// 获取歌曲信息
void CJniResponse::RecvSongInfoList( short totalnum, short curnum, ResultData_SongInfo * info ){}
// 获取音视频服务器列表
void CJniResponse::RecvAvServerList( ResultData_AvServer * list, int count ){}
// 更新音视频服务器列表
void CJniResponse::UpdataAvServerList(){}
// 被提出游戏服务器
void CJniResponse::RecvCheckOutGame( int result ){
	CALLBACKJAVABEGIN(RecvCheckOutGame,"(I)V");
	env->CallVoidMethod(m_obj,jmRecvCheckOutGame,result);
	CALLBACKJAVAEND;
}
// 获取离线消息
void CJniResponse::RecvOffLineMsg( int count, ResultData_OffLineMsg * msg ){}
// 返回分组置顶信息
void CJniResponse::RecvRoomSortInGroup( int idx, int count, ResultData_RoomSort * list ){}
// 通知分组置顶信息改变
void CJniResponse::RecvRoomSortChange( int idx ){}
// 歌手等级
void CJniResponse::RecvSingerLevel(int idx, short curlevel, int expvalue){}
// 财富等级
void CJniResponse::RecvWealthLevel(int idx, short curlevel, int expvalue){}

//////////////////////////////////////////////////////////////////////////

// 礼物失效（印章）的协议
void CJniResponse::RecvGiftInvalid( int idx, int GiftID ){}
// 礼物有效（印章）的协议
void CJniResponse::RecvGiftValid( int idx, int GiftID ){}
// 显示房间可领取收益
void CJniResponse::RecvShowRoomIncom( int income ){}
// 显示收益已领取记录
void CJniResponse::RecvShowRoomGetIncom( ResultData_IncomeDate * data, int count ){}
// 室主领取房间内收益
void CJniResponse::RecvGetRoomIncom( int ret ){}
// 礼物信息更改通知
void CJniResponse::RecvNotice4ChangeFlower(){
	CALLBACKJAVAVOID(RecvNotice4ChangeFlower,"()V");
}
// 权限配置更新通知
void CJniResponse::RecvNotice4ChangePower(){}
// 更改音视频服务器广播
void CJniResponse::RecvNotice4AvserverChange( ResultData_AvChange * info ){}
// 更改gm
void CJniResponse::RecvChangeGM( int idx, char add ){}
// 上麦的人的pc概况
void CJniResponse::RecvMicPcInfo( int idx, const char * buf, int len ){}
// 回应是否邀加入房间成功
void CJniResponse::RecvSendJoinRoomInvite( int wseq, int ret, int err_code ){}
// 回应是否邀加入房间成功
void CJniResponse::RecvInviteJoinRoom( int idx ){}
//取得当日礼物赠送记录
void CJniResponse::RecvGiftSend( int count, ResultData_GiftCount * pInfo ){}
//取得当日礼物接收记录
void CJniResponse::RecvGiftRecv( int count, ResultData_GiftCount * pInfo ){}
//返回封mac结果
void CJniResponse::RecvKickUserByMac( int ret, int idx ){}
//返回封ip结果
void CJniResponse::RecvKickUserByIp( int ret, int idx ){}
//返回设置在麦时间结果
void CJniResponse::RecvSetMicTime( int ret, int idx ){}
//广播在麦时间延长
void CJniResponse::RecvNotice4SetMicTime( int idx, int time ){}
//取排麦列表
void CJniResponse::RecvWaitMicList( short count, int* idxs ){}
//防沉迷时间到,断开连接
void CJniResponse::RecvIndulgeTimeUp(){}
/*家族信息回调
参数：
flag：
1:删除家族
2:添加或更新家族
familyNum：
家族数量(N)
familys：
家族信息，是一个数组familys[N-1]{}
注意：当flag为1时，familyNum必定为1，同时familys[0]里只有familyid是可用的(因为删除的时候，一次只能删除一个，只有家族id信息)
*/
void CJniResponse::RecvFamilyInfo(int flag, int familyNum, ResultData_Family* familys){}

/*
av配置信息
*/
void CJniResponse::RecvAVConfigInfo(int infolen, const char* info){}

/* 私麦                                                                 */
/************************************************************************/
/*上私麦*/
void CJniResponse::RecvOpenPrivateMicSuccess(int idx, short mictype, int posindex, int roomid){}
void CJniResponse::RecvOpenPrivateMicFailed(int errorcode, int idx, int bidx){}
void CJniResponse::RecvOpenPrivateMicNotice(int idx, int mictype){} /*不包括自己*/

/*下私麦*/
void CJniResponse::RecvClosePrivateMicFailed(int errorcode, int idx, int bidx){}
void CJniResponse::RecvClosePriveteMicNotice(int idx){} /*包括自己*/

/*被抱下私麦*/
void CJniResponse::RecvClosedPrivateMicFailed(int errorcode, int idx, int bidx){}
void CJniResponse::RecvClosedPriveteMicNotice(int idx, int bidx){} /*包括自己*/

/*观看私麦*/
void CJniResponse::RecvWatchPrivateMicSuccess(int idx, short mictype, int posindex, int roomid){}
void CJniResponse::RecvWatchPrivateMicFailed(int errorcode, int idx, int bidx){}
void CJniResponse::RecvRequestWatchedPrivateMic(int idx){} /*idx=请求者, 是否同意该idx观看你的私麦*/
void CJniResponse::RecvWatchedPrivateMicFailed(int errorcode, int idx, int bidx){}

void CJniResponse::RecvWatchedPrivateMicReject(int idx){} //对方拒绝，idx=谁拒绝的

/*邀请私麦*/
void CJniResponse::RecvInviteWatchPrivateMicSuccess(){}
void CJniResponse::RecvInviteWatchPrivateMicFailed(int errorcode, int idx, int bidx){}
void CJniResponse::RecvRequestInvitedWatchPrivateMic(int idx){} /*idx=邀请者,是否同意观看idx的私麦*/
void CJniResponse::RecvInvitedWatchPrivateMicSuccess(int idx, short mictype, int posindex, int roomid){}
void CJniResponse::RecvInvitedWatchPrivateMicFailed(int errorcode, int idx, int bidx){}

void CJniResponse::RecvInvitedWatchPrivateMicReject(int idx){}//对方拒绝，idx=谁拒绝的

/*管理房间私麦*/
void CJniResponse::RecvOpenRoomPrivateMicFlag(int ret, char flag){}
void CJniResponse::RecvOpenRoomPrivateMicFlagNotice(int idx, char flag){} /*包括自己*/

/*用户登录房间时，回调房间的私麦信息*/
void CJniResponse::RecvPrivateMicWithLoginRoom(int idx, short mictype, int posindex, int roomid){}
/*	返回皇冠踢人结果
	int ret
	 0: 成功 
	-1: 失败，非法操作
	-2: 失败，剩余人次数不足
	int kickuse
	本次操作消耗人次
	int totalkick
	剩余消耗人次数
	int idx{}	
	被踢者idx
*/
void CJniResponse::RecvCrownKickoutSomeOne(int ret, int kickuse, int totalkick, int idx){}
/*室主禁止用户ip
int ret{}
 0：成功
 -1：该IP已被禁用
 -2：该用户已离开房间

int idx{}
被禁用者idx
*/
void CJniResponse::RecvForbidUserIp(int ret, int idx){}

//要求上传歌曲
void CJniResponse::RecvUpdateSong(){}
//回应服务器的返回用户歌唱信息
//void CJniResponse::RecvUserSingInfo(int ret,int idx,const ResultData_UserSingInfo* pData){}
//获取用户接受礼物个数
void CJniResponse::RecvGiftCount(int recvcount){}

//获取用户最高分歌曲信息及演唱歌曲个数
void CJniResponse::RecvTopSongInfo(int songcount, int topsongscore, const char* topsongname, int len){}

//http请求完成的返回接口
// void CJniResponse::OnFinish(_TaskDownload* downloadInfo,bool success)
// {
// 	//回调到java
// 	CJniResponse* pJniResponse = getSingleton();
// 	//返回URL,并且返回saveBuf里面的内容，如果download为ture，则是空字符串
// }
//手机ktv聊天信息
void CJniResponse::RecvKtvChatInfo(int ret,const int fromIdx,int roomid,const char* roomName,const char* content
	,int chatTime,bool isPublic,bool isSend){
	if(!roomName || !content)
		return;

	CALLBACKJAVABEGIN(RecvKtvChatInfo,"(IIILjava/lang/String;Ljava/lang/String;IZZ)V");
	int len = 0;
	GETUNICODEFROMGBK(jstrRoomName,roomName);
	//GETUNICODEFROMGBK(jstrContent,content);
	jstring jstrContent = env->NewStringUTF(content);
	jboolean jIsPublic = isPublic?1:0;
	jboolean jIsSend = isSend?1:0;

	env->CallVoidMethod(m_obj,jmRecvKtvChatInfo,ret,fromIdx,roomid,jstrRoomName,jstrContent,chatTime
		,jIsPublic,jIsSend);

	env->DeleteLocalRef(jstrContent);
	env->DeleteLocalRef(jstrRoomName);

	CALLBACKJAVAEND;
}
//手机接收到当前的播放列表，即当前ktv的点歌列表，用于更新
/*
 @param pSonglist:信息结构体
 @param listcount:歌曲数量
 */
void CJniResponse::RecvKtvSongList(const ResultData_SongList* pSonglist,int listCount){
	CALLBACKJAVABEGIN(RecvKtvSongList,"([Lcom/jni/netutil/ResultData_SongList;I)V");

	if(!pSonglist || listCount ==0)//如果数量为0，则意味着需要清空已点
		env->CallVoidMethod(m_obj,jmRecvKtvSongList,NULL,0);
	else
	{
		jclass cls_sl = m_mapClass["com/jni/netutil/ResultData_SongList"];
		jobjectArray aobjSL = env->NewObjectArray(listCount,cls_sl,0);
		if(aobjSL)
		{
			GETJAVAFIELDID(songid,cls_sl,"I");
			GETJAVAFIELDID(songname,cls_sl,"Ljava/lang/String;");
			GETJAVAFIELDID(singername,cls_sl,"Ljava/lang/String;");

			for (int i=0;i<listCount;i++)
			{
				jobject objSL = env->AllocObject(cls_sl);
				assert(objSL != NULL);
				int len = 0;

				jstring jstrSongName = env->NewStringUTF(pSonglist[i].songname);
				jstring jstrSingerName = env->NewStringUTF(pSonglist[i].singername);
				env->SetIntField(objSL,jfid_songid,pSonglist[i].songid);
				env->SetObjectField(objSL,jfid_songname,jstrSongName);
				env->SetObjectField(objSL,jfid_singername,jstrSingerName);

				env->SetObjectArrayElement(aobjSL,i,objSL);

				env->DeleteLocalRef(jstrSingerName);
				env->DeleteLocalRef(jstrSongName);
				env->DeleteLocalRef(objSL);
			}
			env->CallVoidMethod(m_obj,jmRecvKtvSongList,aobjSL,listCount);
			env->DeleteLocalRef(aobjSL);
		}
		else
			LOGE("%s : Object Alloc Failed\n",__FUNCTION__);
	}

	CALLBACKJAVAEND;
}
//手机获取当前包厢的开关状态
/*
 @param isOpen:true,包厢开启；false，包厢关闭，用户退出房间
 */
void CJniResponse::RecvKtvRoomState(bool isOpen)
{
	CALLBACKJAVABEGIN(RecvKtvRoomState,"(Z)V");

	jboolean jIsOpen = isOpen?1:0;
	env->CallVoidMethod(m_obj,jmRecvKtvRoomState,jIsOpen);

	CALLBACKJAVAEND;
}
//手机收到唱歌上榜通知,提示用户到个人信息里面去抢榜
void CJniResponse::RecvKtvSongRanking()
{
	//CALLBACKJAVAVOID(RecvKtvSongRanking,"()V");
}
void CJniResponse::RecvKtvSongNormalEnd(int nMusicId)
{
	CALLBACKJAVABEGIN(RecvKtvSongNormalEnd,"(I)V");
	env->CallVoidMethod(m_obj,jmRecvKtvSongNormalEnd,nMusicId);
	CALLBACKJAVAEND;
}
/*
	收到的在线手机用户IDX列表更新数据
	只有IDX
*/
void CJniResponse::RecvKtvIdxList(const int* pIdx,int number)
{
	if(!pIdx || number==0)
		return ;
	CALLBACKJAVABEGIN(RecvKtvIdxList,"([II)V");
	jintArray aIdx = env->NewIntArray(number);
	if(aIdx == NULL)
		return ;
	env->SetIntArrayRegion(aIdx,0,number,(const jint*)pIdx);
	env->CallVoidMethod(m_obj,jmRecvKtvIdxList,aIdx,number);
	env->DeleteLocalRef(aIdx);
	CALLBACKJAVAEND;
}
/*
通知用户头像更新
@param [in] idx：指定的用户IDX
@param [in] picture：用户的新头像
*/
void CJniResponse::RecvUserPicChange(int idx,const char* picture)
{
	if(!picture)
		return ;
	CALLBACKJAVABEGIN(RecvUserPicChange,"(ILjava/lang/String;)V");
	jstring jsPicture = env->NewStringUTF(picture);
	env->CallVoidMethod(m_obj,jmRecvUserPicChange,idx,jsPicture);
	env->DeleteLocalRef(jsPicture);
	CALLBACKJAVAEND;
}
void CJniResponse::RecvUserNameChange(int idx,const char* name)
{
	if(!name)
		return ;
	CALLBACKJAVABEGIN(RecvUserNameChange,"(ILjava/lang/String;)V");
	int len;
	GETUNICODEFROMGBK(jstrName,name);
	env->CallVoidMethod(m_obj,jmRecvUserNameChange,idx,jstrName);
	env->DeleteLocalRef(jstrName);
	CALLBACKJAVAEND;
}
/*
	通知获取到的喇叭用户信息
*/
void CJniResponse::RecvUserInfoForSpeaker(int errorcode,ResultData_UserSampleInfo* info)
{
	if(!info)
		return;
	CALLBACKJAVABEGIN(RecvUserInfoForSpeaker,"(ILcom/jni/netutil/ResultData_UserSampleInfo;)V");
	jclass cls_sl = m_mapClass["com/jni/netutil/ResultData_UserSampleInfo"];
	jobject obj = env->AllocObject(cls_sl);
	if(obj)
	{
		GETJAVAFIELDID(idx,cls_sl,"I");
		GETJAVAFIELDID(nickname,cls_sl,"Ljava/lang/String;");
		GETJAVAFIELDID(headurl,cls_sl,"Ljava/lang/String;");

		unsigned char error_utf8;
		const char* errorOccur = NULL;
		CCharsetCodec::CheckUtfChar(info->nickname,&errorOccur);
		jstring jstrName = NULL;
		if(errorOccur)
		{
			int len;
			GETUNICODEFROMGBK(jstrName_in,info->nickname);
			jstrName = jstrName_in;
		}
		else
		{
			jstrName = env->NewStringUTF(info->nickname);
		}
		jstring jstrHeadUrl = env->NewStringUTF(info->headurl);

		env->SetIntField(obj,jfid_idx,info->idx);
		env->SetObjectField(obj,jfid_nickname,jstrName);
		env->SetObjectField(obj,jfid_headurl,jstrHeadUrl);

		env->CallVoidMethod(m_obj,jmRecvUserInfoForSpeaker,errorcode,obj);

		env->DeleteLocalRef(jstrName);
		env->DeleteLocalRef(jstrHeadUrl);
	}

	CALLBACKJAVAEND;
}
void CJniResponse::RecvUserInfoForSpeakerTimeOut(int idx[],int count)
{

}
void CJniResponse::RecvUserInfoTimeOut(int idx[],int count)
{

}

void CJniResponse::RecvNotifyMessage(int msgId,const char* msgContent)
{
	if(!msgContent)
		return;
	CALLBACKJAVABEGIN(RecvNotifyMessage,"");

	jstring jstrMsg = env->NewStringUTF(msgContent);
	env->CallVoidMethod(m_obj,jmRecvNotifyMessage,msgId,jstrMsg);
	CALLBACKJAVAEND;
}

//单个礼物图片下载完成通知
void CJniResponse::RecvImageDownladFinished(const char* pictureName){
	if(!pictureName)
		return ;

	CALLBACKJAVABEGIN(RecvImageDownladFinished,"(Ljava/lang/String;)V");

	int len = 0;

	GETUNICODEFROMGBK(jstrPictureName,pictureName);

	env->CallVoidMethod(m_obj,jmRecvImageDownladFinished,jstrPictureName);
	env->DeleteLocalRef(jstrPictureName);

	CALLBACKJAVAEND;
}

/*
	收到服务器的通知在线的用户信息IDX for new 
	@param list : 用户列表
	@param size : 列表个数的数量
	*/
void CJniResponse::RecvUserIdxAndType(const ResultData_UIdxWithType* list,int number)
{
	if(!list || number == 0)
		return ;

	CALLBACKJAVABEGIN(RecvUserIdxAndType,"([Lcom/jni/netutil/ResultData_UIdxWithType;)V");
	jclass cls_rut = m_mapClass["com/jni/netutil/ResultData_UIdxWithType"];
	jobjectArray jarra = env->NewObjectArray(number,cls_rut,0);
	if(jarra)
	{
		GETJAVAFIELDID(idx,cls_rut,"I");
		GETJAVAFIELDID(type,cls_rut,"I");

		
		for(int i=0;i<number;i++)
		{
			jobject obj = env->AllocObject(cls_rut);
			if(obj)
			{
				env->SetIntField(obj,jfid_idx,list[i].idx);
				//服务器SB，用'0','1','2','3'
				int iType = list[i].type-'0';
				env->SetIntField(obj,jfid_type,iType);
			}

			env->SetObjectArrayElement(jarra,i,obj);

			//env->DeleteLocalRef(obj);
		}

		env->CallVoidMethod(m_obj,jmRecvUserIdxAndType,jarra);
		env->DeleteLocalRef(jarra);
	}

	CALLBACKJAVAEND;
}

void CJniResponse::RecvUserLoginLobbyNoticeNew(int idx,char type)
{
	CALLBACKJAVABEGIN(RecvUserLoginLobbyNoticeNew,"(IB)V");

	int iType = type-'0';
	env->CallVoidMethod(m_obj,jmRecvUserLoginLobbyNoticeNew,idx,iType);

	CALLBACKJAVAEND;
}

void CJniResponse::RecvSendHBResult(int ret)
{
	CALLBACKJAVABEGIN(RecvSendHBResult,"(I)V");

	env->CallVoidMethod(m_obj,jmRecvSendHBResult,ret);

	CALLBACKJAVAEND;
}

void CJniResponse::RecvHongBao(int idx,const char* hbid,char hbtype,const char* remark)
{
	CALLBACKJAVABEGIN(RecvHongBao,"(ILjava/lang/String;BLjava/lang/String;)V");

	jstring jstr_hbid = env->NewStringUTF(hbid?hbid:"");

	//jstring jstr_mark = env->NewStringUTF(remark?remark:"");
	int len = 0;
	GETUNICODEFROMGBK(jstr_mark,remark);

	env->CallVoidMethod(m_obj,jmRecvHongBao,idx,jstr_hbid,hbtype,jstr_mark);

	env->DeleteLocalRef(jstr_mark);
	env->DeleteLocalRef(jstr_hbid);

	CALLBACKJAVAEND;
}

void CJniResponse::RecvHongBaoStatus(const char* hbid,int ret)
{
	CALLBACKJAVABEGIN(RecvHongBaoStatus,"(Ljava/lang/String;I)V");

	jstring jstr_hbid = env->NewStringUTF(hbid?hbid:"");

	env->CallVoidMethod(m_obj,jmRecvHongBaoStatus,jstr_hbid,ret);

	env->DeleteLocalRef(jstr_hbid);

	CALLBACKJAVAEND;
}

void CJniResponse::RecvVieHongBaoResult(int result,int idx,const char* hbid,char hbtype,const char* remark,int gold)
{
	CALLBACKJAVABEGIN(RecvVieHongBaoResult,"(IILjava/lang/String;BLjava/lang/String;I)V");

	//jstring jstr_mark = env->NewStringUTF(remark?remark:"");
	int len = 0;
	GETUNICODEFROMGBK(jstr_mark,remark);
	jstring jstr_hbid = env->NewStringUTF(hbid?hbid:"");
	 
	env->CallVoidMethod(m_obj,jmRecvVieHongBaoResult,result,idx,jstr_hbid,hbtype,jstr_mark,gold);

	env->DeleteLocalRef(jstr_hbid);
	env->DeleteLocalRef(jstr_mark);
	CALLBACKJAVAEND;
}

void CJniResponse::RecvHongBaoDetail(int ret,int idx,char hbtype,const char* remark,int totalgold,int totalnum
		,int sendtime,const ResultData_HBDetail* list/*[num]*/,int num)
{
	CALLBACKJAVABEGIN(RecvHongBaoDetail,"(IIBLjava/lang/String;III[Lcom/jni/netutil/ResultData_HBDetail;)V");


	//jstring jstr_mark = env->NewStringUTF(remark?remark:"");
	int len = 0;
	GETUNICODEFROMGBK(jstr_mark,remark);

	jclass jcls_hbd = m_mapClass["com/jni/netutil/ResultData_HBDetail"];
	jobjectArray jarra = env->NewObjectArray(num,jcls_hbd,0);

	GETJAVAFIELDID(idx,jcls_hbd,"I");
	GETJAVAFIELDID(gold,jcls_hbd,"I");
	GETJAVAFIELDID(time,jcls_hbd,"I");
	if(jarra && list)
	{
		for(int i=0;i<num;i++)
		{
			jobject obj = env->AllocObject(jcls_hbd);
			if(obj)
			{
				env->SetIntField(obj,jfid_idx,list[i].idx);
				env->SetIntField(obj,jfid_gold,list[i].gold);
				env->SetIntField(obj,jfid_time,list[i].time);
			}

			env->SetObjectArrayElement(jarra,i,obj);
			env->DeleteLocalRef(obj);
		}
	}

	env->CallVoidMethod(m_obj,jmRecvHongBaoDetail,ret,idx,hbtype,jstr_mark,totalgold,totalnum,sendtime,jarra);
	env->DeleteLocalRef(jarra);
	env->DeleteLocalRef(jstr_mark);

	CALLBACKJAVAEND;
}

void CJniResponse::RecvSingSongChange(const char* songname,const char* singername,int score)
{
	CALLBACKJAVABEGIN(RecvSingSongChange,"(Ljava/lang/String;Ljava/lang/String;I)V");

	unsigned int len = 0;
	//GETUNICODEFROMGBK(jstr_songname,songname);
	jstring jstr_songname = env->NewStringUTF(songname);
	//GETUNICODEFROMGBK(jstr_singername,singername);
	jstring jstr_singername = env->NewStringUTF(singername);

	env->CallVoidMethod(m_obj,jmRecvSingSongChange,jstr_songname,jstr_singername,score);

	env->DeleteLocalRef(jstr_singername);
	env->DeleteLocalRef(jstr_songname);
	CALLBACKJAVAEND;
}

void CJniResponse::RecvHongBaoNum(int number)
{
	CALLBACKJAVABEGIN(RecvHongBaoNum,"(I)V");

	env->CallVoidMethod(m_obj,jmRecvHongBaoNum,number);

	CALLBACKJAVAEND;
}

/*
当前包厢红包返回

@param ret : 0-正常获取 9999-服务器超时
@param number : list[number];
@param list : 红包列表
*/
void CJniResponse::RecvRoomHB(int ret,int number,ResultData_RoomHB* list)
{
	CALLBACKJAVABEGIN(RecvRoomHB,"(I[Lcom/jni/netutil/ResultData_RoomHB;)V");

	LOGI("roomhb number %d\n",number);
	jobjectArray jarra = NULL;
	if(ret == 0 && number > 0)
	{
		jclass jcls_rhb = m_mapClass["com/jni/netutil/ResultData_RoomHB"];
		jarra = env->NewObjectArray(number,jcls_rhb,0);

		GETJAVAFIELDID(idx,jcls_rhb,"I");
		GETJAVAFIELDID(hbid,jcls_rhb,"Ljava/lang/String;");
		GETJAVAFIELDID(mark,jcls_rhb,"Ljava/lang/String;");
		GETJAVAFIELDID(hbtype,jcls_rhb,"B");
		if(jarra)
		{
			for(int i=0;i<number;i++)
			{
				jstring jstr_hbid = env->NewStringUTF(list[i].hbid);

				//jstring jstr_mark = env->NewStringUTF(list[i].mark);
				int len = 0;
				GETUNICODEFROMGBK(jstr_mark,list[i].mark);

				jobject obj = env->AllocObject(jcls_rhb);
				if(obj)
				{
					env->SetIntField(obj,jfid_idx,list[i].idx);
					env->SetObjectField(obj,jfid_hbid,jstr_hbid);
					env->SetObjectField(obj,jfid_mark,jstr_mark);
					env->SetByteField(obj,jfid_hbtype,list[i].hbtype);
				}
				env->SetObjectArrayElement(jarra,i,obj);
				env->DeleteLocalRef(obj);

				env->DeleteLocalRef(jstr_hbid);
				env->DeleteLocalRef(jstr_mark);
			}
		}
	}

	env->CallVoidMethod(m_obj,jmRecvRoomHB,ret,jarra);

	CALLBACKJAVAEND;
}

extern JNIEnv* g_env;
extern  jobject g_obj;
void CJniResponse::RecvLocalGiftDatabaseFinished(bool direct)
{
	if(direct)
	{
		//直接回调到Java
		jclass cls = g_env->GetObjectClass(g_obj);
		static jmethodID jmRecvLocalGiftDatabaseFinished = g_env->GetMethodID(cls,"RecvLocalGiftDatabaseFinished","()V");
		g_env->CallVoidMethod(g_obj,jmRecvLocalGiftDatabaseFinished);
	}
	else
	{
		//线程回调
		CALLBACKJAVAVOID(RecvLocalGiftDatabaseFinished,"()V");
	}
}

