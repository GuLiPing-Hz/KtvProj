package assistant.activity;

import java.util.regex.Matcher;
import java.util.regex.Pattern;

import tiange.ktv.assistant.R;
import android.annotation.SuppressLint;
import android.graphics.drawable.Drawable;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.text.Editable;
import android.text.TextUtils;
import android.text.TextWatcher;
import android.view.KeyEvent;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.View.OnKeyListener;
import android.widget.ImageButton;
import android.widget.ImageView;
import android.widget.TextView;
import assistant.global.AppStatus;
import assistant.global.KtvAssistantAPIConfig;
import assistant.global.MessageDef;
import assistant.share.LoginManager;
import assistant.task.userapi.ModifyNickNameTask;
import assistant.util.MobileNetStatUtil;
import assistant.util.ShowLog;
import assistant.util.ShowUtil;
import assistant.util.ToolUtil;
import assistant.view.DrawableClickEditText.DrawableClickListener;
import assistant.view.ShakeEditText;


/**
 * @author 肖平平
 * @version 创建时间：2013-12-9 上午10:50:48 类说明 修改昵称
 */
public class ChangeUserNameActivity extends BaseActivity {

	// private EditText edt_username;
//	private DrawableClickEditText edt_username;
	private ShakeEditText edt_username;
	private TextView tv_show_tips;
	private ImageButton btn_back;
	private ImageView btn_save;
	// private LoadingDialog modfiyNicknameDialog;
	long clickTime = 0L;
	public String userName;// 接收新昵称
	public boolean hasChangeName = false;
	private Drawable imgClear;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		
		super.setContentView(R.layout.ac_account_change_username);
		initView();
	    initData();
		setListener();
		
