package assistant.activity.home;

import android.support.v4.app.Fragment;
import assistant.activity.BaseFragmentActivity;
import assistant.fragment.home.SingedSongFragment;

public class SingedSongActivity extends BaseFragmentActivity {

	@Override
	protected Fragment createFragment() {
		// TODO Auto-generated method stub
		return new SingedSongFragment();
	}

}
