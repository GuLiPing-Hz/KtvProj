package assistant.share;

import org.json.JSONObject;

import tiange.ktv.assistant.R;
import tiange.ktv.assistant.wxapi.WXEntryActivity;
import android.annotation.SuppressLint;
import android.app.Activity;
import android.content.DialogInterface;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.support.v4.app.FragmentActivity;
import android.text.TextUtils;
import android.util.Log;
import android.webkit.CookieManager;
import android.webkit.CookieSyncManager;
import assistant.activity.LoginActivity;
import assistant.entity.ThirdAccount;
import assistant.entity.ThirdAccount.AccountType;
import assistant.entity.WeixinToken;
import assistant.global.AppStatus;
import assistant.global.MessageDef;
import assistant.global.RequestCodeDef;
import assistant.share.task.GetWeixinTokenTask;
import assistant.task.PDialogUtil;
import assistant.task.userapi.LoginTask;
import assistant.task.userapi.ThirdLoginTask;
import assistant.task.userapi.ThirdLogoutTask;
import assistant.util.ShowUtil;
import assistant.view.WaitingDialog;

import com.sina.weibo.sdk.auth.Oauth2AccessToken;
import com.sina.weibo.sdk.auth.WeiboAuthListener;
import com.sina.weibo.sdk.exception.WeiboException;
import com.tencent.mm.sdk.modelbase.BaseResp;
import com.tencent.mm.sdk.modelmsg.SendAuth;
import com.tencent.mm.sdk.openapi.IWXAPIEventHandler;
import com.tencent.tauth.UiError;

/**
 * @author song
 * @version 创建时间：2013-4-23 下午4:24:00 类说明 登录模块
 */
public class LoginManager {

	public static final String Tag = "LoginManager";
	private Activity activity;
	private FragmentActivity userAccountFragment;
	private Handler handler;
	private WaitingDialog waitingDialog;
	private SinaShare sina;
	private TencentShare tencent;
	WXEntryActivity mWeixin;
	/**
	 * 登录模块，非起始页和登录界面调用
	 * 
	 * @param activity
	 *            用来认证和显示等待框
	 */
	public LoginManager(Activity activity) {
		super();
		this.activity = activity;
	}

	void autoLoginTaskExcute(){
		//AutoLoginTask task = new AutoLoginTask(handler,(int)AppStatus.user.userIdx);
		LoginTask task = new LoginTask(handler,AppStatus.user.xgId,AppStatus.user.xgPwd);
		task.execute();
	}
	/** 自动登录，起始页面调用，需要 */
	/*
	 * 这里增加了新的便捷的登陆方式，不需要去向第三方授权认证了。 
	 * 
	 * @return : 0 ：后台成功去执行登陆操作
	 * 					-1：授权文件过期，需要重新登陆
	 * 					-2：从1.1版本升级到2.0.0版本需要重新登陆
	 */
	public int autoLogin(){
		//这里直接从本地取出必须的数据，然后登陆，然后再登陆成功之后更新数据
		AppStatus.user = AccountManager.getMyLocalAccount();
		if(AppStatus.user  == null || AppStatus.user.canAutoLogin==0)
			return -1;
		
		if(TextUtils.isEmpty(AppStatus.user.xgId))
		{
			AccountManager.clearMyLocalAccount();//版本升级的原因，清空下本地账户
			return -2;
		}
		
		//自己注册的用户
		if(AppStatus.user.accountType == 3)
		{
			//注册登录
			autoLoginTaskExcute();
			return 0;
		}
		else//第三方登录
		{
			ThirdAccount account = AppStatus.user.getThirdAccount(AppStatus.user.accountType);
			if(account != null && !TextUtils.isEmpty(account.openid) 
					&& !TextUtils.isEmpty(account.token))
			{
				if(AppStatus.user.accountType == 1)
				{//微博登录
					sina = new SinaShare(activity, handler);
					if(sina.checkAccessToken(account.token, account.expires))
					{
						autoLoginTaskExcute();
						return 0;
					}
				}
				else if(AppStatus.user.accountType == 2)
				{//QQ登录
					if(account.expires > System.currentTimeMillis())
					{
						autoLoginTaskExcute();
						return 0;
					}
				}
				else if(AppStatus.user.accountType == 4)
				{//微信登陆
					autoLoginTaskExcute();
					return 0;
				}
				else
					Log.e(Tag, "user account type error["+AppStatus.user.accountType+"]");
			}
		}
		return -1;
	}
	
