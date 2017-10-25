package assistant.entity;

import java.io.Serializable;

import org.json.JSONObject;

import android.text.TextUtils;
import assistant.task.PCommonUtil;
import assistant.util.ShowLog;

/**
 * @author 宋静
 * @version 创建时间：2013-12-9 下午7:24:26
 * 类说明
 */
public class MessageInfo implements Serializable{
	public long id;		//消息ID
	public int msgType; 	//消息类型（系统消息/歌曲上榜/未知消息 1/2/3…）
	public String content;		//消息内容		
	public String date;		//消息时间（YYYY-MM-DD HH:mm:ss）
	public long workId; 
//	public long roomId;
	public int point;
	public WorkInfo work;
	public int readFlag = 0;//1表示已读，0表示未读, 2表示已删除
	public int tvColor = 0;//1表示已读颜色，0表示未读颜色
	
	private static final long serialVersionUID = -98765534323565L;
	
	public MessageInfo(){
	}
	
	public MessageInfo(JSONObject jsonObject) {
		updateData(jsonObject);
	}

	@Override
	public String toString() {
		return "MessageInfo [id=" + id + ", msgType=" + msgType + ", content="
				+ content + ", date=" + date + ", workId=" + workId
				+ ", point=" + point + ", work=" + work + ", readFlag="
				+ readFlag + "]";
	}
	
	public void updateData(JSONObject jsonObject){
		try {
			if (null != jsonObject) {
				id = jsonObject.optLong("msgid");
				msgType = jsonObject.optInt("msgtype");
				String str = jsonObject.optString("content");
				if(!TextUtils.isEmpty(str))
					content =PCommonUtil.decodeBase64(str);
				
				str = jsonObject.optString("date");
				if(!TextUtils.isEmpty(str))
					date =PCommonUtil.decodeBase64(str);
				workId = jsonObject.optLong("workid");
				point = jsonObject.optInt("point");
				
				JSONObject jsonWork = jsonObject.optJSONObject("work");
				if(jsonWork != null){
					work = new WorkInfo(jsonWork);
				}
			}

		} catch (Exception e) {
			ShowLog.showException(e);
		}

	}
}
