package assistant.entity;

import org.json.JSONObject;

import tiange.ktv.assistant.R;
import android.text.TextUtils;
import assistant.task.PCommonUtil;

//

public class GoldFromInfo {
	
	public enum GoldFromMsg{
		FIRSTLOGIN(1,"第一次登录",R.drawable.user_center_reward1),
		FIRSTUPPHOTO(2,"第一次上传头像",R.drawable.user_center_reward2),
		DAYSIGN(3,"每天登录签到",R.drawable.user_center_reward3),
		DAYENTERROOM(4,"每天登录包厢",R.drawable.user_center_reward4),
		FIRSTODER(5,"第一次点歌",R.drawable.user_center_reward5),
		SONGINTOP(6,"歌曲进入排行榜",R.drawable.user_center_reward6),
		SING(7,"歌曲演唱得分",R.drawable.user_center_reward7),
		FIRSTCHARGE(8,"首次充值98元",R.drawable.user_center_reward8),
		SCANROOMGOLD(9,"扫描包厢金币",R.drawable.user_center_reward9),
		ROOMGOLDGIVEOUT(10,"包厢金币分发",R.drawable.user_center_reward10);
		
		// 枚举对象构造函数
	    private GoldFromMsg(int type,String msg,int img) { 
	        this.type = type; 
	        this.msg = msg;
	        this.imgId = img;
	    }
		public int type;
		private String msg;
		private int imgId;
		
		public static String getMsg(int type){
			for(GoldFromMsg msg : GoldFromMsg.values())
			{
				if(msg.type == type)
					return msg.msg;
			}
			return "";
		}
		public static int getImage(int type){
			for(GoldFromMsg msg : GoldFromMsg.values())
			{
				if(msg.type == type)
					return msg.imgId;
			}
			return 0;
		}
	}
	
	public int gold = 0;
	public int type = 0;
	public String songname = "";
	public String score = "";
	public String time = "";//mm/dd hh:mm
	public String roomname = "";
	public String ktvname = "";
	
	public GoldFromInfo(JSONObject obj) {
		try {
			if (null != obj) {
				gold = obj.optInt("gold");
				type = obj.optInt("type");
				String str =obj.optString("detail");
				String detail = "";
				if(!TextUtils.isEmpty(str))
				{
					detail =PCommonUtil.decodeBase64(str);
					
					String[] details = detail.split("\\|");
					int pos = detail.indexOf("|");
					if(pos != -1)
					{
						songname = detail.substring(0, pos);
						score = detail.substring(pos+1);
					}
					else
						songname = detail;
				}
				
				str = obj.optString("addTime");
				if(!TextUtils.isEmpty(str))
					time =PCommonUtil.decodeBase64(str);
				str = obj.optString("rname");
				if(!TextUtils.isEmpty(str))
					roomname =PCommonUtil.decodeBase64(str);
				str = obj.optString("kname");
				if(!TextUtils.isEmpty(str))
					ktvname =PCommonUtil.decodeBase64(str);
			}

		} catch (Exception e) {
			e.printStackTrace();
		}
	}
	
	public String getMsg(){
		return GoldFromMsg.getMsg(type);
	}

}
