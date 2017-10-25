package assistant.fragment;

import tiange.ktv.assistant.R;
import android.annotation.SuppressLint;
import android.content.Context;
import android.content.Intent;
import android.graphics.Bitmap;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.widget.ImageButton;
import android.widget.ImageView;
import android.widget.TextView;
import assistant.activity.GiftLogActivity;
import assistant.dialog.LoadingDialog;
import assistant.global.AppStatus;
import assistant.global.KtvAssistantAPIConfig;
import assistant.global.MessageDef;
import assistant.manager.DialogManager;
import assistant.manager.DialogManager.OnDialogListener;
import assistant.task.gift.GiftExchangeTask;
import assistant.util.ImageUtil;
import assistant.util.ShowUtil;

public class GiftGetFragment extends BaseFragment {

	public static final  String EXTRA_ARGUMENT_KTVID = "assistant.fragment.UserGiftListFragment.ktvid";
	public static final  String EXTRA_ARGUMENT_GIFTID = "assistant.fragment.UserGiftListFragment.giftID";
	public static final  String EXTRA_ARGUMENT_GIFTNAME = "assistant.fragment.UserGiftListFragment.giftName";
	public static final  String EXTRA_ARGUMENT_GIFTNUM = "assistant.fragment.UserGiftListFragment.giftNum";
	public static final  String EXTRA_ARGUMENT_GIFTGOLD = "assistant.fragment.UserGiftListFragment.giftGold";
	public static final  String EXTRA_ARGUMENT_GIFTBIMG = "assistant.fragment.UserGiftListFragment.BigImg";
	
	public static final  int MSG_GIFT_BIG = 1;
	
	private View vRoot;
	private int _ktvId;
	private int _giftId;
	private String _giftName;
	private int _giftNum;
	private int _giftGold;
	private String _giftBimg;
	private TextView giftLog;// 兑换记录

	LoadingDialog loadingDialog;
	
