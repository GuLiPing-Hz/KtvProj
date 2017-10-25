package order.adapter;

import java.util.ArrayList;
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
import android.widget.ImageButton;
import android.widget.ImageView;
import android.widget.TextView;
import assistant.entity.ErrorSongInfo;
import assistant.entity.SongInfo;
import assistant.global.AppStatus;
import assistant.manager.EnterRoomManager;
import assistant.util.MobileNetStatUtil;
import assistant.util.ShowUtil;

/**
 * @author 赵龙权
 * @version 创建时间：2013-12-18 下午8:48:44 类说明
 */
public class CollectAdapter extends BaseAdapter implements View.OnClickListener{
	private Activity mContext;
	private List<SongInfo> list = new ArrayList<SongInfo>();
	private long clickTime = 0L;

	private static long INTERVAL = 500L;
	private int operationPosition = -1;
	private OrderManager orderManager;
	private CollectManager collectManager;
	private EnterRoomManager enterRoomManager;

	private class CollectSongHolder {
		TextView songname, artist;
		ImageView yidian, pingfen, iv_arrow;
		View vAppearLayout;
		View  vDelete, vTop;
		ImageButton imageButtonDG;
		int position = -1;
	}

	public CollectAdapter(Activity mContext, List<SongInfo> list,
			CollectManager collect) {
		super();
		this.mContext = mContext;
		this.list = list;
		orderManager = new OrderManager(orderSongListener);
		collectManager = collect;
		if (collectManager != null) 
			collectManager.setSongListener(true, collectSongListener);
	}

	@Override
	public int getCount() {
		// TODO Auto-generated method stub
		return list.size();
	}

	@Override
	public Object getItem(int position) {
		// TODO Auto-generated method stub
		return list.get(position);
	}

	@Override
	public long getItemId(int position) {
		// TODO Auto-generated method stub
		return position;
	}

	@Override
	public View getView(int position, View convertView, ViewGroup parent) {
		// TODO Auto-generated method stub
		convertView = collectSongListView(position, convertView);
		return convertView;
	}

	private View collectSongListView(int position, View convertView) {

		// set view
		CollectSongHolder holder;
		if (convertView == null) {
			convertView = LayoutInflater.from(mContext).inflate(
					R.layout.item_songs_collect, null);
			holder = new CollectSongHolder();
			holder.songname = (TextView) convertView.findViewById(R.id.tv_name);
			holder.artist = (TextView) convertView.findViewById(R.id.tv_artist);

			holder.yidian = (ImageView) convertView
					.findViewById(R.id.iv_yidian);
			holder.pingfen = (ImageView) convertView
					.findViewById(R.id.iv_pingfen);
			convertView.findViewById(R.id.arrowlayout);
			holder.iv_arrow = (ImageView) convertView
					.findViewById(R.id.iv_arrow);
			holder.vAppearLayout = convertView.findViewById(R.id.btnLayout);
			holder.imageButtonDG = (ImageButton)convertView.findViewById(R.id.imageButtonDG);
			holder.vTop = convertView.findViewById(R.id.ly_top);
			holder.vDelete = convertView.findViewById(R.id.ly_collect);
			holder.position = position;
			convertView.setTag(holder);
		} else {
			holder = (CollectSongHolder) convertView.getTag();
		}
		// set data
		SongInfo info = list.get(position);
		if(info == null)
			return convertView;
		
		if (info.songIsScore == 1)
			holder.pingfen.setVisibility(View.VISIBLE);
		else
			holder.pingfen.setVisibility(View.GONE);
		
		if (info.isOrdered)
		{
			holder.yidian.setVisibility(View.VISIBLE);
			holder.imageButtonDG.setImageResource(R.drawable.btn_songlist_cancel);
		}
		else
		{
			holder.yidian.setVisibility(View.GONE);
			holder.imageButtonDG.setImageResource(R.drawable.btn_songlist_select);
		}

		if (operationPosition == position) 
		{
			holder.iv_arrow.setImageResource(R.drawable.btn_arrow_top);
			holder.vAppearLayout.setVisibility(View.VISIBLE);
		} 
		else 
		{
			holder.iv_arrow.setImageResource(R.drawable.btn_arrow_bottom);
			holder.vAppearLayout.setVisibility(View.GONE);
		}
		
		holder.songname.setText(info.songName);
		holder.artist.setText(info.songArtist);
		
		convertView.setOnClickListener(parentListener);

		holder.imageButtonDG.setOnClickListener(this);
		holder.imageButtonDG.setTag(info);
		holder.vTop.setOnClickListener(this);
		holder.vTop.setTag(info);
		holder.vDelete.setOnClickListener(this);
		holder.vDelete.setTag(info);
		return convertView;
	}

