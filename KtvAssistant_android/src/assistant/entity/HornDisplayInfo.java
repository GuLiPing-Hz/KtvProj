package assistant.entity;

import com.jni.netutil.out.HornInfoToUi;

public class HornDisplayInfo {

	public int from_idx = 0;
	public String room_name = "";//"0"
	public String msg = "";
	public int time = 0;
	public boolean isSend = false;
	
	public String photo = "";//用户头像
	public String name = "";//用户名字
	public HornDisplayInfo(HornInfoToUi horn) {
		from_idx = horn.from_idx;
		room_name = horn.room_name;
		msg = horn.msg;
		time = horn.time;
		isSend = horn.isSend;
	}

}
