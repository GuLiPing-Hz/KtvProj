package com.jni.netutil.out;

public class HornInfoToUi {

	public int from_idx;
	public String room_name;
	public String msg;
	public int time;
	
	public final static int BT_BROAD = 0;//普通喇叭
	public final static int BT_EXCHANGE = 1;//礼物兑换
	public final static int BT_RANKSONG = 2;//歌曲上榜
	public int horn_type;
	
	public boolean isSend = false;
	
	public HornInfoToUi() {
		// TODO Auto-generated constructor stub
	}

}
