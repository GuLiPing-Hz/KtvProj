package assistant.task;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.URL;
import java.util.Arrays;
import java.util.Comparator;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Environment;
import assistant.global.AppStatus;
import assistant.util.ShowLog;

/**
 * @author 宋静
 * @version 创建时间：2013-12-5 下午3:21:50 类说明
 */
public class FileUtil {
	public static String SDCARD_DIR = Environment.getExternalStorageDirectory()
			.getPath() + "/";

	/**
	 * 从固定目录/sdcard/pigbook/取文件
	 * 
	 * @param filepath
	 * @return
	 */
	public Bitmap getBitmapByPath(String filepath) {
		// MyLog.i("getBitmapByPath: FILE_PATH = " + filepath + ", SAVE_DIR = "
		// + SDCARD_DIR);

		String filename = PCommonUtil.md5Encode(filepath);
		Bitmap bitmap = getBitmapByLocationPath(filename, SDCARD_DIR);
		if (null == bitmap) {
			bitmap = getBitmapByUrl(filepath);
			saveBitmap(bitmap, filename, SDCARD_DIR);
		}

		return bitmap;
	}

	/**
	 * 从指定目录获取指定文件
	 * 
	 * @param filename
	 * @param dir
	 * @return
	 */
	public Bitmap getBitmapByLocationPath(String filename, String dir) {
		// MyLog.i("getBitmapByName: dir+filename = " + dir + filename);
		Bitmap bitmap = null;
		if (null == filename) {
			return bitmap;
		}

		File file = new File(dir + filename);
		if (!file.exists()) {
			return bitmap;
		}
		try {
			FileInputStream fileInputStream = new FileInputStream(file);
			bitmap = BitmapFactory.decodeStream(fileInputStream);
		} catch (FileNotFoundException e) {

			ShowLog.showException(e);
		}

		return bitmap;
	}

	/**
	 * 通过url从网上下载图片
	 * 
	 * @param path
	 * @return
	 */
	public Bitmap getBitmapByUrl(String urlpath) {
		// MyLog.i("getBitmapByUrl..." + urlpath);
		Bitmap bitmap = null;

		try {
			URL url = new URL(urlpath);
			HttpURLConnection con = (HttpURLConnection) url.openConnection();
			con.setRequestMethod("GET");
			con.setConnectTimeout(30000);
			if (con.getResponseCode() == HttpURLConnection.HTTP_OK) {
				InputStream inputStream = con.getInputStream();
				bitmap = BitmapFactory.decodeStream(inputStream);
			} else {
				// MyLog.i("con.getResponseCode() = " + con.getResponseCode());
			}
			con.disconnect();
		} catch (MalformedURLException e) {
			ShowLog.showException(e);
		} catch (IOException e) {
			ShowLog.showException(e);
		}

		return bitmap;
	}

	/**
	 * 临时图片保存
	 * 
	 * @param bitmap
	 * @param filename
	 * @return
	 */
	public String saveBitmap(Bitmap bitmap, String filename) {
		String saveDir = AppStatus.s_appDirUtil.getCacheDir();
		saveBitmap(bitmap, filename,saveDir );
		return saveDir + filename;
	}

	/**
	 * 保存文件
	 * 
	 * @param bitmap
	 * @param filename
	 * @param savepath
	 * @return
	 */
	public boolean saveBitmap(Bitmap bitmap, String filename, String savedir) {
		if (null == bitmap || null == filename || null == savedir) {
			ShowLog.i("bitmap, filename or savedir is null...");
			return false;
		}

		try {
			File file = new File(savedir + filename);

			file.createNewFile();
			FileOutputStream fileOutputStream = new FileOutputStream(file);
			bitmap.compress(Bitmap.CompressFormat.JPEG, 100, fileOutputStream);
			fileOutputStream.flush();
			fileOutputStream.close();
		} catch (FileNotFoundException e) {
			ShowLog.showException(e);
		} catch (IOException e) {
			ShowLog.showException(e);
		}

		return true;
	}

	/**
	 * 更新文件的修改时间
	 * 
	 * @param dir
	 * @param filename
	 */
	public void updateFileModifyTime(String dir, String filename) {
		if (null == dir || null == filename) {
			return;
		}

		File file = new File(dir, filename);
		long now = System.currentTimeMillis();
		file.setLastModified(now);
	}

	/**
	 * 如果指定目录dir下，文件个数大于filecount，则删除指定比例percent的文件 filecount小于0，则直接删除指定比例文件
	 * percent的值为0到1
	 * 
	 * @param dir
	 * @param filecount
	 * @param percent
	 */
	public void deleteSomaFiles(String dir, int filecount, int percent) {
		if (null == dir || filecount == 0 || percent == 0) {
			return;
		}

		File fileDir = new File(dir);
		File[] files = fileDir.listFiles();
		if (files.length > filecount) {
			int deleteFileCount = files.length * percent;
			Arrays.sort(files, new FileLastModifySort());

			for (int i = 0; i < deleteFileCount; i++) {
				files[i].delete();
			}
		}

	}

	class FileLastModifySort implements Comparator<File> {

		public int compare(File file1, File file2) {
			// TODO Auto-generated method stub
			if (file1.lastModified() > file2.lastModified()) {
				return 1;
			} else if (file1.lastModified() == file2.lastModified()) {
				return 0;
			} else {
				return -1;
			}
		}

	}

}
