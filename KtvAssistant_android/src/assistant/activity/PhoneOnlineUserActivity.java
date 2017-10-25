package assistant.activity;

import android.support.v4.app.Fragment;
import assistant.fragment.PhoneOnlineUserFragment;

public class PhoneOnlineUserActivity extends BaseFragmentActivity {

	@Override
	protected Fragment createFragment() {
		// TODO Auto-generated method stub
		return new PhoneOnlineUserFragment();
	}

}
