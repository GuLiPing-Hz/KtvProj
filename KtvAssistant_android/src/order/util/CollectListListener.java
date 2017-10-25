package order.util;

import java.util.List;

import assistant.entity.SongInfo;

/**
 * @author 赵龙权
 * @version 创建时间：2013-12-18 下午9:50:24
 * 类说明
 */

public interface CollectListListener {
	void updateCollectList(List<SongInfo> list);

	void getCollectListFail(String errorMessage);
}
