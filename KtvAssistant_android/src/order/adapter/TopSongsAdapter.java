package order.adapter;

import java.util.List;

import order.manager.CollectManager;
import order.manager.OrderManager;
import order.show.listener.IOrderAnimaListener;
import order.util.CollectSongListener;
import order.util.OrderSongListener;
import tiange.ktv.assistant.R;
import android.app.Activity;
import android.graphics.Bitmap;
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
import assistant.entity.WorkInfo;
import assistant.global.AppStatus;
import assistant.manager.EnterRoomManager;
import assistant.util.ImageUtil;
import assistant.util.MobileNetStatUtil;
import assistant.util.ShowLog;
import assistant.util.ShowUtil;
import assistant.view.RoundedImageView;

/**
 * @author肖平平
 * @version 创建时间：2013-12-18 上午8:09:30 
 * 类说明 评分歌曲榜
 */
public class TopSongsAdapter extends BaseAdapter{
	private static String TAG = "OrderTypeTopSongsAdapter";
	private Activity m_activity;
	private List<WorkInfo> list = null;//
	private long clickTime = 0L;
	private int[] location = new int[2];
	private static long INTERVAL = 500L;
	private OrderManager orderManager;
	private CollectManager collectManager;
	private int operationPosition = -1;
	private WorkInfo workInfo ;

	public TopSongsAdapter(Activity activity, List<WorkInfo> list, OrderManager manager) {
		super();
		m_activity = activity;
		this.list = list;
		setOrderManager(manager);
		collectManager = new CollectManager(collectSongListener);
	}

	@Override
	public int getCount() {
		return list.size();
	}

	@Override
	public Object getItem(int arg0) {
		return list.get(arg0);
	}

	@Override
	public long getItemId(int position) {
		return position;
	}

	@Override
	public View getView(int position, View convertView, ViewGroup parent) {
		convertView = normalOrderSongView(position, convertView);
		return convertView;
	}

	private class TopSongHolder {
		TextView songname, artist, singer, roomname, score, zai;
		ImageView yidian, pingfen, iv_arrow;
		View vAppearLayout, vArrow;
		//ImageView iv_left;
		ImageButton vLeft;
		View vMiddle, vRight;
		//TextView tv_left;
		RoundedImageView head_img;
		int position = -1;
	}
	
