package assistant.entity;

import com.jni.netutil.out.RewardInfoToUi;

public class RewardInfo {
	
	public int idx = 0;
	public String name = "";
	public int roomid = 0;
	public int giftid = 0;
	public int times = 0;//幸运倍数
	
	public String photo = "";//用户头像
	public String room = "";
	public String giftname = "";
	public String gifturl = "";//本地路径的礼物图片
	public int gold = 0;//得到金币的倍数总量

	public RewardInfo(RewardInfoToUi reward) {
		name = reward.nickname;
		idx = reward.idx;
		roomid = reward.roomid;
		giftid = reward.giftid;
		times = reward.luckmultiple;
	}

}
