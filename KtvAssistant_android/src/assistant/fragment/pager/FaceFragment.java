package assistant.fragment.pager;

import tiange.ktv.assistant.R;
import android.graphics.Color;
import android.graphics.drawable.ColorDrawable;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.GridView;
import assistant.fragment.adapter.FaceFragmentAdapter;
import assistant.global.AppStatus;

public class FaceFragment extends Fragment {
	public final static String ARGUMENT_INT_PAGE = "assistant.fragment.pager.FaceFragment.page";
	
	GridView m_gridViewFace;
	int m_page;
	FaceFragmentAdapter m_faceAdapter;
	OnFaceClickListener m_faceListener = null;
	
	public FaceFragment() {
		// TODO Auto-generated constructor stub
	}

	@Override
	public void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		
		m_page = getArguments().getInt(FaceFragment.ARGUMENT_INT_PAGE,0);
		m_faceAdapter = new FaceFragmentAdapter(getActivity(),m_page);
	}

	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup container,
			Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		View root = inflater.inflate(R.layout.gridview_face, null);
		
		m_gridViewFace = (GridView) root.findViewById(R.id.gridViewFace);
		m_gridViewFace.setSelector(new ColorDrawable(Color.TRANSPARENT));
		m_gridViewFace.setOnItemClickListener(new OnItemClickListener(){

			@Override
			public void onItemClick(AdapterView<?> parent, View v, int pos,
					long id) {
				// TODO Auto-generated method stub
				if(m_faceListener != null)
					m_faceListener.onFaceClick(m_page,pos);
			}
			
		});
		m_gridViewFace.setAdapter(m_faceAdapter);
		return root;
	}

	public interface OnFaceClickListener{
		public void onFaceClick(int page,int pos);
	}
	
	public void setOnFaceClickListener(OnFaceClickListener listener){
		m_faceListener = listener;
	}
	
	public static Fragment newInstance(int page,OnFaceClickListener listener){
		
		if(!AppStatus.s_faceMgr.isPageLegal(page))
			return null;
		
		FaceFragment fragment = new FaceFragment();
		fragment.setOnFaceClickListener(listener);
		Bundle data = new Bundle();
		data.putInt(FaceFragment.ARGUMENT_INT_PAGE, page);
		fragment.setArguments(data);
		
		return fragment;
	}
}
