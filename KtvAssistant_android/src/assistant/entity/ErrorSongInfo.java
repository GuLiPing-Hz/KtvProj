package assistant.entity;
/**
 * @author 宋静
 * @version 创建时间：2013-11-14 上午11:09:10
 * 类说明 错误歌曲返回定义
 */
public class ErrorSongInfo {
	public String errorMessage;
	public int songid;
	
	public ErrorSongInfo(String errorMessage, int songid) {
		super();
		this.errorMessage = errorMessage;
		this.songid = songid;
	}

	
	@Override
	public String toString() {
		return "ErrorSongInfo [errorMessage=" + errorMessage + ", songid=" + songid+ "]";
	}
}