	@SuppressLint("HandlerLeak")
    Handler m_giftHandler = new Handler(){

		@Override
		public void handleMessage(Message msg) {
			// TODO Auto-generated method stub
			super.handleMessage(msg);
			switch(msg.what)
			{
			case MSG_GIFT_BIG:
				setGiftImage(false);
				break;
			}
		}
    	
    };
    
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
		_giftId = this.getArguments().getInt(EXTRA_ARGUMENT_GIFTID);
		_giftName = this.getArguments().getString(EXTRA_ARGUMENT_GIFTNAME);
		_giftNum = this.getArguments().getInt(EXTRA_ARGUMENT_GIFTNUM);
		_giftGold = this.getArguments().getInt(EXTRA_ARGUMENT_GIFTGOLD);
		_giftBimg = this.getArguments().getString(EXTRA_ARGUMENT_GIFTBIMG);
	}

	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup container,
			Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreateView(inflater, container, savedInstanceState);
		vRoot = inflater.inflate(R.layout.percenter_gift_duihuan, container,
				false);
		InitData();
		// Binddata();
		// setListener();
		return vRoot;
	}

	@Override
	public void onViewCreated(View view, Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onViewCreated(view, savedInstanceState);
	}

	// -----------------------------------------------------------------------------
	private Context context;
	private ImageButton btn_backToHome;// 返回菜单图标
	private TextView title_lable;// 标题
	private TextView tv_giftName;// 礼物名称
	private TextView tv_giftNum;// 礼物数量
	private TextView tv_giftGold;// 礼物所需金币
	private ImageView iView_Bimg;// 图片

	private View btn_giftexchange;// 
	private View btn_cancel;//

	private void InitData() {

		btn_backToHome = (ImageButton) vRoot.findViewById(R.id.iv_back);
		btn_backToHome.setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View arg0) {
				// TODO Auto-generated method stub
				getActivity().finish();
			}
		});
		giftLog = (TextView) vRoot.findViewById(R.id.tv_giftlog);
		giftLog.setClickable(true);
		giftLog.setFocusable(true);
		giftLog.setVisibility(View.VISIBLE);// View.GONE
		giftLog.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View arg0) {
				// TODO Auto-generated method stub
				// Log.i("Zgz", "兑换记录点击一次" );
				Intent intent = new Intent(getActivity(),
						GiftLogActivity.class);
				startActivity(intent);
			}

		});
		title_lable = (TextView) vRoot.findViewById(R.id.tv_title);
		title_lable.setText("确认兑换");
		tv_giftName = (TextView) vRoot
				.findViewById(R.id.pc_gift_dhuan_tv_giftname);
		tv_giftNum = (TextView) vRoot.findViewById(R.id.pc_giftnum);
		tv_giftGold = (TextView) vRoot.findViewById(R.id.pc_giftnedgold);
		iView_Bimg = (ImageView) vRoot.findViewById(R.id.pc_gift_dhuan_bimg);

		btn_giftexchange = vRoot.findViewById(R.id.pc_gift_btndhuan_qtai);
		btn_giftexchange.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View arg0) {
				// TODO Auto-generated method stub
				// 兑换礼物
				loadingDialog = new LoadingDialog(getActivity(),"");
				loadingDialog.show();
				
				GetGift();
			}

		});
		btn_cancel = vRoot.findViewById(R.id.pc_gift_btndhuan_baox);
		btn_cancel.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View arg0) {
				// TODO Auto-generated method stub
				// 取消兑换
				getActivity().finish();
			}
		});
			
		tv_giftName.setText(_giftName);
		tv_giftNum.setText("" + _giftNum);
		tv_giftGold.setText("" + _giftGold);

		setGiftImage(true);
		context = this.getActivity();
	}
   
	void setGiftImage(boolean needDown){
		if(iView_Bimg != null)
		{
			Bitmap bitmap = ImageUtil.getBitmapGift(_giftBimg);
			if (bitmap != null)
				iView_Bimg.setImageBitmap(bitmap);
			else
				ImageUtil.showGiftImage(getActivity(), m_giftHandler, _giftBimg, MSG_GIFT_BIG);
		}
		
	}
	
	void GetGift(){
		GiftExchangeTask task = new GiftExchangeTask(m_handler);
		task.setArgu(_ktvId, _giftId, _giftNum);
		task.execute();
	}

	@SuppressLint("HandlerLeak")
	Handler m_handler = new Handler() {
		@Override
		public void handleMessage(Message msg) {
			// TODO Auto-generated method stub
			if(loadingDialog != null && loadingDialog.isShowing())
				loadingDialog.dismiss();
			
			super.handleMessage(msg);
			switch (msg.what) {
			case MessageDef.HTTP_GIFTEXCHANGE:
				if(msg.arg1 == KtvAssistantAPIConfig.APIErrorCode.Nothing)
				{
					// 提示兑换成功
					ShowUtil.showToast(context, R.string.pc_duihuan_seccess);
					giftLog.performClick();
					
					if(AppStatus.user != null)
						AppStatus.user.gold = msg.arg2;
				}
				else if(msg.arg1 == KtvAssistantAPIConfig.APIErrorCode.GoldNotEnough)
				{
					DialogManager.showTopUpDialog(getActivity(),new OnDialogListener(){

						@Override
						public void onOk(int type) {
							// TODO Auto-generated method stub
							AppStatus.s_isNeedToTopup = true;
						}

						@Override
						public void onCancel() {
							// TODO Auto-generated method stub
							
						}
						
					});
				}
				else
					ShowUtil.showToast(context, "兑换失败:" + (String)msg.obj);
				break;
			}
		}

	};

	public static Fragment newInstance(int ktvid, int giftid, String giftname,
			int giftnum, int giftgold, String bimg) {
		Fragment fragment = new GiftGetFragment();
		Bundle data = new Bundle();
		data.putInt(EXTRA_ARGUMENT_KTVID, ktvid);
		data.putInt(EXTRA_ARGUMENT_GIFTID, giftid);
		data.putString(EXTRA_ARGUMENT_GIFTNAME, giftname);
		data.putInt(EXTRA_ARGUMENT_GIFTNUM, giftnum);
		data.putInt(EXTRA_ARGUMENT_GIFTGOLD, giftgold);
		data.putString(EXTRA_ARGUMENT_GIFTBIMG, bimg);
		fragment.setArguments(data);
		return fragment;
	}
}
