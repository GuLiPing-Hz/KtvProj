package order.util;

import assistant.entity.SongInfo;

/**
 * @author 宋静
 * @version 创建时间：2013-12-20 下午1:45:38
 * 类说明 切歌的监听器
 */
public interface PlayListener {
	void cutSongSuccess(SongInfo song);
	void cutSongFail(String errorMsg);
}
