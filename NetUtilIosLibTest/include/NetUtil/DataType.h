/*
 * DataType.h
 *
 *  Created on: 2014-5-13
 *  author: glp
 */

#ifndef DATATYPE__H__
#define DATATYPE__H__

#include "ResultData.h"

typedef struct _ROOM_DATA
{
	ResultData_RoomInfo base;
	short lobby_count;
	short lobby_boy;
	short lobby_girl;
	short re_player_count;
	char logopath[256];			//logo路径
	int updata_time;
	int family_idx;				//所属家族

	char get_mic;
	char on_mic;
	char privatemicflag;
}ROOM_DATA;

enum eINFO_STATE
{
	INFO_STATE_NULL,
	INFO_STATE_GETING,
	IF_STATE_OK
};

enum eRELATIONSHIP
{
	RELATIONSHIP_NONE,
	RELATIONSHIP_FRIEND,
	RELATIONSHIP_BLACK,
	RELATIONSHIP_SELF,
	RELATIONSHIP_SYSTEM,
	RELATIONSHIP_MAX,
};

typedef struct _USER_INFO
{
	ResultData_UserFullInfo	base;	// 基本信息

	int lobbyid;					// 大厅id
	int roomid;						// 房间id
	int notify;						//
	eRELATIONSHIP relationship;				// 与本人的关系

}USER_INFO;

typedef struct _USER_DATA_BASE
{
    eINFO_STATE name_state;//昵称是否有效
	eINFO_STATE info_state;//ResultData_UserFullInfo 是否有效
	eINFO_STATE location_state;//位置是否有效
	eINFO_STATE part_state;//

	USER_INFO user_info;
	int version;
}USER_DATA_BASE;

typedef struct _GiftInfo
{
    int  id;					    //商品id
    int order;					//排序
    char name[200];                 //名称
	char image_name[200];           //图片文件名(含后缀)
	char image_url[200];            //图片URL
	int  cash;				        //金币
	int  coin;				        //银币 deprecated
    char unit_name[200];	//单位
    int  effectId;                //礼物特效ID 0认为是没有特效
	int gift_type;				//礼物类别,幸运1，普通2，奢侈3
}GiftInfo;

#endif//
