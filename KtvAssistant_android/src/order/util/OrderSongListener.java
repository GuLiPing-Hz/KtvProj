package order.util;

import assistant.entity.ErrorSongInfo;
import assistant.entity.SongInfo;

/**
 * @author 宋静
 * @version 创建时间：2013-12-17 下午8:19:02
 * 类说明 点歌监听器（适用于adapter里面的点歌删歌等操作）
 */
public interface OrderSongListener {
	void orderSongSuccess(SongInfo song);
	
	void orderSongFail(ErrorSongInfo error);
	
	void deleteSongSuccess(SongInfo song);
	
	void deleteSongFail(ErrorSongInfo error);
	
	void topSongSuccess(SongInfo song);
	
	void topSongFail(ErrorSongInfo error);
}	
