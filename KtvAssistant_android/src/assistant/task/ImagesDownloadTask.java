package assistant.task;

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.util.Set;

import org.apache.http.HttpEntity;
import org.apache.http.HttpResponse;
import org.apache.http.HttpStatus;
import org.apache.http.client.ClientProtocolException;
import org.apache.http.client.HttpClient;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.params.HttpConnectionParams;
import org.apache.http.params.HttpParams;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.net.http.AndroidHttpClient;
import android.os.AsyncTask;
import android.os.Handler;
import android.util.Log;
import assistant.util.ImageUtil;
import assistant.util.ShowLog;
import assistant.util.ToolUtil;

/**
 * @author song
 * @version 创建时间：2012-11-28 下午2:06:26 类说明 批量下载图片并通知adapter更新
 * 
 * 通过URL进行http请求下载图片，
 * 然后通过对URL进行MD5加密得到的加密值来作为文件名进行保存，一般质量保存为70%
 */
public class ImagesDownloadTask extends AsyncTask<Void, Void, Object> {
	private final static String TAG = "ImagesDownloadTask";
	//回调Handler
	private Handler handler;
	//当前下载量
	private int count = 0;
	//指定路径
	private String path;
	//指定的回调Message
	private int msgWhat;
	//下载之后是否需要休息下
	private boolean sleep;
	//指定下载完几个图片后通知UI
	private int every = 3;
	//需要下载的队列
	private Set<String> images;
	
	public ImagesDownloadTask(Handler handler,Set<String> list, String path,
			int msgWhat, boolean needSleep) {
		this.handler = handler;
		images = list;
		this.path = path;
		this.msgWhat = msgWhat;
		sleep = needSleep;
	}

	public ImagesDownloadTask(Handler handler, Set<String> list, String path,
			int msgWhat, boolean needSleep, int space) {
		this.handler = handler;
		images = list;
		this.path = path;
		this.msgWhat = msgWhat;
		sleep = needSleep;
		if (space > 0)
			every = space;
	}

	@Override
	protected Object doInBackground(Void... params) {
		// TODO Auto-generated method stub
		if (images == null || images.size() <= 0)
			return null;

		try {
			Bitmap bitmap = null;

			for (String url : images) { // 只遍历一次
				try {
					if (url == null || url.length() <= 0)
						continue;

					Log.i(TAG,"try to download "+url);
					String imageName = ToolUtil.md5(url);
					
					if (new File(path + imageName).exists()) {
						bitmap = ImageUtil.getBitmap(path + imageName);
						if (bitmap != null) {
							bitmap.recycle();
							bitmap = null;
							continue;
						}
					}
					
					byte[] data = downloadInputStream(url);//通过URL同步下载图片

					if (data != null) //解析下载的图片内容
						bitmap = BitmapFactory.decodeByteArray(data, 0,data.length);

					if (data != null && bitmap != null) 
					{
						ImageUtil.saveBitmap(bitmap, path + imageName);
						bitmap.recycle();
					} 
					else 
					{
						 ShowLog.e(TAG, "图片下载失败：" + url);
					}
					
					if (count != 0 && count % every == 0)
						handler.sendEmptyMessage(msgWhat);

				} catch (Exception e) {
					e.printStackTrace();

				} catch (OutOfMemoryError error) {
					error.printStackTrace();

				} finally {
					count++;
				}
			}
				
			handler.sendEmptyMessage(msgWhat);
			count = 0;
		} catch (Exception e) {
			e.printStackTrace();
		}
		return null;
	}

	//通过指定的URL，获取到具体的Content数据
	private byte[] downloadInputStream(String url) throws Exception {
		HttpClient client = AndroidHttpClient.newInstance("Android");
		HttpParams params = client.getParams();
		HttpConnectionParams.setConnectionTimeout(params, 3000);
		HttpConnectionParams.setSocketBufferSize(params, 3000);
		HttpGet httpGet = null;
		InputStream inputStream = null;
		byte[] data = null;
		try {
			httpGet = new HttpGet(url);
			HttpResponse response = client.execute(httpGet);
			int stateCode = response.getStatusLine().getStatusCode();
			if (stateCode != HttpStatus.SC_OK) {
				return null;
			}
			HttpEntity entity = response.getEntity();
			if (entity != null) {
				try {
					inputStream = entity.getContent();
					data = readStream(inputStream, sleep);
				} finally {
					if (inputStream != null) {
						inputStream.close();
					}
					entity.consumeContent();
				}
			}
		} catch (ClientProtocolException e) {
			httpGet.abort();
			e.printStackTrace();
		} catch (IOException e) {
			httpGet.abort();
			e.printStackTrace();
		} finally {
			((AndroidHttpClient) client).close();
		}
		return data;

	}

	/*
	 * 从http的内容InputStream中得到图片字节流
	 */
	private static byte[] readStream(InputStream inStream, boolean sleep)
			throws Exception {
		ByteArrayOutputStream outStream = new ByteArrayOutputStream();
		byte[] buffer = new byte[1024];
		int len = -1;
		while ((len = inStream.read(buffer)) != -1) {
			outStream.write(buffer, 0, len);
			if (sleep)
				Thread.sleep(100);
		}
		outStream.close();
		inStream.close();
		return outStream.toByteArray();
	}
}
