package assistant.share;

public interface AddShareCallback {
	public static int success = 1;//发布成功
	public static int retain = 2; //发布失败，留在发布页面
	public static int error = 3;  //发布失败（关闭页面）
	public void execute(int shareStatus,String message,String errorcode,int ag0, long workWeiboId);
}
