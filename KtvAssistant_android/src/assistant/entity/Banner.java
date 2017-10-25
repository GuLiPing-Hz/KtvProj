package assistant.entity;

import org.json.JSONObject;

import android.text.TextUtils;
import assistant.task.PCommonUtil;
import assistant.util.ShowLog;

/**
 * @author 宋静
 * @version 创建时间：2013-12-9 下午5:42:52 类说明
 */
public class Banner {
	public long id; // bannerID
	public String photoUrl; // banner图片
	public String activityUrl="";//活动跳转地址,当bannertype为URL_ACTIVITY的时候有效
	public String title; // banner标题
	public int bannertype; // banner类型
	public SongParameter songparameter; // 该字段只有在bannertype为1到10时才有

	public Banner(JSONObject jsonObject) {

		try {
			if (null != jsonObject) {
				this.id = jsonObject.optLong("id");
				String temp = jsonObject.optString("activityurl");
				if(temp != null && !TextUtils.isEmpty(temp))
					activityUrl = PCommonUtil.decodeBase64(temp);
				this.photoUrl =PCommonUtil.decodeBase64(jsonObject
						.optString("imgpath"));
				this.title = PCommonUtil.decodeBase64(jsonObject
						.optString("title"));
				this.bannertype = jsonObject.optInt("bannertype");
				JSONObject jsonSong = jsonObject.optJSONObject("songparameter");
				if(jsonSong != null)
					songparameter = new SongParameter(jsonSong);
			}

		} catch (Exception e) {
			ShowLog.showException(e);
		}

	}

	public Banner(long id, String photoUrl, String title, int bannertype, int modelId, int topicId) {
		super();
		this.id = id;
		this.photoUrl = photoUrl;
		this.title = title;
		this.bannertype = bannertype;
		if(modelId > 0){
			songparameter = new SongParameter(modelId, topicId);
		}
	}
	
	@Override
	public String toString() {
		return "Banner [id=" + id + ", photoUrl=" + photoUrl + ", title="
				+ title + ", bannertype=" + bannertype + ", songparameter="
				+ (songparameter==null?"null":songparameter.toString()) + "]";
	}

	public class SongParameter {
		public int modelId;
		public int topicId;
		public int artistId; // 当需要链接到歌手分类时需要增加对应值
		public String artistName; // 当需要链接到歌手分类时需要增加对应值
		
		public SongParameter(int modelId, int topicId) {
			super();
			this.modelId = modelId;
			this.topicId = topicId;
		}

		public SongParameter(JSONObject jsonObject) {

			try {
				if (null != jsonObject) {
					this.modelId = jsonObject.optInt("modelid");
					this.topicId = jsonObject.optInt("topicid");
					this.artistId = jsonObject.optInt("artistid");
					this.artistName =PCommonUtil.decodeBase64(jsonObject
							.optString("artistName"));
				}

			} catch (Exception e) {
				ShowLog.showException(e);
			}

		}

		@Override
		public String toString() {
			return "SongParameter [modelId=" + modelId + ", topicId=" + topicId
					+ ", artistId=" + artistId + ", artistName=" + artistName
					+ "]";
		}
	}

	public interface BannerType{
		int NORMAL_SONGS = 1;
		int CHART_SONGS = 2;
		int URL_ACTIVITY = 3;
	}	
}
