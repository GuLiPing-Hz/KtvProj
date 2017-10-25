package assistant.global;

public class XGConfig {
	public static String testKtvid = "18";
	public static String testRoomid = "169";
	public static String testRoompwd = "";
	
	public static String tokenhost = "token.xg.91555.com";
	//public static String tokenhost = "60.191.222.65";//测试IP地址
	public static short tokenport = 8500;
	
	public static String lobbyhost = "section.xg.91555.com";
	//public static String lobbyhost = "60.191.222.65";//测试IP地址
	public static short lobbyport = 7200;
	
	public static String[] DownloadUrl = {
		"http://xg.91555.com/res/giftxml2.html",//正式礼物
		//"http://res2.17xg.com/res/giftxml2.html",//测试礼物
		"http://xg.91555.com/res/rightex.html"
	};
	//判断是否为网通用户
	public static String wangtong = "60.12.149.84";
	
	public static int ROOMTYPE_PRACTICE = 2;//练歌房 
	public static int ROOMTYPE_BASE = 99;//普通房间
	
	public static int version = 300;
}
