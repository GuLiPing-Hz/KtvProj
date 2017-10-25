package assistant.fragment.pager;

import java.util.List;

import tiange.ktv.assistant.R;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.GridView;
import assistant.fragment.adapter.GiftFragmentAdapter;

import com.jni.netutil.GiftInfo;

public class GiftFragment extends Fragment {

	public final static String EXTRA_FRAGMENT_PAGE = "assistant.fragment.pager.GiftFragment.Page";
	
	public final static int Gift_Normal = 0;
	public final static int Gift_Luck = 1;
	public final static int Gift_Luxury = 2;
	
	List<GiftInfo> m_listGift = null;
	GridView m_gridViewGift;
	GiftFragmentAdapter m_giftAdapter;
	
	public static int sCurPage = -1;
	public static int sCurSelect = -1;
	
	int m_type;
	int m_page;
	
	OnGiftClickListener m_giftClickListener;
	
	public GiftFragment() {
		// TODO Auto-generated constructor stub
	}

	@Override
	public void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		
		m_page = getArguments().getInt(EXTRA_FRAGMENT_PAGE,0);
		m_giftAdapter = new GiftFragmentAdapter(getActivity(),m_page);
	}

	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup container,
			Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		View root = inflater.inflate(R.layout.gridview_gift, null);
		m_gridViewGift = (GridView) root.findViewById(R.id.gridViewGift);
		//设置girdview监听事件
		m_gridViewGift.setOnItemClickListener(new OnItemClickListener(){

			@Override
			public void onItemClick(AdapterView<?> adapter, View v, int pos,
					long id) {
				// TODO Auto-generated method stub
				sCurPage = m_page;
				sCurSelect = pos;
				m_giftAdapter.notifyDataSetChanged();
				//v.setSelected(true);
				//v.setPressed(true);
				if(m_giftClickListener != null)
					m_giftClickListener.onGiftClick((GiftInfo)adapter.getItemAtPosition(pos));
			}
			
		});
		
		m_gridViewGift.setAdapter(m_giftAdapter);
		
		return root;
	}
	
	public void setGiftClickListener(OnGiftClickListener giftClickListener) {
		m_giftClickListener = giftClickListener;
	}

	public interface OnGiftClickListener{
		public void onGiftClick(GiftInfo info);
	}
	
	public static GiftFragment newInstance(int page,OnGiftClickListener listener){
		GiftFragment fragment = new GiftFragment();
		Bundle data = new Bundle();
		data.putInt(EXTRA_FRAGMENT_PAGE,page);
		fragment.setArguments(data);
		fragment.setGiftClickListener(listener);
		return fragment;
	}
}
