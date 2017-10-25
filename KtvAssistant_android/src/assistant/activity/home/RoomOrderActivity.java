package assistant.activity.home;

import android.support.v4.app.Fragment;
import assistant.activity.BaseFragmentActivity;
import assistant.fragment.home.RoomOrderFragment;

public class RoomOrderActivity extends BaseFragmentActivity {

	@Override
	protected Fragment createFragment() {
		// TODO Auto-generated method stub
		RoomOrderFragment fragment = new RoomOrderFragment();
		mFragmentHelper = fragment.fragmentHelper;
		return fragment;
	}

}