	/** 检查登录状态，非登录界面调用 */
	public boolean checkLoginState() throws Exception {
		if (activity == null)
			throw new Exception("activity is null, please init first.");

		if (!AppStatus.isAleadyLogin()) {
			showLoginTips();
			return false;
		} else
			return true;
	}

	private void showLoginTips() {
		String title = activity.getString(R.string.tip);
		String msg = activity.getString(R.string.nologin);
		PDialogUtil.normalAlertDialog(activity, title, msg,
				new DialogInterface.OnClickListener() {

					@Override
					public void onClick(DialogInterface dialog, int which) {
						intentToLoginActivity();
					}
				});
	}

	public void intentToLoginActivity() {
		Intent intent = new Intent(activity, LoginActivity.class);
		activity.startActivityForResult(intent, RequestCodeDef.REQUEST_LOGIN);
	}

	/**
	 * 登录模块，起始页和登录界面调用，登录界面需要在onActivityResult中调用authorizeCallBack方法。
	 * 
	 * @param activity
	 *            用来认证和显示等待框
	 * @param handler
	 *            有四个回调消息：<br>
	 *            GlobalDef.WM_LOGIN：登录成功；别忘了需要调用stopDialog()来取消验证框。<br>
	 *            GlobalDef.WM_LOGIN_FAIL：登录失败；<br>
	 *            GlobalDef.WM_LOGIN_CANCEL：登录取消；<br>
	 *            GlobalDef.WM_UPDATE_USER_INFO：更新用户资料成功。<br>
	 */
	public LoginManager(Activity activity, Handler handler) {
		super();
		this.activity = activity;
		this.handler = handler;
	}

	@SuppressLint("HandlerLeak")
	Handler mWeixinHandler = new Handler(){

		@Override
		public void handleMessage(Message msg) {
			// TODO Auto-generated method stub
			super.handleMessage(msg);
			
			switch(msg.what)
			{
				case GetWeixinTokenTask.GETTOKEN:
				{
					if(msg.arg1 == 1)
					{
						WeixinToken data = (WeixinToken)msg.obj;
						GetWeixinTokenTask task = new GetWeixinTokenTask(mWeixinHandler
								,GetWeixinTokenTask.GETREFRESHTOKEN,data.reortoken);
						task.execute();
					}
					else
					{
						stopDialog();
						loginFail();
					}
					break;
				}
				case GetWeixinTokenTask.GETREFRESHTOKEN:
				{
					if(msg.arg1 == 1)
					{
						WeixinToken data = (WeixinToken)msg.obj;
						
						//第二步，通过获得的账号信息，登录KTV助手服务器
						ThirdAccount weixinAccount = new ThirdAccount(AccountType.WEIXIN,
								data.openId, data.reortoken);

						//登录请求
						new ThirdLoginTask(handler, weixinAccount, AppStatus.roomId,
								AppStatus.room_password).execute();
					}
					else
					{
						stopDialog();
						loginFail();
					}
					break;
				}
			}
		}
		
	};
	/** 登录过程中需要调用此方法，回调方法 */
	public boolean handleIntent(Intent intent){
		if(mWeixin != null)
			return mWeixin.handleIntent(intent);
		return false;
	}
	
	public void dealWithWeixinAuth(BaseResp resp){
		if(resp != null)
		{
			if(resp.errCode == BaseResp.ErrCode.ERR_OK)
			{
				//登陆微信
				initDialog();//弹出等待框
				
				SendAuth.Resp auth_resp = (SendAuth.Resp) resp;
				GetWeixinTokenTask task = new GetWeixinTokenTask(mWeixinHandler,GetWeixinTokenTask.GETTOKEN
						,auth_resp.code);
				task.execute();
			}
			else if(resp.errCode == BaseResp.ErrCode.ERR_USER_CANCEL)
				loginCancel();
			else
				loginFail();
		}
	}
	
