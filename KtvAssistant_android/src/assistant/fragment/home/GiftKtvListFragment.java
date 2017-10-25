package assistant.fragment.home;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;

import tiange.ktv.assistant.R;
import android.annotation.SuppressLint;
import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.text.TextUtils;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.FrameLayout;
import android.widget.ImageButton;
import android.widget.ListView;
import android.widget.ProgressBar;
import android.widget.TextView;
import assistant.activity.GiftListFragmentActivity;
import assistant.activity.GiftLogActivity;
import assistant.dialog.LoadingDialog;
import assistant.entity.KtvShopInfo;
import assistant.fragment.BaseFragment;
import assistant.fragment.GiftListFragment;
import assistant.fragment.adapter.UserGiftDuihuanAdapter;
import assistant.global.AppStatus;
import assistant.global.KtvAssistantAPIConfig;
import assistant.global.MessageDef;
import assistant.global.RequestCodeDef;
import assistant.manager.DialogManager;
import assistant.manager.DialogManager.OnDialogListener;
import assistant.task.gift.GetKtvShopInfoTask;
import assistant.task.gift.GiftExchangeTask;
import assistant.util.ShowUtil;

import com.zxing.activity.CaptureActivity;

public class GiftKtvListFragment extends BaseFragment implements OnDialogListener{

	private View vRoot;
	private ListView pc_listview;
	//private Home mHome;
	private ArrayList<KtvShopInfo> Ktv_list;
	private ImageButton btn_backToHome;// 返回菜单图标
	private TextView title_lable;// 标题
	private TextView giftLog;// 兑换记录
	private FrameLayout fralayout;//
	private TextView textViewTip;// 
	private ProgressBar load_progress;//
	UserGiftDuihuanAdapter UG;
	
	LoadingDialog mDialogLoading;
	Map<String,String> mMapData = new HashMap<String,String>();
	
	View mScanBtn;

	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup container,
			Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreateView(inflater, container, savedInstanceState);
		vRoot = inflater.inflate(R.layout.percenter_gift_ktvlist, container,
				false);
		
		InitData();
		setListener();
		
