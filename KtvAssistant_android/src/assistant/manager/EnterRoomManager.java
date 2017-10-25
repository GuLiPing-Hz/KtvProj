/**
 * 
 */
package assistant.manager;

import tiange.ktv.assistant.R;
import android.annotation.SuppressLint;
import android.app.Activity;
import android.content.Intent;
import android.os.AsyncTask;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.View;
import assistant.activity.CommandLogin;
import assistant.activity.Home;
import assistant.dialog.LoadingDialog;
import assistant.dialog.MenuDialog;
import assistant.dialog.MenuDialog.OnMenuClick;
import assistant.global.AppStatus;
import assistant.global.KtvAssistantAPIConfig;
import assistant.global.MessageDef;
import assistant.global.RequestCodeDef;
import assistant.manager.DialogManager.OnDialogListener;
import assistant.task.room.LoginRoomTask;
import assistant.task.room.LogoutRoomTask;
import assistant.util.MobileNetStatUtil;
import assistant.util.ShowUtil;

import com.jni.netutil.XGMessage;
import com.zxing.activity.CaptureActivity;

/**
 * @author 宋静
 * @version 创建时间：2013-12-10 下午5:07:08 类说明
 */
public class EnterRoomManager {
	static final String Tag = "EnterRoomManager";
	private Activity mActivity;
	private LoadingDialog loadingDialog;
	private OnRoomListener roomListener;

	// ======================登录包厢===================//
	private LoginRoomTask loginRoomTask;
	// ======================退出包厢===================//
	private LogoutRoomTask logoutRoomTask;
	public boolean m_isSlientExitRoom = false;
	public boolean m_isSlientEnterRoom = false;
	
	public EnterRoomManager(Activity mActivity, OnRoomListener roomListener) {
		super();
		this.mActivity = mActivity;
		this.roomListener = roomListener;
	}

