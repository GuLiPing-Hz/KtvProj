package assistant.share;

import android.text.TextUtils;
import assistant.entity.UserInfo;
import assistant.global.AppStatus;
import assistant.task.PDataCache;
import assistant.util.ShowLog;

/**
 * @author 宋静
 * @version 创建时间：2013-12-13 下午4:13:16 类说明 用户账户管理
 */
public class AccountManager {
	private final static String TAG = "AccountManager";
	private final static String ACCOUNTID = "account";

	/** 获取本地账户信息 */
	public static UserInfo getMyLocalAccount() {
		UserInfo mUser = null;
		try {
			String name = PDataCache.getInstance().stringForKey(ACCOUNTID);

			if (!TextUtils.isEmpty(name)) {
				String s = PDataCache.getInstance().stringForKey(name);

				if (!TextUtils.isEmpty(s)) {
					mUser = new UserInfo(s);
					if (mUser == null || mUser.userIdx <= 0)
						return null;
				}
			}
		} catch (Exception e) {
			ShowLog.e(TAG, "getMyLocalAccount error.");
		}

		return mUser;
	}
	
	/** 记录本地账户信息 */
	public static void writeMyLocalAccount() {
		UserInfo user = AppStatus.user;

		try {
			if (user == null || user.userIdx <= 0) {
				return;
			}

			String s = user.write();

			String name = String.valueOf(user.userIdx);

			PDataCache.getInstance().setString(ACCOUNTID, name);
			PDataCache.getInstance().setString(name, s);

		} catch (Exception e) {
			ShowLog.e(TAG, "记录本地账户信息 error.");
		}

	}

	/** 清理本地账户信息 */
	public static void clearMyLocalAccount() {
		PDataCache.getInstance().removeCacheForKey(ACCOUNTID);
	}
}
