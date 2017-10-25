package assistant.entity;

import com.jni.netutil.ResultData_SendFlowerInfo;

public class GiftDisplayInfo {
	public int s_idx = 0;						//送礼者IDX
	public int s_roomid = 0;              //发送者房间
	public String s_room = "";      //发送者房间名字
	public int r_idx = 0;						//收礼者IDX
	public int r_roomid = 0;               //接受者房间
	public String r_room = "";       //收礼者房间名字
	public int cate_idx = 0;				//礼物id
	public int number = 0;			        //礼物总数
	public int leavings = 0;		        //礼物剩余数
	public int time = 0;
	public int once_num = 0;				//每次送礼的数量
	
	public String s_name = "";//送礼者名字
	public String r_name = "";//收礼者名字
	public String s_photo = "";//送礼者头像
	public String giftname = "";//礼物名称
	public String gifturl = "";//礼物本地路径图片
	public String giftunit = "";//礼物单位
	
	public GiftDisplayInfo(ResultData_SendFlowerInfo gift) {
		s_idx = gift.s_idx;						//送礼者IDX
		s_roomid = gift.s_roomid;              //发送者房间
		if(s_roomid != 0)
			s_room = gift.s_room;      //发送者房间名字
		r_idx = gift.r_idx;						//收礼者IDX
		r_roomid = gift.r_roomid;               //接受者房间
		if(r_roomid != 0)
			r_room = gift.r_room;       //收礼者房间名字
		cate_idx = gift.cate_idx;				//礼物类别
		number = gift.number;			        //礼物总数
		leavings = gift.leavings;		        //礼物剩余数
		time = gift.time;
		once_num = gift.once_num;				//每次送礼的数量
	}

}
