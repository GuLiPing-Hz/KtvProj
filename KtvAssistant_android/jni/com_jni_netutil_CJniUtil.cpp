/***
 * @author GLP
 */

#include "com_jni_netutil_CJniUtil.h"
#include "NetUtil/RequestBase.h"
#include "NetUtil/wrap/ClientSocket.h"
#include "JniResponse.h"
#include "NetUtil/localstorage/DataMgr.h"
#include <jniloghelp.h>
#include "NetUtil/NetApp.h"
#include "NetUtil/MessageFilter.h"
#include "FilterDog.h"

CDataMgr g_DM;
bool g_bWordFilterInit = false;

#define CHARLEN 260

#undef JNIUTFSTRINGCOPY
#define JNIUTFSTRINGCOPY(name,src) \
char name[CHARLEN] = {0}; \
int name##len = sizeof(name); \
const char *name##str = env->GetStringUTFChars(src,0); \
int name##str_len = strlen(name##str)\
assert(name##len > name##str_len);\
strncpy(name,name##str,name##str_len);\
env->ReleaseStringUTFChars(src,name##str)

#undef GETUTFFROMJSTRINGBEGIN
#define GETUTFFROMJSTRINGBEGIN(name,src) \
const char* name = env->GetStringUTFChars(src,0)

#undef GETUTFFROMJSTRINGEND
#define GETUTFFROMJSTRINGEND(name,src) \
env->ReleaseStringUTFChars(src,name)

#undef  GETGBKFROMJSTRINGBEGIN
#define GETGBKFROMJSTRINGBEGIN(to,from) \
const jchar * from##_jchar = env->GetStringChars(from,0); \
char* to = g_NetApp.m_pCharsetCodecGBK->convertFromUnicode(from##_jchar)

#undef  GETGBKFROMJSTRINGEND
#define GETGBKFROMJSTRINGEND(to,from) \
if(to) delete to;\
env->ReleaseStringChars(from,from##_jchar)

RequestBase g_RB;

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    StartClient
 * Signature: ()I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_StartClient
  (JNIEnv * env, jobject obj)
{
	CJniResponse* pJniResponse = CJniResponse::getSingleton();
	if(!pJniResponse->initJniEnv(env,obj))
		return -1;

	jint ret = g_RB.StartClient();
	g_RB.SetResponseHandle((ResponseBase*)pJniResponse);
	return ret;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    StopClient
 * Signature: ()V
 */
 void JNICALL Java_com_jni_netutil_CJniUtil_StopClient
  (JNIEnv *env, jobject)
{
	g_RB.StopClient();

	CJniResponse* pJniResponse = CJniResponse::getSingleton();
	pJniResponse->uninitJniEnv(env);
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    GetToken
 * Signature: ()Ljava/lang/String;
 */
 jstring JNICALL Java_com_jni_netutil_CJniUtil_GetToken
  (JNIEnv *env, jobject)
{
	jstring rStr = env->NewStringUTF(g_RB.GetToken());
	return rStr;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    GetTokenlen
 * Signature: ()I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_GetTokenlen
  (JNIEnv *, jobject)
{
	jint rLen = g_RB.GetTokenlen();
	return rLen;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    IsConnectAuth
 * Signature: ()Z
 */
 jboolean JNICALL Java_com_jni_netutil_CJniUtil_IsConnectAuth
  (JNIEnv *, jobject)
{
	jboolean rIsConnect = g_RB.IsConnectAuth();
	return rIsConnect;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    IsConnectLobby
 * Signature: ()Z
 */
 jboolean JNICALL Java_com_jni_netutil_CJniUtil_IsConnectLobby
  (JNIEnv *, jobject)
{
	jboolean rIsConnect = g_RB.IsConnectLobby();
	return rIsConnect;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    IsConnectRoom
 * Signature: ()Z
 */
 jboolean JNICALL Java_com_jni_netutil_CJniUtil_IsConnectRoom
  (JNIEnv *, jobject)
{
	jboolean rIsConnect = g_RB.IsConnectRoom();
	return rIsConnect;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    IsTokenLoginLobby
 * Signature: ()Z
 */
 jboolean JNICALL Java_com_jni_netutil_CJniUtil_IsTokenLoginLobby
  (JNIEnv *, jobject)
{
	jboolean rIsConnect = g_RB.IsTokenLoginLobby();
	return rIsConnect;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    IsTokenLoginRoom
 * Signature: ()Z
 */
 jboolean JNICALL Java_com_jni_netutil_CJniUtil_IsTokenLoginRoom
  (JNIEnv *, jobject)
{
	jboolean rIsToken = g_RB.IsTokenLoginRoom();
	return rIsToken;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    ConnectAuth
 * Signature: (Ljava/lang/String;SI)I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_ConnectAuth
  (JNIEnv * env, jobject, jstring host, jshort port, jint timeout)
{
	const char *str = env->GetStringUTFChars(host, 0);
	jint ret = g_RB.ConnectAuth(str,port,timeout);
	env->ReleaseStringUTFChars(host,str);

	return ret;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    DisConnectAuth
 * Signature: ()I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_DisConnectAuth
  (JNIEnv *, jobject)
{
	jint ret = g_RB.DisConnectAuth();
	return ret;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    ConnectLobby
 * Signature: (Ljava/lang/String;SI)I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_ConnectLobby
  (JNIEnv *env, jobject, jstring host, jshort port, jint timeout)
{
	const char *str = env->GetStringUTFChars(host, 0);
	jint ret = g_RB.ConnectLobby(str,port,timeout);
	env->ReleaseStringUTFChars(host,str);

	return ret;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    DisConnectLobby
 * Signature: ()I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_DisConnectLobby
  (JNIEnv *, jobject)
{
	jint ret = g_RB.DisConnectLobby();
	return ret;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    ConnectRoom
 * Signature: (Ljava/lang/String;SII)I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_ConnectRoom
  (JNIEnv *env, jobject, jstring host, jshort port, jint type, jint timeout)
{
	const char *str = env->GetStringUTFChars(host, 0);
	jint ret = g_RB.ConnectRoom(str,port,type,timeout);
	env->ReleaseStringUTFChars(host,str);

	return ret;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    DisConnectRoom
 * Signature: ()I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_DisConnectRoom
  (JNIEnv *, jobject)
{
	jint ret = g_RB.DisConnectRoom();
	return ret;
}

 /*
  * Class:     com_jni_netutil_CJniUtil
  * Method:    IdPassAuth
  * Signature: (Ljava/lang/String;Ljava/lang/String;I)I
  */
 jint JNICALL Java_com_jni_netutil_CJniUtil_IdPassAuth
  (JNIEnv *env, jobject, jstring id, jstring password,jint again)
{
	GETUTFFROMJSTRINGBEGIN(cId,id);
	GETUTFFROMJSTRINGBEGIN(cPassword,password);
	jint ret = g_RB.IdPassAuth(cId,cPassword,again);
	GETUTFFROMJSTRINGEND(cPassword,password);
	GETUTFFROMJSTRINGEND(cId,id);
	return ret;
}

 /*
  * Class:     com_jni_netutil_CJniUtil
  * Method:    TouristAuth
  * Signature: (I)I
  */
 jint JNICALL Java_com_jni_netutil_CJniUtil_TouristAuth
  (JNIEnv *, jobject,jint again)
{
	jint ret = g_RB.TouristAuth(again);
	return ret;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    ConfirmToken
 * Signature: ()I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_ConfirmToken
  (JNIEnv *, jobject)
{
	jint ret = g_RB.ConfirmToken();
	return ret;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    EnterRoom
 * Signature: (IILjava/lang/String;S)I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_EnterRoom
  (JNIEnv *env, jobject, jint seq , jint roomid, jstring password, jshort type)
{
	GETUTFFROMJSTRINGBEGIN(cPassword,password);
	jint ret = g_RB.EnterRoom(seq,roomid,cPassword,type);
	GETUTFFROMJSTRINGEND(cPassword,password);
	return ret;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    GetRoomAllInfomation
 * Signature: (I)I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_GetRoomAllInfomation
  (JNIEnv *, jobject, jint seq)
{
	jint ret = g_RB.GetRoomAllInfomation(seq);
	return ret;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    GetWaitMicList
 * Signature: ()I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_GetWaitMicList
  (JNIEnv *, jobject)
{
	jint ret = g_RB.GetWaitMicList();
	return ret;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    RoomWaitMic
 * Signature: (ISBLjava/lang/String;B)I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_RoomWaitMic
  (JNIEnv *env, jobject, jint musicid, jshort bk, jbyte level, jstring name, jbyte speed)
{
	GETUTFFROMJSTRINGBEGIN(cName,name);
	jint ret = g_RB.RoomWaitMic(musicid,bk,level,cName,speed);
	GETUTFFROMJSTRINGEND(cName,name);
	return ret;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    RoomCancleWaitMic
 * Signature: (I)I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_RoomCancleWaitMic
  (JNIEnv *, jobject, jint idx)
{
	jint ret = g_RB.RoomCancleWaitMic(idx);
	return ret;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    RoomUpWaitMic
 * Signature: (BIB)I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_RoomUpWaitMic
  (JNIEnv *, jobject, jbyte tag, jint idx, jbyte flag)
{
	jint ret = g_RB.RoomUpWaitMic(tag,idx,flag);
	return ret;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    RoomDownWaitMic
 * Signature: (BIB)I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_RoomDownWaitMic
  (JNIEnv *, jobject, jbyte tag, jint idx, jbyte flag)
{
	jint ret = g_RB.RoomDownWaitMic(tag,idx,flag);
	return ret;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    RoomKickSomeOne
 * Signature: (I)I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_RoomKickSomeOne
  (JNIEnv *, jobject, jint idx)
{
	jint ret = g_RB.RoomKickSomeOne(idx);
	return ret;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    RoomForbidenSomeOne
 * Signature: (IB)I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_RoomForbidenSomeOne
  (JNIEnv *, jobject, jint idx, jbyte flag)
{
	jint ret = g_RB.RoomForbidenSomeOne(idx,flag);
	return ret;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    RoomOnvjMic
 * Signature: ()I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_RoomOnvjMic
  (JNIEnv *, jobject)
{
	jint ret = g_RB.RoomOnvjMic();
	return ret;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    RoomOffvjMic
 * Signature: ()I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_RoomOffvjMic
  (JNIEnv *, jobject)
{
	jint ret = g_RB.RoomOffvjMic();
	return ret;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    RoomGivOnevjMic
 * Signature: (I)I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_RoomGivOnevjMic
  (JNIEnv *, jobject, jint idx)
{
	jint ret = g_RB.RoomGivOnevjMic(idx);
	return ret;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    ApplyGetVJmic
 * Signature: (IB)I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_ApplyGetVJmic
  (JNIEnv *, jobject, jint idx, jbyte flag)
{
	jint ret = g_RB.ApplyGetVJmic(idx,flag);
	return ret;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    RoomGiveOffvjMic
 * Signature: (I)I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_RoomGiveOffvjMic
  (JNIEnv *, jobject, jint idx)
{
	jint ret = g_RB.RoomGiveOffvjMic(idx);
	return ret;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    RoomGiveOffMic
 * Signature: (I)I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_RoomGiveOffMic
  (JNIEnv *, jobject, jint idx)
{
	jint ret = g_RB.RoomGiveOffMic(idx);
	return ret;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    RoomUpdateBlackList
 * Signature: (IIB)I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_RoomUpdateBlackList
  (JNIEnv *, jobject, jint seq, jint idx, jbyte flag)
{
	jint ret = g_RB.RoomUpdateBlackList(seq,idx,flag);
	return ret;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    RoomUpdateMgrList
 * Signature: (IBB)I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_RoomUpdateMgrList
  (JNIEnv *, jobject, jint idx, jbyte flag, jbyte identity)
{
	jint ret = g_RB.RoomUpdateMgrList(idx,flag,identity);
	return ret;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    LoginLobby
 * Signature: (ILjava/lang/String;)I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_LoginLobby
  (JNIEnv *env, jobject, jint lobbyId, jstring mac,jint loginway)
{
	//保存LobbyId 
	CJniResponse::getSingleton()->m_nLobbyId = lobbyId;
	GETUTFFROMJSTRINGBEGIN(cMac,mac);
	jint ret = g_RB.LoginLobby(cMac,1);//Android 登陆
	GETUTFFROMJSTRINGEND(cMac,mac);
	return ret;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    LoginLobbyAuto
 * Signature: (Ljava/lang/String;I)I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_LoginLobbyAuto
  (JNIEnv *env, jobject, jstring mac, jint roomid)
{
	GETUTFFROMJSTRINGBEGIN(cMac,mac);
	jint ret = g_RB.LoginLobbyAuto(cMac,roomid);
	GETUTFFROMJSTRINGEND(cMac,mac);
	return ret;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    GetUserInfo_AII
 * Signature: ([II)I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_GetUserInfo_1AII
  (JNIEnv *env, jobject, jintArray aIdx, jint count)
{
	int length = env->GetArrayLength(aIdx);
	jint* pInt = (jint*)env->GetPrimitiveArrayCritical(aIdx,0);
// 	for(int i = 0;i<length;i++)
// 	{
// 		pInt[i];//或者保存在新的数组中
// 	}
	int* pIdx = (int*)pInt;
	jint ret = g_RB.GetUserInfo(pIdx,length);
	env->ReleasePrimitiveArrayCritical(aIdx,pInt,0);

	return ret;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    GetFriendListWithoutUserInfo
 * Signature: ()I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_GetFriendListWithoutUserInfo
  (JNIEnv *, jobject)
{
	jint ret = g_RB.GetFriendListWithoutUserInfo();
	return ret;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    GetBlackListWithoutUserInfo
 * Signature: ()I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_GetBlackListWithoutUserInfo
  (JNIEnv *, jobject)
{
	jint ret = g_RB.GetBlackListWithoutUserInfo();
	return ret;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    AddFriend
 * Signature: (I)I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_AddFriend
  (JNIEnv *, jobject, jint idx)
{
	jint ret = g_RB.AddFriend(idx);
	return ret;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    AddBlack
 * Signature: (II)I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_AddBlack
  (JNIEnv *, jobject, jint seq, jint idx)
{
	jint ret = g_RB.AddBlack(seq,idx);
	return ret;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    DelFriend
 * Signature: (II)I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_DelFriend
  (JNIEnv *, jobject, jint seq, jint idx)
{
	jint ret = g_RB.DelFriend(seq,idx);
	return ret;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    DelBlack
 * Signature: (II)I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_DelBlack
  (JNIEnv *, jobject, jint seq, jint idx)
{
	jint ret = g_RB.DelBlack(seq,idx);
	return ret;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    GetChannellist
 * Signature: (ILjava/lang/String;)I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_GetChannellist
  (JNIEnv *env, jobject, jint version, jstring mac)
{
	GETUTFFROMJSTRINGBEGIN(cMac,mac);
	jint ret = g_RB.GetChannellist(version,cMac);
	GETUTFFROMJSTRINGEND(cMac,mac);
	return ret;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    GetRoomList
 * Signature: (I)I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_GetRoomList__I
  (JNIEnv *, jobject, jint seq)
{
	jint ret = g_RB.GetRoomList((int)seq);
	return ret;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    GetRoomListEx
 * Signature: (II)I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_GetRoomListEx
  (JNIEnv *, jobject, jint seq, jint type)
{
	jint ret = g_RB.GetRoomListEx(seq,type);
	return ret;
}

 /*
  * Class:     com_jni_netutil_CJniUtil
  * Method:    GetRoomInfo
  * Signature: (II[I)I
  */
  jint JNICALL Java_com_jni_netutil_CJniUtil_GetRoomInfo__II_3I
  (JNIEnv *env, jobject, jint seq, jint roomcount, jintArray roomlist)
{
	
	int length = env->GetArrayLength(roomlist);
	jint* pInt = (jint*)env->GetPrimitiveArrayCritical(roomlist,0);
	jint ret = g_RB.GetRoomInfo(seq,length,(int*)pInt);
	env->ReleasePrimitiveArrayCritical(roomlist,pInt,0);
	return ret;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    GetTmpRoomInfo
 * Signature: (I)I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_GetTmpRoomInfo
  (JNIEnv *, jobject, jint roomId)
{
	jint ret = g_RB.GetTmpRoomInfo(roomId);
	return ret;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    CreateAvatar
 * Signature: (Ljava/lang/String;[Lcom/jni/netutil/ResultData_Avatar_Updata;)I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_CreateAvatar
  (JNIEnv *env, jobject, jstring name, jobjectArray avarat)
{
	return -1;
 }

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    ChangeAvatar
 * Signature: ([Lcom/jni/netutil/ResultData_Avatar_Updata;)I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_ChangeAvatar
  (JNIEnv *, jobject, jobjectArray)
{
	return -1;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    ChangeNickName
 * Signature: (ILjava/lang/String;I)I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_ChangeNickName
  (JNIEnv *env, jobject, jint idx, jstring nickname, jint len)
{
	GETGBKFROMJSTRINGBEGIN(cNickname,nickname);
	jint ret = g_RB.ChangeNickName(idx, cNickname);
	GETGBKFROMJSTRINGEND(cNickname,nickname);
	return ret;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    SendFlower
 * Signature: (IIII)I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_SendFlower
  (JNIEnv *, jobject, jint s_idx, jint r_idx, jint cate_id, jint count)
{
	jint ret = g_RB.SendFlower(s_idx, r_idx, cate_id, count);
	return ret;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    BroadAll
 * Signature: (Ljava/lang/String;)I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_BroadAll
  (JNIEnv *env, jobject, jstring content)
{
	GETGBKFROMJSTRINGBEGIN(cContent,content);
	jint ret = g_RB.BroadAll(cContent);
	GETGBKFROMJSTRINGEND(cContent,content);
	return ret;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    BroadLobby
 * Signature: (ILjava/lang/String;)I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_BroadLobby
  (JNIEnv *env, jobject, jint lobbyId, jstring content)
{
	GETGBKFROMJSTRINGBEGIN(cContent,content);
	jint ret = g_RB.BroadLobby(lobbyId,cContent);
	GETGBKFROMJSTRINGEND(cContent,content);
	return ret;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    SendRoomNotice
 * Signature: (Ljava/lang/String;)I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_SendRoomNotice
  (JNIEnv *env, jobject, jstring content)
{
	GETGBKFROMJSTRINGBEGIN(cContent,content);
	jint ret = g_RB.SendRoomNotice(cContent);
	GETGBKFROMJSTRINGEND(cContent,content);
	return ret;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    GetLobbyInfo
 * Signature: (II)I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_GetLobbyInfo
  (JNIEnv *, jobject, jint cseq, jint lobbyId)
{
	jint ret = g_RB.GetLobbyInfo(cseq,lobbyId);
	return ret;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    InviteMic
 * Signature: (I)I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_InviteMic
  (JNIEnv *, jobject, jint userIdx)
{
	jint ret = g_RB.InviteMic(userIdx);
	return ret;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    SendChatToUser
 * Signature: (ILjava/lang/String;I)I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_SendChatToUser
  (JNIEnv *env, jobject, jint to_idx, jstring content)
{
	GETGBKFROMJSTRINGBEGIN(cContent,content);
	jint ret = g_RB.SendChatToUser(to_idx,cContent);
	GETGBKFROMJSTRINGEND(cContent,content);
	return ret;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    SendChatToClub
 * Signature: (Ljava/lang/String;I)I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_SendChatToClub
  (JNIEnv *env, jobject, jstring content)
{
	GETGBKFROMJSTRINGBEGIN(cContent,content);
	jint ret = g_RB.SendChatToClub(cContent);
	GETGBKFROMJSTRINGEND(cContent,content);
	return ret;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    SendChatToRoom
 * Signature: (Ljava/lang/String;I)I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_SendChatToRoom
  (JNIEnv *env, jobject, jstring content)
{
	GETGBKFROMJSTRINGBEGIN(cContent,content);
	jint ret = g_RB.SendChatToRoom(cContent);
	GETGBKFROMJSTRINGEND(cContent,content);
	return ret;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    SendSpeakerToAll
 * Signature: (Ljava/lang/String;I)I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_SendSpeakerToAll
  (JNIEnv *env, jobject, jstring content)
{
	GETGBKFROMJSTRINGBEGIN(cContent,content);
	jint ret = g_RB.SendSpeakerToAll(cContent);
	GETGBKFROMJSTRINGEND(cContent,content);
	return ret;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    SendSpeakerToLobby
 * Signature: (ILjava/lang/String;I)I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_SendSpeakerToLobby
  (JNIEnv *env, jobject, jint lobbyId, jstring content)
{
	//GETGBKFROMJSTRINGBEGIN(cContent,content);
	const char* cContent = env->GetStringUTFChars(content,0);
	int len = env->GetStringUTFLength(content);

	char* pContent = new char[len+1];
	if(!pContent)
		return -1;
	memcpy(pContent,cContent,len);
	pContent[len] = 0;

	jint ret = g_RB.SendSpeakerToLobby(lobbyId,pContent);

	delete pContent;
	env->ReleaseStringUTFChars(content,cContent);
	//GETGBKFROMJSTRINGEND(cContent,content);
	return ret;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    QueryFriendLocation
 * Signature: (II)I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_QueryFriendLocation
  (JNIEnv *, jobject, jint cseq, jint friend_idx)
{
	jint ret = g_RB.QueryFriendLocation(cseq,friend_idx);
	return ret;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    GetRoomApplyList
 * Signature: (I)I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_GetRoomApplyList
  (JNIEnv *, jobject, jint cseq)
{
	jint ret = g_RB.GetRoomApplyList(cseq);
	return ret;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    ApplyRoomMember
 * Signature: (I)I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_ApplyRoomMember
  (JNIEnv *, jobject, jint cseq)
{
	jint ret = g_RB.ApplyRoomMember(cseq);
	return ret;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    AuditAddApply
 * Signature: (IIB)I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_AuditAddApply
  (JNIEnv *, jobject, jint cseq, jint idx, jbyte type)
{
	jint ret = g_RB.AuditAddApply(cseq,idx,type);
	return ret;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    GetRoomMemberList
 * Signature: (I)I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_GetRoomMemberList
  (JNIEnv *, jobject, jint cseq)
{
	jint ret = g_RB.GetRoomMemberList(cseq);
	return ret;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    UnchainMember
 * Signature: (II)I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_UnchainMember
  (JNIEnv *, jobject, jint cseq, jint idx)
{
	jint ret = g_RB.UnchainMember(cseq,idx);
	return ret;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    SetRoomPesideAssistant
 * Signature: (II)I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_SetRoomPesideAssistant
  (JNIEnv *, jobject, jint cseq, jint idx)
{
	jint ret = g_RB.SetRoomPesideAssistant(cseq,idx);
	return ret;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    SetRoomPeside
 * Signature: (II)I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_SetRoomPeside
  (JNIEnv *, jobject, jint cseq, jint idx)
{
	jint ret = g_RB.SetRoomPeside(cseq,idx);
	return ret;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    SetRoomSubHost
 * Signature: (II)I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_SetRoomSubHost
  (JNIEnv *, jobject, jint cseq, jint idx)
{
	jint ret = g_RB.SetRoomSubHost(cseq,idx);
	return ret;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    SetRoomMember
 * Signature: (II)I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_SetRoomMember
  (JNIEnv *, jobject, jint cseq, jint idx)
{
	jint ret = g_RB.SetRoomMember(cseq,idx);
	return ret;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    GetBlackList
 * Signature: (I)I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_GetBlackList
  (JNIEnv *, jobject, jint cseq)
{
	jint ret = g_RB.GetBlackList(cseq);
	return ret;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    AddDelBlack
 * Signature: (IB)I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_AddDelBlack
  (JNIEnv *, jobject, jint idx, jbyte type)
{
	jint ret = g_RB.AddDelBlack(idx,type);
	return ret;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    GetRoomPanelInfo
 * Signature: (I)I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_GetRoomPanelInfo
  (JNIEnv *, jobject, jint cseq)
{
	jint ret = g_RB.GetRoomPanelInfo(cseq);
	return ret;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    SetRoomPassword
 * Signature: (ILjava/lang/String;I)I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_SetRoomPassword
  (JNIEnv *env, jobject, jint cseq, jstring password)
{
	GETUTFFROMJSTRINGBEGIN(cPassword,password);
	jint ret = g_RB.SetRoomPassword(cseq,cPassword);
	GETUTFFROMJSTRINGEND(cPassword,password);
	return ret;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    SetRoomSwitch
 * Signature: (IB)I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_SetRoomSwitch
  (JNIEnv *, jobject, jint cseq, jbyte roomflag)
{
	jint ret = g_RB.SetRoomSwitch(cseq,roomflag);
	return ret;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    SetRoomPrivate
 * Signature: (IB)I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_SetRoomPrivate
  (JNIEnv *, jobject, jint cseq, jbyte accessflag)
{
	jint ret = g_RB.SetRoomPrivate(cseq,accessflag);
	return ret;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    SetUserInOutSwitch
 * Signature: (IB)I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_SetUserInOutSwitch
  (JNIEnv *, jobject, jint cseq, jbyte inoutflag)
{
	jint ret = g_RB.SetUserInOutSwitch(cseq,inoutflag);
	return ret;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    SetGetMicSwitch
 * Signature: (IB)I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_SetGetMicSwitch
  (JNIEnv *, jobject, jint cseq, jbyte getmicflag)
{
	jint ret = g_RB.SetGetMicSwitch(cseq,getmicflag);
	return ret;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    SetOnMicSwitch
 * Signature: (IB)I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_SetOnMicSwitch
  (JNIEnv *, jobject, jint cseq, jbyte onmicflag)
{
	jint ret = g_RB.SetOnMicSwitch(cseq,onmicflag);
	return ret;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    ModifyRoomName
 * Signature: (ILjava/lang/String;I)I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_ModifyRoomName
  (JNIEnv *env, jobject, jint cseq, jstring roomname)
{
	GETGBKFROMJSTRINGBEGIN(cRoomname,roomname);
	jint ret = g_RB.ModifyRoomName(cseq,cRoomname);
	GETGBKFROMJSTRINGEND(cRoomname,roomname);
	return ret;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    SetRoomAffiche
 * Signature: (ILjava/lang/String;I)
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_SetRoomAffiche
  (JNIEnv *env, jobject, jint cseq, jstring content)
{
	GETGBKFROMJSTRINGBEGIN(cContetn,content);
	jint ret = g_RB.SetRoomAffiche(cseq,cContetn);
	GETGBKFROMJSTRINGEND(cContetn,content);
	return ret;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    AllowRoomAllChat
 * Signature: (IB)I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_AllowRoomAllChat
  (JNIEnv *, jobject, jint cseq, jbyte chatflag)
{
	jint ret = g_RB.AllowRoomAllChat(cseq,chatflag);
	return ret;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    SetRoomSalutatory
 * Signature: (ILjava/lang/String;I)
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_SetRoomSalutatory
  (JNIEnv *env, jobject, jint cseq, jstring content)
{
	GETGBKFROMJSTRINGBEGIN(cContent,content);
	jint ret = g_RB.SetRoomSalutatory(cseq,cContent);
	GETGBKFROMJSTRINGEND(cContent,content);
	return ret;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    SetRoomLogo
 * Signature: (ILjava/lang/String;I)I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_SetRoomLogo
  (JNIEnv *env, jobject, jint cseq, jstring path)
{
	GETGBKFROMJSTRINGBEGIN(cPath,path);
	jint ret = g_RB.SetRoomLogo(cseq,cPath);
	GETGBKFROMJSTRINGEND(cPath,path);
	return ret;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    GetPartInfo
 * Signature: (II)I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_GetPartInfo
  (JNIEnv *, jobject, jint cseq, jint idx)
{
	 jint jret = g_RB.GetPartInfo(cseq,idx);
	return jret;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    GetRoomList
 * Signature: (II)I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_GetRoomList__II
  (JNIEnv *, jobject, jint cseq, jint lobbyId)
{
	jint ret = g_RB.GetRoomList(cseq,lobbyId);
	return ret;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    LeaveRoom
 * Signature: ()I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_LeaveRoom
  (JNIEnv *, jobject)
{
	jint ret = g_RB.LeaveRoom();
	return ret;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    ApplyAddFriend
 * Signature: (IB)I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_ApplyAddFriend
  (JNIEnv *, jobject, jint idx, jbyte flag)
{
	jint ret = g_RB.ApplyAddFriend(idx,flag);
	return ret;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    GetRoomSimpleInfo
 * Signature: ()I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_GetRoomSimpleInfo
  (JNIEnv *, jobject)
{
	jint ret = g_RB.GetRoomSimpleInfo();
	return ret;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    GetRoomSimpleInfoEx
 * Signature: (I)I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_GetRoomSimpleInfoEx
  (JNIEnv *, jobject, jint type)
{
	jint ret = g_RB.GetRoomSimpleInfoEx(type);
	return ret;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    ExitRoomMember
 * Signature: ()I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_ExitRoomMember
  (JNIEnv *, jobject)
{
	jint ret = g_RB.ExitRoomMember();
	return ret;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    GetLobbyOnlineNum
 * Signature: ()I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_GetLobbyOnlineNum
  (JNIEnv *, jobject)
{
	jint ret = g_RB.GetLobbyOnlineNum();
	return ret;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    BuyPack
 * Signature: ([Lcom/jni/netutil/ResultData_Buy;III)I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_BuyPack
  (JNIEnv *, jobject, jobjectArray, jint, jint, jint)
{
	return -1;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    GetPack
 * Signature: ()I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_GetPack
  (JNIEnv *, jobject)
{
	return -1;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    DelPack
 * Signature: (I[II)I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_DelPack
  (JNIEnv *, jobject, jint, jintArray, jint)
{
	return -1;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    UsePack
 * Signature: (I)I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_UsePack
  (JNIEnv *, jobject, jint)
{
	return -1;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    CreateRoom
 * Signature: ([Lcom/jni/netutil/ResultData_CreateRoom;)I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_CreateRoom
  (JNIEnv *, jobject, jobjectArray)
{
	return -1;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    SendFireworks
 * Signature: (II)I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_SendFireworks
  (JNIEnv *, jobject, jint cate_idx, jint count)
{
	jint ret = g_RB.SendFireworks(cate_idx,count);
	return ret;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    GetGiftTopList
 * Signature: ()I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_GetGiftTopList
  (JNIEnv *, jobject)
{
	jint ret = g_RB.GetGiftTopList();
	return ret;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    GetOnlineTime
 * Signature: (I)I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_GetOnlineTime
  (JNIEnv *, jobject, jint idx)
{
	jint ret = g_RB.GetOnlineTime(idx);
	return ret;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    GetNewUserTask
 * Signature: ()I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_GetNewUserTask
  (JNIEnv *, jobject)
{
	return -1;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    TaskStep
 * Signature: (S)I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_TaskStep
  (JNIEnv *, jobject, jshort)
{
	return -1;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    TaskGift
 * Signature: (S)I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_TaskGift
  (JNIEnv *, jobject, jshort)
{
	return -1;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    GetGiftInfo
 * Signature: ()I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_GetGiftInfo
  (JNIEnv *, jobject)
{
	jint ret = g_RB.GetGiftInfo();
	return ret;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    GetSystemBroad
 * Signature: ()I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_GetSystemBroad
  (JNIEnv *, jobject)
{
	jint ret = g_RB.GetSystemBroad();
	return ret;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    GetUserSeel
 * Signature: ([II)I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_GetUserSeel
  (JNIEnv *env, jobject, jintArray aIdx)
{
	int count = env->GetArrayLength(aIdx);
	jint* pIdx = (jint*)env->GetPrimitiveArrayCritical(aIdx,0);
	jint ret = g_RB.GetUserSeel((const int*)pIdx,count);
	env->ReleasePrimitiveArrayCritical(aIdx,pIdx,0);
	return ret;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    SendSysteInfo
 * Signature: (Ljava/lang/String;I)I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_SendSysteInfo
  (JNIEnv *env, jobject, jstring data)
{
	GETGBKFROMJSTRINGBEGIN(cData,data);
	jint ret = g_RB.SendSysteInfo(cData);
	GETGBKFROMJSTRINGEND(cData,data);
	return ret;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    GetGroupInfo
 * Signature: ()I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_GetGroupInfo
  (JNIEnv *, jobject)
{
	 jint jret = g_RB.GetGroupInfo();
	return jret;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    GetRoomListInGroup
 * Signature: (I)I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_GetRoomListInGroup
  (JNIEnv *, jobject, jint groupid)
{
	 jint jret = g_RB.GetRoomListInGroup(groupid);
	return jret;
}

 /*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    GetRoomListInGroupBatch
 * Signature: ([SS)I
 */
jint JNICALL Java_com_jni_netutil_CJniUtil_GetRoomListInGroupBatch
  (JNIEnv *env, jobject, jshortArray aGroupId)
{
	int count = env->GetArrayLength(aGroupId);
	jshort* pGroupId = (jshort*)env->GetPrimitiveArrayCritical(aGroupId,0);
	jint ret = g_RB.GetRoomListInGroupBatch(pGroupId,count);
	env->ReleasePrimitiveArrayCritical(aGroupId,pGroupId,0);
	return ret;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    GetSongInfoList
 * Signature: ()I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_GetSongInfoList
  (JNIEnv *, jobject)
{
	jint ret = g_RB.GetSongInfoList();
	return ret;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    GetAvServerList
 * Signature: (S)I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_GetAvServerList
  (JNIEnv *, jobject, jshort net_type)
{
	jint ret = g_RB.GetAvServerList(net_type);
	return ret;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    GetOffLineMsg
 * Signature: ()I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_GetOffLineMsg
  (JNIEnv *, jobject)
{
	jint ret = g_RB.GetOffLineMsg();
	return ret;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    GetRoomSortInGroup
 * Signature: (S)I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_GetRoomSortInGroup
  (JNIEnv *, jobject, jshort group_id)
{
	jint ret = g_RB.GetRoomSortInGroup(group_id);
	return ret;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    GetUserSpeakerInfo
 * Signature: ()I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_GetUserSpeakerInfo
  (JNIEnv *, jobject)
{
	jint ret = g_RB.GetUserSpeakerInfo();
	return ret;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    GetFamilyInfo
 * Signature: ()I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_GetFamilyInfo
  (JNIEnv *, jobject)
{
	jint ret = g_RB.GetFamilyInfo();
	return ret;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    RoomScore
 * Signature: (Ljava/lang/String;I)I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_RoomScore
  (JNIEnv *env, jobject, jstring scoreinfo)
{
	GETGBKFROMJSTRINGBEGIN(cScoreInfo,scoreinfo);
	jint ret = g_RB.RoomScore(cScoreInfo);
	GETGBKFROMJSTRINGEND(cScoreInfo,scoreinfo);
	return ret;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    ShowRoomIncome
 * Signature: ()I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_ShowRoomIncome
  (JNIEnv *, jobject)
{
	return -1;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    ShowRoomGetIncome
 * Signature: ()I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_ShowRoomGetIncome
  (JNIEnv *, jobject)
{
	return -1;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    GetRoomIncome
 * Signature: ()I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_GetRoomIncome
  (JNIEnv *, jobject)
{
	return -1;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    SendPcData
 * Signature: (Ljava/lang/String;I)I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_SendPcData
  (JNIEnv *env, jobject, jstring data)
{
	GETGBKFROMJSTRINGBEGIN(cData,data);
	jint ret = g_RB.SendPcData(cData);
	GETGBKFROMJSTRINGEND(cData,data);
	return ret;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    InviteJoinRoom
 * Signature: (II)I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_InviteJoinRoom
  (JNIEnv *, jobject, jint idx, jint wseq)
{
	jint ret = g_RB.InviteJoinRoom(idx,wseq);
	return ret;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    ReInviteJoinRoom
 * Signature: (B)I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_ReInviteJoinRoom
  (JNIEnv *, jobject, jbyte type)
{
	jint ret = g_RB.ReInviteJoinRoom(type);
	return ret;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    GetGiftSend
 * Signature: ()I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_GetGiftSend
  (JNIEnv *, jobject)
{
	jint ret = g_RB.GetGiftSend();
	return ret;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    GetGiftRecv
 * Signature: ()I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_GetGiftRecv
  (JNIEnv *, jobject)
{
	jint ret = g_RB.GetGiftRecv();
	return ret;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    KickUserByMac
 * Signature: (I)I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_KickUserByMac
  (JNIEnv *, jobject, jint idx)
{
	jint ret = g_RB.KickUserByMac(idx);
	return ret;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    KickUserByIp
 * Signature: (I)I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_KickUserByIp
  (JNIEnv *, jobject, jint idx)
{
	jint ret = g_RB.KickUserByIp(idx);
	return ret;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    SetOnMicTemainTime
 * Signature: (II)I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_SetOnMicTemainTime
  (JNIEnv *, jobject, jint idx, jint time)
{
	jint ret = g_RB.SetOnMicTemainTime(idx,time);
	return ret;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    SendSongRemnantsTime
 * Signature: (S)I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_SendSongRemnantsTime
  (JNIEnv *, jobject, jshort time)
{
	jint ret = g_RB.SendSongRemnantsTime(time);
	return ret;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    OpenPrivateMic
 * Signature: (S)I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_OpenPrivateMic
  (JNIEnv *, jobject, jshort mictype)
{
	jint ret = g_RB.OpenPrivateMic(mictype);
	return ret;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    ClosePrivateMic
 * Signature: ()I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_ClosePrivateMic
  (JNIEnv *, jobject)
{
	jint ret = g_RB.ClosePrivateMic();
	return ret;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    ClosedPrivateMic
 * Signature: (I)I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_ClosedPrivateMic
  (JNIEnv *, jobject, jint bidx)
{
	jint ret = g_RB.ClosedPrivateMic(bidx);
	return ret;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    RequestWatchPrivateMic
 * Signature: (I)I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_RequestWatchPrivateMic
  (JNIEnv *, jobject, jint idx)
{
	jint ret = g_RB.RequestWatchPrivateMic(idx);
	return ret;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    ResponseWatchPrivateMic
 * Signature: (II)I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_ResponseWatchPrivateMic
  (JNIEnv *, jobject, jint allow, jint idx)
{
	jint ret = g_RB.ResponseWatchPrivateMic(allow,idx);
	return ret;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    InviteWatchPrivateMic
 * Signature: (I[I)I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_InviteWatchPrivateMic
  (JNIEnv *env, jobject, jintArray aIdx)
{
	int count = env->GetArrayLength(aIdx);
	jint* pIdx = (jint*)env->GetPrimitiveArrayCritical(aIdx,0);
	jint ret = g_RB.InviteWatchPrivateMic(count,(int*)pIdx);
	env->ReleasePrimitiveArrayCritical(aIdx,pIdx,0);
	return ret;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    ResponseInviteWatchPrivateMic
 * Signature: (II)I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_ResponseInviteWatchPrivateMic
  (JNIEnv *, jobject, jint allow, jint idx)
{
	jint ret = g_RB.ResponseInviteWatchPrivateMic(allow,idx);
	return ret;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    OpenRoomPrivateMicFlag
 * Signature: (B)I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_OpenRoomPrivateMicFlag
  (JNIEnv *, jobject, jbyte flag)
{
	jint ret = g_RB.OpenRoomPrivateMicFlag(flag);
	return ret;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    CrownKickoutSomeOne
 * Signature: (I)I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_CrownKickoutSomeOne
  (JNIEnv *, jobject, jint idx)
{
	jint ret = g_RB.CrownKickoutSomeOne(idx);
	return ret;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    ForbidUserIp
 * Signature: (I)I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_ForbidUserIp
  (JNIEnv *, jobject, jint idx)
{
	jint ret = g_RB.ForbidUserIp(idx);
	return ret;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    SingerScores
 * Signature: (I[SI)I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_SingerScores
  (JNIEnv *env, jobject, jint idx, jshortArray aScores)
{
	int count = env->GetArrayLength(aScores);
	jshort* pScores = (jshort*)env->GetPrimitiveArrayCritical(aScores,0);
	jint ret = g_RB.SingerScores(idx,pScores,count);
	env->ReleasePrimitiveArrayCritical(aScores,pScores,0);
	return ret;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    UpdateSong
 * Signature: (I)I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_UpdateSong
  (JNIEnv *, jobject, jint idx)
{
	jint ret = g_RB.UpdateSong(idx);
	return ret;
}

 /*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    GetUserInfoForSpeaker
 * Signature: ([I)I
 */
JNIEXPORT jint JNICALL Java_com_jni_netutil_CJniUtil_GetUserInfoForSpeaker
  (JNIEnv *env, jobject, jintArray idx)
{
	jint len = env->GetArrayLength(idx);
	jint* pInt = (jint*)env->GetPrimitiveArrayCritical(idx,0);
	jint ret = g_RB.GetUserInfoForSpeaker((int*)pInt,len);
	env->ReleasePrimitiveArrayCritical(idx,pInt,0);
	return ret;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    GetUserInfoWithVersion
 * Signature: ([Lcom/jni/netutil/RequestData_IdxWithVersion;)I
 */
JNIEXPORT jint JNICALL Java_com_jni_netutil_CJniUtil_GetUserInfoWithVersion
  (JNIEnv *env, jobject, jobjectArray idx)
{
	jint ret = -1;

	/*
	 * static 修饰 jclass 对于Android5.0以下的版本使用static 保存jclass是没有问题的
	 * 但是Android5.0下使用会导致崩溃,所以这里把它去掉了
	*/
	jclass cls_RequestData_IdxWithVersion =
		env->FindClass("com/jni/netutil/RequestData_IdxWithVersion");
	if(!cls_RequestData_IdxWithVersion)
	{
		LOGW("%s : com/jni/netutil/RequestData_IdxWithVersion class can't find!\n",__FUNCTION__);
		return ret;
	}
	static jfieldID jf_id = env->GetFieldID(cls_RequestData_IdxWithVersion,"id","I");
	static jfieldID jf_version = env->GetFieldID(cls_RequestData_IdxWithVersion,"version","I");

	if(!jf_id || !jf_version)
		return ret;

	jint len = env->GetArrayLength(idx);

	RequestData_IdxWithVersion* pIdx = new RequestData_IdxWithVersion[len];
	if(!pIdx)
	{
		LOGW("%s : OOM!\n");
		return ret;
	}
	for(int i = 0;i<len;i++)
	{
		jobject obj = env->GetObjectArrayElement(idx,i);
		pIdx[i].id = (int)env->GetIntField(obj,jf_id);
		pIdx[i].version = (int)env->GetIntField(obj,jf_version);
		env->DeleteLocalRef(obj);
	}

	int remain = len;
	int cur = 0;
	do {
		if(remain == 0)
			break;

		if(remain > 200)
		{
			ret = g_RB.GetUserInfoWithVersion(&pIdx[cur], 200);
			if(ret == -1)
				break;
			cur += 200;
			remain -= 200;
		}
		else
		{
			ret = g_RB.GetUserInfoWithVersion(&pIdx[cur], remain);
			remain = 0;
		}
	} while (remain > 0);

	delete pIdx;
	return ret;
}
 /*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    InitDownLoadUrl
 * Signature: (Ljava/lang/String;I)I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_InitDownLoadUrl
  (JNIEnv *env, jobject, jstring url, jint forUse)
{
	GETUTFFROMJSTRINGBEGIN(cUrl,url);
	g_DM.initDownloadUrl(cUrl,forUse);
	GETUTFFROMJSTRINGEND(cUrl,url);
	return 0;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    InitLoaclDatabaseDir
 * Signature: (Ljava/lang/String;)I
 */
 JNIEnv *g_env = NULL;
 jobject g_obj = NULL;
 jint JNICALL Java_com_jni_netutil_CJniUtil_InitLocalDatabaseDir
  (JNIEnv *env, jobject obj, jstring path)
{
	g_env = env;
	g_obj = obj;
	GETUTFFROMJSTRINGBEGIN(cPath,path);
	bool b = g_DM.initRootDir(cPath);
	GETUTFFROMJSTRINGEND(cPath,path);

	return b?0:-1;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    GetMyInfo
 * Signature: ()Lcom/jni/netutil/LocalData_UserInfo;
 */
 jobject JNICALL Java_com_jni_netutil_CJniUtil_GetMyInfo
  (JNIEnv *env, jobject)
{
	USER_DATA_BASE* pMy = g_DM.getMyInfo();

	return GetMyInfo(env,pMy);
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    GetSelfData
 * Signature: ()Lcom/jni/netutil/ResultData_SelfData;
 */
 jobject JNICALL Java_com_jni_netutil_CJniUtil_GetSelfData
  (JNIEnv *env, jobject)
{
	return GetSelfData(env);
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    GetMyIdx
 * Signature: ()I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_GetMyIdx
  (JNIEnv *env, jobject)
{
	return g_DM.getMyIdx();
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    IsWangtong
 * Signature: (Ljava/lang/String;Ljava/lang/String;)I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_IsWangtong
  (JNIEnv *env, jobject, jstring host, jstring strWangtong)
{
	jint ret = 0;
	GETUTFFROMJSTRINGBEGIN(cHost,host);
	GETUTFFROMJSTRINGBEGIN(cStrWT,strWangtong);

	const char* pIp = NetworkUtil::ClientSocket::GetIpv4FromHostname(cHost);
	if (pIp[0] == 0)
		ret = 0;
	else
	{
		if(strstr(cStrWT, pIp))
			ret = 1;
		else
			ret = 0;
	}
	GETUTFFROMJSTRINGEND(cStrWT,strWangtong);
	GETUTFFROMJSTRINGEND(cHost,host);

	return ret;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    GetRoomInfoFromLocal
 * Signature: (I)Lcom/jni/netutil/ResultData_RoomInfo;
 */
 jobject JNICALL Java_com_jni_netutil_CJniUtil_GetRoomInfoFromLocal
  (JNIEnv *env, jobject, jint roomid)
{
	return CJniResponse::getSingleton()->getRoomInfo(env,roomid);
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    GetUserInfoFromLocal
 * Signature: (I)Lcom/jni/netutil/LocalData_UserInfo;
 */
 jobject JNICALL Java_com_jni_netutil_CJniUtil_GetUserInfoFromLocal
  (JNIEnv *env, jobject, jint idx)
{
	return GetUserInfo(env,idx);
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    GetGiftInfoFromLocal
 * Signature: (I)Lcom/jni/netutil/GiftInfo;
 */
 jobject JNICALL Java_com_jni_netutil_CJniUtil_GetGiftInfoFromLocal
  (JNIEnv *env, jobject, jint giftId)
{
	return GetGiftInfo(env,giftId);
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    GetAllGiftId
 * Signature: ()[I
 */
 jintArray JNICALL Java_com_jni_netutil_CJniUtil_GetAllGiftId
  (JNIEnv *env, jobject)
{
	
	int nNum = 0;
	int* pGiftId = NULL;
	g_DM.getAllGiftId(0,nNum);
	jintArray ret = env->NewIntArray(nNum);

	if(!ret)
		return ret;

	if(nNum > 0)
	{
		pGiftId = new int[nNum];
		if(!pGiftId)
		{
			LOGE("gift id array new failed!\n");
			return ret;
		}
		g_DM.getAllGiftId(pGiftId,nNum);
		env->SetIntArrayRegion(ret,0,nNum,(const jint*)pGiftId);

		delete pGiftId;
	}

	return ret;
}
/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    SendChatKTV
 * Signature: (ILjava/lang/String;)I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_SendChatKTV
  (JNIEnv *env, jobject, jint toIdx, jstring content)
{
	//GETGBKFROMJSTRINGBEGIN(cContent,content);
	 const char* cContent = env->GetStringUTFChars(content,0);
	jint ret = g_RB.SendChatKTV(toIdx,cContent);
	env->ReleaseStringUTFChars(content,cContent);
	//GETGBKFROMJSTRINGEND(cContent,content);

	return ret;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    SendGiftInLobby
 * Signature: (IIII)I
 */
 jint JNICALL Java_com_jni_netutil_CJniUtil_SendGiftInLobby
  (JNIEnv *, jobject, jint toIdx,jint giftId,jint number,jint roomId)
{
	return g_RB.SendGiftInLobby(toIdx,giftId,number,roomId);
}
 /*
  * Class:     com_jni_netutil_CJniUtil
  * Method:    MFInitFilter
  * Signature: ()Z
  */
 jboolean JNICALL Java_com_jni_netutil_CJniUtil_MFInitFilter
   (JNIEnv *env, jobject)
 {
	 if(g_bWordFilterInit)
		 return 1;
	 g_bWordFilterInit = CMessageFilter::getSingle()->initFilterForAndroid(DOG_FILTER1
			 ,strlen(DOG_FILTER1),DOG_FILTER2,strlen(DOG_FILTER2));
	 jboolean ret =g_bWordFilterInit;
	 return ret;
 }

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    MFFilter
 * Signature: (Ljava/lang/String;I)Ljava/lang/String;
 */
 jstring JNICALL Java_com_jni_netutil_CJniUtil_MFFilter
  (JNIEnv *env, jobject, jstring utf8)
 {
	 jstring ret = NULL;
	 GETUTFFROMJSTRINGBEGIN(cUtf8,utf8);
	 int len = strlen(cUtf8?cUtf8:"");
	 if(len > 0)
	 {
		 int size = len+1;
		 char* pStr = new char[size];
		 memset(pStr,0,size);
		 if(!pStr)
			 ret = env->NewStringUTF("");
		 else
		 {
			 memcpy(pStr,cUtf8,len);
			 bool b = CMessageFilter::getSingle()->filter_first(pStr,len);
			 if(b)
				ret = env->NewStringUTF(pStr);
			 else 
				 ret = env->NewStringUTF("");
			 delete pStr;
		 }
	 }
	 else
		 ret = env->NewStringUTF("");
	 GETUTFFROMJSTRINGEND(cUtf8,utf8);
	 
	 return ret;
 }

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    MFFindSensWord
 * Signature: (Ljava/lang/String;)Z
 */
 jboolean JNICALL Java_com_jni_netutil_CJniUtil_MFFindSensWord
  (JNIEnv *env, jobject, jstring utf8)
 {
	 GETUTFFROMJSTRINGBEGIN(cUtf8,utf8);
	 jboolean ret = CMessageFilter::getSingle()->find_sens_word(cUtf8);
	 GETUTFFROMJSTRINGEND(cUtf8,utf8);
	 return ret;
 }

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    MFFindSensWord2
 * Signature: (Ljava/lang/String;)Z
 */
 jboolean JNICALL Java_com_jni_netutil_CJniUtil_MFFindSensWord2
  (JNIEnv *env, jobject, jstring utf8)
 {
	 GETUTFFROMJSTRINGBEGIN(cUtf8,utf8);
	 jboolean ret = CMessageFilter::getSingle()->find_sens_word2(cUtf8);
	 GETUTFFROMJSTRINGEND(cUtf8,utf8);
	 return ret;
 }

 /*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    RequestSendHB
 * Signature: (BIILjava/lang/String;I)I
 */
jint JNICALL Java_com_jni_netutil_CJniUtil_RequestSendHB
  (JNIEnv * env, jobject, jbyte hbtype, jint number, jint gold, jstring mark, jint extradata)
{
	//GETUTFFROMJSTRINGBEGIN(cMark,mark);
	GETGBKFROMJSTRINGBEGIN(cMark,mark);
	//LOGI("%s,%d\n",cMark,strlen(cMark));
	jint ret = g_RB.RequestSendHB(hbtype,number,gold,cMark,extradata);
	//GETUTFFROMJSTRINGEND(cMark,mark);
	GETGBKFROMJSTRINGEND(cMark,mark);

	return ret;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    RequestHBStatus
 * Signature: (Ljava/lang/String;)I
 */
jint JNICALL Java_com_jni_netutil_CJniUtil_RequestHBStatus
  (JNIEnv * env, jobject, jstring hbid)
{
	GETUTFFROMJSTRINGBEGIN(cHbId,hbid);
	jint ret = g_RB.RequestHBStatus(cHbId);
	GETUTFFROMJSTRINGEND(cHbId,hbid);
	return ret;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    RequestVieHB
 * Signature: (Ljava/lang/String;)I
 */
jint JNICALL Java_com_jni_netutil_CJniUtil_RequestVieHB
  (JNIEnv * env, jobject, jstring hbid)
{
	GETUTFFROMJSTRINGBEGIN(cHbId,hbid);
	jint ret = g_RB.RequestVieHB(cHbId);
	GETUTFFROMJSTRINGEND(cHbId,hbid);
	return ret;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    RequestHBDetail
 * Signature: (Ljava/lang/String;)I
 */
jint JNICALL Java_com_jni_netutil_CJniUtil_RequestHBDetail
  (JNIEnv * env, jobject, jstring hbid)
{
	GETUTFFROMJSTRINGBEGIN(cHbId,hbid);
	jint ret = g_RB.RequestHBDetail(cHbId);
	GETUTFFROMJSTRINGEND(cHbId,hbid);
	return ret;
}

/*
 * Class:     com_jni_netutil_CJniUtil
 * Method:    RequestCurrentRoomHB
 * Signature: (I)I
 */
jint JNICALL Java_com_jni_netutil_CJniUtil_RequestCurrentRoomHB
  (JNIEnv *env, jobject, jint roomid)
{
	jint ret = g_RB.RequestCurrentRoomHB(roomid);
	return ret;
}

