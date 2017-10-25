package assistant.activity;

import android.os.Bundle;
import android.support.v4.app.Fragment;
import assistant.fragment.GiftGetFragment;

public class GiftGetFragmentActivity extends BaseFragmentActivity {

	int m_ktvId;
	int m_giftId;
	String m_ktvname;
	int m_giftNum;
	int m_giftGold;
	String m_Bimg;
	@Override
	public void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		m_ktvId = this.getIntent().getIntExtra(GiftGetFragment.EXTRA_ARGUMENT_KTVID,0);
		m_giftId= this.getIntent().getIntExtra(GiftGetFragment.EXTRA_ARGUMENT_GIFTID,0);
		m_ktvname= this.getIntent().getStringExtra(GiftGetFragment.EXTRA_ARGUMENT_GIFTNAME);
		m_giftNum= this.getIntent().getIntExtra(GiftGetFragment.EXTRA_ARGUMENT_GIFTNUM,0);
		m_giftGold= this.getIntent().getIntExtra(GiftGetFragment.EXTRA_ARGUMENT_GIFTGOLD,0);
		m_Bimg= this.getIntent().getStringExtra(GiftGetFragment.EXTRA_ARGUMENT_GIFTBIMG);
		super.onCreate(savedInstanceState);
		
	}

	@Override
	protected Fragment createFragment() {
		// TODO Auto-generated method stub
		return GiftGetFragment.newInstance(m_ktvId,m_giftId,m_ktvname,m_giftNum,m_giftGold,m_Bimg);
	}
    
}
