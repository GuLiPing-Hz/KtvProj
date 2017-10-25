package assistant.fragment;

import android.view.KeyEvent;

/**
 * @author 宋静
 * @version 创建时间：2013-12-11 下午10:41:00
 * 类说明 为了方便Fragment调用Activity的方法
 */
public interface BaseFragmentHelper {

	public boolean onKeyDown(int keyCode, KeyEvent event);
}
