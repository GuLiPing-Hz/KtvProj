package assistant.activity.home;

import android.content.Intent;
import android.support.v4.app.Fragment;
import assistant.activity.BaseFragmentActivity;
import assistant.fragment.home.ChatFragment;

public class ChatActivity extends BaseFragmentActivity {

	ChatFragment mFragment;
	@Override
	protected Fragment createFragment() {
		// TODO Auto-generated method stub
		mFragment = new ChatFragment();
		return mFragment;
	}

	@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
		// TODO Auto-generated method stub
		super.onActivityResult(requestCode, resultCode, data);
		
		if(mFragment != null)
			mFragment.onActivityResult(requestCode, resultCode, data);
	}

	
}
