package assistant.entity;

import org.json.JSONObject;

import android.text.TextUtils;
import assistant.task.PCommonUtil;

public class GiftSendRecv {
	
	public int type;//1 收礼  2 送礼
	public String name = "";
	public String photo = "";
	public String date = "";
	public String giftname = "";
	public int giftnum = 0;
	public String giftUnit = "个";
	public int gold = 0;
	public String shopname = "";
	public String roomname = "";
	public GiftSendRecv(JSONObject obj,int type) {
		try {
			if (null != obj) {
				this.type = type;
				String str =obj.optString("name");
				if(!TextUtils.isEmpty(str))
					name = PCommonUtil.decodeBase64(str);
				str = obj.optString("photo");
				if(!TextUtils.isEmpty(str))
					photo = PCommonUtil.decodeBase64(str);
				str = obj.optString("tdate");
				if(!TextUtils.isEmpty(str))
					date = PCommonUtil.decodeBase64(str);
				str = obj.optString("gname");
				if(!TextUtils.isEmpty(str))
					giftname = PCommonUtil.decodeBase64(str);
				giftnum = obj.optInt("gnum");
				str = obj.optString("unit");
				if(!TextUtils.isEmpty(str))
					giftUnit = PCommonUtil.decodeBase64(str);
				gold = obj.optInt("gold");
				str = obj.optString("kname");
				if(!TextUtils.isEmpty(str))
					shopname = PCommonUtil.decodeBase64(str);
				str = obj.optString("rname");
				if(!TextUtils.isEmpty(str))
					roomname = PCommonUtil.decodeBase64(str);
			}

		} catch (Exception e) {
			e.printStackTrace();
		}
	}

}
