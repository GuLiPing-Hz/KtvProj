package assistant.entity;

import org.json.JSONObject;

import android.text.TextUtils;
import assistant.task.PCommonUtil;
import assistant.util.ShowLog;

/**
 * @author 宋静
 * @version 创建时间：2013-12-9 下午2:44:44
 * 类说明 高分榜歌曲
 */
public class WorkInfo {	
	public long workId;		//演唱记录ID
	public int score;				//演唱的分数
	public String address;	//演唱地点
	public SongInfo song;
	public UserInfo user;
	
	public WorkInfo(JSONObject jsonObject) {

		try {
			if (null != jsonObject) {
				this.workId = jsonObject.optLong("workid");
				this.score =jsonObject.optInt("score");
				String str = jsonObject.optString("address");
				if(!TextUtils.isEmpty(str))
					address =PCommonUtil.decodeBase64(str);
				
				JSONObject jsonSong = jsonObject.optJSONObject("song");
				if(jsonSong != null)
					song = new SongInfo(jsonSong);
				JSONObject jsonUser = jsonObject.optJSONObject("user");
				if(jsonUser != null)
					user = new UserInfo(jsonUser);
			}

		} catch (Exception e) {
			ShowLog.showException(e);
		}

	}

	@Override
	public String toString() {
		return "WorkInfo [workId=" + workId + ", score=" + score + ", address="
				+ address + ", song=" + (song == null ? "NULL":song.toString()) + ", user=" + (user== null ? "NULL":user.toString()) + "]";
	}

}
