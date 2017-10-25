package order.manager;

import java.util.ArrayList;
import java.util.List;

import order.util.CollectListListener;
import order.util.CollectSongListener;
import android.annotation.SuppressLint;
import android.os.AsyncTask;
import android.os.Handler;
import android.os.Message;
import android.text.TextUtils;
import assistant.entity.ErrorSongInfo;
import assistant.entity.SongInfo;
import assistant.global.AppStatus;
import assistant.global.KtvAssistantAPIConfig;
import assistant.global.MessageDef;
import assistant.task.collectapi.CollectSongTask;
import assistant.task.collectapi.DeleteCollectSongTask;
import assistant.task.collectapi.GetUserCollectListTask;
import assistant.util.ShowLog;

/**
 * @author 宋静
 * @version 创建时间：2013-11-4 下午3:25:14 类说明 收藏歌曲管理
 */
public class CollectManager {
	static String TAG = "CollectManager";

	// ================== 获取收藏歌曲列表 ===================//
	public List<SongInfo> collectList = new ArrayList<SongInfo>();
	private CollectListListener listListener;
	private GetUserCollectListTask getUserCollectTask;
	
	// ================== 增加收藏、删除收藏 ===================//
	private CollectSongListener songListener;
	private CollectSongTask collectSongTask;
	private DeleteCollectSongTask deleteSongTask;
	//是否重新获取当前收藏列表
	private boolean isUpdateList = false;

	public CollectManager(CollectListListener listener) {
		listListener = listener;
	}

	public void setListListener(CollectListListener listener) {
		this.listListener = listener;
	}

	//获取当前用户的收藏歌曲列表
	public void getCollectData() {
		clearTask(getUserCollectTask);

		getUserCollectTask = new GetUserCollectListTask(listHandler,AppStatus.userIdx);
		getUserCollectTask.execute();
	}

	@SuppressLint("HandlerLeak")
	private Handler listHandler = new Handler() {

		@Override
		public void handleMessage(Message msg) {
			// TODO Auto-generated method stub
			super.handleMessage(msg);
			try {
				switch (msg.what) {
				case MessageDef.WM_GET_COLLECT_LIST:
					if(msg.arg1 == KtvAssistantAPIConfig.APIErrorCode.Nothing)
					{
						@SuppressWarnings("unchecked")
						List<SongInfo> list = (List<SongInfo>) msg.obj;
						//删除一些错误的元素
						showList(list);
					}
					else
					{
						// 获取失败
						ShowLog.e(TAG, "listHandler error：" + msg.what + ","+ msg.obj.toString());
						
						collectList.clear();
						
						if (listListener != null){
							listListener.updateCollectList(collectList);
							listListener.getCollectListFail(msg.obj.toString());
						}
					}
					break;
				}
			} catch (Exception e) {
				// TODO: handle exception
				ShowLog.showException(e);
			}
		}
	};

	private void showList(List<SongInfo> list) {
		collectList.clear();

		// 排除错误单元
		for (int i = 0; i < list.size(); i++) {
			SongInfo s = list.get(i);
			if (s == null || s.songId <= 0 || TextUtils.isEmpty(s.songName)) {
				list.remove(s);
				i--;
			} 
			else 
				collectList.add(s);
		}

		if (listListener != null) {
			listListener.updateCollectList(collectList);
		}
	}

	public CollectManager(CollectSongListener listener) {
		this.songListener = listener;
	}

	public void setSongListener(boolean isUpdateList,
			CollectSongListener listener) {
		this.isUpdateList = isUpdateList;
		this.songListener = listener;
	}

	public void collectSong(int songId) {
		clearTask(getUserCollectTask);

		collectSongTask = new CollectSongTask(songHandler, AppStatus.userIdx,songId);
		collectSongTask.execute();
	}

	public void deleteCollectSong(int songId) {
		clearTask(deleteSongTask);

		deleteSongTask = new DeleteCollectSongTask(songHandler,
				AppStatus.userIdx, songId);
		deleteSongTask.execute();
	}

	private void clearTask(AsyncTask task) {
		if (task != null && task.getStatus() == AsyncTask.Status.RUNNING) {
			task.cancel(true);
		}
	}

	@SuppressLint("HandlerLeak")
	private Handler songHandler = new Handler() {

		@Override
		public void handleMessage(Message msg) {
			// TODO Auto-generated method stub
			super.handleMessage(msg);
			try {
				switch (msg.what) {
				// 收藏
				case MessageDef.WM_COLLECT_SONG:
					if(msg.arg1 == KtvAssistantAPIConfig.APIErrorCode.Nothing)
					{
						if (songListener != null) 
						{
							songListener.collectSongSuccess();
							if (isUpdateList && listListener != null)
								getCollectData();
						} 
					}
					else 
						songListener.collectSongFail((ErrorSongInfo) msg.obj);
					break;
				// 删除收藏
				case MessageDef.WM_DELETE_COLLECT_SONG:
					if(msg.arg1 == KtvAssistantAPIConfig.APIErrorCode.Nothing)
					{
						if (songListener != null) 
						{
							songListener.deleteCollectSuccess(msg.arg2);
							for(SongInfo info : collectList)
							{
								if(info.songId == msg.arg2)
								{
									collectList.remove(info);
									break;
								}
							}
							if (isUpdateList && listListener != null)
								listListener.updateCollectList(collectList);
						} 
					}
					else
						songListener.deleteCollectFail((ErrorSongInfo) msg.obj);
					break;
				}
			} catch (Exception e) {
				// TODO: handle exception
				ShowLog.showException(e);
			}
		}
	};

	public void setUpdateList(boolean isUpdateList) {
		this.isUpdateList = isUpdateList;
	}

}
