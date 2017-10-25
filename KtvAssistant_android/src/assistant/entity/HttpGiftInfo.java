package assistant.entity;

import org.json.JSONObject;

import android.text.TextUtils;
import assistant.task.PCommonUtil;

public class HttpGiftInfo {

	public int id;//礼物ID
	public String info = "";//礼物介绍
	public String name = "";//礼物名字
	public String bimg = "";//礼物大图片
	public String simg = "";//礼物小图片
	public int price;//价格
	public HttpGiftInfo(JSONObject obj) {
		try {
			if (null != obj) {
				id = obj.optInt("id");
				String str =obj.optString("info");
				if(!TextUtils.isEmpty(str))
					info =PCommonUtil.decodeBase64(str);
				str = obj.optString("name");
				if(!TextUtils.isEmpty(str))
					name =PCommonUtil.decodeBase64(str);
				str = obj.optString("bimg");
				if(!TextUtils.isEmpty(str))
					bimg =PCommonUtil.decodeBase64(str);
				str = obj.optString("simg");
				if(!TextUtils.isEmpty(str))
					simg =PCommonUtil.decodeBase64(str);
				price = obj.optInt("score");
			}

		} catch (Exception e) {
			e.printStackTrace();
		}
	}

}
