package assistant.pager.fragment;

import tiange.ktv.assistant.R;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import assistant.global.AppStatus;
import assistant.manager.HttpPlayControlManager;
//播放控制第二页 - adapter
public class PlayControlExFragment extends Fragment implements View.OnClickListener {

	Button m_btnToneSub;
	Button m_btnToneAdd;
	Button m_btnToneOrigin;
	Button m_btnEffectQuick;
	Button m_btnEffectTop;
	Button m_btnEffectHero;
	Button m_btnEffectK;
	
	HttpPlayControlManager m_httpControlMgr;
	
	public PlayControlExFragment() {
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
		View root = inflater.inflate(R.layout.fragment_playc_ex, null);
		
		m_btnToneSub = (Button) root.findViewById(R.id.btn_playc_tone_sub);
		m_btnToneAdd = (Button) root.findViewById(R.id.btn_playc_tone_add);
		m_btnToneOrigin = (Button) root.findViewById(R.id.btn_playc_tone_origin);
		m_btnEffectQuick = (Button) root.findViewById(R.id.btn_playc_effect_quick);
		m_btnEffectTop = (Button) root.findViewById(R.id.btn_playc_effect_top);
		m_btnEffectHero = (Button) root.findViewById(R.id.btn_playc_effect_hero);
		m_btnEffectK = (Button) root.findViewById(R.id.btn_playc_effect_k);
		
		m_btnToneSub.setOnClickListener(this);
		m_btnToneAdd.setOnClickListener(this);
		m_btnToneOrigin.setOnClickListener(this);
		m_btnEffectQuick.setOnClickListener(this);
		m_btnEffectTop.setOnClickListener(this);
		m_btnEffectHero.setOnClickListener(this);
		m_btnEffectK.setOnClickListener(this);
		
		return root;
	}
	
	@Override
	public void onClick(View arg0) {
		// TODO Auto-generated method stub
		switch(arg0.getId())
		{
		case R.id.btn_playc_tone_sub:
			m_httpControlMgr.subTone();
			break;
		case R.id.btn_playc_tone_add:
			m_httpControlMgr.addTone();
			break;
		case R.id.btn_playc_tone_origin:
			m_httpControlMgr.originTone();
			break;
		case R.id.btn_playc_effect_quick:
			m_httpControlMgr.setEffect(0);
			break;
		case R.id.btn_playc_effect_top:
			m_httpControlMgr.setEffect(1);
			break;
		case R.id.btn_playc_effect_hero:
			m_httpControlMgr.setEffect(2);
			break;
		case R.id.btn_playc_effect_k:
			m_httpControlMgr.setEffect(3);
			break;
		}
	}

}
