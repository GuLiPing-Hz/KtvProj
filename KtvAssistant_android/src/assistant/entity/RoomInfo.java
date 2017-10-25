package assistant.entity;

import org.json.JSONObject;

import assistant.task.PCommonUtil;
import assistant.util.ShowLog;


public class RoomInfo {
	
	public String ktvId;		//实体店id
	public String roomId;
	public String roomName;
	public String beginTime;	//包厢开启时间
	public String loginTime;	//客户端登录时间
	public long periodsTime;	//单位秒
	public String address;		//实体店地址
	
	public RoomInfo(){
		this.ktvId = "0";
		this.roomId = "0";
	}
	
	public RoomInfo(JSONObject jsonObject){
		
		try {
			if(null != jsonObject){
				this.ktvId = jsonObject.optString("ktvid", "0");
				this.roomId = jsonObject.optString("roomid");
				this.roomName = PCommonUtil.decodeBase64(jsonObject.optString("roomname"));
				this.beginTime = PCommonUtil.decodeBase64(jsonObject.optString("begintime"));
				this.loginTime = PCommonUtil.decodeBase64(jsonObject.optString("logintime"));
				this.periodsTime = jsonObject.optLong("periodstime");
				this.address = PCommonUtil.decodeBase64(jsonObject.optString("address"));
			}
			
		} catch (Exception e) {
			ShowLog.i("Parser RoomInfo failed...please check");
		}
		
	}
	
	public void log() {
		//ShowLog.i("Print RoomInfo: ktvId(" + ktvId + "), roomId(" + roomId + "), roomName(" + roomName + "), beginTime(" + beginTime + "), loginTime(" + loginTime + "), periodsTime(" + periodsTime + "), address(" + address + ")");
	}
	
}
