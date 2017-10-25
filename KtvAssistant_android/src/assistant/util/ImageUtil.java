package assistant.util;

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Set;

import tiange.ktv.assistant.R;
import android.content.Context;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.Bitmap.CompressFormat;
import android.graphics.Bitmap.Config;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Matrix;
import android.graphics.Paint;
import android.graphics.PixelFormat;
import android.graphics.PorterDuff.Mode;
import android.graphics.PorterDuffXfermode;
import android.graphics.Rect;
import android.graphics.RectF;
import android.graphics.drawable.BitmapDrawable;
import android.graphics.drawable.Drawable;
import android.graphics.drawable.NinePatchDrawable;
import android.os.Environment;
import android.os.Handler;
import android.text.TextUtils;
import android.view.View;
import assistant.global.AppStatus;
import assistant.task.ImagesDownloadTask;

/**
 * @author shou_peng
 * @version 创建时间：2012-11-15 下午05:56:04 类说明
 */
public class ImageUtil {
	/**
	 * 设置图片裁剪的一些参数
	 * 
	 * @param intent
	 * @return
	 */
	public static Intent setCropExtra(Intent intent) {
		intent.putExtra("crop", "true");
		// aspectX aspectY 是宽高的比例
		intent.putExtra("aspectX", 1);
		intent.putExtra("aspectY", 1);
		// outputX outputY 是裁剪图片宽高
		intent.putExtra("outputX", 640);
		intent.putExtra("outputY", 640);
		intent.putExtra("return-data", false);

		return intent;
	}

	/**
	 * 保存图片，以100%的质量保存
	 * 
	 * @param bitmap
	 *            图片资源
	 * @param picPath
	 *            保存路径(含文件名)
	 */
	public static void saveBitmap(Bitmap bitmap, String picPath) {
		saveBitmap(bitmap,picPath,100);
	}
	
	/**
	 * 保存图片,以70%的质量保存
	 * 
	 * @param bitmap 图片资源
	 * @param picPath保存路径(含文件名)
	 */
	public static void saveBitmapOther(Bitmap bitmap, String picPath) {
		saveBitmap(bitmap,picPath,70);
	}

