package assistant.manager;

import tiange.ktv.assistant.R;
import android.content.Context;
import android.os.Handler;
import assistant.task.playcontrol.AtmosphereTask;
import assistant.task.playcontrol.SwitchSongTask;
import assistant.task.playcontrol.MuteOOTask;
import assistant.task.playcontrol.PauseOOTask;
import assistant.task.playcontrol.ResingTask;
import assistant.task.playcontrol.ScoreStatTask;
import assistant.task.playcontrol.SingModeTask;
import assistant.task.playcontrol.VoicePadTask;
import assistant.util.ShowUtil;

public class HttpPlayControlManager {
	Handler m_handler;
	long m_switchTime = 0;
	Context m_context;
	public HttpPlayControlManager(Context context,Handler handler){
		m_context = context;
		m_handler = handler;
	}
	
	private void setVoicePad(int music,int mic,int tone,int effect){
		VoicePadTask voicePadTask = new VoicePadTask(m_handler);
		voicePadTask.setArgu(music, mic, tone, effect);
		voicePadTask.execute();
	}
	public void addMusic(){
		setVoicePad(1,0,0,-1);
	}
	public void subMusic(){
		setVoicePad(-1,0,0,-1);
	}
	public void addMic(){
		setVoicePad(0,1,0,-1);
	}
	public void subMic(){
		setVoicePad(0,-1,0,-1);
	}
	public void addTone(){
		setVoicePad(0,0,1,-1);
	}
	public void subTone(){
		setVoicePad(0,0,-1,-1);
	}
	public void originTone(){
		setVoicePad(0,0,2,-1);
	}
	/*
	 * 0：快歌，1：顶尖，2：唱将，3：K歌
	 * */
	public void setEffect(int effect){
		if(effect<0 || effect>3)
			return ;
		setVoicePad(0,0,0,effect);
	}
	public void switchPause(){
		PauseOOTask pauseOOTask = new PauseOOTask(m_handler);
		pauseOOTask.execute();
	}
	public void setMute(){
		MuteOOTask muteOOTask = new MuteOOTask(m_handler);
		muteOOTask.execute();
	}
	public void switchSingMode(){
		SingModeTask singModeTask = new SingModeTask(m_handler);
		singModeTask.execute();
	}
	public void switchScoreStat(){
		ScoreStatTask scoreStatTask = new ScoreStatTask(m_handler);
		scoreStatTask.execute();
	}
	/*
	 * 1：喝彩，2鼓掌
	 * */
	public void setAtmosphere(int atmosphere){
		if(atmosphere!=1 && atmosphere!=2)
			return ;
		AtmosphereTask atmosphereTask = new AtmosphereTask(m_handler);
		atmosphereTask.setArgu(atmosphere);
		atmosphereTask.execute();
	}
	public void setResing(){
		ResingTask resingTask = new ResingTask(m_handler);
		resingTask.execute();
	}
	public void switchSong(int musicid){
		long time = System.currentTimeMillis();
		if (time - m_switchTime > 2000L)
		{
			m_switchTime = time;
			SwitchSongTask task = new SwitchSongTask(m_handler,musicid);
			task.execute();
		}
		else
			ShowUtil.showToast(m_context, R.string.playc_notify);
	}
}
