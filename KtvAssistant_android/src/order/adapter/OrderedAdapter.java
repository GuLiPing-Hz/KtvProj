package order.adapter;

import java.util.List;

import order.manager.CollectManager;
import order.manager.OrderManager;
import order.util.CollectSongListener;
import order.util.OrderSongListener;
import tiange.ktv.assistant.R;
import android.app.Activity;
import android.text.TextUtils;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.ImageView;
import android.widget.TextView;
import assistant.entity.ErrorSongInfo;
import assistant.entity.SongInfo;
import assistant.fragment.home.MainHomeFragment;
import assistant.util.MobileNetStatUtil;
import assistant.util.ShowUtil;

import com.jni.netutil.ResultData_SongList;

/**
 * @author 宋静
 * @version 创建时间：2013-11-12 下午3:21:54 类说明 已点歌曲的适配器
 */
public class OrderedAdapter extends BaseAdapter implements View.OnClickListener
	,OrderSongListener {
	private Activity m_activity;
	private List<ResultData_SongList> list = null;
	private long clickTime = 0L;
	private static long INTERVAL = 500L;
	private CollectManager collectManager;
	private int operationPosition = -1;
	OrderManager orderManager;
	
	public OrderedAdapter(Activity activity) {
		super();
		m_activity = activity;
		list = MainHomeFragment.s_songList;
		orderManager = new OrderManager();
		if (orderManager != null)
			orderManager.setSongListener(this,null);
		
		collectManager = new CollectManager(collectSongListener);
	}

	@Override
	public void notifyDataSetChanged() {
		// TODO Auto-generated method stub
		list = MainHomeFragment.s_songList;
		
		super.notifyDataSetChanged();
	}

	@Override
	public int getCount() {
		// TODO Auto-generated method stub
		return list.size();
	}

	@Override
	public Object getItem(int arg0) {
		// TODO Auto-generated method stub
		return list.get(arg0);
	}

	@Override
	public long getItemId(int position) {
		// TODO Auto-generated method stub
		return position;
	}

	@Override
	public View getView(int position, View convertView, ViewGroup parent) {
		convertView = getOrderedView(position, convertView);
		return convertView;
	}

	private View getOrderedView(int position, View convertView) {
		// set view
		OrderSongHolder holder;
		if (convertView == null) {
			convertView = LayoutInflater.from(m_activity).inflate(R.layout.item_cursong_list, null);
			holder = new OrderSongHolder();
			holder.songname = (TextView) convertView.findViewById(R.id.tv_name);
			holder.artist = (TextView) convertView.findViewById(R.id.tv_artist);

			holder.iv_arrow = (ImageView) convertView.findViewById(R.id.iv_arrow);
			holder.vAppearLayout = convertView.findViewById(R.id.btnLayout);
			
			holder.vDelete = convertView.findViewById(R.id.ly_order_song);//点歌，删歌
			holder.vFavourit = convertView.findViewById(R.id.ly_collect);//收藏
			holder.vTop = convertView.findViewById(R.id.imageButtonTop);//置顶
			
			holder.imgViewTag = (ImageView) convertView.findViewById(R.id.imgViewTag);//歌曲状态
			
			convertView.setTag(holder);
		} else
			holder = (OrderSongHolder) convertView.getTag();

		if(position == 0 || position == 1)
		{
			holder.vTop.setVisibility(View.GONE);//正在演唱的歌曲,隐藏置顶按钮
			if(position == 0)
			{
				holder.iv_arrow.setVisibility(View.INVISIBLE);
				holder.imgViewTag.setImageResource(R.drawable.song_list_tag_now);
			}
		}
		else
		{
			holder.vTop.setVisibility(View.VISIBLE);
			holder.iv_arrow.setVisibility(View.VISIBLE);
			holder.imgViewTag.setImageResource(R.drawable.song_list_tag_selected);
		}
		
		ResultData_SongList info = list.get(position);
		holder.songname.setText(info.songname);
		holder.artist.setText(info.singername);

		if (operationPosition == position) {//如果是正在操作的，则需要显示下拉操作栏
			holder.iv_arrow.setImageResource(R.drawable.btn_arrow_top);
			holder.vAppearLayout.setVisibility(View.VISIBLE);
		} else {
			holder.iv_arrow.setImageResource(R.drawable.btn_arrow_bottom);
			holder.vAppearLayout.setVisibility(View.GONE);
		}
		
		convertView.setOnClickListener(parentListener);
		holder.position = position;
		
		holder.vDelete.setOnClickListener(this);//删除
		holder.vDelete.setTag(info);

		holder.vFavourit.setOnClickListener(this);
		holder.vFavourit.setTag(info);

		holder.vTop.setOnClickListener(this);
		holder.vTop.setTag(info);
		return convertView;
	}

	static class OrderSongHolder {
		ImageView imgViewTag;
		TextView songname, artist;
		ImageView iv_arrow;
		View vAppearLayout;
		View vDelete, vFavourit, vTop;
		
		int position = -1;
	}
	
	/** 出现操作栏的监听器 */
	private OnClickListener parentListener = new OnClickListener() {

		@Override
		public void onClick(View v) {
			try {
				long time = System.currentTimeMillis();
				if (time - clickTime > INTERVAL) {
					clickTime = time;
					OrderSongHolder holder = (OrderSongHolder) v.getTag();
					if (holder == null || holder.position <= 0 || holder.position > list.size())
						return;

					if (holder.position != operationPosition) {
						operationPosition = holder.position;
					} else {
						operationPosition = -1;
					}
					OrderedAdapter.this.notifyDataSetChanged();
				}
			} catch (Exception e) {
				// TODO: handle exception
			}
		}
	};

	/** 判断网络是否存在 */
	protected boolean checkNetWork() {
		return MobileNetStatUtil.checkCurrentNetState(m_activity);
	}

	/** 收藏返回事件监听 */
	private CollectSongListener collectSongListener = new CollectSongListener() {

		@Override
		public void deleteCollectSuccess(int songId) {
		}

		@Override
		public void deleteCollectFail(ErrorSongInfo error) {
		}

		@Override
		public void collectSongSuccess() {
			ShowUtil.showToast(m_activity, R.string.collect_success);
		}

		@Override
		public void collectSongFail(ErrorSongInfo error) {
			if(error != null && !TextUtils.isEmpty(error.errorMessage))
				ShowUtil.showToast(m_activity, error.errorMessage);
			else
				ShowUtil.showToast(m_activity, R.string.collect_fail);
		}
	};


	@Override
	public void onClick(View v) {
		// TODO Auto-generated method stub
		
		long time = System.currentTimeMillis();
		if (time - clickTime < INTERVAL) 
		{
			ShowUtil.showToast(m_activity, R.string.click_quickly);
			return ;
		}
		else 
		{
			clickTime = time;
			if(!checkNetWork())
				return;
		}
		
		ResultData_SongList song = (ResultData_SongList) v.getTag();
		if(song == null || song.songid == 0 || orderManager == null)
			return ;
		
		switch(v.getId())
		{
		case R.id.ly_order_song:
		{
			orderManager.deleteSong(song.songid);
			break;
		}
		case R.id.ly_collect:
		{
			collectManager.collectSong(song.songid);
			break;
		}
		case R.id.imageButtonTop:
		{
			orderManager.topSong(song.songid);
			break;
		}
		}
		
		operationPosition = -1;
		notifyDataSetChanged();
	}

	@Override
	public void orderSongSuccess(SongInfo song) {
		// TODO Auto-generated method stub
		//ShowUtil.showToast(m_context, R.string.order_success);
	}

	@Override
	public void orderSongFail(ErrorSongInfo error) {
		// TODO Auto-generated method stub
		//ShowUtil.showToast(m_context, R.string.order_fail);
	}

	@Override
	public void deleteSongSuccess(SongInfo song) {
		// TODO Auto-generated method stub
		ShowUtil.showToast(m_activity, R.string.delete_success);
	}

	@Override
	public void deleteSongFail(ErrorSongInfo error) {
		// TODO Auto-generated method stub
		if(error != null && !TextUtils.isEmpty(error.errorMessage))
			ShowUtil.showToast(m_activity, error.errorMessage);
		else
			ShowUtil.showToast(m_activity, R.string.delete_fail);
	}

	@Override
	public void topSongSuccess(SongInfo song) {
		// TODO Auto-generated method stub
		ShowUtil.showToast(m_activity, R.string.top_success);
	}

	@Override
	public void topSongFail(ErrorSongInfo error) {
		// TODO Auto-generated method stub
		if(error != null && !TextUtils.isEmpty(error.errorMessage))
			ShowUtil.showToast(m_activity, error.errorMessage);
		else
			ShowUtil.showToast(m_activity, R.string.top_fail);
	}
}
