package assistant.share;

import tiange.ktv.assistant.R;
import android.app.Activity;
import android.content.DialogInterface;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.text.TextUtils;
import assistant.entity.ThirdAccount;
import assistant.entity.ThirdAccount.AccountType;
import assistant.entity.UserInfo;
import assistant.global.AppStatus;
import assistant.global.MessageDef;
import assistant.task.PDataCache;
import assistant.task.PDialogUtil;
import assistant.util.ShowLog;
import assistant.util.ShowUtil;
import assistant.util.ToolUtil;
import assistant.view.WaitingDialog;

import com.sina.weibo.sdk.auth.Oauth2AccessToken;
import com.sina.weibo.sdk.auth.WeiboAuth;
import com.sina.weibo.sdk.auth.WeiboAuthListener;
import com.sina.weibo.sdk.auth.sso.SsoHandler;
import com.sina.weibo.sdk.exception.WeiboDialogException;
import com.sina.weibo.sdk.exception.WeiboException;

/**
 * @author 宋静
 * @version 创建时间：2013-8-19 上午10:19:34 类说明 新浪微博的登录，分享
 */
public class SinaShare extends Activity {
	private final static String TAG = "SinaShare";
	public static final String APP_KEY = "516616172";
	public static final String REDIRECT_URL = "https://api.weibo.com/oauth2/default.html";
	public static final String SCOPE = "all";
            /*"email,direct_messages_read,direct_messages_write,"
            + "friendships_groups_read,friendships_groups_write,statuses_to_me_read,"
            + "follow_app_official_microblog," + "invitation_write";*/
	
	private Activity activity;
	private Handler handler;
	//private WeiboInfo mWeibo;
	private WeiboAuth mWeiboAuth;
	private SsoHandler mSsoHandler;
	private WeiboAuthListener listener;
	private WaitingDialog waitingDialog;
	
	private final static String FILENAME = "WeiboAccount";
	private long userIdx;
	private ThirdAccount account;

	// private static final String SCOPE =
	// "add_share,add_topic,list_album,upload_pic,add_album,add_t,add_pic_t";
	/** 分享专用 */
	public SinaShare() {
	}

	/** 绑定账号用 */
	public SinaShare(Activity ac, Handler handler) {
		this.activity = ac;
		this.handler = handler;
	}

	/** 登录账号用 */
	public SinaShare(Activity ac, WeiboAuthListener listener) {
		this.activity = ac;
		this.listener = listener;
	}

	/** 登录 */
	public void authrize() {
		if (listener == null) {
			listener = bindListener;
		}

		mWeiboAuth = new WeiboAuth(activity, APP_KEY,  
				REDIRECT_URL, SCOPE);  

		mSsoHandler = new SsoHandler(activity, mWeiboAuth);
		mSsoHandler.authorize(listener);
	}

	/** 回调页面用 */
	public void authorizeCallBack(int requestCode, int resultCode, Intent data) {
		if (mSsoHandler != null)
			mSsoHandler.authorizeCallBack(requestCode, resultCode, data);
	}

	private WeiboAuthListener bindListener = new WeiboAuthListener() {

		@Override
		public void onCancel() {
			ShowLog.e(TAG, "sina weibo bind cancel.");
			bindCancel();
		}

		@Override
		public void onComplete(Bundle values) {
			ShowLog.v(TAG, "sina weibo bind onComplete.");

			String weiboID = values.getString("uid");
			String token = values.getString("access_token");
			String expires_in = values.getString("expires_in");
			Oauth2AccessToken accessToken = new Oauth2AccessToken(token,
					expires_in);
			if (accessToken.isSessionValid()) {
				ThirdAccount sinaAccount = new ThirdAccount(AccountType.SINA,
						weiboID, token, Long.parseLong(expires_in));
			} else {
				ShowLog.e(TAG, "accessToken.isSessionValid() == false");
				bindFail(true);
			}
		}

		public void onError(WeiboDialogException arg0) {
			bindFail(true);
		}

		@Override
		public void onWeiboException(WeiboException arg0) {
			bindFail(true);
		}

	};

	/** 绑定成功调用 */
	public void bindSuccess() {
		stopDialog();
		ShowUtil.showToast(activity, R.string.bind_success);
	}

	public void bindCancel() {
		stopDialog();
		ShowUtil.showToast(activity, R.string.bind_cancel);
		if (handler != null)
			handler.sendEmptyMessage(MessageDef.WM_BIND_CANCEL);
	}

