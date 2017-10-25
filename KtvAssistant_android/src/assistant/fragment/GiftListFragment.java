package assistant.fragment;

import java.util.ArrayList;

import tiange.ktv.assistant.R;
import android.annotation.SuppressLint;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.widget.FrameLayout;
import android.widget.ImageButton;
import android.widget.ListView;
import android.widget.ProgressBar;
import android.widget.TextView;
import assistant.activity.GiftLogActivity;
import assistant.entity.HttpGiftInfo;
import assistant.fragment.adapter.UserGiftListAdapter;
import assistant.global.MessageDef;
import assistant.task.gift.GetGiftInfoTask;

public class GiftListFragment extends BaseFragment{

	public final static String EXTRA_ARGUMENT_KTVID = "assistant.fragment.UserGiftListFragment.ktvid";
	public final static String EXTRA_ARGUMENT_KTVName = "assistant.fragment.UserGiftListFragment.ktvname";
	private View vRoot;
	private int _ktvId;
	private String _ktvName;
	private TextView giftLog;//兑换记录
	@Override
	public void onActivityCreated(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onActivityCreated(savedInstanceState);
	}

	@Override
	public void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		_ktvId = this.getArguments().getInt(EXTRA_ARGUMENT_KTVID);
		_ktvName=this.getArguments().getString(EXTRA_ARGUMENT_KTVName);
	}

	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup container,
			Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreateView(inflater, container, savedInstanceState);
		vRoot = inflater
				.inflate(R.layout.percenter_gift_list, container, false);
		
		InitData();
		//setListener();
		return vRoot;
	}

	// -----------------------------------------------------------------------------
	private ListView pc_listview;
	//private Home mHome;
	private ArrayList<HttpGiftInfo> Gift_list;
	private ImageButton btn_backToHome;// 返回菜单图标
	private TextView title_lable;// 标题
	private TextView textKtvName;
	private FrameLayout fralayout;//
	private TextView textViewTip;// 
	private ProgressBar load_progress;//
	
	@SuppressLint("HandlerLeak") 
	Handler m_handler = new Handler() {
		@Override
		public void handleMessage(Message msg) {
			// TODO Auto-generated method stub
			super.handleMessage(msg);
			switch (msg.what) {
			case MessageDef.HTTP_GETGIFTLIST:

				@SuppressWarnings("unchecked")
				ArrayList<HttpGiftInfo> giftlist = (ArrayList<HttpGiftInfo>) msg.obj;
				Gift_list = giftlist;
				Binddata();
				break;
			}
			if(Gift_list.size()>0)
			{
				fralayout.setVisibility(View.GONE);
			}
			else
			{
				if (load_progress != null
						&& load_progress.getVisibility() == View.VISIBLE) {
					load_progress.setVisibility(View.GONE);
				}
				textViewTip.setVisibility(View.VISIBLE);
			}
		}

	};

	private void InitData() {

		btn_backToHome = (ImageButton) vRoot.findViewById(R.id.iv_back);
		btn_backToHome.setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View arg0) {
				// TODO Auto-generated method stub
				getActivity().finish();
			}
		});
		title_lable = (TextView) vRoot.findViewById(R.id.tv_title);
		title_lable.setText(R.string.gift_select);
		textKtvName = (TextView) vRoot.findViewById(R.id.pc_gift_ktvname);
		fralayout=(FrameLayout) vRoot.findViewById(R.id.fralayout1);
		textViewTip=(TextView) vRoot.findViewById(R.id.pc_ltextViewTip);
		textViewTip.setVisibility(View.GONE);
		load_progress=(ProgressBar) vRoot.findViewById(R.id.load_progress);
		
		giftLog= (TextView) vRoot.findViewById(R.id.tv_giftlog);
		giftLog.setClickable(true);
		giftLog.setFocusable(true);
		giftLog.setVisibility(View.VISIBLE);//View.GONE
		giftLog.setOnClickListener(new OnClickListener(){

			@Override
			public void onClick(View arg0) {
				// TODO Auto-generated method stub
				//Log.i("Zgz", "兑换记录点击一次" );
				Intent intent = new Intent(getActivity(),GiftLogActivity.class);
				startActivity(intent);
			}
			
		});
		GetGiftInfoTask task = new GetGiftInfoTask(m_handler);
		task.setArgu(_ktvId);
		task.execute();
	}

	private void Binddata() {
		pc_listview = (ListView) vRoot.findViewById(R.id.pc_giftlist_listv);

		UserGiftListAdapter UG = new UserGiftListAdapter(this.getActivity(),
				Gift_list,_ktvId);

		pc_listview.setAdapter(UG);
		pc_listview.setClickable(true);
		pc_listview.setFocusable(true);
		
		textKtvName.setText(_ktvName);
	}

	protected void setListener() {

		// 回到首页
		btn_backToHome.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				//mHome.showMenu();
			}
		});
	}

	public static Fragment newInstance(int ktvid,String ktvname){
		Fragment fragment = new GiftListFragment();
		Bundle data = new Bundle();
		data.putInt(EXTRA_ARGUMENT_KTVID, ktvid);
		data.putString(EXTRA_ARGUMENT_KTVName, ktvname);
		fragment.setArguments(data);
		return fragment;
	}
	
}
