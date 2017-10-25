package assistant.util;

import android.content.Context;
import android.widget.Toast;

/**
 * @author 宋静
 * @version 创建时间：2013-10-21 下午4:06:02
 * 类说明 显示提示工具类，包括Toast和Dialog显示
 */
public class ShowUtil {
	
	public synchronized static void showToast(Context con, String content){
		Toast.makeText(con, content, Toast.LENGTH_SHORT).show();
	}
	
	public synchronized static void showToast(Context con, int resource){
		Toast.makeText(con, resource, Toast.LENGTH_SHORT).show();
	}
	
	public synchronized static void showLongToast(Context con, String content){
		Toast.makeText(con, content, Toast.LENGTH_LONG).show();
	}
	
	public synchronized static void showLongToast(Context con, int resource){
		Toast.makeText(con, resource, Toast.LENGTH_LONG).show();
	}
}
