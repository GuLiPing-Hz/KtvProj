package assistant.entity;

import java.io.Serializable;

import org.json.JSONException;
import org.json.JSONObject;

import android.text.TextUtils;
import assistant.task.PCommonUtil;
import assistant.util.ShowLog;


/**
 * 
 * @author apple 点播歌曲:Assistant/OrderSong 参数int ktvid, int roomid, int Songid,
 *         string Songname, string Artistname, char Isscore
 */
public class SongInfo implements Serializable {
	private static final long serialVersionUID = 4002645020810359739L;
	public int songId;
	public String songName;
	public String songArtist;
	public int songIsScore;// 1-可评分歌曲，0-不可评分歌曲
	public boolean isOrdered;// 是否已经被点歌
	public boolean isPreOrdered;// 是否已经被预点歌
	public long preTime;
	public String photo;   //歌曲头像
	
	public SongInfo(){}
	
	public SongInfo(JSONObject jsonObject) {

		try {
			if (null != jsonObject) {
				this.songId = jsonObject.optInt("music_id");
				String str = jsonObject.optString("music_name");
				if(!TextUtils.isEmpty(str))
					songName =PCommonUtil.decodeBase64(str);
				
				str = jsonObject.optString("music_singer");
				if(!TextUtils.isEmpty(str))
					songArtist =PCommonUtil.decodeBase64(str);
				
				this.songIsScore = jsonObject.optInt("music_isScore");
				this.preTime = jsonObject.optLong("preTime");
				
				str = jsonObject.optString("music_photo");
				if(!TextUtils.isEmpty(str))
					photo =PCommonUtil.decodeBase64(str);
			}

		} catch (Exception e) {
			ShowLog.i("Parser SongInfo failed...please check");
		}

	}

	public void log() {
		//ShowLog.i("Print [songId=" + songId + ", songName=" + songName
				//+ ", songArtist=" + songArtist + ", songIsScore=" + songIsScore
				//+ ", isOrdered=" + isOrdered + ", isPreOrdered=" + isPreOrdered
				//+ ", preTime=" + preTime + "]");
	}
	
	public String toString() {
			return ("SongInfo [songId=" + songId + ", songName=" + songName
					+ ", songArtist=" + songArtist + ", songIsScore=" + songIsScore
					+ ", isOrdered=" + isOrdered + ", isPreOrdered=" + isPreOrdered
					+ ", preTime=" + preTime + "]");
		
	}

	//写成本地文件用
	public JSONObject toJson(){
		isPreOrdered = true;
		
		JSONObject json = new JSONObject();
		try {
			json.put("music_id", songId);
			json.put("music_name", PCommonUtil.encodeBase64(songName));
			json.put("music_singer", PCommonUtil.encodeBase64(songArtist));
			json.put("music_isScore", songIsScore);
			json.put("preTime", preTime);
		} catch (JSONException e) {
			ShowLog.showException(e);
		}
		
		return json;
	}
	
	public SongInfo(int songId, String songName, String songArtist,
			int songIsScore) {
		super();
		this.songId = songId;
		this.songName = songName;
		this.songArtist = songArtist;
		this.songIsScore = songIsScore;
	}
}
