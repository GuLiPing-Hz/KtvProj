package assistant.task.orderapi;

import java.net.URLEncoder;

import org.json.JSONObject;

import android.os.AsyncTask;
import android.os.Handler;
import android.os.Message;
import assistant.entity.ErrorSongInfo;
import assistant.entity.SongInfo;
import assistant.global.AppStatus;
import assistant.global.KtvAssistantAPIConfig;
import assistant.global.MessageDef;
import assistant.task.PCommonUtil;
import assistant.util.ShowLog;

import com.tiange.phttprequest.PHttpRequest;

public class OrderSongTask extends AsyncTask<Void, String, String> {

	protected Handler handler;
	protected String ktvid;
	protected String roomid;
	protected SongInfo song;
	protected String password;
	protected String requestUrl = null;

	public OrderSongTask(Handler tHandler, String tKtvId, String tRoomId,
			String tPassword, SongInfo song) {
		this.handler = tHandler;
		this.ktvid = tKtvId;
		this.roomid = tRoomId;
		this.password = tPassword;
		this.song = song;
	}

	protected String getOrderSongUrl(String tKtvId, String tRoomId,
			String tPassword, long tSongId, String tSongName,
			String tArtistName, int tIsScore) {

		String encodeSongName = URLEncoder.encode(tSongName);
		String encodeArtistName = URLEncoder.encode(tArtistName);

		String baseUrl = KtvAssistantAPIConfig.KtvAssistantAPIDomain
				+ KtvAssistantAPIConfig.APIUrl.SongOrderSong;
		String param = "?ktvid=" + tKtvId +"&idx="+AppStatus.userIdx+ "&roomid=" + tRoomId
				+ "&roompassword=" + tPassword + "&Songid=" + tSongId
				+ "&Songname=" + encodeSongName + "&Artistname="
				+ encodeArtistName + "&Isscore=" + tIsScore;

		return PCommonUtil.generateAPIStringWithSecret(baseUrl, param);
	}

	@Override
	protected String doInBackground(Void... params) {
		// TODO Auto-generated method stub
		requestUrl = getOrderSongUrl(ktvid, roomid, password, song.songId,
				song.songName, song.songArtist, song.songIsScore);

		PHttpRequest request = PHttpRequest.requestWithURL(requestUrl);
		request.setRequestSocketTimeOut(20000);
		String result = request.startSyncRequestString();
		return result;
	}

	@Override
	protected void onPostExecute(String result) {
		// TODO Auto-generated method stub
		super.onPostExecute(result);

		JSONObject jsonObject = PCommonUtil.parseString2JsonObjectGold(result);
		int nStatus = 0, errorcode = KtvAssistantAPIConfig.APIErrorCode.Error;
		int gold = 0;
		String errorMsg = KtvAssistantAPIConfig.ErrorMsgUnknow;

		try {

			if (null != jsonObject) {
				ShowLog.i("OrderSongTask result: " + jsonObject.toString());

				nStatus = jsonObject.optInt("status");
				errorMsg = jsonObject.optString("msg");
				errorcode = jsonObject.optInt("errorcode");
				if (1 == nStatus) 
					gold = jsonObject.optInt("gold");
			} 
			else
			{
				errorMsg = "服务器异常";
			}
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}

		// 这里加入handler事件，通知刷新界面
		boolean isError = (0 == nStatus);
		Message msg = handler.obtainMessage();
		msg.what = MessageDef.WM_ORDER_SONG;// command
		msg.arg1 =  isError ? errorcode : KtvAssistantAPIConfig.APIErrorCode.Nothing;
		msg.arg2 = gold;
		msg.obj = isError ? new ErrorSongInfo(errorMsg, (int)song.songId) : null;
		handler.sendMessage(msg);
	}

}
