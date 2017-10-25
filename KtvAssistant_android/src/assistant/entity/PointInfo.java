package assistant.entity;

import org.json.JSONObject;

import assistant.task.PCommonUtil;
import assistant.util.ShowLog;

/**
 * @author 宋静
 * @version 创建时间：2013-12-4 下午3:07:48 类说明 积分信息
 */
public class PointInfo {
	public long id; // 积分获取渠道ID
	public int pointType; // 积分类型
	public String name; // 积分获取渠道内容
	public int points; // 可获取的积分值
	public int flag; // 0未完成；1已完成

	public PointInfo(int id, String name, int points, int flag) {
		super();
		this.id = id;
		this.name = name;
		this.points = points;
		this.flag = flag;
	}

	public PointInfo(JSONObject jsonObject) {
		try {
			if (null != jsonObject) {
				id = jsonObject.optLong("sourceid");
				pointType = jsonObject.optInt("sourcetype");
				name = PCommonUtil.decodeBase64(jsonObject
						.optString("source"));
				points = jsonObject.optInt("points");
				flag = jsonObject.optInt("flag");
			}
		} catch (Exception e) {
			ShowLog.showException(e);
		}
	}

	@Override
	public String toString() {
		return "PointInfo [id=" + id + ", name=" + name + ", points=" + points
				+ ", flag=" + flag + "]";
	}
}
