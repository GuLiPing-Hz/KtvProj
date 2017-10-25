package assistant.entity;

import org.json.JSONException;
import org.json.JSONObject;

import assistant.task.PCommonUtil;

public class KtvHongBaoInfo {
	//0-新人奖励，1-每日奖励，2-个人红包
	public int hbnclass = -1;
	public String hbclass = "";//红包分类
	public String hbmission = "";//红包说明
	//0-未完成，1-完成，待领取，2-已领取
	public int hbstatus = 0;//红包状态
	public String hbid = "";//红包ID
	public String hbtime = "";//红包时间
	public int gold = 0;//红包金额
	
	public KtvHongBaoInfo(JSONObject jsonObject){
		super();
		
		if(jsonObject != null)
		{
			try {
				hbid = jsonObject.getString("ID");
				hbnclass = jsonObject.getInt("RewardType");
				switch(hbnclass)
				{
					case 0:
					{
						hbclass = "新人红包";
						break;
					}
					case 1:
					{
						hbclass = "每日奖励";
						break;
					}
					case 2:
					{
						hbclass = "个人红包";
						break;
					}
				}
				
				String str = jsonObject.getString("DescrInfo");
				if(str != null)
					hbmission = PCommonUtil.decodeBase64(str);
				hbstatus = jsonObject.getInt("Status");
				str = jsonObject.getString("AddTime");
				if(str != null)
					hbtime = PCommonUtil.decodeBase64(str);
				gold = jsonObject.getInt("RewardGold");
				
			} catch (JSONException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}	
		}
	}
}
