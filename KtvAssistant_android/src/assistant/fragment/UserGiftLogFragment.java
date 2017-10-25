package assistant.fragment;

import tiange.ktv.assistant.R;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentManager;
import android.support.v4.app.FragmentPagerAdapter;
import android.support.v4.view.ViewPager;
import android.util.DisplayMetrics;
import android.view.Display;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.animation.Animation;
import android.view.animation.TranslateAnimation;
import android.view.animation.Animation.AnimationListener;
import android.widget.Button;
import android.widget.ImageButton;
import android.widget.TextView;
import assistant.fragment.pager.GiftDisplayFragment;

public class UserGiftLogFragment extends BaseFragment {

	private View vRoot;
	
	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup container,
			Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreateView(inflater, container, savedInstanceState);
		vRoot = inflater.inflate(R.layout.percenter_gift_dhuanlog, container,
				false);
		FindView();
		setListener();
		InitData();
		
		return vRoot;
	}

	// -----------------------------------------------------------------------------
	private ImageButton btn_backToHome;// 返回菜单图标
	private TextView title_lable;// 标题
	public final static Long AnimaDuration = 100L;
	
	private Button btn_unget;// 未领取图标
	private Button btn_geted;// 已领取
	private Button btn_invalid;// 过期
	private View view_Selected;// 选中显示状态
	private ViewPager gift_viewpager;// 选项卡

	private int m_curIndex = 0;
	private int m_nextIndex = 0;
	private int m_viewSelectWidth = 0;
	private Animation animation = null;

	private void FindView() {
		btn_backToHome = (ImageButton) vRoot.findViewById(R.id.iv_back);
		btn_backToHome.setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View arg0) {
				// TODO Auto-generated method stub
				getActivity().finish();
			}
		});
		title_lable = (TextView) vRoot.findViewById(R.id.tv_title);
		btn_unget = (Button) vRoot.findViewById(R.id.btn_unget);
		btn_geted = (Button) vRoot.findViewById(R.id.btn_geted);
		btn_invalid = (Button) vRoot.findViewById(R.id.btn_invalid);
		view_Selected = (View) vRoot.findViewById(R.id.viewSelected);
		gift_viewpager = (ViewPager) vRoot.findViewById(R.id.gift_dh_viewpager);

	}
	@Override
	public void onResume() {
		// TODO Auto-generated method stub
		super.onResume();
		gift_viewpager.getAdapter().notifyDataSetChanged();
	}
	private void InitData() {
		title_lable.setText(R.string.pc_giftlog);
		
		Display display = getActivity().getWindowManager().getDefaultDisplay();  
		DisplayMetrics dm = new DisplayMetrics();
		display.getMetrics(dm);
		m_viewSelectWidth = dm.widthPixels/3; 
	
		ViewGroup.LayoutParams params = view_Selected.getLayoutParams();
		params.width = m_viewSelectWidth;
		view_Selected.setLayoutParams(params);
		
		
		FragmentManager fm = this.getActivity().getSupportFragmentManager();
		gift_viewpager.setAdapter(new FragmentPagerAdapter(fm){

			@Override
			public Fragment getItem(int arg0) {
				// TODO Auto-generated method stub
				return GiftDisplayFragment.newInstance(arg0);
			}

			@Override
			public int getCount() {
				// TODO Auto-generated method stub
				//已领取，未领取，已过期
				return 3;
			}
			
		});
		switchMenuState(0);
	}

	void setListener() {
		btn_unget.setOnClickListener(m_OnClickListenerSwitchMenu);
		btn_geted.setOnClickListener(m_OnClickListenerSwitchMenu);
		btn_invalid.setOnClickListener(m_OnClickListenerSwitchMenu);
		
		gift_viewpager.setOnPageChangeListener(new ViewPager.OnPageChangeListener() {
			
			@Override
			public void onPageSelected(int position) {
				// TODO Auto-generated method stub
				switchMenuAnimation(position);
			}
			
			@Override
			public void onPageScrolled(int arg0, float arg1, int arg2) {
				// TODO Auto-generated method stub
			}
			
			@Override
			public void onPageScrollStateChanged(int arg0) {
				// TODO Auto-generated method stub
				
			}
		});
	}

	private View.OnClickListener m_OnClickListenerSwitchMenu = new View.OnClickListener() {

		@Override
		public void onClick(View arg0) {
			// TODO Auto-generated method stub
			switch (arg0.getId()) {
			case R.id.btn_unget:
				gift_viewpager.setCurrentItem(0, true);
				break;
			case R.id.btn_geted:
				gift_viewpager.setCurrentItem(1, true);
				break;
			case R.id.btn_invalid:
				gift_viewpager.setCurrentItem(2, true);
				break;
			}
		}
	};
	//更新选择栏的状态显示
	void switchMenuAnimation(int index){
		if(m_curIndex == index || m_nextIndex==index)
			return ;
		
		if(animation!=null)
			animation.cancel();
		
		m_nextIndex = index;
		
		animation = new TranslateAnimation(m_viewSelectWidth*m_curIndex,m_viewSelectWidth*index,0,0);
		animation.setDuration(AnimaDuration);
		animation.setFillAfter(true);
		animation.setFillEnabled(true);
		animation.setAnimationListener(new AnimationListener(){

			@Override
			public void onAnimationEnd(Animation arg0) {
				// TODO Auto-generated method stub
				//再调整序号值
				m_curIndex = m_nextIndex;
				switchMenuState(m_curIndex);
			}

			@Override
			public void onAnimationRepeat(Animation arg0) {
				// TODO Auto-generated method stub
				
			}

			@Override
			public void onAnimationStart(Animation arg0) {
				// TODO Auto-generated method stub
				
			}
			
		});
		view_Selected.startAnimation(animation);
	}
	void switchMenuState(int index){
		switch(m_curIndex)
		{
		case 0:
			btn_unget.setSelected(true);
			btn_geted.setSelected(false);
			btn_invalid.setSelected(false);
			break;
		case 1:
			btn_unget.setSelected(false);
			btn_geted.setSelected(true);
			btn_invalid.setSelected(false);
			break;
		case 2:
			btn_unget.setSelected(false);
			btn_geted.setSelected(false);
			btn_invalid.setSelected(true);
			break;
		}
	}
	public static Fragment newInstance() {
		Fragment fragment = new UserGiftLogFragment();
		return fragment;
	}
	

}
