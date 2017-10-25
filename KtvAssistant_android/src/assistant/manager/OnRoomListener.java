package assistant.manager;

/**
 * @author 宋静
 * @version 创建时间：2013-12-21 下午7:14:32
 * 类说明
 */
public interface OnRoomListener {
	void EnterRoomSuccess();
	void EnterRoomFail(String msg);
	void ExitRoomSuccess();
	void ExitRoomFail(String msg);
}