	boolean m_isShowText = false;
	/**
	 * 弹出登录选择框
	 * 
	 * 说明：使用此方法进行登录的页面，必须在onActivityResult中调用activityResult方法
	 * ，来处理二维码扫描返回的结果登录包厢
	 * 调用代码：
	 * // 处理扫描结果
	 * if (requestCode == EnterRoomManager.SCAN_REQUEST_CODE) {<br>
	 * enterRoomManager.activityResult(resultCode, data);<br>
	 * }
	 */
	public void showEnterRoomMenuDialog(boolean isShowText) {
		if (AppStatus.isInRoom)
			return;

		m_isShowText = isShowText;
		MenuDialog.showChoiceMenu(mActivity, isShowText, new OnMenuClick() {

			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				switch (v.getId()) {
				case R.id.menu_btn1:
					// 打开扫描界面扫描条形码或二维码
					Intent openCameraIntent = new Intent(mActivity,CaptureActivity.class);
					openCameraIntent.putExtra(CaptureActivity.EXTRA_SACN_TYPE, CaptureActivity.SCANTYPE_ROOMPWD);
					mActivity.startActivityForResult(openCameraIntent,RequestCodeDef.REQUEST_SCAN_CODE);
					break;

				case R.id.menu_btn2:
					Intent intent = new Intent(mActivity, CommandLogin.class);
					mActivity.startActivityForResult(intent, RequestCodeDef.REQUEST_PWD_ENTER);
					break;

				default:
					break;
				}

			}
		});
	}

	public boolean checkLoginRoomState() {
		return AppStatus.isInRoom;
	}

	public void activityResult(int requestCode, int resultCode, Intent data) {
		// 二维码扫描返回结果
		if (requestCode == RequestCodeDef.REQUEST_SCAN_CODE) {
			if (resultCode == Activity.RESULT_OK) {
				Bundle bundle = data.getExtras();
				String scanResult = bundle.getString(CaptureActivity.EXTRA_RESULT_DATA);
				startLoginRoom(scanResult,false);//执行登陆房间的过程
			} else if (resultCode == Activity.RESULT_CANCELED) 
				ShowUtil.showToast(mActivity, "扫描取消");
			else
				ShowUtil.showToast(mActivity, "扫描失败");

			// 输入口令登录界面返回,该界面会有登陆房间的过程。
		} else if (requestCode == RequestCodeDef.REQUEST_PWD_ENTER) {
			if (resultCode == Activity.RESULT_OK) {
				if (roomListener != null)
					roomListener.EnterRoomSuccess();
			} else if (resultCode == Activity.RESULT_CANCELED)
				ShowUtil.showToast(mActivity, "登录取消");
			else 
				ShowUtil.showToast(mActivity, "登录失败");
		}

	}

	/**
	 * Http请求使用密码进行包厢登录
	 * 
	 * @param key
	 *            包厢密码
	 */
	public void startLoginRoom(String key,boolean isSlient) {
		// TODO Auto-generated method stub
		if (MobileNetStatUtil.unNetConnectionTips(mActivity)) {
			return;
		}
		m_isSlientEnterRoom = isSlient;
		if(!m_isSlientEnterRoom)
		{
			loadingDialog = new LoadingDialog(mActivity,mActivity.getString(R.string.msg_login_ing));
			loadingDialog.show();
		}

		if (loginRoomTask != null
				&& loginRoomTask.getStatus() == AsyncTask.Status.RUNNING) {
			loginRoomTask.cancel(true);
		}

		loginRoomTask = new LoginRoomTask(loginRoomHandler, key,AppStatus.userIdx);
		loginRoomTask.execute();
	}

	@SuppressLint("HandlerLeak")
	private Handler loginRoomHandler = new Handler() {

		@Override
		public void handleMessage(Message msg) {
			// TODO Auto-generated method stub
			super.handleMessage(msg);
			try {
				if (loadingDialog != null && loadingDialog.isShowing()) {
					loadingDialog.dismiss();
				}

				Home home = (Home) AppStatus.getActivityByName("Home");
				switch (msg.what) {
				case MessageDef.WM_LOGIN_ROOM:
					// 登陆成功
					if (roomListener != null)
						roomListener.EnterRoomSuccess();
					
					if(home != null)
						home.updateGold(msg.arg2);
					
					AppStatus.s_isNeedConnectedRoom = true;
					Log.i(Tag,"进入星光房间");
					AppStatus.s_NetUtilMgr.doCurrentWork(NetUtilManager.TransactionRoomLogin, null);
					
					if(m_isShowText)
						ShowUtil.showToast(mActivity, R.string.order_after_enterroom);
					m_isShowText = false;
					break;
				default:
					// 失败
					if (roomListener != null)
						roomListener.EnterRoomFail((String) msg.obj);
					break;
				}
			} catch (Exception e) {
				// TODO: handle exception
				e.printStackTrace();
			}
		}

	};

	/**
	 * 退出包厢
	 */
	public void showExitRoomDialog() {
		
		DialogManager.showTipDialog(mActivity, mActivity.getString(R.string.room_exit_title)
				, mActivity.getString(R.string.room_exit_content), new OnDialogListener(){

					@Override
					public void onOk(int type) {
						// TODO Auto-generated method stub
						startExitRoom(false);
					}

					@Override
					public void onCancel() {
						// TODO Auto-generated method stub
						
					}
			
		}, true);
	}

	public void startExitRoom(boolean isSlient) {
		// TODO Auto-generated method stub
		if (MobileNetStatUtil.unNetConnectionTips(mActivity)) {
			return;
		}
		m_isSlientExitRoom = isSlient;
		if(!m_isSlientExitRoom)
		{
			loadingDialog = new LoadingDialog(mActivity,mActivity.getString(R.string.msg_logout_ing));
			loadingDialog.show();
		}

		if (logoutRoomTask != null
				&& logoutRoomTask.getStatus() == AsyncTask.Status.RUNNING) {
			logoutRoomTask.cancel(true);
		}

		logoutRoomTask = new LogoutRoomTask(logoutRoomHandler,
				AppStatus.roomId, AppStatus.room_password, AppStatus.userIdx);
		logoutRoomTask.execute();
		
		// global
		AppStatus.isInRoom = false;
		AppStatus.currentRoomInfo = null;
		AppStatus.roomId = "0";
		AppStatus.ktvId = "0";
		AppStatus.room_password = "";
		
		if (roomListener != null)
			roomListener.ExitRoomSuccess();
	}

	@SuppressLint("HandlerLeak")
	private Handler logoutRoomHandler = new Handler() {

		@Override
		public void handleMessage(Message msg) {
			// TODO Auto-generated method stub
			super.handleMessage(msg);
			try {
				if (loadingDialog != null && loadingDialog.isShowing()) {
					loadingDialog.dismiss();
				}

				switch (msg.what) {
				case MessageDef.WM_LOGOUT_ROOM:
				case KtvAssistantAPIConfig.APIErrorCode.RoomClosed: // 房间关闭
				case KtvAssistantAPIConfig.APIErrorCode.RoomRepair: // 房间维护
				case KtvAssistantAPIConfig.APIErrorCode.NoRoom: // 房间不存在
					//主动离开房间
					AppStatus.s_isNeedConnectedRoom = false;
					//直接断开房间连接
					Log.i(Tag,"退出星光房间");
					AppStatus.s_nRoomId = 0;
					AppStatus.s_NetUtilMgr.m_JniUtil.DisConnectRoom();
					//发送一个消息，清空当前点歌列表
					Message netutilMsg = AppStatus.s_NetUtilMgr.obtainMessage();
					netutilMsg.what = XGMessage.XGMSG_CUR_SONGLIST;
					netutilMsg.obj = null;
					AppStatus.s_NetUtilMgr.sendMessage(netutilMsg);
					break;

				default:
					break;
				}
			} catch (Exception e) {
				// TODO: handle exception
				e.printStackTrace();
			}
		}

	};

}
