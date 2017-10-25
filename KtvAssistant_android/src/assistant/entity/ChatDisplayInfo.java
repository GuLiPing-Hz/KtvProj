package assistant.entity;

import com.jni.netutil.out.HongBaoInfoToUi;


public class ChatDisplayInfo {

	public ChatInfo chat = null;
	public GiftDisplayInfo gift = null;
	public HornDisplayInfo horn = null;
	public RewardInfo reward = null;
	public HongBaoInfoToUi hongbao= null;
	
	//公聊
	public final static int ChatPublic = 0;
	//私聊
	public final static int ChatPrivate = 1;
	//送礼信息
	public final static int ChatGift = 2;
	//喇叭
	public final static int ChatHorn = 3;
	//兑换礼物
	public final static int ChatExchange = 4;
	//演唱上榜通知
	public final static int ChatRank = 5;
	//送礼中奖
	public final static int ChatReword = 6;
	//红包
	public final static int ChatRedPacket = 7;
	public int type = -1;//聊天类型 为以上几种类型
	
	//0或者是自己的idx代表自己
	public int tagIdx = 0;//代表无
	public String tagName = "";
	
	public final static int PosLeft = 1;
	public final static int PosRight = 2;
	public final static int PosCenter = 3;
	public int position = 1;
	public ChatDisplayInfo() {
		// TODO Auto-generated constructor stub
	}

}
