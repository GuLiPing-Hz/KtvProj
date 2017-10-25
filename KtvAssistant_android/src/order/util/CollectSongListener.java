package order.util;

import assistant.entity.ErrorSongInfo;

/**
 * @author 宋静
 * @version 创建时间：2013-12-17 下午10:01:29
 * 类说明 收藏歌曲或者删除收藏歌曲的返回监听
 */
public interface CollectSongListener {
	void collectSongSuccess();
	
	void collectSongFail(ErrorSongInfo error);
	
	void deleteCollectSuccess(int songId);
	
	void deleteCollectFail(ErrorSongInfo error);
}
