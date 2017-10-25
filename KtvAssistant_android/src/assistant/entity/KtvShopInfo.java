package assistant.entity;

import org.json.JSONObject;

import android.text.TextUtils;
import assistant.task.PCommonUtil;

public class KtvShopInfo {

	public int id;// id
	public String name = "";// ktv店名称
	public String address = "";// ktv店地址

	public KtvShopInfo(JSONObject jsonObject) {
		try {
			if (null != jsonObject) {
				id = jsonObject.optInt("id");
				String str = jsonObject.optString("name");
				if (!TextUtils.isEmpty(str))
					name = PCommonUtil.decodeBase64(str);
				str = jsonObject.optString("address");
				if (!TextUtils.isEmpty(str))
					address = PCommonUtil.decodeBase64(str);
			}

		} catch (Exception e) {
			e.printStackTrace();
		}
	}
}
