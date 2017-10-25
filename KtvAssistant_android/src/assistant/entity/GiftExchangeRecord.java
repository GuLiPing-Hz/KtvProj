package assistant.entity;

import org.json.JSONObject;

import android.text.TextUtils;
import assistant.task.PCommonUtil;

public class GiftExchangeRecord {

	public String code = "";
	public int cost;
	public String begin = "";//yyyy/mm/dd
	public String end = "";
	public String giftname = "";
	public String shopname = "";
	public String unit = "";
	public int number;
	//0:未领取  1:已领取  2:过期
	public int type;
	public GiftExchangeRecord(JSONObject obj,int type) {
		try {
			
			this.type = type;
			if (null != obj) {
				String str =obj.optString("code");
				if(!TextUtils.isEmpty(str))
					code =PCommonUtil.decodeBase64(str);
				cost = obj.optInt("cost");
				str = obj.optString("begin");
				if(!TextUtils.isEmpty(str))
					begin =PCommonUtil.decodeBase64(str);
				str = obj.optString("end");
				if(!TextUtils.isEmpty(str))
					end =PCommonUtil.decodeBase64(str);
				str = obj.optString("gname");
				if(!TextUtils.isEmpty(str))
					giftname =PCommonUtil.decodeBase64(str);
				str = obj.optString("kname");
				if(!TextUtils.isEmpty(str))
					shopname =PCommonUtil.decodeBase64(str);
				str = obj.optString("unit");
				if(!TextUtils.isEmpty(str))
					unit =PCommonUtil.decodeBase64(str);
				number = obj.optInt("num");
			}

		} catch (Exception e) {
			e.printStackTrace();
		}
	}

}