	public void bindFail(boolean isShow) {
		stopDialog();
		if(isShow)
			ShowUtil.showToast(activity, R.string.bind_fail);
		
		if (handler != null)
			handler.sendEmptyMessage(MessageDef.WM_BIND_FAIL);
		
		if(activity != null)
			ToolUtil.clearCookie(activity);
	}

	private void stopDialog() {
		if (waitingDialog != null) {
			waitingDialog.stop();
			waitingDialog = null;
		}
	}

	private UserInfo accounts;

	/** 检查当前账户是否绑定了账户 */
	public boolean checkBind(boolean isShowDialog) {
		ThirdAccount account = getAccount(isShowDialog);
		if (account == null)
			return false;

		if (!checkAccessToken(account.token, account.expires, true))
			return false;

		return true;
	}

	private ThirdAccount getAccount(boolean isShowDialog) {
		accounts = AppStatus.user;
		if (accounts == null) {
			try {
				LoginManager login = new LoginManager(activity);
				login.checkLoginState();
			} catch (Exception e) {
				if (ShowLog.isShow())
					e.printStackTrace();
			}
			return null;
		}

		ThirdAccount account = accounts.getThirdAccount(AccountType.SINA);
		if (account == null || TextUtils.isEmpty(account.token)) {
			if(isShowDialog)
				showBindTips(SinaBindMessage.UNBIND);
			else
				authrize();
			return null;
		}

		return account;
	}

	private void showBindTips(SinaBindMessage s) {
		try {
			String title = activity.getString(R.string.tip);
			String msg = s.message;
			PDialogUtil.normalAlertDialog(activity, title, msg,
					new DialogInterface.OnClickListener() {

						@Override
						public void onClick(DialogInterface dialog, int which) {
							authrize();
						}
					});
		} catch (Exception e) {
			if (ShowLog.isShow())
				e.printStackTrace();
		}
	}

	/** 检查Token，如果过期则刷新，如果isBind传入值为true，则必须在构造函数中传入activity值 */
	public boolean checkAccessToken(String token, long expiresTime,
			boolean isBind) {
		
		long currentTime = System.currentTimeMillis() / 1000;
		long expire = expiresTime - currentTime;

		if (expire <= 0) { // Token过期，请求新的Token
			if (isBind)
				showBindTips(SinaBindMessage.TOKEN_EXPIRES);
			return false;
		}
		return true;
	}

	/** 检查Token是否过期 */
	public boolean checkAccessToken(String token, long expiresTime) {
		Oauth2AccessToken accessToken = new Oauth2AccessToken(token,
				String.valueOf(expiresTime));
		if (accessToken.isSessionValid()) {
			return true;
		} 
		return false;
	}

	
	private enum SinaBindMessage {

		UNBIND(30001, "您没有绑定新浪账户，是否绑定？"), TOKEN_EXPIRES(30002,
				"您的新浪授权许可已经过期，是否重新授权？");

		private int code;
		private String message;

		SinaBindMessage(int code, String message) {
			this.code = code;
			this.message = message;
		}

		public int getCode() {
			return code;
		}

		public String getMessage() {
			return message;
		}

		public static SinaBindMessage getEnumByCode(int number) {
			for (SinaBindMessage s : values()) {
				if (s.code == number) {
					return s;
				}
			}
			return UNBIND;
		}

		public static SinaBindMessage getTypeByInt(String message) {
			for (SinaBindMessage s : values()) {
				if (s.message.equals(message)) {
					return s;
				}
			}
			return UNBIND;
		}

	}
	
	/** 记录账户的信息 */
	public static void writeAccount(ThirdAccount account) {
		try {
			String str = account.write();
			ShowLog.i(TAG, "记录账户的信息---str: " + str);
			if (!TextUtils.isEmpty(str)) {
				PDataCache.getInstance().setString(FILENAME + account.idx, str);
			}
		} catch (Exception e) {
			ShowLog.showException(e);
		}
	}
	
	/** 读取本地记录的账户的信息 */
	public ThirdAccount readAccount() {
		try {
			if (userIdx == 0)
				return null;
			String str = PDataCache.getInstance().stringForKey(
					FILENAME + userIdx);
			ShowLog.i(TAG, "获取本地记录的账号---str: " + str);
			if (!TextUtils.isEmpty(str)) {
				account = new ThirdAccount(str);
				if (account == null || TextUtils.isEmpty(account.openid))
					return null;
				return account;
			}
		} catch (Exception e) {
			ShowLog.showException(e);
		}
		return null;
	}

}
