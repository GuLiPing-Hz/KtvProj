package assistant.activity;

import android.os.Bundle;
import android.support.v4.app.Fragment;
import assistant.fragment.HongBaoDetailFragment;

public class HongBDetailActivity extends BaseFragmentActivity {
	
	@Override
	public void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
	}

	@Override
	protected Fragment createFragment() {
		// TODO Auto-generated method stub
		
		return new HongBaoDetailFragment();
	}

}
