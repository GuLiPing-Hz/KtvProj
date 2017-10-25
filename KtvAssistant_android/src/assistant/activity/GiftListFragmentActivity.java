package assistant.activity;

import android.os.Bundle;
import android.support.v4.app.Fragment;
import assistant.fragment.GiftListFragment;
public class GiftListFragmentActivity extends BaseFragmentActivity {

	int m_ktvId;
	String m_ktvName;
	@Override
	public void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		m_ktvId = this.getIntent().getIntExtra(GiftListFragment.EXTRA_ARGUMENT_KTVID,0);
		m_ktvName= this.getIntent().getStringExtra(GiftListFragment.EXTRA_ARGUMENT_KTVName);
		super.onCreate(savedInstanceState);
		
	}

	@Override
	protected Fragment createFragment() {
		// TODO Auto-generated method stub
		return GiftListFragment.newInstance(m_ktvId,m_ktvName);
	}
    
}