	private View normalOrderSongView(int position, View convertView) {

		// 加载视图
		TopSongHolder holder;
		if (convertView == null) {
			convertView = LayoutInflater.from(m_activity).inflate(
					R.layout.item_order_top_songs, null);
			holder = new TopSongHolder();
			holder.songname = (TextView) convertView.findViewById(R.id.tv_name);
			holder.artist = (TextView) convertView.findViewById(R.id.tv_artist);
			holder.singer = (TextView) convertView.findViewById(R.id.tv_singer);
			holder.roomname = (TextView) convertView.findViewById(R.id.tv_room);
			holder.score = (TextView) convertView.findViewById(R.id.tv_score);
			holder.head_img = (RoundedImageView) convertView.findViewById(R.id.iv_head_img);
			holder.yidian = (ImageView) convertView.findViewById(R.id.iv_yidian);
			holder.pingfen = (ImageView) convertView.findViewById(R.id.iv_pingfen);
			holder.vArrow = convertView.findViewById(R.id.arrowlayout);
			holder.iv_arrow = (ImageView) convertView.findViewById(R.id.iv_arrow);
			holder.vAppearLayout = convertView.findViewById(R.id.btnLayout);
			holder.vLeft = (ImageButton)convertView.findViewById(R.id.imageButtonDG);
			holder.vMiddle = convertView.findViewById(R.id.ly_collect);
			holder.vRight = convertView.findViewById(R.id.ly_top);
			convertView.setTag(holder);
		} else {
			holder = (TopSongHolder) convertView.getTag();
		}

		// 设置数据
		workInfo = list.get(position);
		
		//判断显示数据
		if(TextUtils.isEmpty(workInfo.address)) {
			holder.roomname.setText("未绑定包厢");
			holder.singer.setText("匿名");
			
		}else if(workInfo.user == null || TextUtils.isEmpty(workInfo.user.nickname)) {
				holder.singer.setText("匿名");
				holder.roomname.setText(workInfo.address);
		}else {
			holder.singer.setText(workInfo.user.nickname);
			holder.roomname.setText(workInfo.address);
		}
		
		Bitmap bitmap = null;
		//判断显示头像
		if(workInfo.user != null && !TextUtils.isEmpty(workInfo.user.headphoto)) {
			//如果存在演唱用户，则使用演唱头像
			bitmap = ImageUtil.getBitmapPhoto(workInfo.user.headphoto);
			if(bitmap != null)
				holder.head_img.setImageBitmap(bitmap);
			else
				holder.head_img.setImageResource(R.drawable.ic_user);
		//使用歌手头像
		} else if(workInfo.song != null && !TextUtils.isEmpty(workInfo.song.photo)) {
			bitmap = ImageUtil.getBitmapSinger(workInfo.song.photo);
			if(bitmap != null)
				holder.head_img.setImageBitmap(bitmap);
			else
				holder.head_img.setImageResource(R.drawable.ic_user);
		}
		else//重置为默认头像
			holder.head_img.setImageResource(R.drawable.ic_user);
		
		holder.songname.setText(workInfo.song.songName);
		holder.artist.setText(workInfo.song.songArtist);
		float score = (float)workInfo.score / 10f; 
		holder.score.setText(score + "");
		
		if (workInfo.song.songIsScore == 1)
			holder.pingfen.setVisibility(View.VISIBLE);

		//设置是否已点标识
		if (workInfo.song.isOrdered) {
			holder.yidian.setVisibility(View.VISIBLE);
			holder.yidian.bringToFront();
			holder.vLeft.setImageResource(R.drawable.btn_songlist_cancel);
		} else {
			holder.yidian.setVisibility(View.GONE);
			holder.vLeft.setImageResource(R.drawable.btn_songlist_select);
		}

		if (operationPosition == position) {
			holder.iv_arrow.setImageResource(R.drawable.btn_arrow_top);
			holder.vAppearLayout.setVisibility(View.VISIBLE);
		} else {
			holder.iv_arrow.setImageResource(R.drawable.btn_arrow_bottom);
			holder.vAppearLayout.setVisibility(View.GONE);
		}
		
		holder.vArrow.setOnClickListener(arrowListener);
		holder.vArrow.setTag(position);
		convertView.setOnClickListener(parentListener);
		holder.position = position;
		
		holder.vLeft.setOnClickListener(orderListener);
		holder.vLeft.setTag(workInfo.song);
		holder.vRight.setOnClickListener(topListener);
		holder.vRight.setTag(workInfo.song);
		holder.vMiddle.setOnClickListener(collectListener);
		holder.vMiddle.setTag(workInfo.song);
		
		return convertView;
	}
	
	/** 出现操作栏的监听器 */
	private OnClickListener arrowListener = new OnClickListener() {

		@Override
		public void onClick(View v) {
			try {
				long time = System.currentTimeMillis();
				if (time - clickTime > INTERVAL) {
					clickTime = time;
					int position = (Integer) v.getTag();
					if (position < 0 || position > list.size())
						return;

					if (position != operationPosition) {
						operationPosition = position;
					}else{
						operationPosition = -1;
					}
					TopSongsAdapter.this.notifyDataSetChanged();
				}
			} catch (Exception e) {
				// TODO: handle exception
			}
		}
	};

	/** 点歌 或删歌*/
	private OnClickListener orderListener = new OnClickListener() {

		@Override
		public void onClick(View v) {
			try {
				long time = System.currentTimeMillis();
				if (time - clickTime > INTERVAL) {
					clickTime = time;
					if(!checkNetWork())
						return;
					
					if(AppStatus.isInRoom)
					{
						SongInfo song = (SongInfo) v.getTag();
						if (song != null && song.songId > 0) 
						{
							if(orderManager != null)
							{
								if (song.isOrdered) 
									orderManager.deleteSong((int)song.songId);
								else
									orderManager.orderSong(song);
							}
						}
						v.getLocationOnScreen(location);
						location[0] = location[0] + v.getWidth()/2;
						location[1] = location[1] - v.getHeight()/2;
					} 
					else if(enterRoomManager != null){
						enterRoomManager.showEnterRoomMenuDialog(true);
					} 
					else{
						ShowUtil.showToast(m_activity, R.string.msg_login_room);
						ShowLog.w(TAG, "不应该走到这里. 点歌没登陆房间，显示Toast提示。");
					}
				} else {
					// ShowUtil.showToast(mContext, R.string.click_quickly);
				}
			} catch (Exception e) {
				// TODO: handle exception
			}

		}

	};

