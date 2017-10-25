package order.manager;

import java.util.ArrayList;
import java.util.List;

import order.show.listener.IOrderAnimaListener;
import order.util.OrderSongListener;
import android.annotation.SuppressLint;
import android.os.AsyncTask;
import android.os.Handler;
import android.os.Message;
import android.view.View;
import android.view.animation.AccelerateInterpolator;
import android.view.animation.Animation;
import android.view.animation.Animation.AnimationListener;
import android.view.animation.TranslateAnimation;
import android.widget.ImageView;
import assistant.activity.Home;
import assistant.entity.ErrorSongInfo;
import assistant.entity.SongInfo;
import assistant.entity.WorkInfo;
import assistant.global.AppStatus;
import assistant.global.KtvAssistantAPIConfig;
import assistant.global.MessageDef;
import assistant.task.orderapi.AddAndSetSongToTopTask;
import assistant.task.orderapi.DeleteSongFromOrderedTask;
import assistant.task.orderapi.OrderSongTask;
import assistant.task.orderapi.SetSongToTopTask;

/**
 * @author 宋静
 * @version 创建时间：2013-12-12 下午3:29:38 类说明 点歌管理
 */
public class OrderManager {
	// ================== 获取已点歌曲列表 ===================//
	private List<SongInfo> orderedlist = new ArrayList<SongInfo>();
	public boolean isLoadingOrderedData = false;

	public static final int AnimaDuration = 600;
	// ================= 点歌、删除、置顶操作 ================//
	private OrderSongListener songListener;
	private OrderSongTask orderSongTask;
	private DeleteSongFromOrderedTask deleteSongTask;
	private SetSongToTopTask setSongToTopTask;
	private AddAndSetSongToTopTask addAndSetSongToTopTask;
	/**************** 点歌飞盘特效 ***********************/
	public int[] startLocation = new int[2];
	public int[] endLocation = new int[2];
	public ImageView viewPan;
	
	IOrderAnimaListener m_orderAnimaListener;
	public OrderManager() {
	}
	public OrderManager(OrderSongListener listener) {
		songListener = listener;
	}
	
	public void setSongListener(OrderSongListener orderListener,IOrderAnimaListener animaListener) {
		this.songListener = orderListener;
		m_orderAnimaListener = animaListener;
	}

	public boolean checkWorkList(List<WorkInfo> list) {
		
		for (int i = 0; i < list.size(); i++) {
			WorkInfo info = list.get(i);
			if (info == null || info.song == null) {
				list.remove(info);
				//调整 i
				i--;
			}
		}

		return false;
	}

	public boolean isOrderedListExisted(){
		return orderedlist.isEmpty()?false:true;
	}

	private void clearTask(AsyncTask task) {
		if (task != null && task.getStatus() == AsyncTask.Status.RUNNING) {
			task.cancel(true);
		}
	}

	/** 点歌 */
	public void orderSong(SongInfo song) {
		clearTask(orderSongTask);

		orderSongTask = new OrderSongTask(orderSongHandler, AppStatus.ktvId,
				AppStatus.roomId, AppStatus.room_password, song);
		orderSongTask.execute();
	}

	/** 删除已点歌 */
	public void deleteSong(int song) {
		clearTask(deleteSongTask);

		deleteSongTask = new DeleteSongFromOrderedTask(orderSongHandler,
				AppStatus.ktvId, AppStatus.roomId, AppStatus.room_password, song);
		deleteSongTask.execute();
	}

	/** 置顶 */
	public void topSong(SongInfo song){
		if (song.isOrdered)
			topSong((int)song.songId);
		else 
		{
			clearTask(addAndSetSongToTopTask);
			addAndSetSongToTopTask = new AddAndSetSongToTopTask(orderSongHandler
					, AppStatus.ktvId, AppStatus.roomId,AppStatus.room_password, song);
			addAndSetSongToTopTask.execute();
		}
	}
	//用于已点歌曲的置顶
	public void topSong(int songid) {
		clearTask(setSongToTopTask);
		setSongToTopTask = new SetSongToTopTask(orderSongHandler,AppStatus.ktvId
				, AppStatus.roomId, AppStatus.room_password, songid);
		setSongToTopTask.execute();
	}