		modifyNicknameHandler.postDelayed(new Runnable() {
			
			@Override
			public void run() {
				/**
				 * 警告：对于刚跳到一个新的界面就要弹出软键盘的情况
				 * 直接在onCreate()中调用requestFocus()可能由于界面没加载完全而无法弹出软键盘。
				 * 此时应该适当的延迟弹出软键盘如500毫秒（保证界面的数据加载完成）
				 */
				requestFocus();
			}
		}, 500);
	}
	
	private void requestFocus() {
		edt_username.setFocusable(true);
		edt_username.setFocusableInTouchMode(true);
		edt_username.requestFocus();
		ToolUtil.showKeyBoard(this);
	}

	private void initView() {
		edt_username = (ShakeEditText) findViewById(R.id.edt_username);
		btn_back = (ImageButton) findViewById(R.id.iv_back);
		btn_save = (ImageView) findViewById(R.id.iv_daoru);
		tv_show_tips = (TextView) findViewById(R.id.show_tips);
	}

	private void initData() {
		imgClear = getResources().getDrawable(R.drawable.btn_change_username_clear_edittext);
		if(AppStatus.user !=null && !TextUtils.isEmpty(AppStatus.user.nickname))
			edt_username.setText(AppStatus.user.nickname);
		
		int length = edt_username.getText().toString().length();
		//设置光标初始停靠的位置
		if (length > 0) {
			edt_username.setSelection(length);
		}
		if (edt_username.getText() != null) {
			edt_username.setCompoundDrawablesWithIntrinsicBounds(null, null, imgClear, null);
		}
	}
	
	private boolean checkBlank(String str) {
		Pattern pattern = Pattern.compile("[\\s]+");
		Matcher matcher = pattern.matcher(str);
		boolean flag = false;
		while(matcher.find()) {
			flag = true;
		}
		return flag;
	}
	
	private boolean checkSpecil(String str) {
		Pattern pattern = Pattern.compile("^[_a-zA-Z0-9\u4E00-\u9FA5]+$");
		Matcher matcher = pattern.matcher(str);
		boolean flag = matcher.matches();
		if (flag) {
			return false;
		} else {
			return true;
		}
	}

	/**
	 * 判断新昵称
	 * @return
	 */
	private boolean checkName() {
		edt_username.setEnabled(false);
		// 输入的长度在(0,10]之间
		userName = edt_username.getText().toString().trim();
		//空
		if (TextUtils.isEmpty(userName)) {
			tv_show_tips.setText(R.string.username_empty);
			setEdit();
			return false;
		}
		//长度不正确
		if (chineseLength(userName) > 10 || chineseLength(userName) < 2) {
			tv_show_tips.setText(R.string.illegal_username_length);
			setEdit();
			return false;
		}
		//过滤星光不可用文字
		if(AppStatus.s_NetUtilMgr.m_JniUtil.MFFindSensWord(userName) ||
				AppStatus.s_NetUtilMgr.m_JniUtil.MFFindSensWord2(userName))
		{
			tv_show_tips.setText(R.string.illegal_username_value);
			setEdit();
			return false;
		}
		//含有空格
		if (checkBlank(userName)) {
			tv_show_tips.setText(R.string.username_blank);
			setEdit();
			return false;
		}
		//过滤特殊字符
		if (checkSpecil(userName) ) {
			tv_show_tips.setText(R.string.username_has_specil);
			setEdit();
			return false;
		}
		return true;
	}
	
	/**
	 * 获取含有汉字的字符串长度
	 * @param str
	 * @return
	 */
	private int chineseLength(String str) {
		int strLen = 0;
		String chinese = "[\u0391-\uFFE5]";
		//获取字段值的长度，如果含中文字符，则每个中文字符长度为2，否则为1
		for (int i = 0; i < str.length(); i++) {
			//获取每一个字符
			String temp = str.substring(i, i + 1);
			//如果是中文
			if (temp.matches(chinese)) {
				strLen += 2;
			} else {
				strLen += 1;
			}
		}
		return strLen;
	}

	private void setEdit() {
		edt_username.setEnabled(true);
		//开启晃动动画
	    edt_username.startShakeAnimation();
	}

	private boolean checkNet() {
		boolean isConnect =  MobileNetStatUtil.getCurrentNetState(this) > 0;
		if(!isConnect)
			refreshTextView("");
		return isConnect;
	}

	private boolean checkLogin() {
		if (!AppStatus.isAleadyLogin()) {
			try {
				LoginManager login = new LoginManager(this);
				login.checkLoginState();
			} catch (Exception e) {
				ShowLog.showException(e);
			}
			return false;
		}
		return true;
	}

	private void setListener() {
		btn_save.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View v) {
				long time = System.currentTimeMillis();
				if (time - clickTime > 2000L) {
					clickTime = time;
					setName();
				}
			}

		});

		edt_username.setOnKeyListener(new OnKeyListener() {

			@Override
			public boolean onKey(View v, int keyCode, KeyEvent event) {
				long time = System.currentTimeMillis();
				if (keyCode == KeyEvent.KEYCODE_ENTER
						&& time - clickTime > 2000L) {
					clickTime = time;
					setName();
					return true;
				}
				return false;
			}
		});

		edt_username.setDrawableClickListener(new DrawableClickListener() {

			@Override
			public void onClick(DrawablePosition target) {
				edt_username.setText("");
				edt_username.setCompoundDrawablesWithIntrinsicBounds(null, null, null, null);
			}

		});

		edt_username.addTextChangedListener(new TextWatcher() {

			@Override
			public void onTextChanged(CharSequence s, int start, int before,
					int count) {
				int length = chineseLength(edt_username.getText().toString().trim());
				edt_username.setCompoundDrawablesWithIntrinsicBounds(null, null, imgClear, null);
				if (length > 10 || length < 2) {
					tv_show_tips.setText(R.string.illegal_username_length);
				} else {
					tv_show_tips.setText("");
				}
				if (length == 0) {
					tv_show_tips.setText(R.string.username_empty);
				}
			}

			@Override
			public void beforeTextChanged(CharSequence s, int start, int count,
					int after) {
				// TODO Auto-generated method stub

			}

			@Override
			public void afterTextChanged(Editable s) {
				// TODO Auto-generated method stub

			}
		});

		btn_back.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				getBack();
			}
		});
	}

	protected void setName() {
		ToolUtil.closeKeyBoard(this);
		if (checkName()) {
			// 判断是否连接网络
			if (checkLogin() && checkNet()) {
				ModifyNickNameTask modifyNickNameTask = new ModifyNickNameTask(
						modifyNicknameHandler, AppStatus.userIdx,
						AppStatus.user.getOpenId(), userName,
						AppStatus.user.gender);
				modifyNickNameTask.execute();
				tv_show_tips.setText("");
			}
		}
	}

	private void getBack() {
		ToolUtil.closeKeyBoard(ChangeUserNameActivity.this);
		finish();
	}

	@SuppressLint("HandlerLeak")
	private Handler modifyNicknameHandler = new Handler() {

		@Override
		public void handleMessage(Message msg) {
			// TODO Auto-generated method stub
			super.handleMessage(msg);
			switch (msg.what) {
			case MessageDef.WM_MODIFT_NICKNAME:
				if(msg.arg1 == KtvAssistantAPIConfig.APIErrorCode.Nothing)
				{
					ShowUtil.showToast(ChangeUserNameActivity.this,
							R.string.change_username_success);
					AppStatus.user.nickname = userName;
					ChangeUserNameActivity.this.setResult(RESULT_OK);
					getBack();
				}
				else if(msg.arg1 == KtvAssistantAPIConfig.APIErrorCode.NetConnOutOfTime)
				{
					ShowUtil.showToast(ChangeUserNameActivity.this, R.string.no_networking);
					refreshTextView("");
				}
				else
				{
					setEdit();
					String msgError = (String) msg.obj;
					refreshTextView(msgError);
				}
				break;
			}
		}

	};

	private void refreshTextView(String str) {
		tv_show_tips.setText(str);
		edt_username.setEnabled(true);
	}
}
