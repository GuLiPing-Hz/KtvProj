package assistant.fragment.home;

import java.util.ArrayList;
import java.util.List;

import tiange.ktv.assistant.R;
import android.annotation.SuppressLint;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ListView;
import android.widget.TextView;
import assistant.activity.HongBRecordActivity;
import assistant.dialog.HongBaoDialog;
import assistant.dialog.HongBaoDialog.OnTaskSuccess;
import assistant.entity.KtvHongBaoInfo;
import assistant.fragment.BaseFragment;
import assistant.fragment.adapter.HongBGetAdapter;
import assistant.fragment.adapter.HongBGetAdapter.OnGetHBListener;
import assistant.global.KtvAssistantAPIConfig;
import assistant.global.MessageDef;
import assistant.task.honbao.HongBaoTask;

public class HongbaoFragment extends BaseFragment implements OnGetHBListener,OnTaskSuccess{

	View mUiBack;
	TextView mUiTitle;
	TextView mUiRight;
	ListView mUiList;
	View mUiLoading;
	TextView mUiEmpty;
	HongBGetAdapter mDapter;
	
	List<KtvHongBaoInfo> mDataList = new ArrayList<KtvHongBaoInfo>();
	@SuppressLint("HandlerLeak")
	Handler mHandler = new Handler(){

		@Override
		public void handleMessage(Message msg) {
			// TODO Auto-generated method stub
			super.handleMessage(msg);
			
			mUiLoading.setVisibility(View.GONE);
			switch(msg.what)
			{
				case MessageDef.WM_GETHONGBAO_LIST:
					if(msg.arg1 == KtvAssistantAPIConfig.APIErrorCode.Nothing)
					{
						@SuppressWarnings("unchecked")
						List<KtvHongBaoInfo> dataList = (List<KtvHongBaoInfo>)msg.obj;
						mDataList.clear();
						mDataList.addAll(dataList);
						if(mDataList != null && mDataList.size() != 0)
						{
							if(mDapter == null)
							{
								mDapter = new HongBGetAdapter(getActivity(),HongbaoFragment.this,HongBGetAdapter.HongBaoGet,mDataList);
								mUiList.setAdapter(mDapter);
							}
							else
								mDapter.notifyDataSetChanged();
							
						}
						else
							mUiEmpty.setVisibility(View.VISIBLE);
					}
					else
					{
						mUiEmpty.setVisibility(View.VISIBLE);
						mUiEmpty.setText((String)msg.obj);
					}
					break;
			}
		}
		
	};
	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup container,
			Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		View root = null;
		
		root = inflater.inflate(R.layout.fm_hongbao_get, null);
		mUiBack = root.findViewById(R.id.iv_back);
		mUiTitle = (TextView) root.findViewById(R.id.tv_title);
		mUiRight = (TextView) root.findViewById(R.id.tv_giftlog);
		mUiList = (ListView) root.findViewById(R.id.lv_hongbao_canget);
		mUiLoading = root.findViewById(R.id.load_progress);
		mUiEmpty = (TextView) root.findViewById(R.id.tv_emptytip);
		
		mUiTitle.setText(getActivity().getString(R.string.hongbao_get));
		mUiRight.setVisibility(View.VISIBLE);
		mUiRight.setText(getActivity().getString(R.string.hongbao_record));
		mUiBack.setOnClickListener(new View.OnClickListener() {
			
			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				getActivity().finish();
			}
		});
		
		mUiRight.setOnClickListener(new View.OnClickListener() {
			
			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				Intent i = new Intent(getActivity(),HongBRecordActivity.class);
				startActivity(i);
			}
		});
		
		HongBaoTask task = new HongBaoTask(mHandler,HongBGetAdapter.HongBaoGet);
		task.execute();
		
		return root;
	}
	
	@Override
	public void onClick(KtvHongBaoInfo info) {
		// TODO Auto-generated method stub
		HongBaoDialog dialog = new HongBaoDialog(getActivity(),info,this);
		dialog.show();
	}
	
	@Override
	public void onTaskSuccess() {
		// TODO Auto-generated method stub
		//重新获取一下红包信息
		HongBaoTask task = new HongBaoTask(mHandler,HongBGetAdapter.HongBaoGet);
		task.execute();
	}
}
