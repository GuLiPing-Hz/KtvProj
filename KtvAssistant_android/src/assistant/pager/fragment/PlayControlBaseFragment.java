package assistant.pager.fragment;


import java.util.List;

import com.jni.netutil.ResultData_SongList;

import tiange.ktv.assistant.R;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import assistant.global.AppStatus;
import assistant.manager.HttpPlayControlManager;
//播放控制第一页 - adapter
public class PlayControlBaseFragment extends Fragment implements
	View.OnClickListener {

	Button m_btnSingMode;
	Button m_btnScoreStat;
	Button m_btnPauseOO;
	Button m_btnResing;
	Button m_btnMusicSub;
	Button m_btnMusicAdd;
	Button m_btnMicSub;
	Button m_btnMicAdd;
	Button m_btnMusicMute;
	Button m_btnSwitchSong;
	
	HttpPlayControlManager m_httpControlMgr;
	
	public PlayControlBaseFragment() {
		// TODO Auto-generated constructor stub
	}

	@Override
	public void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		m_httpControlMgr = AppStatus.s_playControMgr;
	}

	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup container,
			Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		View root = inflater.inflate(R.layout.fragment_playcontrol_base, null);
		
		m_btnSingMode = (Button) root.findViewById(R.id.btnSingMode);
		m_btnScoreStat = (Button) root.findViewById(R.id.btnScoreStat);
		m_btnPauseOO = (Button) root.findViewById(R.id.btnPauseOO);
		m_btnResing = (Button) root.findViewById(R.id.btnResing);
		m_btnMusicSub = (Button) root.findViewById(R.id.btn_playc_music_sub);
		m_btnMusicAdd = (Button) root.findViewById(R.id.btn_playc_music_add);
		m_btnMicSub = (Button) root.findViewById(R.id.btn_playc_mic_sub);
		m_btnMicAdd = (Button) root.findViewById(R.id.btn_playc_mic_add);
		m_btnMusicMute = (Button) root.findViewById(R.id.btn_playc_music_mute);
		m_btnSwitchSong = (Button) root.findViewById(R.id.btnSwitchSong);
		
		m_btnSingMode.setOnClickListener(this);
		m_btnScoreStat.setOnClickListener(this);
		m_btnPauseOO.setOnClickListener(this);
		m_btnResing.setOnClickListener(this);
		m_btnMusicSub.setOnClickListener(this);
		m_btnMusicAdd.setOnClickListener(this);
		m_btnMicSub.setOnClickListener(this);
		m_btnMicAdd.setOnClickListener(this);
		m_btnMusicMute.setOnClickListener(this);
		m_btnSwitchSong.setOnClickListener(this);
		
		return root;
	}

	@Override
	public void onClick(View arg0) {
		// TODO Auto-generated method stub
		switch(arg0.getId())
		{
		case R.id.btnSingMode:
			m_httpControlMgr.switchSingMode();
			break;
		case R.id.btnScoreStat:
			m_httpControlMgr.switchScoreStat();
			break;
		case R.id.btnPauseOO:
			m_httpControlMgr.switchPause();
			break;
		case R.id.btnResing:
			m_httpControlMgr.setResing();
			break;
		case R.id.btn_playc_music_sub:
			m_httpControlMgr.subMusic();
			break;
		case R.id.btn_playc_music_add:
			m_httpControlMgr.addMusic();
			break;
		case R.id.btn_playc_mic_sub:
			m_httpControlMgr.subMic();
			break;
		case R.id.btn_playc_mic_add:
			m_httpControlMgr.addMic();
			break;
		case R.id.btn_playc_music_mute:
			m_httpControlMgr.setMute();
			break;
		case R.id.btnSwitchSong://to be continue
			List<ResultData_SongList> list = AppStatus.s_NetUtilMgr.getSonglist();
			if(!list.isEmpty())
				m_httpControlMgr.switchSong(list.get(0).songid);
			break;
		}
	}

}
