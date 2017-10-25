package assistant.entity;

import org.json.JSONObject;

import android.text.TextUtils;
import assistant.task.PCommonUtil;
import assistant.util.ShowLog;

public class SongRecord {
	public String songname = "";
	public String singername = "";
	public int score = 0;
	public String room = "";
	public String time = "";
	
	public SongRecord(JSONObject jsonObject) {

		try {
			if (null != jsonObject) {
				String str = jsonObject.optString("music_name");
				if(!TextUtils.isEmpty(str))
					songname =PCommonUtil.decodeBase64(str);
				
				str = jsonObject.optString("music_singer");
				if(!TextUtils.isEmpty(str))
					singername =PCommonUtil.decodeBase64(str);
				
				score = jsonObject.optInt("top_score");
				
				str = jsonObject.optString("Room_name");
				if(!TextUtils.isEmpty(str))
					room =PCommonUtil.decodeBase64(str);
				
				str = jsonObject.optString("top_date");
				if(!TextUtils.isEmpty(str))
					time =PCommonUtil.decodeBase64(str);
			}

		} catch (Exception e) {
			ShowLog.e("Parser SongInfo failed...please check");
		}

	}
}