	public void authorizeCallBack(int requestCode, int resultCode, Intent data) {
		if (sina != null)
			sina.authorizeCallBack(requestCode, resultCode, data);

		if (tencent != null)
			tencent.authorizeCallBack(requestCode, resultCode, data);
	}

	/** 新浪微博登录（非登录界面必须要赋值handler） */
	public void sinaLogin() {
		tencent = null;
		mWeixin = null;
		sina = new SinaShare(activity, wbListener);
		sina.authrize();
	}
	
	/**  微信登陆*/
	public void weixinLogin(IWXAPIEventHandler handler){
		tencent = null;
		sina = null;
		mWeixin = new WXEntryActivity(activity,handler);
		
		if(mWeixin.isWXInstalled())
			mWeixin.authWeixin();
		else
			ShowUtil.showToast(activity, R.string.no_weixin);
	}

	/** QQ登录（非登录界面必须要赋值handler） */
	public void qqLogin() {
		sina = null;
		mWeixin = null;
		//第一步，获取qq的认证信息，并这设置回调监听接口qqListener
		tencent = new TencentShare(activity, qqListener);
		tencent.authrize();//执行认证
	}

	//监听微博登陆
	private WeiboAuthListener wbListener = new WeiboAuthListener() {

		@Override
		public void onCancel() {
			loginCancel();
		}

		@Override
		public void onComplete(Bundle values) {
			String weiboID = values.getString("uid");
			String token = values.getString("access_token");
			String expires_in = values.getString("expires_in");
			Oauth2AccessToken accessToken = new Oauth2AccessToken(token,expires_in);
			if (accessToken.isSessionValid()) {
				ThirdAccount sinaAccount = new ThirdAccount(AccountType.SINA,
						weiboID, token, accessToken.getExpiresTime());

				initDialog();

				new ThirdLoginTask(handler, sinaAccount, AppStatus.roomId,
						AppStatus.room_password).execute();

				// ToolUtil.followOfficial(accessToken);

			} else {
				loginFail();
			}
		}

		@Override
		public void onWeiboException(WeiboException arg0) {
			loginFail();
		}

	};
	//监听qq认证的回调 
	private TencentListener qqListener = new TencentListener() {

		@Override
		public void onCancel() {
			super.onCancel();
			loginCancel();
		}

		@Override
		public void onComplete(JSONObject response) {
			//父类解析json数据，并保存到类中
			super.onComplete(response);
			if (response != null) {
				//第二步，通过获得的账号信息，登录KTV助手服务器
				ThirdAccount qqAccount = new ThirdAccount(AccountType.QQ,
						TencentListener.openId, TencentListener.accesstoken, TencentListener.expires_in);

				initDialog();//弹出等待框

				//登录请求
				new ThirdLoginTask(handler, qqAccount, AppStatus.roomId,
						AppStatus.room_password).execute();
			} else
				loginFail();
		}

		@Override
		public void onError(UiError arg0) {
			super.onError(arg0);
			loginFail();
		}
	};

	public void initDialog() {
		if (waitingDialog != null) {
			waitingDialog.stop();
			waitingDialog = null;
		}
		
		try
		{
			waitingDialog = new WaitingDialog(activity,
					activity.getString(R.string.more_waitingDialog_msg), false);
			waitingDialog.show();
		}
		catch(Exception e)
		{
			e.printStackTrace();
		}
	}

	public void stopDialog() {
		if (waitingDialog != null) {
			waitingDialog.stop();
			waitingDialog = null;
		}
	}

	/** 登录成功调用 */
	public void loginSuccess() {
		
	}

	private void loginCancel() {
		handler.sendEmptyMessage(MessageDef.WM_LOGIN_CANCEL);
	}

	private void loginFail() {
		handler.sendEmptyMessage(MessageDef.WM_LOGIN_FAIL);
	}

	public void logout() {
		ThirdLogoutTask ThirdLogoutTask = new ThirdLogoutTask(handler,
				AppStatus.userIdx, AppStatus.roomId);
		ThirdLogoutTask.execute();
		clearCookie();
	}

	/**
	 * 清理Cookie，避免非SSO登录时，无法切换账号的问题
	 */
	private void clearCookie() {
		CookieSyncManager.createInstance(userAccountFragment);
		CookieManager cookieManager = CookieManager.getInstance();
		cookieManager.removeAllCookie();
	}
}
