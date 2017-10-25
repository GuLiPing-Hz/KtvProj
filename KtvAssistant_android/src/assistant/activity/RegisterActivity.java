package assistant.activity;

import android.os.Bundle;
import android.support.v4.app.Fragment;
import assistant.fragment.RegisterFragment;

public class RegisterActivity extends BaseFragmentActivity {

	@Override
	public void onCreate(Bundle savedInstanceState) {
		m_isNeedCheck = false;
		super.onCreate(savedInstanceState);
	}

	@Override
	protected Fragment createFragment() {
		// TODO Auto-generated method stub
		return new RegisterFragment();
	}

}
