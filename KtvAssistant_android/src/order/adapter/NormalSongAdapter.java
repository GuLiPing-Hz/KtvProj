package order.adapter;

import java.util.ArrayList;
import java.util.List;

import order.manager.CollectManager;
import order.manager.OrderManager;
import order.show.listener.IOrderAnimaListener;
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
 * @author 宋静
 * @version 创建时间：2013-12-16 下午8:09:30 类说明 1.1版本的点歌列表适配器
 */
public class NormalSongAdapter extends BaseAdapter {
	private Activity m_activity;
	private List<SongInfo> list = new ArrayList<SongInfo>();
	private long clickTime = 0L;
	// private OnOrderLocationListener orderCallback;
	private int[] location = new int[2];
	private static long INTERVAL = 500L;
	private OrderManager orderManager;
	private CollectManager collectManager;
	private int operationPosition = -1;
	private EnterRoomManager enterRoomManager;
	public NormalSongAdapter(Activity activity, List<SongInfo> list,
			OrderManager manager) {
		super();
		this.m_activity = activity;
		this.list = list;
		setOrderManager(manager);
		collectManager = new CollectManager(collectSongListener);
	}

	public void resetOperationPos(){
		operationPosition = -1;
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

	private class OrderSongHolder {
		TextView songname, artist;
		ImageView yidian, pingfen, iv_arrow;
		View vAppearLayout;
		//ImageView iv_left;
		ImageButton vLeft;
		View vMiddle, vRight;
		//TextView tv_left;
		
		int position = -1;
	}

	private View normalOrderSongView(int position, View convertView) {
		// set view
		OrderSongHolder holder;
		if (convertView == null) {
			convertView = LayoutInflater.from(m_activity).inflate(R.layout.item_songs, null);
			holder = new OrderSongHolder();
			holder.songname = (TextView) convertView.findViewById(R.id.tv_name);
			holder.artist = (TextView) convertView.findViewById(R.id.tv_artist);

			holder.yidian = (ImageView) convertView
					.findViewById(R.id.iv_yidian);
			holder.pingfen = (ImageView) convertView
					.findViewById(R.id.iv_pingfen);
			//holder.vArrow = convertView.findViewById(R.id.arrowlayout);
			holder.iv_arrow = (ImageView) convertView.findViewById(R.id.iv_arrow);
			holder.vAppearLayout = convertView.findViewById(R.id.btnLayout);
			holder.vLeft = (ImageButton)convertView.findViewById(R.id.imageButtonDG);
			holder.vMiddle = convertView.findViewById(R.id.ly_collect);
			holder.vRight = convertView.findViewById(R.id.ly_top);
			//holder.iv_left = (ImageView) convertView.findViewById(R.id.btn_order);
			//holder.tv_left = (TextView) convertView.findViewById(R.id.tv_order);
			convertView.setTag(holder);
		} else {
			holder = (OrderSongHolder) convertView.getTag();
		}

		// set data
		SongInfo info = list.get(position);

		holder.songname.setText(info.songName);
		holder.artist.setText(info.songArtist);
		if (info.songIsScore == 1)
			holder.pingfen.setVisibility(View.VISIBLE);

		if (info.isOrdered) {
			holder.yidian.setVisibility(View.VISIBLE);
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

		holder.iv_arrow.setOnClickListener(arrowListener);
		holder.iv_arrow.setTag(position);
		
		convertView.setOnClickListener(parentListener);
		holder.position = position;
		
		holder.vLeft.setOnClickListener(orderListener);
		holder.vLeft.setTag(info);

		holder.vMiddle.setOnClickListener(collectListener);
		holder.vMiddle.setTag(info);

		holder.vRight.setOnClickListener(topListener);
		holder.vRight.setTag(info);
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
					} else {
						operationPosition = -1;
					}
					NormalSongAdapter.this.notifyDataSetChanged();
				}
			} catch (Exception e) {
				// TODO: handle exception
			}
		}
	};

	/** 点歌 */
	private OnClickListener orderListener = new OnClickListener() {

		@Override
		public void onClick(View v) {
			try {
				long time = System.currentTimeMillis();
				if (time - clickTime > INTERVAL) {
					clickTime = time;
					if (!checkNetWork())
						return;

					if (AppStatus.isInRoom) {
						SongInfo song = (SongInfo) v.getTag();
						if (song != null && song.songId > 0) {
							if(orderManager != null)
							{
								if (!song.isOrdered ) 
									orderManager.orderSong(song);
								else
									orderManager.deleteSong((int)song.songId);
							}
						}
						v.getLocationOnScreen(location);
						location[0] = location[0] + v.getWidth() / 2;
						location[1] = location[1] - v.getHeight() / 2;
						// MyLog.v("bill","X:"+location[0]+"; Y:"+location[1]);
					} else if (enterRoomManager != null) {
						enterRoomManager.showEnterRoomMenuDialog(true);
					} else {
						ShowUtil.showToast(m_activity, R.string.msg_login_room);
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
					if (!checkNetWork())
						return;

					if (AppStatus.isInRoom) {
						SongInfo song = (SongInfo) v.getTag();
						if (song != null && song.songId > 0
								&& orderManager != null) {
							orderManager.topSong(song);
						}
					} else if (enterRoomManager != null) {
						enterRoomManager.showEnterRoomMenuDialog(true);
					} else {
						ShowUtil.showToast(m_activity, R.string.msg_login_room);
					}

				} else {
					// ShowUtil.showToast(mContext, R.string.click_quickly);
				}
			} catch (Exception e) {
				// TODO: handle exception
			}
		}

	};

	/** 收藏 */
	private OnClickListener collectListener = new OnClickListener() {

		@Override
		public void onClick(View v) {
			try {
				long time = System.currentTimeMillis();
				if (time - clickTime > INTERVAL) 
				{
					clickTime = time;
					if (!checkNetWork())
						return;

					SongInfo song = (SongInfo) v.getTag();
					if (song != null && song.songId > 0 && collectManager != null) 
						collectManager.collectSong((int)song.songId);
				} 
				else 
					ShowUtil.showToast(m_activity, R.string.click_quickly);
			} catch (Exception e) {
				e.printStackTrace();
			}
		}

	};

	/** 判断网络是否存在 */
	protected boolean checkNetWork() {
		return MobileNetStatUtil.checkCurrentNetState(m_activity);
	}

	/** 点歌等返回事件监听 */
	private OrderSongListener orderSongListener = new OrderSongListener() {

		@Override
		public void orderSongSuccess(SongInfo song) {
			NormalSongAdapter.this.notifyDataSetChanged();
			ShowUtil.showToast(m_activity, R.string.order_success);
			if (orderManager != null) {
				orderManager.setStartLocation(location);
				orderManager.startAnimation();
			}
		}

		@Override
		public void orderSongFail(ErrorSongInfo error) {
			if (error != null && !TextUtils.isEmpty(error.errorMessage))
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
			if (error != null && !TextUtils.isEmpty(error.errorMessage))
				ShowUtil.showToast(m_activity, error.errorMessage);
			else
				ShowUtil.showToast(m_activity, R.string.delete_fail);
		}

		@Override
		public void topSongSuccess(SongInfo song) {
			ShowUtil.showToast(m_activity, R.string.top_success);
			NormalSongAdapter.this.notifyDataSetChanged();
		}

		@Override
		public void topSongFail(ErrorSongInfo error) {
			if (error != null && !TextUtils.isEmpty(error.errorMessage))
				ShowUtil.showToast(m_activity, error.errorMessage);
			else
				ShowUtil.showToast(m_activity, R.string.top_fail);
		}

	};

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
			if (error != null && !TextUtils.isEmpty(error.errorMessage))
				ShowUtil.showToast(m_activity, error.errorMessage);
			else
				ShowUtil.showToast(m_activity, R.string.collect_fail);
		}
	};

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
					OrderSongHolder holder = (OrderSongHolder) v.getTag();
					if (holder == null || holder.position < 0 || holder.position > list.size())
						return;

					if (holder.position != operationPosition) {

						operationPosition = holder.position;
					} else {
						operationPosition = -1;
					}
					NormalSongAdapter.this.notifyDataSetChanged();
				}
			} catch (Exception e) {
				// TODO: handle exception
			}
		}
	};

}