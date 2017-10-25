package assistant.entity;

import java.io.Serializable;

import org.json.JSONObject;

import assistant.task.PCommonUtil;
import assistant.util.ShowLog;


/**
 * 1.3版专题信息
 * @author pig
 *
 */
public class TopicInfo implements Serializable{
	private static final long serialVersionUID = 6637964644647682119L;
	public int topicId = 0;					//专题ID
	public String topicTitle = "";			//专题名称
	public String topicUrl = "";			//歌星头像url，只针对歌星有效
	public String topicContent = "";	//专题详细
	public String pinyin = "";				//拼音，只针对歌星有效
	public int hot = 0;						//热度
	public String topicImg = "";			//Top图片 - 新增图片，用于排行榜的图片显示
	
	public TopicInfo(JSONObject jsonObject){
		
		try {
			if(null != jsonObject){
				topicId = jsonObject.optInt("topicid");
				topicTitle = PCommonUtil.decodeBase64(jsonObject.optString("topicname"));
				topicUrl = PCommonUtil.decodeBase64(jsonObject.optString("topicurl"));
				pinyin = PCommonUtil.decodeBase64(jsonObject.optString("pinyin"));
				hot = jsonObject.optInt("artisthotval");
				topicImg = PCommonUtil.decodeBase64(jsonObject.optString("topicimg"));
			}
			
		} catch (Exception e) {
			ShowLog.i("Parser TopicInfo failed...please check");
		}
		
	}

	public TopicInfo(int topicId, String topicTitle) {
		super();
		this.topicId = topicId;
		this.topicTitle = topicTitle;
	}

	public TopicInfo(int topicId, String topicTitle, String topicUrl,
			String topicContent) {
		super();
		this.topicId = topicId;
		this.topicTitle = topicTitle;
		this.topicUrl = topicUrl;
		this.topicContent = topicContent;
	}	
	
	@Override
	public String toString() {
		return "TopicInfo [topicId=" + topicId + ", topicTitle=" + topicTitle
				+ ", topicUrl=" + topicUrl + ", topicContent=" + topicContent
				+ "]";
	}
}
