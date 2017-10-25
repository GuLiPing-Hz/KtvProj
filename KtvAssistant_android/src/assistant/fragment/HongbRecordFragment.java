package assistant.fragment;

import java.util.List;

import tiange.ktv.assistant.R;
import android.annotation.SuppressLint;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ListView;
import android.widget.TextView;
import assistant.entity.KtvHongBaoInfo;
import assistant.fragment.adapter.HongBGetAdapter;
import assistant.global.KtvAssistantAPIConfig;
import assistant.global.MessageDef;
import assistant.task.honbao.HongBaoTask;

public class HongbRecordFragment extends BaseFragment {

	View mUiBack;
	TextView mUiTitle;
	ListView mUiList;
	View mUiLoading;
	TextView mUiEmpty;
	HongBGetAdapter mDapter;
	
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
						List<KtvHongBaoInfo> dataList= (List<KtvHongBaoInfo>)msg.obj;
						if(dataList == null || dataList.size() == 0)
						{
							mUiEmpty.setVisibility(View.VISIBLE);
						}
						else
						{
							mDapter = new HongBGetAdapter(getActivity(),null,HongBGetAdapter.HongBaoRecord,dataList);
							mUiList.setAdapter(mDapter);
						}
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
		mUiList = (ListView) root.findViewById(R.id.lv_hongbao_canget);
		mUiLoading = root.findViewById(R.id.load_progress);
		mUiEmpty = (TextView) root.findViewById(R.id.tv_emptytip);
		
		mUiTitle.setText(getActivity().getString(R.string.hongbao_record));
		mUiBack.setOnClickListener(new View.OnClickListener() {
			
			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				getActivity().finish();
			}
		});
		
		HongBaoTask task = new HongBaoTask(mHandler,HongBGetAdapter.HongBaoRecord);
		task.execute();
		
		return root;
	}
}
