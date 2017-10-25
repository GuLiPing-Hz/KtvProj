package assistant.fragment.adapter;

import tiange.ktv.assistant.R;
import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.ImageView;
import assistant.global.AppStatus;

public class FaceFragmentAdapter extends BaseAdapter {

	int m_page;
	Context m_context;
	public FaceFragmentAdapter(Context context,int page) {
		// TODO Auto-generated constructor stub
		m_context = context;
		m_page = page;
	}

	@Override
	public int getCount() {
		// TODO Auto-generated method stub
		return AppStatus.s_faceMgr.getFaceSize(m_page);
	}

	@Override
	public Object getItem(int arg0) {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public long getItemId(int arg0) {
		// TODO Auto-generated method stub
		return 0;
	}

	@Override
	public View getView(int pos, View convertView, ViewGroup arg2) {
		// TODO Auto-generated method stub
		ViewHolder viewHolder = null;
		if(convertView == null)
		{
			viewHolder = new ViewHolder();
			convertView = LayoutInflater.from(m_context).inflate(R.layout.adapter_face_single, null);
			viewHolder.face = (ImageView) convertView.findViewById(R.id.imgViewFace);
			convertView.setTag(viewHolder);
		}
		else
			viewHolder = (ViewHolder) convertView.getTag();
		
		viewHolder.face.setImageResource(getFace(pos));
		
		return convertView;
	}

	int getFace(int pos){
		return AppStatus.s_faceMgr.getFace(m_page, pos);
	}
	static class ViewHolder{
		ImageView face;
	}
}
