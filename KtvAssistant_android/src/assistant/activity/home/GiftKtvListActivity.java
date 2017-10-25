package assistant.activity.home;

import android.support.v4.app.Fragment;
import assistant.activity.BaseFragmentActivity;
import assistant.fragment.home.GiftKtvListFragment;

public class GiftKtvListActivity extends BaseFragmentActivity {

	@Override
	protected Fragment createFragment() {
		// TODO Auto-generated method stub
		return new GiftKtvListFragment();
	}

}
