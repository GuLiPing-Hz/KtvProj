package assistant.activity;

import android.content.Intent;
import android.support.v4.app.Fragment;
import assistant.fragment.CollectFragment;

public class SongCollectActivity extends BaseFragmentActivity {

	CollectFragment m_fragment;
	@Override
	protected Fragment createFragment() {
		// TODO Auto-generated method stub
		m_fragment = new CollectFragment();
		return m_fragment;
	}

	@Override
	protected void onActivityResult(int arg0, int arg1, Intent arg2) {
		// TODO Auto-generated method stub
		super.onActivityResult(arg0, arg1, arg2);
		
		if(m_fragment != null)
			m_fragment.onEnterRoomActivityResult(arg0, arg1, arg2);
	}
}
