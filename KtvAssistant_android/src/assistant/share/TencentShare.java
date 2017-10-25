package assistant.share;

import org.json.JSONObject;

import tiange.ktv.assistant.R;
import android.app.Activity;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
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
import assistant.view.WaitingDialog;

import com.tencent.tauth.Tencent;
import com.tencent.tauth.UiError;

public class TencentShare {

	private final static String TAG = "TencentShare";
	// 你的应用ID
	
	private static final String APP_ID = "1101215509";
	// 应用的API Key
	// private static final String APP_KEY = "zhRqhw6tduk7gkn8";

	//private final int ADD_SHARE = 10001;
	//private final int ADD_SHARE_WITH_PIC = 10002;

	//private final int QZONE = 1;
	//private final int WEIBO = 2;

	private Tencent mTencent;
	private Activity mActivity;
	private UserInfo accounts;
	private Handler handler;
	private TencentListener listener;
	
	private final static String FILENAME = "QQAccount";
	private long userIdx;
	private ThirdAccount account;

	private static final String SCOPE = "get_simple_userinfo, add_share,add_topic,list_album,upload_pic,add_album,add_t,add_pic_t";

	/** 检查有效性专用 */
	public TencentShare() {
	}
	
	/** 分享专用 */
	public TencentShare(Context con) {
		mTencent = Tencent.createInstance(APP_ID,
				con.getApplicationContext());
	}

	/** 绑定账号用 */
	public TencentShare(Activity ac, Handler handler) {
		mActivity = ac;
		this.handler = handler;
		mTencent = Tencent.createInstance(APP_ID,mActivity.getApplicationContext());
	}

	/** 登录账号用 */
	public TencentShare(Activity ac, TencentListener listener) {
		mActivity = ac;
		this.listener = listener;
		mTencent = Tencent.createInstance(APP_ID,
				mActivity.getApplicationContext());
	}

	public void authrize() {
		if (listener == null) {
			listener = bindListener;
		}

		mTencent.login(mActivity, SCOPE, listener);
	}

	/** 回调页面用 */
	public void authorizeCallBack(int requestCode, int resultCode, Intent data) {
		if (mTencent != null)
			mTencent.onActivityResult(requestCode, resultCode, data);
	}

	/** 检查当前账户是否绑定了账户 */
	public boolean checkBind(boolean isShowDialog) {
		ThirdAccount account = getAccount(isShowDialog);
		if (account == null)
			return false;

		if (!checkAccessToken(account.expires, true))
			return false;

		if(mTencent != null && TextUtils.isEmpty(mTencent.getOpenId())){
			mTencent.setOpenId(account.openid);
			mTencent.setAccessToken(account.token, String.valueOf(account.expires));
		}
		return true;
	}

	private ThirdAccount getAccount(boolean isShowDialog) {
		accounts = AppStatus.user;
		if (accounts == null) {
			try {
				LoginManager loginUtil = new LoginManager(mActivity);
				loginUtil.checkLoginState();
			} catch (Exception e) {
				if (ShowLog.isShow())
					e.printStackTrace();
			}
			return null;
		}

		ThirdAccount account = accounts.getThirdAccount(AccountType.QQ);
		if (account == null || TextUtils.isEmpty(account.token)) {
			if(isShowDialog)
				showBindTips(TencentBindMessage.UNBIND);
			else
				authrize();
			return null;
		}

		return account;
	}

	private void showBindTips(TencentBindMessage t) {
		try {
			String title = mActivity.getString(R.string.tip);
			String msg = t.message;
			PDialogUtil.normalAlertDialog(mActivity, title, msg,
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
	public boolean checkAccessToken(long expiresTime, boolean isBind) {
		if (expiresTime <= System.currentTimeMillis()) { // Token过期，请求新的Token
			if (isBind)
				showBindTips(TencentBindMessage.TOKEN_EXPIRES);
			return false;
		}
		return true;
	}

	private enum TencentBindMessage {

		UNBIND(-30001, "您没有绑定腾讯账户，是否绑定？"), 
		UNSESSION(100030,"您的腾讯账户未对应用授权，是否授权？"), 
		TOKEN_EXPIRES(100014,"您的腾讯授权许可已经过期，是否重新授权？"), 
		TOKEN_LOSE(100015,"您的腾讯授权许可已经被取消了，是否重新授权？"), 
		TOKEN_ERROR(100016,"您的腾讯授权许可校验失败，是否重新授权？");

		private int code;
		private String message;

		TencentBindMessage(int code, String message) {
			this.code = code;
			this.message = message;
		}

		public static TencentBindMessage getEnumByCode(int number) {
			for (TencentBindMessage t : values()) {
				if (t.code == number) {
					return t;
				}
			}
			return UNBIND;
		}

	}

	private TencentListener bindListener = new TencentListener() {

		@Override
		public void onCancel() {
			super.onCancel();
			bindCancel();
		}

		@Override
		public void onComplete(JSONObject response) {
			super.onComplete(response);
			if (response != null) {
//				ThirdAccount qqAccount = new ThirdAccount(AccountType.QQ, openId,
//						accesstoken, expires_in);
//				initDialog();
//
//				new UserAccountBindTask(handler,
//						UserAccountManager.getCurrentUserIDstr(),
//						UserAccountManager.getCurrentLoginType().getType(),
//						qqAccount).execute();
			}
		}

		@Override
		public void onError(UiError arg0) {
			super.onError(arg0);
			if (arg0.errorCode < 0)
				bindFail(true);
			else
				showBindTips(TencentBindMessage.getEnumByCode(arg0.errorCode));
		}
	};

	/** 绑定成功调用 */
	public void bindSuccess() {
		stopDialog();
		ShowUtil.showToast(mActivity, R.string.bind_success);
	}

	public void bindCancel() {
		stopDialog();
		ShowUtil.showToast(mActivity, R.string.bind_cancel);
		if (handler != null)
			handler.sendEmptyMessage(MessageDef.WM_BIND_CANCEL);
	}

	public void bindFail(boolean isShow) {
		stopDialog();
		if(isShow)
			ShowUtil.showToast(mActivity, R.string.bind_fail);
		if (handler != null)
			handler.sendEmptyMessage(MessageDef.WM_BIND_FAIL);
	}

	private WaitingDialog waitingDialog;

	public void initDialog() {
		if (waitingDialog != null) {
			waitingDialog.stop();
			waitingDialog = null;
		}
		waitingDialog = new WaitingDialog(mActivity,
				mActivity.getString(R.string.more_waitingDialog_msg), false);
		waitingDialog.show();
	}

	public void stopDialog() {
		if (waitingDialog != null) {
			waitingDialog.stop();
			waitingDialog = null;
		}
	}	

	public void unBind() {
		AppStatus.user.unbindAccount(AccountType.QQ);
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
