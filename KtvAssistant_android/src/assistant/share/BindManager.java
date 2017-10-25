package assistant.share;

import tiange.ktv.assistant.R;
import android.app.Activity;
import android.content.Intent;
import android.os.Handler;
import assistant.entity.ThirdAccount.AccountType;
import assistant.util.ShowUtil;
import assistant.view.WaitingDialog;

/**
 * @author 宋静
 * @version 创建时间：2013-8-19 下午5:07:54 类说明 绑定管理
 */
public class BindManager {
	private final static String TAG = "BindManager";
	private Handler handler;
	private Activity activity;
	private SinaShare sina;
	private TencentShare tencent;
	private AccountType currentType;

	/** 绑定账号用 */
	public BindManager(Activity ac, Handler handler){
		this.handler = handler;
		activity = ac;
		sina = new SinaShare(activity, this.handler);
		tencent = new TencentShare(activity, this.handler);
	};

	/** 检查当前账户是否绑定了账户 */
	public boolean checkBind(AccountType type, boolean isShowDialog) {
		currentType = type;
		switch (type) {
		case SINA:
			return sina.checkBind(isShowDialog);
		case QQ:
			return tencent.checkBind(isShowDialog);		
		default:
			return false;
		}
	}

	/** 解绑账户 
	public void unBind(AccountType type) {
		initUnbindDialog();
		if (type == AccountType.RENREN) {
			UserAccountManager.getInstance().clearAccount(type);
			handler.sendEmptyMessage(GlobalDef.WM_UNBIND);
		} else {			
			UserAccounts u = UserAccountManager.getInstance()
					.getLoginAccounts();
			if (u.canUnbindAccount(type)) {
				ThirdAccount a = u.getUserAccount(type);
				
				UserAccountUnBindTask task = new UserAccountUnBindTask(handler,
						UserAccountManager.getCurrentUserIDstr(), u.accounttypeid, a.openid, type.getType());
				task.execute();
			}else{
				unbindFail();
			}
		}
	}
	*/

	/** 回调页面用 */
	public void authorizeCallBack(int requestCode, int resultCode, Intent data) {
		switch (currentType) {
		case SINA:
			sina.authorizeCallBack(requestCode, resultCode, data);
			break;
		case QQ:
			tencent.authorizeCallBack(requestCode, resultCode, data);
			break;
		
		default:
			break;
		}
	}
	
	/** 绑定成功用 */
	public void bindSuccess() {
		switch (currentType) {
		case SINA:
			sina.bindSuccess();
			break;
		case QQ:
			tencent.bindSuccess();
			break;
		default:
			break;
		}
	}
	
	/** 绑定失败用 */
	public void bindError() {
		switch (currentType) {
		case SINA:
			sina.bindFail(false);
			ShowUtil.showLongToast(activity, R.string.bind_repeating_sina);
			break;
		case QQ:
			tencent.bindFail(false);
			ShowUtil.showLongToast(activity, R.string.bind_repeating_qq);
			break;
		default:
			break;
		}
	}

	private WaitingDialog waitingDialog;
	
	public void unbindSuccess(){
		stopUnbindDialog();
		ShowUtil.showToast(activity, R.string.unbind_success);
	}
	
	public void unbindFail(){
		stopUnbindDialog();
		ShowUtil.showToast(activity, R.string.unbind_fail);
	}
	
	public void initUnbindDialog() {
		if (waitingDialog != null) {
			waitingDialog.stop();
			waitingDialog = null;
		}
		
		if(activity != null){
			waitingDialog = new WaitingDialog(activity,
					activity.getString(R.string.unbind_waiting_msg), false);
			waitingDialog.show();
		}
		
	}

	public void stopUnbindDialog() {
		if (activity != null && waitingDialog != null) {
			waitingDialog.stop();
			waitingDialog = null;
		}
	}

}
