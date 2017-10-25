package assistant.global;

public interface RequestCodeDef {

	//扫码登陆包厢请求
	public static final int REQUEST_SCAN_CODE = 470;
	//输入密码进入房间请求
	public static final int REQUEST_PWD_ENTER = 471;
	//房间状态更改请求
	public static final int REQUEST_ROOM_STATUS = 472;
	//登陆房间请求
	public static final int REQUEST_LOGIN = 473;
	//打开摄像头请求
	public static final int REQUEST_OPEN_CAREMA = 474;
	//修改昵称
	public static final int NICKNAME_MODIFY = 475;
	//扫码获取金币
	public static final int REQUEST_SCAN_GOLD=476;
	// 直接从相册获取
	public static final int REQUEST_HEADFROM_IMAGES = 477;
	// 使用相机拍照
	public static final int REQUEST_HEADFROM_CAREMA = 478;
	// 取得裁剪后的图片
	public static final int REQUEST_HEAD_CUT = 479;
	//注册回调
	public static final int REQUEST_REGISTER_USER = 480;
	//在线用户界面回调
	public static final int REQUEST_ONLINE_USERS = 481;
	//扫码获取商品信息
	public static final int REQUEST_SCAN_GOODS = 482;
	//通用扫码
	public static final int REQUEST_SCAN_COMMON = 483;
	/*
	RESULT_CANCELED RESULT_OK
	*/
	//RESULT_FIRST_USER; - custom result code
}