	/** 判断网络是否存在 */
	protected boolean checkNetWork() {
		return MobileNetStatUtil.checkCurrentNetState(mContext);
	}

	/** 点歌等返回事件监听 */
	private OrderSongListener orderSongListener = new OrderSongListener() {

		@Override
		public void orderSongSuccess(SongInfo song) {
			ShowUtil.showToast(mContext, R.string.order_success);
		}

		@Override
		public void orderSongFail(ErrorSongInfo error) {
			if (error != null && !TextUtils.isEmpty(error.errorMessage))
				ShowUtil.showToast(mContext, error.errorMessage);
			else
				ShowUtil.showToast(mContext, R.string.order_fail);
		}

		@Override
		public void deleteSongSuccess(SongInfo song) {
			ShowUtil.showToast(mContext, R.string.delete_success);
		}

		@Override
		public void deleteSongFail(ErrorSongInfo error) {
			if (error != null && !TextUtils.isEmpty(error.errorMessage))
				ShowUtil.showToast(mContext, error.errorMessage);
			else
				ShowUtil.showToast(mContext, R.string.delete_fail);
		}

		@Override
		public void topSongSuccess(SongInfo song) {
			ShowUtil.showToast(mContext, R.string.top_success);
		}

		@Override
		public void topSongFail(ErrorSongInfo error) {
			if (error != null && !TextUtils.isEmpty(error.errorMessage))
				ShowUtil.showToast(mContext, error.errorMessage);
			else
				ShowUtil.showToast(mContext, R.string.top_fail);
		}

	};

	/** 收藏返回事件监听 */
	private CollectSongListener collectSongListener = new CollectSongListener() {

		@Override
		public void deleteCollectSuccess(int songId) {
			ShowUtil.showToast(mContext, R.string.delete_collect_success);
			operationPosition = -1;
			CollectAdapter.this.notifyDataSetChanged();
		}

		@Override
		public void deleteCollectFail(ErrorSongInfo error) {
			if (error != null && !TextUtils.isEmpty(error.errorMessage))
				ShowUtil.showToast(mContext, error.errorMessage);
			else
				ShowUtil.showToast(mContext, R.string.delete_collect_fail);
		}

		@Override
		public void collectSongSuccess() {
		}

		@Override
		public void collectSongFail(ErrorSongInfo error) {
		}
	};

	public void setEnterRoomManager(EnterRoomManager enterRoomManager) {
		this.enterRoomManager = enterRoomManager;
	}

	/** 出现操作栏的监听器 */
	private OnClickListener parentListener = new OnClickListener() {

		@Override
		public void onClick(View v) {
			try {
				long time = System.currentTimeMillis();
				if (time - clickTime > INTERVAL) {
					clickTime = time;
					CollectSongHolder holder = (CollectSongHolder) v.getTag();
					if (holder == null || holder.position < 0 || holder.position > list.size())
						return;

					if (holder.position != operationPosition) {
						operationPosition = holder.position;
					} else {
						operationPosition = -1;
					}
					CollectAdapter.this.notifyDataSetChanged();
				}
			} catch (Exception e) {
				e.printStackTrace();
			}
		}
	};

	@Override
	public void onClick(View v) {
		// TODO Auto-generated method stub
		
		long time = System.currentTimeMillis();
		if (time - clickTime > INTERVAL) 
		{
			clickTime = time;
			if(!checkNetWork())
				return;
			
			SongInfo song = (SongInfo) v.getTag();
			if (song == null || song.songId <= 0) 
				return;
			
			switch(v.getId())
			{
			case R.id.imageButtonDG:
				if (AppStatus.isInRoom)
				{
					if(orderManager != null)
					{
						if (song.isOrdered) 
							orderManager.deleteSong(song.songId);
						else
							orderManager.orderSong(song);
					}
				} 
				else if (enterRoomManager != null) 
					enterRoomManager.showEnterRoomMenuDialog(true);
				else 
					ShowUtil.showToast(mContext, R.string.msg_login_room);
				break;
			case R.id.ly_top:
				if (AppStatus.isInRoom)
				{
					if(orderManager != null)
						orderManager.topSong(song);
				}
				else if (enterRoomManager != null) 
					enterRoomManager.showEnterRoomMenuDialog(true);
				else 
					ShowUtil.showToast(mContext, R.string.msg_login_room);
				break;
			case R.id.ly_collect:
				if (collectManager != null) 
					collectManager.deleteCollectSong((int)song.songId);
				break;
			}
		} 
		else 
			ShowUtil.showToast(mContext, R.string.click_quickly);
	}
}
