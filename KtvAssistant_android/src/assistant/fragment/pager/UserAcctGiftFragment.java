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
import android.widget.TextView;
import assistant.entity.GiftSendRecv;
import assistant.entity.GoldFromInfo;
import assistant.fragment.adapter.UserAcctAwardAdapter;
import assistant.fragment.adapter.UserAcctGiftAdapter;
import assistant.global.AppStatus;
import assistant.global.KtvAssistantAPIConfig;
import assistant.global.MessageDef;
import assistant.task.gift.GetGoldComeFromTask;
import assistant.task.gift.GiftSRRecordTask;
import assistant.util.ShowUtil;
import assistant.view.LoadMoreListView;
import assistant.view.LoadMoreListView.LoadMoreCallBack;

public class UserAcctGiftFragment extends Fragment implements LoadMoreCallBack {

	public static final String FRAGMENT_TYPE_EXT = "assistant.fragment.UserAcctGiftFragment.type";

	public static final int SysAward = 0;
	public static final int GiftGet = 1;
	public static final int GiftSend = 2;

	public static final int NUMBER_PER_PAGE = 20;

	int m_curPageIndex = 1;
	// private Home mHome;
	int m_type = UserAcctGiftFragment.SysAward;
	// 提供加载监听
	LoadMoreListView m_listViewDisplay;
	TextView m_viewTip;
	View view_process;
	UserAcctAwardAdapter SysAward_Adapter = null;
	List<GoldFromInfo> m_listGoldFromInfo;
	List<GiftSendRecv> m_listGiftGet;
	List<GiftSendRecv> m_listGiftSend;
	UserAcctGiftAdapter GiftGet_Adapter = null;
	UserAcctGiftAdapter GiftSend_Adapter = null;

