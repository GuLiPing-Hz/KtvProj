package assistant.entity;

import org.json.JSONObject;

public class GradeSongTopInfo {
	public int index; //某榜单
	public SongInfo songinfo; //歌曲信息
	//public UserInfo user;		//演唱者信息
	public int score;				//演唱的分数
	
	public GradeSongTopInfo(JSONObject jsonObject) {
		super();
		
		try
		{
			if(jsonObject != null)
			{
				index = jsonObject.optInt("index");
				songinfo = new SongInfo(jsonObject.optJSONObject("song"));
				//user = new UserInfo(jsonObject.optJSONObject("user"));
				score = jsonObject.optInt("score");
			}
		}
		catch(Exception e)
		{
			e.printStackTrace();
		}
	}
	
	
}
