package assistant.util;

import android.content.Context;
import android.os.Handler;
import android.telephony.PhoneStateListener;
import android.telephony.TelephonyManager;
import assistant.global.MessageDef;

/**
 * @author 宋静
 * @version 创建时间：2013-7-11 下午5:37:57 类说明 来电去电状态监听
 */
public class MyPhoneCallController {
	private final static String TAG = "MyPhoneCallController";
	private Context context;
	private Handler handler;
	private TelephonyManager tm;
	private MyPhoneCallListener myPhoneCallListener;

	public MyPhoneCallController(Context context, Handler handler) {
		this.context = context;
		this.handler = handler;
		//registerPhoneListener();
	}

	public void registerPhoneListener() {
		tm = (TelephonyManager) context
				.getSystemService(Context.TELEPHONY_SERVICE);
		myPhoneCallListener = new MyPhoneCallListener(handler);
		// 设置电话状态监听器
		tm.listen(myPhoneCallListener, PhoneStateListener.LISTEN_CALL_STATE);
	}

	public void cancelPhoneListener() {
		ShowLog.v(TAG, "stop listener. ");
		tm.listen(myPhoneCallListener, PhoneStateListener.LISTEN_NONE);
	}

	class MyPhoneCallListener extends PhoneStateListener {
		private final static String TAG = "MyPhoneCallListener";
		Handler handler;

		public MyPhoneCallListener(Handler handler) {
			this.handler = handler;
		}

		@Override
		public void onCallStateChanged(int state, String incomingNumber) {
			switch (state) {
			// 通话状态
			case TelephonyManager.CALL_STATE_OFFHOOK:
				ShowLog.e(TAG, "电话通话");
				if (handler != null)
					handler.sendEmptyMessage(MessageDef.PHONE_GOING);
				break;

			// 挂断状态
			case TelephonyManager.CALL_STATE_IDLE:
				ShowLog.e(TAG, "电话挂断");
				if (handler != null)
					handler.sendEmptyMessageDelayed(MessageDef.PHONE_ENDING,
							2000);
				break;

			// 响铃状态
			case TelephonyManager.CALL_STATE_RINGING:
				ShowLog.e(TAG, "电话响铃");
				if (handler != null)
					handler.sendEmptyMessage(MessageDef.PHONE_COMING);
				break;
			}
			super.onCallStateChanged(state, incomingNumber);
		}

	}
}