	public UserAcctGiftFragment() {

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

		m_listViewDisplay = (LoadMoreListView) root
				.findViewById(R.id.pc_listViewModel);
		m_viewTip = (TextView) root.findViewById(R.id.pc_ltextViewTip);
		view_process = root.findViewById(R.id.load_progress);

		m_listViewDisplay.setloadMoreListener(this);
		m_curPageIndex = 1;
		// 区分系统奖励还是礼物信息
		switch (m_type) {
		// 系统奖励
		case UserAcctGiftFragment.SysAward: {
			GetSysAward();
			break;
		}
		// 收到的礼物
		case UserAcctGiftFragment.GiftGet: {
			GetGiftLog();
			break;
		}
		// 送出的礼物
		case UserAcctGiftFragment.GiftSend: {
			GetGiftLog();
			break;
		}

		}

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
				List<GiftSendRecv> giftlog = (List<GiftSendRecv>) msg.obj;
				if (msg.arg2 == 1)// 说明是第一次取到数据
				{
					// 第一次没取到数据说明是服务器没有任何数据
					if (giftlog.isEmpty())
						m_viewTip.setVisibility(View.VISIBLE);
					// 装载第一次获取到的数据
					else {
						// 根据不同的页面添加不同的适配器Adapter
						switch (m_type) {
						// 收到的礼物
						case UserAcctGiftFragment.GiftGet: {
							m_listGiftGet = giftlog;
							if (GiftGet_Adapter == null)
								GiftGet_Adapter = new UserAcctGiftAdapter(
										getActivity(), m_listGiftGet);
							m_listViewDisplay.setAdapter(GiftGet_Adapter);
							break;
						}
						// 送出的礼物
						case UserAcctGiftFragment.GiftSend: {
							m_listGiftSend = giftlog;
							if (GiftSend_Adapter == null)
								GiftSend_Adapter = new UserAcctGiftAdapter(
										getActivity(), m_listGiftSend);
							m_listViewDisplay.setAdapter(GiftSend_Adapter);
							break;
						}
						}
						// 增加页脚
						m_listViewDisplay.addFooter();
						// 页数自加1
						m_curPageIndex++;
					}
				}
				// 后面加载更多取到的数据
				else {
					// 说明服务器已经没有数据了
					if (giftlog.isEmpty())
						m_listViewDisplay.removeFooter();// 删除页脚
					else {
						// 根据不同的页面添加不同的适配器Adapter
						switch (m_type) {
						// 收到的礼物
						case UserAcctGiftFragment.GiftGet: {
							m_listGiftGet.addAll(giftlog);
							if (GiftGet_Adapter != null)
								GiftGet_Adapter.notifyDataSetChanged();
							break;
						}
						// 送出的礼物
						case UserAcctGiftFragment.GiftSend: {
							m_listGiftSend.addAll(giftlog);
							if (GiftSend_Adapter != null)
								GiftSend_Adapter.notifyDataSetChanged();
							break;
						}
						}
						// 页数自加1
						m_curPageIndex++;
					}
				}
				break;
			case MessageDef.HTTP_GOLDFROM:
				@SuppressWarnings("unchecked")
				List<GoldFromInfo> list = (List<GoldFromInfo>) msg.obj;

				if (msg.arg2 == 1)// 说明是第一次取到数据
				{
					// 第一次没取到数据说明是服务器没有任何数据
					if (list.isEmpty())
						m_viewTip.setVisibility(View.VISIBLE);
					// 装载第一次获取到的数据
					else {
						m_listGoldFromInfo = list;
						if (SysAward_Adapter == null)
							SysAward_Adapter = new UserAcctAwardAdapter(
									getActivity(), m_listGoldFromInfo);
						m_listViewDisplay.setAdapter(SysAward_Adapter);
						// 增加页脚
						m_listViewDisplay.addFooter();
						// 页数自加1
						m_curPageIndex++;
					}
				}
				// 后面加载更多取到的数据
				else {
					// 说明服务器已经没有数据了
					if (list.isEmpty())
						m_listViewDisplay.removeFooter();// 删除页脚
					else {
						m_listGoldFromInfo.addAll(list);
						if (SysAward_Adapter != null)
							SysAward_Adapter.notifyDataSetChanged();
						// 页数自加1
						m_curPageIndex++;
					}
				}
				break;
			default: {
				if (msg.what == KtvAssistantAPIConfig.APIErrorCode.DataNotFound) {
					// 删除页脚
					m_listViewDisplay.removeFooter();
				} else {
					// 提示失败原因
					//
					if(msg.arg2 == 1)//如果是第一次发生错误
					{
						m_viewTip.setVisibility(View.VISIBLE);
						m_viewTip.setText((String) msg.obj);
					}
					else//如果是下拉发生错误，则提示一下
						ShowUtil.showToast(getActivity(), (String) msg.obj);
				}
			}
			}
		}

	};

	// 系统奖励
	private void GetSysAward() {
		GetGoldComeFromTask task = new GetGoldComeFromTask(m_handler);
		task.setArgu((int) AppStatus.userIdx, m_curPageIndex, NUMBER_PER_PAGE);
		task.execute();
	}

	// 礼物信息
	private void GetGiftLog() {
		GiftSRRecordTask task = new GiftSRRecordTask(m_handler);
		task.setArgu((int) AppStatus.userIdx, m_type);
		task.setArguBase(m_curPageIndex, NUMBER_PER_PAGE);
		task.execute();
	}

	// 创建一个新的实例，做一些初始化操作
	public static Fragment newInstance(int type) {

		if (type < 0 || type > 2)
			return null;

		UserAcctGiftFragment fragment = new UserAcctGiftFragment();
		Bundle args = new Bundle();
		args.putInt(UserAcctGiftFragment.FRAGMENT_TYPE_EXT, type);

		fragment.setArguments(args);
		return fragment;
	}

	// 加载更多数据
	@Override
	public void loadMore() {
		// TODO Auto-generated method stub
		// 显示加载中
		// view_process.setVisibility(View.VISIBLE);

		switch (m_type) {
		case SysAward: {
			GetSysAward();
			break;
		}
		case GiftGet:
		case GiftSend: {
			GetGiftLog();
			break;
		}

		}
	}

}