		mDialogLoading = new LoadingDialog(getActivity());
		return vRoot;
	}

	@Override
	public void onResume() {
		// TODO Auto-generated method stub
		super.onResume();
		
		if(AppStatus.s_isNeedToTopup)
		{
			//mHome.switchMenu(HomeMenuItemManager.MENU_ACCOUNT_RECHARGE);
			AppStatus.s_isNeedToTopup = false;
		}
	}

	@SuppressLint("HandlerLeak") 
	Handler m_handler = new Handler() {
		@Override
		public void handleMessage(Message msg) {
			// TODO Auto-generated method stub
			super.handleMessage(msg);
			switch (msg.what) {
			case MessageDef.HTTP_GIFTEXCHANGE:
			{
				mDialogLoading.dismiss();
				
				if(msg.arg1 == KtvAssistantAPIConfig.APIErrorCode.Nothing)
				{
					// 提示兑换成功
					ShowUtil.showToast(getActivity(), R.string.pc_duihuan_seccess);
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
					ShowUtil.showToast(getActivity(), "兑换失败:" + (String)msg.obj);
				break;
			}
			case MessageDef.HTTP_GETKTVSHOPLIST:

				@SuppressWarnings("unchecked")
				ArrayList<KtvShopInfo> ktvList = (ArrayList<KtvShopInfo>) msg.obj;
				Ktv_list = ktvList;
				Binddata();
				break;
			}
			if(Ktv_list.size()>0)
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
		
		mScanBtn = vRoot.findViewById(R.id.pc_ktvlist_rl1);
		fralayout=(FrameLayout) vRoot.findViewById(R.id.fralayout1);
		textViewTip=(TextView) vRoot.findViewById(R.id.pc_ltextViewTip);
		textViewTip.setVisibility(View.GONE);
		load_progress=(ProgressBar) vRoot.findViewById(R.id.load_progress);
		
		title_lable = (TextView) vRoot.findViewById(R.id.tv_title);
		title_lable.setText(R.string.menuitem_giftexchange);
		giftLog = (TextView) vRoot.findViewById(R.id.tv_giftlog);
		giftLog.setVisibility(View.VISIBLE);// View.GONE
		giftLog.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View arg0) {
				Intent intent = new Intent(getActivity(), GiftLogActivity.class);
				startActivity(intent);
			}

		});
		GetKtvShopInfoTask task = new GetKtvShopInfoTask(m_handler);
		// task.setArguBase(1, 20, false);
		task.execute();
		//mHome = (Home) this.getActivity();
	}

	private void Binddata() {
		pc_listview = (ListView) vRoot.findViewById(R.id.pc_ktvlist_listv);

		UG = new UserGiftDuihuanAdapter(
				this.getActivity(), Ktv_list);

		pc_listview.setAdapter(UG);
		pc_listview.setClickable(true);
		pc_listview.setFocusable(true);
		pc_listview.setOnItemClickListener(new OnItemClickListener() {

			@Override
			public void onItemClick(AdapterView<?> adapter, View v,
					int position, long id) {
				KtvShopInfo info = Ktv_list.get(position);
				// KtvId info.id
				// 监听 跳转
				Bundle bundle = new Bundle();
				bundle.putInt(GiftListFragment.EXTRA_ARGUMENT_KTVID,
						info.id);
				bundle.putString(GiftListFragment.EXTRA_ARGUMENT_KTVName,
						"" + info.name);

				Intent intent = new Intent(getActivity(),GiftListFragmentActivity.class);
				// Intent intent = new Intent(mHome,SendHornActivity.class);
				intent.putExtras(bundle);
				startActivity(intent);
			}

		});

	}

	protected void setListener() {

		// 回到首页
		btn_backToHome.setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				//mHome.showMenu();
				getActivity().finish();
			}
		});
		
		mScanBtn.setOnClickListener(new View.OnClickListener() {
			
			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				Intent i = new Intent(GiftKtvListFragment.this.getActivity(),CaptureActivity.class);
				startActivityForResult(i,RequestCodeDef.REQUEST_SCAN_GOODS);
			}
		});
	}

	@Override
	public void onActivityResult(int requestCode, int resultCode, Intent data) {
		// TODO Auto-generated method stub
		super.onActivityResult(requestCode, resultCode, data);
		
		if(requestCode == RequestCodeDef.REQUEST_SCAN_GOODS)
		{
			if(resultCode == Activity.RESULT_OK)
			{
				String scanData = data.getStringExtra(CaptureActivity.EXTRA_RESULT_DATA);
				//String scanData = "scanpay-type:1;ktvid:123456;giftid:123456;giftnum:5;giftname:啤酒;giftunit:瓶;gold:789;";

				mMapData.clear();
				String[] strKeyValues = scanData.split(";");
				for(String str : strKeyValues)
				{
					String[] strKeyValue = str.split(":");
					if(strKeyValue == null || strKeyValue.length != 2)
					{
						ShowUtil.showToast(getActivity(), R.string.scan_parse_fail);
						return ;
					}
					mMapData.put(strKeyValue[0], strKeyValue[1]);
				}
				
				if(mMapData.isEmpty())
				{
					ShowUtil.showToast(getActivity(), R.string.scan_parse_fail);
					return ;
				}
				
				String payType = mMapData.get("scanpay-type");
				if(!TextUtils.equals(payType, "1"))
				{
					ShowUtil.showToast(getActivity(), R.string.tip_wrong_scaneg);
					return ;
				}
				
				String content = "确认要兑换"+mMapData.get("giftnum")+mMapData.get("giftunit")
						+mMapData.get("giftname")+"("+mMapData.get("gold")+"金币)?";
				DialogManager.showTipDialog(getActivity()
						,this.getResources().getString(R.string.text_exchangegift) , content, this, true);
			}
		}
	}

	
	
	@Override
	public void onOk(int type) {
		// TODO Auto-generated method stub
		m_handler.postDelayed(new Runnable(){

			@Override
			public void run() {
				// TODO Auto-generated method stub
				mDialogLoading.show();
				
				GiftExchangeTask task = new GiftExchangeTask(m_handler);
				
				String tempStr = mMapData.get("ktvid");
				int ktvid = Integer.parseInt(tempStr==null?"":tempStr);
				tempStr = mMapData.get("giftid");
				int giftid = Integer.parseInt(tempStr==null?"":tempStr);
				tempStr = mMapData.get("giftnum");
				int giftnum = Integer.parseInt(tempStr==null?"":tempStr);
				task.setArgu( ktvid, giftid, giftnum);
				task.execute();
			}
			
		}, 100);
	}

	@Override
	public void onCancel() {
		// TODO Auto-generated method stub
		
	}
}