	/** 置顶 */
	private OnClickListener topListener = new OnClickListener() {

		@Override
		public void onClick(View v) {
			try {
				long time = System.currentTimeMillis();
				if (time - clickTime > INTERVAL) {
					clickTime = time;
					if(!checkNetWork())
						return;
					
					if(AppStatus.isInRoom){
						SongInfo song = (SongInfo) v.getTag();
						if (song != null && song.songId > 0 && orderManager != null){
							orderManager.topSong(song);
						}
					} else if (enterRoomManager != null) {
						enterRoomManager.showEnterRoomMenuDialog(true);
					} else {
						ShowUtil.showToast(m_activity, R.string.msg_login_room);
						ShowLog.w(TAG, "不应该走到这里. 置顶没登陆房间，显示Toast提示。");
					}
					
				} else {
					// ShowUtil.showToast(mContext, R.string.click_quickly);
				}
			} catch (Exception e) {
				// TODO: handle exception
			}
		}

	};

	/** 收藏  */
	private OnClickListener collectListener = new OnClickListener() {

		@Override
		public void onClick(View v) {
			try {
				long time = System.currentTimeMillis();
				if (time - clickTime > INTERVAL) {
					clickTime = time;
					if(!checkNetWork())
						return;
					
					if(AppStatus.isAleadyLogin()){
						SongInfo song = (SongInfo) v.getTag();
						if (song != null && song.songId > 0 && collectManager != null){
							collectManager.collectSong((int)song.songId);
						}
					}else{
//						ShowUtil.showToast(mContext, R.string.nologin);
						//loginManager.checkLoginState();
					}
					
				} else {
					// ShowUtil.showToast(mContext, R.string.click_quickly);
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

	/** 点歌等返回事件监听 */
	private OrderSongListener orderSongListener = new OrderSongListener(){

		@Override
		public void orderSongSuccess(SongInfo song) {
			TopSongsAdapter.this.notifyDataSetChanged();
			ShowUtil.showToast(m_activity, R.string.order_success);
			if (orderManager != null) {
				orderManager.setStartLocation(location);
				orderManager.startAnimation();
			}
		}

		@Override
		public void orderSongFail(ErrorSongInfo error) {
			if(error != null && !TextUtils.isEmpty(error.errorMessage))
				ShowUtil.showToast(m_activity, error.errorMessage);
			else
				ShowUtil.showToast(m_activity, R.string.order_fail);
		}

		@Override
		public void deleteSongSuccess(SongInfo song) {
			ShowUtil.showToast(m_activity, R.string.delete_success);
		}

		@Override
		public void deleteSongFail(ErrorSongInfo error) {
			ShowUtil.showToast(m_activity, R.string.delete_fail);
		}

		@Override
		public void topSongSuccess(SongInfo song) {
			ShowUtil.showToast(m_activity, R.string.top_success);
		}

		@Override
		public void topSongFail(ErrorSongInfo error) {
			if(error != null && !TextUtils.isEmpty(error.errorMessage))
				ShowUtil.showToast(m_activity, error.errorMessage);
			else
				ShowUtil.showToast(m_activity, R.string.top_fail);
		}
		
	};

	/** 收藏返回事件监听 */
	private CollectSongListener collectSongListener = new CollectSongListener(){


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
	
	private EnterRoomManager enterRoomManager;
	public void setEnterRoomManager(EnterRoomManager enterRoomManager) {
		this.enterRoomManager = enterRoomManager;
	}

	public void setOrderManager(OrderManager manager) {
		orderManager = manager;
		if (orderManager != null)
			orderManager.setSongListener(orderSongListener,(IOrderAnimaListener)m_activity);
	}
	
	/** 出现操作栏的监听器 */
	private OnClickListener parentListener = new OnClickListener() {

		@Override
		public void onClick(View v) {
			try {
				long time = System.currentTimeMillis();
				if (time - clickTime > INTERVAL) {
					clickTime = time;
					TopSongHolder holder = (TopSongHolder) v.getTag();
					if (holder == null || holder.position < 0 || holder.position > list.size())
						return;

					if (holder.position != operationPosition) {

						operationPosition = holder.position;
					} else {
						operationPosition = -1;
					}
					TopSongsAdapter.this.notifyDataSetChanged();
				}
			} catch (Exception e) {
				// TODO: handle exception
			}
		}
	};
}