package assistant.activity;

import android.support.v4.app.Fragment;
import assistant.fragment.UserGiftLogFragment;

public class GiftLogActivity extends BaseFragmentActivity {

	@Override
	protected Fragment createFragment() {
		// TODO Auto-generated method stub
		return UserGiftLogFragment.newInstance();
	}
    
}
