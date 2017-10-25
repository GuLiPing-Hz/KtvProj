package assistant.fragment.pager;

import java.util.List;

import tiange.ktv.assistant.R;
import android.annotation.SuppressLint;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import assistant.entity.GiftExchangeRecord;
import assistant.fragment.adapter.GiftLogFragmentAdapter;
import assistant.global.AppStatus;
import assistant.global.KtvAssistantAPIConfig;
import assistant.global.MessageDef;
import assistant.task.gift.GetGiftExchangeRecordTask;
import assistant.util.ShowUtil;
import assistant.view.LoadMoreListView;
import assistant.view.LoadMoreListView.LoadMoreCallBack;

public class GiftDisplayFragment extends Fragment implements LoadMoreCallBack{

	public static final int NUMBER_PER_PAGE = 20;
	public static final String FRAGMENT_TYPE_EXT = "assistant.fragment.GiftDisplayFragment.type";

	public static final int GiftType_Nuget = 0;
	public static final int GiftType_Geted = 1;
	public static final int GiftType_Invaild = 2;
	
	
	int m_type = GiftDisplayFragment.GiftType_Nuget;
	LoadMoreListView m_listViewDisplay;
	View m_viewTip;
	View view_process;
	GiftLogFragmentAdapter GiftType_Adapter = null;
	int m_curPageIndex = 1;
	List<GiftExchangeRecord> m_listGiftlog;
	public GiftDisplayFragment() {

	}

	@Override
	public void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		m_type = this.getArguments().getInt(FRAGMENT_TYPE_EXT);

	}

	@Override
	public void onDestroy() {
		// TODO Auto-generated method stub
		super.onDestroy();

	}

	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup container,
			Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		View root = inflater.inflate(R.layout.percenter_viewlist_frame, null);

		m_listViewDisplay = (LoadMoreListView) root.findViewById(R.id.pc_listViewModel);
		m_viewTip = root.findViewById(R.id.pc_ltextViewTip);
		view_process = root.findViewById(R.id.load_progress);
		
		m_listViewDisplay.setloadMoreListener(this);
		
		m_curPageIndex = 1;
		GetGiftLogData();
		
		return root;
	}

	// Handle
	@SuppressLint("HandlerLeak")
	Handler m_handler = new Handler() {
		@Override
		public void handleMessage(Message msg) {
			// TODO Auto-generated method stub
			super.handleMessage(msg);
			
			view_process.setVisibility(View.GONE);
			
			switch (msg.what) {
			case MessageDef.HTTP_GETGIFTRECORD:
				
				@SuppressWarnings("unchecked")
				List<GiftExchangeRecord> list = (List<GiftExchangeRecord>) msg.obj;
				
				if(msg.arg2 == 1)
				{
					if(list.isEmpty())
					{
						m_listViewDisplay.setVisibility(View.GONE);
						m_viewTip.setVisibility(View.VISIBLE);
					}
					else
					{
						m_listGiftlog = list;
						BindData(m_listGiftlog);
						m_curPageIndex ++;
					}
				}
				else
				{
					if(list.isEmpty())
						m_listViewDisplay.removeFooter();
					else
					{
						m_listGiftlog.addAll(list);
						if(GiftType_Adapter != null)
							GiftType_Adapter.notifyDataSetChanged();
						m_curPageIndex ++;
					}
				}
				break;
				default:
				{
					if(msg.what == KtvAssistantAPIConfig.APIErrorCode.DataNotFound)
					{
						//删除页脚
						m_listViewDisplay.removeFooter();
					}
					else
					{
						//提示失败原因
						ShowUtil.showToast(getActivity(), (String)msg.obj);
					}
				}
			}
		}

	};

	// 获取数据
	private void GetGiftLogData() {
		GetGiftExchangeRecordTask task = new GetGiftExchangeRecordTask(
				m_handler);
		task.setArgu(Integer.parseInt("" + AppStatus.userIdx), m_type);
		task.setArguBase(m_curPageIndex, NUMBER_PER_PAGE);
		task.execute();
	}

	// List数据填充
	private void BindData(List<GiftExchangeRecord> giftlog) {
		// 根据不同的页面添加不同的适配器Adapter
		switch (m_type) {
		// 未领取
		case GiftDisplayFragment.GiftType_Nuget: {
			if (GiftType_Adapter == null)
				GiftType_Adapter = new GiftLogFragmentAdapter(
						this.getActivity(), giftlog);
			break;
		}
		// 已领取
		case GiftDisplayFragment.GiftType_Geted: {
			if (GiftType_Adapter == null)
				GiftType_Adapter = new GiftLogFragmentAdapter(
						getActivity(), giftlog);
			break;
		}
		// 过期
		case GiftDisplayFragment.GiftType_Invaild: {
			if (GiftType_Adapter == null)
				GiftType_Adapter = new GiftLogFragmentAdapter(
						getActivity(), giftlog);
			break;
		}
		}

		m_listViewDisplay.setAdapter(GiftType_Adapter);
		m_listViewDisplay.addFooter();
	}

	// 创建一个新的实例，做一些初始化操作
	public static Fragment newInstance(int type) {

		if (type < 0 || type > 2)
			return null;

		GiftDisplayFragment fragment = new GiftDisplayFragment();
		Bundle args = new Bundle();
		args.putInt(GiftDisplayFragment.FRAGMENT_TYPE_EXT, type);
		fragment.setArguments(args);
		return fragment;
	}

	@Override
	public void loadMore() {
		// TODO Auto-generated method stub
		GetGiftLogData();
	}
}
