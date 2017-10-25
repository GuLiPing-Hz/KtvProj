package assistant.task.userapi;

import org.json.JSONObject;

import android.graphics.Bitmap;
import android.os.AsyncTask;
import android.os.Handler;
import android.os.Message;
import android.text.TextUtils;
import assistant.entity.UserInfo;
import assistant.global.AppStatus;
import assistant.global.KtvAssistantAPIConfig;
import assistant.global.MessageDef;
import assistant.share.AccountManager;
import assistant.task.FileUtil;
import assistant.task.PCommonUtil;
import assistant.util.ShowLog;

import com.tiange.phttprequest.PHttpRequest;
import com.tiange.phttprequest.PHttpRequest.RequestMethodType;
import com.tiange.phttprequest.PHttpUploadFileCallback;

/**
 * @author 宋静
 * @version 创建时间：2013-12-5 下午3:16:18 类说明 4.2用户头像上传
 */
public class UploadPhotoTask extends AsyncTask<Void, Float, String> {
	private Handler handler;
	private long userId;
	private String roomId;
	private String imageName;
	public Bitmap photo;
	public String filePath;

	public UploadPhotoTask(Handler handler, long userId, String roomId,
			String imageName, String filePath) {
		super();
		this.handler = handler;
		this.userId = userId;
		this.roomId = roomId;
		this.imageName = imageName;
		this.filePath = filePath;
	}

	public UploadPhotoTask(Handler handler, long userId, String roomId,
			String imageName, Bitmap photo) {
		super();
		this.handler = handler;
		this.userId = userId;
		this.roomId = roomId;
		this.imageName = imageName;
		this.photo = photo;

		FileUtil fileUtil = new FileUtil();
		this.filePath = fileUtil.saveBitmap(photo, "photo.jpg");
	}

	public String getUrl() {
		if (userId <= 0 || TextUtils.isEmpty(imageName)
				|| TextUtils.isEmpty(roomId))
			return "";

		String baseUrl = KtvAssistantAPIConfig.KtvAssistantAPIDomain
				+ KtvAssistantAPIConfig.APIUrl.UploadPhoto;
		String param = "?userID=" + userId + "&roomID=" + roomId
				+ "&imageName=" + imageName;

		return PCommonUtil.generateAPIStringWithSecret(baseUrl, param);
	}

	@Override
	protected String doInBackground(Void... params) {
		String url = getUrl();
		if (TextUtils.isEmpty(url))
			return "";

		PHttpRequest request = PHttpRequest.requestWithURL(url);
		request.setRequestMethodType(RequestMethodType.RequestMethodTypeMultipart);
		request.setRequestSocketTimeOut(20000);
		request.setUploadFilePath(filePath, "imagedata");
		request.setUploadFileCallback(new PHttpUploadFileCallback() {

			@Override
			public void requestSendBytes(long totalSize, long num) {
				// TODO Auto-generated method stub

			}

			@Override
			public void reqeustProgress(float progress) {
				// TODO Auto-generated method stub
				publishProgress(progress);
			}
		});
		String result = request.startSyncRequestString();
		return result;
	}

	@Override
	protected void onProgressUpdate(Float... values) {
		// TODO Auto-generated method stub
		super.onProgressUpdate(values);

		// MyLog.d("这里加入handler事件，通知刷新界面: values: " + values[0]);

		// 这里加入handler事件，通知刷新界面
		// Message msg = handler.obtainMessage();
		// msg.what = GlobalDef.WM_UPLOAD_PHOTO_PROCESS;
		// msg.arg1 = 0;//0-上传时的进度，1-上传完成（失败或者成功）
		// msg.obj = values[0];
		// handler.sendMessage(msg);

	}

	@Override
	protected void onPostExecute(String result) {
		// TODO Auto-generated method stub
		super.onPostExecute(result);

		JSONObject jsonObject = PCommonUtil.parseString2JsonObject(result);
		int nStatus = 0, errorcode = KtvAssistantAPIConfig.APIErrorCode.Error;
		String errorMsg = KtvAssistantAPIConfig.ErrorMsgUnknow;
		try {

			if (null != jsonObject) {
				ShowLog.i("UploadPhotoTask result: " + jsonObject.toString());

				nStatus = jsonObject.optInt("status");
				errorMsg = jsonObject.optString("msg");
				errorcode = jsonObject.optInt("errorcode");
				if (1 == nStatus) {

					JSONObject jsonResultObject = jsonObject
							.optJSONObject("result");
					JSONObject jsonUser = jsonResultObject
							.optJSONObject("User");
					UserInfo user = new UserInfo(jsonUser);
					
					if(user.userIdx != 0){
						AppStatus.user.headphoto = user.headphoto;
						AccountManager.writeMyLocalAccount();
					}
						
				}
			}
			else
			{
				errorMsg = "服务器异常";
			}
		} catch (Exception e) {
			ShowLog.showException(e);
		}

		// 这里加入handler事件，通知刷新界面
		boolean isError = (0 == nStatus);
		Message msg = handler.obtainMessage();
		msg.what = MessageDef.WM_UPLOAD_PHOTO;// command
		msg.arg1 = isError ? errorcode : KtvAssistantAPIConfig.APIErrorCode.Nothing;
		msg.obj = errorMsg;
		handler.sendMessage(msg);
	}
}
