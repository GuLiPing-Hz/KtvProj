package assistant.callback;

import com.jni.netutil.LocalData_UserInfo;

public interface OnOptionClickListener {
	public abstract void onGiftSendObjectClick(LocalData_UserInfo info);
	public abstract void onTalkClick(LocalData_UserInfo info);
	public abstract void onHongBao(LocalData_UserInfo info);
	public abstract void onUserInfoClick();
}