	private static void saveBitmap(Bitmap bitmap, String picPath,int percent) {
		if (bitmap == null || picPath == null) {
			return;
		}
		try {
			if (Environment.getExternalStorageState().equals(Environment.MEDIA_MOUNTED)) {
				// 清理目录
				AppStatus.s_appDirUtil.removeCache(new File(picPath).getParent());
				// 判断SDCARD上的空间
				if (AppDirUtil.FREE_SD_SPACE_NEEDED_TO_CACHE > AppDirUtil.freeSpaceOnSD()) {
					return;
				}
			}
			
			percent = percent>100?100:(percent<50?50:percent);
			String picTemp = picPath+".temp";
			File file = new File(picTemp);
			if(file.exists())
				file.delete();
			FileOutputStream out;
			try {
				out = new FileOutputStream(file);
				if (bitmap.compress(Bitmap.CompressFormat.PNG, percent, out)) {
					out.flush();
					out.close();
				}
			} catch (FileNotFoundException e) {
				e.printStackTrace();
			} catch (IOException e) {
				e.printStackTrace();
			}
			
			file.renameTo(new File(picPath));
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
	/**
	 * 获取图片
	 * 
	 * @param path
	 *            图片的保存路径
	 * @return
	 */
	public static Bitmap getBitmapBanner(String url){
		String path = AppStatus.s_appDirUtil.getBannerDir();
		String name = ToolUtil.md5(url);
		return getBitmap(path+name);
	}
	public static Bitmap getBitmapSinger(String url){
		String path = AppStatus.s_appDirUtil.getSingerDir();
		String name = ToolUtil.md5(url);
		return getBitmap(path+name);
	}
	public static Bitmap getBitmapPhoto(String headUrl){
		String path = AppStatus.s_appDirUtil.getHeadDir();
		String name = ToolUtil.md5(headUrl);
		return getBitmap(path+name);
	}
	public static Bitmap getBitmapGift(String url){
		String path = AppStatus.s_appDirUtil.getGiftDir();
		String name = ToolUtil.md5(url);
		return getBitmap(path+name);
	}
	
	/**
	 * 获取指定路径的图片，如果路径不正确，文件不存在返回null，
	 * 		如果文件存在，但是不能解析出bitmap则会把该文件删除。
	 */
	public static Bitmap getBitmap(String path) {
		 if(TextUtils.isEmpty(path)){
			 return null;
		 }
		 File f = new File(path);
		 if(f==null || !f.exists() || f.isDirectory()){
			 return null;
		 }
		Bitmap b = null;
		try {
			BitmapFactory.Options opts = new BitmapFactory.Options();
			opts.inJustDecodeBounds = true;
			BitmapFactory.decodeStream(new FileInputStream(path), null, opts);
			opts.inSampleSize = ImageUtil.computeSampleSize(opts, -1, 320 * 320);
			opts.inJustDecodeBounds = false;
			b = BitmapFactory.decodeStream(new FileInputStream(path), null,opts);
		} catch (Exception e) {
			e.printStackTrace();
			f.delete();
		} catch (OutOfMemoryError error) {
			if (b != null) {
				b.recycle();
			}
			error.printStackTrace();
			f.delete();
		}
		return b;
	}

	/**
	 * 从本地获取图片
	 * 
	 * @param path
	 *            路径
	 * @param maxWidth
	 *            取样最大宽度(单位px)
	 * @param maxHeight
	 *            取样最大高度(单位px)
	 * @return
	 */
	public static Bitmap getBitmap(String path, int maxWidth, int maxHeight) {
		Bitmap b = null;
		try {
			BitmapFactory.Options opts = new BitmapFactory.Options();
			opts.inJustDecodeBounds = true;
			BitmapFactory.decodeStream(new FileInputStream(path), null, opts);
			opts.inSampleSize = ImageUtil.computeSampleSize(opts, -1, maxWidth* maxHeight);
			opts.inJustDecodeBounds = false;
			b = BitmapFactory.decodeStream(new FileInputStream(path), null,opts);
		} catch (Exception e) {
			e.printStackTrace();
		} catch (OutOfMemoryError error) {
			if (b != null) {
				b.recycle();
			}
			error.printStackTrace();
		}
		return b;
	}

	public static Bitmap getBitmapFromFileCache(Context context, String url) {
		Bitmap bm = null;
		try {
			String filePath = AppStatus.s_appDirUtil.getHeadDir();
			String imageName = ToolUtil.md5(url);
			if (new File(filePath + imageName).exists()) {
				bm = getBitmap(filePath + imageName);
			} else {
				bm = ((BitmapDrawable) context.getResources().getDrawable(
						R.drawable.pic_touxiang_admin)).getBitmap();
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
		return bm;
	}

	/**
	 * 将File转成byte[]
	 * 
	 * @param file
	 * @return
	 */
	public static byte[] getBytesFromFile(File file) {
		if (file == null) {
			return null;
		}
		try {
			FileInputStream stream = new FileInputStream(file);
			ByteArrayOutputStream out = new ByteArrayOutputStream(1000);
			byte[] b = new byte[1000];
			int n;
			while ((n = stream.read(b)) != -1) {
				out.write(b, 0, n);
			}
			stream.close();
			out.close();
			return out.toByteArray();
		} catch (IOException e) {
			e.printStackTrace();
			return null;
		}
	}

	/**
	 * 将图片IO流数据转成byte[] 随后可以调用Bitmap b =
	 * BitmapFactory.decodeByteArray(byte,0,byte.length)获得Bitmap
	 * 
	 * @param is
	 * @return
	 * @throws IOException
	 */
	public static byte[] getBytesFromIO(InputStream is) throws IOException {
		ByteArrayOutputStream outstream = new ByteArrayOutputStream();
		byte[] buffer = new byte[1024]; // 用数据装
		int len = -1;
		while ((len = is.read(buffer)) != -1) {
			outstream.write(buffer, 0, len);
		}
		outstream.close();
		// 关闭流一定要记得。
		return outstream.toByteArray();
	}

	/**
	 * 截取图片，去除图片左右的暗色 暂时只处理左右两边
	 * 
	 * @param srcImage
	 * @return
	 */
	public static Bitmap cutRealImage(Bitmap srcImage) {
		if (null == srcImage) {
			return null;
		}

		int srcWidth = srcImage.getWidth();
		int srcHeight = srcImage.getHeight();
		int middleIndexWidth = srcWidth / 2;
		int middleIndexHeight = srcHeight / 2;

		int widthLeft = 1;
		int widthRight = srcWidth - 1;
		int heightTop = 1;
		int heightBottom = srcHeight - 1;
		int indexColor = 0;

		// left,从中间往左计算
		int beginIndexWidth = 0;
		int endIndexWidth = middleIndexWidth;
		while (endIndexWidth > beginIndexWidth) {

			indexColor = srcImage.getPixel(endIndexWidth, middleIndexHeight);
			if (checkWithRemoveColor(indexColor)) {
				widthLeft = endIndexWidth + 10;
				break;
			}
			endIndexWidth = endIndexWidth - 5;
		}

		// right,从中间往右计算
		beginIndexWidth = middleIndexWidth;
		endIndexWidth = srcWidth;
		while (endIndexWidth > beginIndexWidth) {

			indexColor = srcImage.getPixel(beginIndexWidth, middleIndexHeight);
			if (checkWithRemoveColor(indexColor)) {
				widthRight = beginIndexWidth - 10;
				break;
			}
			beginIndexWidth = beginIndexWidth + 5;

		}

		// top
		int beginIndexHeight = 0;
		int endIndexHeight = middleIndexHeight;
		while (endIndexHeight > beginIndexHeight) {

			indexColor = srcImage.getPixel(middleIndexWidth, endIndexHeight);
			if (checkWithRemoveColor(indexColor)) {
				heightTop = endIndexHeight + 10;
				break;
			}
			endIndexHeight = endIndexHeight - 5;

		}

		// bottom
		beginIndexHeight = middleIndexHeight;
		endIndexHeight = srcHeight;
		while (endIndexHeight > beginIndexHeight) {

			indexColor = srcImage.getPixel(middleIndexWidth, beginIndexHeight);
			if (checkWithRemoveColor(indexColor)) {
				heightBottom = beginIndexHeight - 10;
				break;
			}
			beginIndexHeight = beginIndexHeight + 5;

		}

		// 截取中间的图片
		int dstWidth = widthRight - widthLeft;
		int dstHeight = heightBottom - heightTop;
		Bitmap dstImage = Bitmap.createBitmap(srcImage, widthLeft, heightTop,
				dstWidth, dstHeight);

		return dstImage;
	}

	private static boolean checkWithRemoveColor(int indexColor) {

		final int removeRed = 75;
		final int removeGreen = 75;
		final int removeBlue = 75;

		int red = Color.red(indexColor);
		int green = Color.green(indexColor);
		int blue = Color.blue(indexColor);

		if (red == removeRed && green == removeGreen && blue == removeBlue) {
			return true;
		}

		return false;
	}

	public static int computeSampleSize(BitmapFactory.Options options,
			int minSideLength, int maxNumOfPixels) {
		int initialSize = computeInitialSampleSize(options, minSideLength,
				maxNumOfPixels);
		int roundedSize;
		if (initialSize <= 8) {
			roundedSize = 1;
			while (roundedSize < initialSize) {
				roundedSize <<= 1;
			}
		} else {
			roundedSize = (initialSize + 7) / 8 * 8;
		}
		return roundedSize;
	}

	private static int computeInitialSampleSize(BitmapFactory.Options options,
			int minSideLength, int maxNumOfPixels) {
		double w = options.outWidth;
		double h = options.outHeight;
		int lowerBound = (maxNumOfPixels == -1) ? 1 : (int) Math.ceil(Math
				.sqrt(w * h / maxNumOfPixels));
		int upperBound = (minSideLength == -1) ? 128 : (int) Math.min(
				Math.floor(w / minSideLength), Math.floor(h / minSideLength));
		if (upperBound < lowerBound) {
			// return the larger one when there is no overlapping zone.
			return lowerBound;
		}
		if ((maxNumOfPixels == -1) && (minSideLength == -1)) {
			return 1;
		} else if (minSideLength == -1) {
			return lowerBound;
		} else {
			return upperBound;
		}
	}

	/**
	 * 将头像源图处理成UI显示的图片(用于用户头像)
	 * 
	 * @param con
	 * @param photo
	 *            源图,想显示默认图该参数传null
	 * @return
	 */
	public static Bitmap dealShape(Context con, Bitmap photo) {
		Bitmap b_zoom = null;
		Bitmap b_default = null;
		Bitmap bitmapWithReflection = null;
		Bitmap b_mask = null;
		Bitmap b_bg = null;
		while (true) {
			try {
				b_mask = BitmapFactory.decodeResource(con.getResources(),
						R.drawable.ic_headphoto_mask);
				int maskWidth = b_mask.getWidth();
				int maskHeight = b_mask.getHeight();

				b_bg = BitmapFactory.decodeResource(con.getResources(),
						R.drawable.ic_headphoto_bg);
				if (photo != null) {
					b_zoom = Bitmap.createScaledBitmap(photo, maskWidth,
							maskHeight, true);
				} else {
					b_default = BitmapFactory.decodeResource(
							con.getResources(),
							R.drawable.pic_touxiang_admin);
					b_zoom = Bitmap.createScaledBitmap(b_default, maskWidth,
							maskHeight, true);
				}
				// MyLog.v(TAG, "dealShape()::maskWidth=="+maskWidth
				// +"; maskHeight=="+maskHeight);
				bitmapWithReflection = Bitmap.createBitmap(maskWidth,
						maskHeight, Config.ARGB_8888);

				Canvas canvas = new Canvas(bitmapWithReflection);
				canvas.drawBitmap(b_mask, 0f, 0f, null);

				Paint paint = new Paint();
				paint.setXfermode(new PorterDuffXfermode(Mode.SRC_IN));
				// paint.setAntiAlias(true);//抗锯齿

				canvas.drawBitmap(b_zoom, 0f, 0f, paint);
				canvas.drawBitmap(b_bg, 0f, 0f, null);
				canvas.save();
				break;
			} catch (OutOfMemoryError error) {
				if (b_zoom != null) {
					b_zoom.recycle();
				}
				if (b_default != null) {
					b_default.recycle();
				}
				if (bitmapWithReflection != null) {
					bitmapWithReflection.recycle();
				}
				if (b_mask != null) {
					b_mask.recycle();
				}
				if (b_bg != null) {
					b_bg.recycle();
				}
				error.printStackTrace();
			}
		}

		return bitmapWithReflection;
	}

	/**
	 * 截取图片的中间部分
	 * 
	 * @param con
	 *            环境变量
	 * @param photo
	 *            要截取的图
	 * @param referenceResource
	 *            参照图的资源id
	 * @return 截完后的图
	 */
	public static Bitmap cutTheMiddleOfBitmap(Context con, Bitmap photo,
			int referenceResource) {
		Bitmap cutBitmap = null;
		try {
			Bitmap referenceBitmap = BitmapFactory.decodeResource(
					con.getResources(), referenceResource);
			int width = referenceBitmap.getWidth(); // 参照图宽度
			int height = referenceBitmap.getHeight(); // 参照图高度
			int photoWidth = photo.getWidth(); // 要截的图原宽度（目标图）
			int photoHeight = photo.getHeight(); // 要截的图原高度
			float scaleWidth = ((float) width) / photoWidth; // 目标图与参照图宽度的比例

			Matrix matrix = new Matrix();
			matrix.postScale(scaleWidth, scaleWidth);
			// 将目标图按比例缩放原图直至等于某边参照图的宽高显示。
			Bitmap resizedBitmap = Bitmap.createBitmap(photo, 0, 0, photoWidth,
					photoHeight, matrix, true);

			// 取图形左上角坐标
			int retX = width > height ? 0 : (height - width) / 2;
			int retY = width > height ? (width - height) / 2 : 0;
			// 截图
			cutBitmap = Bitmap.createBitmap(resizedBitmap, retX, retY, width,
					height, null, false);

		} catch (Exception e) {
			ShowLog.showException(e);
		} catch (OutOfMemoryError error) {
			if (cutBitmap != null) {
				cutBitmap.recycle();
			}
			error.printStackTrace();
		}

		return cutBitmap;
	}
		
	//显示礼物图片
	public static void showGiftImage(Context con,Handler uiHandler,String image,int msg){
		List<String> list = new ArrayList<String>();
		list.add(image);
		showImage(con,uiHandler,list,msg,false,AppStatus.s_appDirUtil.getGiftDir());
	}
	public static void showGiftImage(Context con,Handler uiHandler,List<String> imgs,int msgWhat){
		showImage(con,uiHandler,imgs,msgWhat,false,AppStatus.s_appDirUtil.getGiftDir());
	}
	/** 显示头像 */
	public static void showPhotoImage(Context con,Handler uiHandler,String img,int msgWhat){
		List<String> images = new ArrayList<String>();
		images.add(img);
		showPhotoImage(con,uiHandler,images,msgWhat);
	}
	public static void showPhotoImage(Context con, Handler uiHandler,List<String> images, int msgWhat) {
		showImage(con, uiHandler,images, msgWhat, false, AppStatus.s_appDirUtil.getHeadDir());
	}

	/** 显示背景图 */
	public static void showCashImage(Context con, Handler uiHandler,
			List<String> images, int msgWhat, boolean needSleep) {
		showImage(con, uiHandler,images, msgWhat, needSleep, AppStatus.s_appDirUtil.getSingerDir());
	}
	
	/** 显示背景图 */
	public static void showCashImage(Context con, Handler uiHandler,
			List<String> images, int msgWhat, boolean needSleep, int every) {
		showImage(con, uiHandler,images, msgWhat, needSleep, AppStatus.s_appDirUtil.getSingerDir(), every);
	}
	
	/** 显示横幅图 */
	public static void showBannerImage(Context con, Handler uiHandler,
			List<String> images, int msgWhat, boolean needSleep) {
		showImage(con, uiHandler,images, msgWhat, needSleep, AppStatus.s_appDirUtil.getBannerDir());
	}
	
	/** 显示横幅图 */
	public static void showBannerImage(Context con, Handler uiHandler,
			List<String> images, int msgWhat, boolean needSleep, int every) {
		showImage(con, uiHandler,images, msgWhat, needSleep, AppStatus.s_appDirUtil.getBannerDir(), every);
	}
	
	/** 显示图片 */
	private static void showImage(Context con, Handler uiHandler,
			List<String> images, int msgWhat, boolean needSleep,String pathtype) {
		showImage(con, uiHandler,images, msgWhat, needSleep, pathtype,3);
	}
	
	/** 显示图片 */
	private static void showImage(Context con, Handler uiHandler,List<String> images
			, int msgWhat, boolean needSleep,String pathtype, int every) {
		if (images.isEmpty())
			return;

		try {
			Set<String> setList = new HashSet<String>();
			setList.addAll(images);

			String path = pathtype;

			ImagesDownloadTask task = new ImagesDownloadTask(uiHandler, setList,
					path, msgWhat, needSleep, every);
			task.execute();

		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	/**
	 * 截屏
	 * 
	 * @param v
	 *            视图
	 * @param filePath
	 *            保存路径
	 */
	public static String getScreenHot(View v, String filePath) {
		try {
			Bitmap bitmap = Bitmap.createBitmap(v.getWidth(), v.getHeight(),
					Config.ARGB_8888);
			Canvas canvas = new Canvas();
			canvas.setBitmap(bitmap);
			v.draw(canvas);
			try {
				FileOutputStream fos = new FileOutputStream(filePath);
				bitmap.compress(CompressFormat.PNG, 100, fos);
				return filePath;
			} catch (FileNotFoundException e) {
				e.printStackTrace();

			}
			return null;
		} catch (Exception e) {
			e.printStackTrace();
		} catch (OutOfMemoryError e) {
			e.printStackTrace();
		}
		return null;
	}

	public static Bitmap drawableToBitmap(Drawable drawable) {
		Bitmap bitmap = Bitmap
				.createBitmap(
						drawable.getIntrinsicWidth(),
						drawable.getIntrinsicHeight(),
						drawable.getOpacity() != PixelFormat.OPAQUE ? Bitmap.Config.ARGB_8888
								: Bitmap.Config.RGB_565);
		Canvas canvas = new Canvas(bitmap);
		drawable.setBounds(0, 0, drawable.getIntrinsicWidth(),
				drawable.getIntrinsicHeight());
		drawable.draw(canvas);
		return bitmap;
	}

	public static byte[] bitmap2Bytes(Bitmap bm) {
		ByteArrayOutputStream baos = new ByteArrayOutputStream();
		bm.compress(Bitmap.CompressFormat.JPEG, 10, baos);
		byte[] result = baos.toByteArray();
		try {
			baos.close();
		} catch (Exception e) {
			e.printStackTrace();
		}
		return result;
	}

	public static byte[] bmpToByteArray(final Bitmap bmp,
			final boolean needRecycle) {
		ByteArrayOutputStream output = new ByteArrayOutputStream();
		bmp.compress(CompressFormat.PNG, 100, output);
		if (needRecycle) {
			bmp.recycle();
		}

		byte[] result = output.toByteArray();
		try {
			output.close();
		} catch (Exception e) {
			e.printStackTrace();
		}

		return result;
	}

	public static Bitmap bytes2Bimap(byte[] b) {
		if (b.length != 0) {
			return BitmapFactory.decodeByteArray(b, 0, b.length);
		} else {
			return null;
		}
	}

	/**
	 * 获得圆角图片的方法
	 * @param bitmap
	 * @param roundPx
	 * @return
	 */
	public static Bitmap getRoundedCornerBitmap(Bitmap bitmap, float roundPx) {
		Bitmap output = Bitmap.createBitmap(bitmap.getWidth(),
				bitmap.getHeight(), Config.ARGB_8888);
		Canvas canvas = new Canvas(output);

		final int color = 0xff424242;
		final Paint paint = new Paint();
		final Rect rect = new Rect(0, 0, bitmap.getWidth(), bitmap.getHeight());
		final RectF rectF = new RectF(rect);

		paint.setAntiAlias(true);
		canvas.drawARGB(0, 0, 0, 0);
		paint.setColor(color);
		canvas.drawRoundRect(rectF, roundPx, roundPx, paint);

		paint.setXfermode(new PorterDuffXfermode(Mode.SRC_IN));
		canvas.drawBitmap(bitmap, rect, rect, paint);

		return output;
	}

	/** 
     * Drawable 转 bitmap 
     * @param drawable 
     * @return 
     */  
    public static Bitmap drawable2Bitmap(Drawable drawable){  
        if(drawable instanceof BitmapDrawable){  
            return ((BitmapDrawable)drawable).getBitmap() ;  
        }else if(drawable instanceof NinePatchDrawable){  
            Bitmap bitmap = Bitmap  
                    .createBitmap(  
                            drawable.getIntrinsicWidth(),  
                            drawable.getIntrinsicHeight(),  
                            drawable.getOpacity() != PixelFormat.OPAQUE ? Bitmap.Config.ARGB_8888  
                                    : Bitmap.Config.RGB_565);  
            Canvas canvas = new Canvas(bitmap);  
            drawable.setBounds(0, 0, drawable.getIntrinsicWidth(),  
                    drawable.getIntrinsicHeight());  
            drawable.draw(canvas);  
            return bitmap;  
        }else{  
            return null ;  
        }  
    }  

    /** 
     * 获得圆形的图像
     * @param drawable 
     * @return 
     */  
    public static Bitmap toRoundImage(Bitmap bitmap, float pixels) {
    	Bitmap output = Bitmap.createBitmap(bitmap.getWidth(),
    			bitmap.getHeight(), Config.ARGB_8888);
    	Canvas canvas = new Canvas(output);
    	
    	final int color = 0xff424242;
    	final Paint paint = new Paint();
    	final Rect rect = new Rect(0, 0, bitmap.getWidth(), bitmap.getHeight());
    	final RectF rectF = new RectF(rect);
    	final float roundPx = pixels;
    	
    	paint.setAntiAlias(true);
    	canvas.drawARGB(0, 0, 0, 0);
    	
    	paint.setColor(color);
    	canvas.drawRoundRect(rectF, roundPx, roundPx, paint);
    	paint.setXfermode(new PorterDuffXfermode(Mode.SRC_IN));
    	canvas.drawBitmap(bitmap, rect,rect, paint);
    	
    	return output;
    }
}
