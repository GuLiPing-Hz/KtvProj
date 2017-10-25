package assistant.activity;

import android.support.v4.app.Fragment;
import assistant.fragment.HongBaoSendFragment;

public class MoneyBagRedPacketActivity extends BaseFragmentActivity {
	@Override
	protected Fragment createFragment() {
		// TODO Auto-generated method stub
		int type = getIntent().getIntExtra(HongBaoSendFragment.EXTRA_HONGBAO_TYPE
				, HongBaoSendFragment.HONGBAO_LOBBY);
		int idxorroom = getIntent().getIntExtra(HongBaoSendFragment.EXTRA_HONGBAO_IDX, 0);
		
		return HongBaoSendFragment.newInstance(type,idxorroom);
	}
}
