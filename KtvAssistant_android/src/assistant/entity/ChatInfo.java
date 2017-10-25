package assistant.entity;

import com.jni.netutil.out.ChatInfoToUi;


public class ChatInfo {
	public boolean isSend = true;//如果是true，则sayidx是自己，如果为false，则toidx可能是自己
	public int  sayidx = 0;//说话者IDX 0的时候说明是我自己
	public String sayname = "";//说话者名字
	public int  toidx = 0;//被说话者IDX 0的时候说明是公聊或对我说
	public String toname = "";//被说话者名字
	public int  roomid = 0;//房间ID 0为在大厅里
	public String room = "";//房间名
	public String content = "";//聊天内容
	public int chatTime = 0;//聊天时间
	
	public String sayphoto = "";//说话者头像
	public ChatInfo(ChatInfoToUi chat) {
		roomid = chat.roomid;
		room = chat.room;
		content = chat.content;
		chatTime = chat.chatTime;
		isSend = chat.isSend;
	}

}