	@SuppressLint("HandlerLeak")
	protected Handler orderSongHandler = new Handler() {

		@Override
		public void handleMessage(Message msg) {
			super.handleMessage(msg);
			
			Home home = (Home)AppStatus.getActivityByName("Home");
			try {
				switch (msg.what) {
				// 点歌 
				case MessageDef.WM_ORDER_SONG:
					if(msg.arg1 == KtvAssistantAPIConfig.APIErrorCode.Nothing)//KtvAssistantAPIConfig.APIErrorCode.SongExisted: // 歌曲已存在
					{
						if(home != null)
							home.updateGold(msg.arg2);
						if(songListener!=null)
							songListener.orderSongSuccess(null);
					}
					else
					{
						if(songListener!=null)
							songListener.orderSongFail((ErrorSongInfo) msg.obj);
					}
					break;
				// 删歌
				case MessageDef.WM_DELETE_SONG_FROM_ORDERED:
					if(msg.arg1 == KtvAssistantAPIConfig.APIErrorCode.Nothing)
					{
						if(songListener!=null)
							songListener.deleteSongSuccess(null);
					}
					else
					{
						if(songListener!=null)
							songListener.deleteSongFail((ErrorSongInfo) msg.obj);
					}
					break;
				// 置顶
				case MessageDef.WM_SET_SONG_TO_TOP:
					if(msg.arg1 == KtvAssistantAPIConfig.APIErrorCode.Nothing)
					{
						if(songListener!=null)
							songListener.topSongSuccess(null);
					}
					else
					{
						if(songListener!=null)
							songListener.topSongFail((ErrorSongInfo) msg.obj);
					}
					break;
				case MessageDef.WM_ADD_AND_SET_SONG_TO_TOP:
					if(msg.arg1 == KtvAssistantAPIConfig.APIErrorCode.Nothing)
					{
						if(home != null)
							home.updateGold(msg.arg2);
						if(songListener!=null)
							songListener.topSongSuccess(null);
					}
					else
					{
						if(songListener!=null)
							songListener.topSongFail((ErrorSongInfo) msg.obj);
					}
					break;
				}
			} catch (Exception e) {
				e.printStackTrace();
			}

		}
	};

	public void setUpdateList(boolean isUpdateList) {
		//this.isUpdateList = isUpdateList;
	}

	public void setStartLocation(int[] pStartLocation) {
		startLocation = pStartLocation;
	}

	public void setEndLocation(int[] pEndLocation) {
		endLocation = pEndLocation;
	}

	public void setCDView(ImageView tView) {
		viewPan = tView;
	}

	public void startAnimation() {
		
		if(viewPan == null)
			return ;
		
		int fx, fy, tx, ty;

		fx = startLocation[0];
		// fx = 50;
		fy = startLocation[1];
		tx = endLocation[0];
		// tx = 300;
		ty = endLocation[1];

		TranslateAnimation an = new TranslateAnimation(fx, tx, fy, ty);
		an.setDuration(AnimaDuration);
		// an.setFillAfter(true);
		an.setInterpolator(new AccelerateInterpolator());
		an.setAnimationListener(new AnimationListener() {

			@Override
			public void onAnimationStart(Animation animation) {
				// TODO Auto-generated method stub
				viewPan.bringToFront();
				viewPan.setVisibility(View.VISIBLE);
			}

			@Override
			public void onAnimationRepeat(Animation animation) {
				// TODO Auto-generated method stub

			}

			@Override
			public void onAnimationEnd(Animation animation) {
				// TODO Auto-generated method stub
				viewPan.setVisibility(View.INVISIBLE);
				if(m_orderAnimaListener != null)
					m_orderAnimaListener.onAnimaEnd();
			}
		});

		viewPan.startAnimation(an);
	}
}
