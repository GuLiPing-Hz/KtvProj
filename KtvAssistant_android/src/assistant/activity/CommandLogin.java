package assistant.activity;

import tiange.ktv.assistant.R;
import android.os.Bundle;
import android.text.TextUtils;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageButton;
import android.widget.TextView;
import assistant.manager.EnterRoomManager;
import assistant.manager.OnRoomListener;
import assistant.util.MobileNetStatUtil;
import assistant.util.ShowUtil;
import assistant.util.ToolUtil;

/**
 * @author 赵龙权
 * @version 创建时间：2013-10-30 下午5:30:07 类说明 口令输入
 */
public class CommandLogin extends BaseActivity {
	private ImageButton view_back;
	private Button btn_into_box;
	private EditText password;
	private EnterRoomManager enterRoomManager;

	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		
		setContentView(R.layout.ac_commandlogin);
		initView();
		enterRoomManager = new EnterRoomManager(this, roomListener);
	}

	private void initView() {
		TextView title = (TextView) findViewById(R.id.tv_title);
		title.setText(R.string.input_command);
		view_back = (ImageButton) findViewById(R.id.iv_back);
		view_back.setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				ToolUtil.closeKeyBoard(CommandLogin.this);
				finish();
			}
		});
		
		password = (EditText) findViewById(R.id.pwdEditText);
		btn_into_box = (Button) findViewById(R.id.btn_into_box);
		btn_into_box.setOnClickListener(new View.OnClickListener() {
			
			@Override
			public void onClick(View v) {
				btn_into_box.setEnabled(false);
				if (MobileNetStatUtil.unNetConnectionTips(CommandLogin.this)) {
					btn_into_box.setEnabled(true);
					return;
				}
				String pwd = password.getText().toString();
				if (ToolUtil.matcherLoginKey(pwd)) {
					loginPro(pwd);
				} else {
					ShowUtil.showToast(CommandLogin.this,
							R.string.command_qualified_text);
					btn_into_box.setEnabled(true);
				}
			}
		});
		
	}

	private void loginPro(String password) {
		ToolUtil.closeKeyBoard(CommandLogin.this);
		enterRoomManager.startLoginRoom(password,false);
	}

	private OnRoomListener roomListener = new OnRoomListener() {
		@Override
		public void EnterRoomSuccess() {
			btn_into_box.setEnabled(true);
			CommandLogin.this.setResult(RESULT_OK);
			CommandLogin.this.finish();
		}

		@Override
		public void EnterRoomFail(String msg) {
			btn_into_box.setEnabled(true);
			if(!TextUtils.isEmpty(msg))
				ShowUtil.showToast(CommandLogin.this, msg);
			else
				ShowUtil.showToast(CommandLogin.this, R.string.login_fail);			
		}

		@Override
		public void ExitRoomSuccess() {
			// TODO Auto-generated method stub

		}

		@Override
		public void ExitRoomFail(String msg) {
			// TODO Auto-generated method stub

		}
	};

}
