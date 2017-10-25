package assistant.fragment;

import tiange.ktv.assistant.R;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.text.Editable;
import android.text.TextUtils;
import android.text.TextWatcher;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.animation.Animation;
import android.view.animation.AnimationUtils;
import android.widget.Button;
import android.widget.EditText;
import android.widget.RelativeLayout;
import android.widget.TextView;
import assistant.global.AppStatus;
import assistant.manager.NetUtilManager.INetUtilListener;
import assistant.util.FaceUtil;
import assistant.util.ShowUtil;
import assistant.util.ToolUtil;

import com.jni.netutil.JniMessage;
import com.jni.netutil.XGMessage;

public class HongBaoSendFragment extends BaseFragment implements INetUtilListener{
    
	public static final String EXTRA_HONGBAO_TYPE = "assistant.fragment.MoneyBagRedPacketFragment.type";
    public static final String EXTRA_HONGBAO_IDX = "assistant.fragment.MoneyBagRedPacketFragment.idx";
	//三类红包
	public static final int HONGBAO_PERSON = 0;//个人
    public static final int HONGBAO_ROOM = 1;//房间手气
    public static final int HONGBAO_LOBBY = 3;//大厅手气
	int mHongBaoType = 0;
	int mSendIdxOrRoom = 0;
    
	View     m_viewBack;                  //后退 按钮
    TextView m_textViewTitle;             //红包的标题“大款红包”
	RelativeLayout m_titeLayout;          //整个“大款红包”的布局
	
	View mUiNum;
	EditText m_editTextRedPacketNum;      //编辑红包个数
	EditText m_editTextRedPacketCoinNum;  //金币总值编辑框
	TextView mUiGoldMiss;
	EditText m_editTextRedPacketMsg;  //想对接收者说的话
	
	Button   m_BtnSend;                   //发送按钮
	TextView mHBTips;
	TextView mHBErrorTips;
	
	Animation mAppear;
	Animation mDisappear;
	
	boolean mIsEnableNum = false;
	
	Handler mHandler = new Handler();
	
	private HongBaoSendFragment(){
		super();
	}
	
	@Override
	public void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		
		mAppear = AnimationUtils.loadAnimation(getActivity(), R.anim.appear);
		mAppear.setFillEnabled(true);
		mAppear.setFillAfter(true);
		mDisappear = AnimationUtils.loadAnimation(getActivity(), R.anim.disappear);
		mDisappear.setFillEnabled(true);
		mDisappear.setFillAfter(true);
		
		mHongBaoType = getArguments().getInt(EXTRA_HONGBAO_TYPE, HONGBAO_LOBBY);
		mSendIdxOrRoom = getArguments().getInt(EXTRA_HONGBAO_IDX,0);
	}
	
	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup container,Bundle savedInstanceState)
	{	
		View root = inflater.inflate(R.layout.fragment_send_moneybag_redpacket, null);
		
		//后退按钮
		m_viewBack = root.findViewById(R.id.iv_back);
		mUiNum = root.findViewById(R.id.rl_hb_number);
		m_textViewTitle = (TextView) root.findViewById(R.id.tv_title);
		mUiGoldMiss = (TextView) root.findViewById(R.id.tv_hb_system_tip);
		m_editTextRedPacketNum = (EditText) root.findViewById(R.id.MoneyBag_Redpacket_Num_TextEdit);
		m_BtnSend = (Button)root.findViewById(R.id.moneybag_redpacket_send);
		m_editTextRedPacketCoinNum = (EditText)root.findViewById(R.id.MoneyBag_Redpacket_Coin_Num);
		m_editTextRedPacketMsg = (EditText) root.findViewById(R.id.moneybag_redpacket_message_edittext);
		mHBTips = (TextView) root.findViewById(R.id.tv_hongbao_tips);
		mHBErrorTips = (TextView) root.findViewById(R.id.tv_error_tip);
		
		if(mHongBaoType == HONGBAO_LOBBY)
		{
			m_textViewTitle.setText(R.string.moneybag_redpacket_title);
			m_editTextRedPacketNum.addTextChangedListener(textwatcher);
		}
		else if(mHongBaoType == HONGBAO_ROOM)
		{
			m_textViewTitle.setText(R.string.room_redpacket_title);
			mHBTips.setText(this.getResources().getString(R.string.room_redpacket_tips));
			m_editTextRedPacketNum.addTextChangedListener(textwatcher);
		}
		else//HONGBAO_PERSON
		{
			m_textViewTitle.setText(R.string.personal_redpacket_title);
			mHBTips.setText(this.getResources().getString(R.string.personal_redpacket_tips));
			mUiNum.setVisibility(View.GONE);
			mIsEnableNum = true;
		}
		
		mUiGoldMiss.setText(getResources().getString(R.string.moneybag_redpacket_Consume_Coin)+0);
		//回退
		m_viewBack.setOnClickListener(listener);
		//发送
		m_BtnSend.setOnClickListener(listener);
		m_BtnSend.setEnabled(false);
		
		m_editTextRedPacketMsg.addTextChangedListener(new TextWatcher(){

			@Override
			public void afterTextChanged(Editable arg0) {
				// TODO Auto-generated method stub
				
			}

			@Override
			public void beforeTextChanged(CharSequence s, int start, int count,
					int after) {
				// TODO Auto-generated method stub
				
			}

			@Override
			public void onTextChanged(CharSequence s, int start, int before,
					int count) {
				// TODO Auto-generated method stub
				if (before == 0)// 说明是字符增加，大于0说明是减少
				{// 字符增加的时候判断下
					CharSequence seq = s.subSequence(start, start + count);

					if (FaceUtil.containsEmoji(seq.toString())) {
						m_editTextRedPacketMsg.getText().delete(start, start + count);
						m_editTextRedPacketMsg.setSelection(start);
						ShowUtil.showToast(getActivity(), R.string.sorry_emoji);
					}
				}
			}
			
		});
		//金币监听
		m_editTextRedPacketCoinNum.addTextChangedListener(new TextWatcher(){

			@Override
			public void afterTextChanged(Editable s) {
				// TODO Auto-generated method stub
				String number = s.toString();
				int miss = Integer.parseInt(TextUtils.isEmpty(number)?"0":number)/10;
				mUiGoldMiss.setText(getResources().getString(
						R.string.moneybag_redpacket_Consume_Coin)+miss);
				changeSendBtnState();
			}

			@Override
			public void beforeTextChanged(CharSequence s, int start, int count,
					int after) {
				// TODO Auto-generated method stub
				
			}

			@Override
			public void onTextChanged(CharSequence s, int start, int before,
					int count) {
				// TODO Auto-generated method stub
				
			}
			
		});
        
		AppStatus.s_NetUtilMgr.registerListener(this);
		
		return root;
	}
	
	@Override
	public void onDestroyView() {
		// TODO Auto-generated method stub
		AppStatus.s_NetUtilMgr.unregisterListener(this);
		
		super.onDestroyView();
	}

	View.OnClickListener listener = new View.OnClickListener()
	{
		@Override
		public void onClick(View v) {
			// TODO Auto-generated method stub
			switch (v.getId()) {
			case R.id.iv_back:
			{
				ToolUtil.closeKeyBoard(getActivity());
				getActivity().finish();
				break;
			}
			case R.id.moneybag_redpacket_send:
			{
				
				if(mHongBaoType == HONGBAO_ROOM && !AppStatus.isInRoom)
				{
					ShowUtil.showToast(getActivity(), R.string.should_bind_room);
					break;
				}
				
				int gold = Integer.parseInt(m_editTextRedPacketCoinNum.getEditableText().toString());
				
				if(gold < 1000 && mHongBaoType == HONGBAO_LOBBY)
				{
					mHBErrorTips.setText("大款红包金币需要大于或等于1000");
					mHBErrorTips.setVisibility(View.VISIBLE);
					mHBErrorTips.startAnimation(mAppear);
					
					mHandler.postDelayed(new Runnable(){

						@Override
						public void run() {
							// TODO Auto-generated method stub
							mHBErrorTips.startAnimation(mDisappear);
						}
						
					}, 3000);
					break;
				}
				
				if(gold < 120 && mHongBaoType == HONGBAO_ROOM)
				{
					mHBErrorTips.setText("包厢红包金币需要大于或等于120");
					mHBErrorTips.setVisibility(View.VISIBLE);
					mHBErrorTips.startAnimation(mAppear);
					
					mHandler.postDelayed(new Runnable(){

						@Override
						public void run() {
							// TODO Auto-generated method stub
							mHBErrorTips.startAnimation(mDisappear);
						}
						
					}, 3000);
					break;
				}
				
				int number = (mHongBaoType== HONGBAO_PERSON)? 1
						: Integer.parseInt(m_editTextRedPacketNum.getEditableText().toString());
				String mark = m_editTextRedPacketMsg.getEditableText().toString();
				if(TextUtils.isEmpty(mark))
					mark = m_editTextRedPacketMsg.getHint().toString();
				
				//Log.i("HongBao",mark);
				AppStatus.s_NetUtilMgr.m_JniUtil.RequestSendHB((byte)mHongBaoType
						, number, gold, mark, mSendIdxOrRoom);
				ToolUtil.closeKeyBoard(getActivity());
				break;
			}
			default:
				break;
			}
		}
	};
	
	void changeSendBtnState(){
		
		boolean isEnable = true;
		
		if(mHongBaoType != HONGBAO_PERSON)//群发红包
		{
			String strNum=m_editTextRedPacketNum.getText().toString();
			if(TextUtils.isEmpty(strNum))
			{
				isEnable = false;
			}
		}
		
		if(isEnable)
		{
			String strCoin=m_editTextRedPacketCoinNum.getText().toString();
			if(TextUtils.isEmpty(strCoin) || !mIsEnableNum)
			{
				isEnable = false;
			}
		}
		
		m_BtnSend.setEnabled(isEnable);
	}
	
	//红包个数监听
	TextWatcher textwatcher = new TextWatcher()
	{
		@Override
		public void onTextChanged(CharSequence s, int start, int before, int count)
		{
			// TODO Auto-generated method stub
		}
		
		@Override
		public void beforeTextChanged(CharSequence s, int start, int count,int after)
		{
			// TODO Auto-generated method stub
		}
		
		@Override
		public void afterTextChanged(Editable s)
		{
			String strNum=m_editTextRedPacketNum.getText().toString();
			
			if(!TextUtils.isEmpty(strNum) && Integer.parseInt(strNum) > 100)
			{
				mHBErrorTips.setText("一次最多发100个红包");
				mHBErrorTips.setVisibility(View.VISIBLE);
				mHBErrorTips.startAnimation(mAppear);
				mIsEnableNum = false;
			}
			else
			{
				if(!mIsEnableNum)
				{
					mHBErrorTips.startAnimation(mDisappear);
					mIsEnableNum = true;
				}
				//mHBErrorTips.setVisibility(View.GONE);
			}
			
			changeSendBtnState();
		}
	};
	
	public static HongBaoSendFragment newInstance(int type,int idxorroom){
		if(type < HONGBAO_PERSON || type > HONGBAO_LOBBY)
			return null;
		
		HongBaoSendFragment fragment = new HongBaoSendFragment();
		Bundle data = new Bundle();
		data.putInt(EXTRA_HONGBAO_TYPE, type);
		data.putInt(EXTRA_HONGBAO_IDX, idxorroom);
		fragment.setArguments(data);
		return fragment;
	}

	@Override
	public void dealWithMsg(Message msg) {
		// TODO Auto-generated method stub
		switch(msg.what)
		{
			case XGMessage.XGMSG_HONGBAO_SENDRESULT:
			{
				switch(msg.arg1)
				{
				case 0://发送红包成功
					ShowUtil.showToast(getActivity(), R.string.hongbao_send_ok);
					getActivity().finish();
					break;
				case 1://金币不足
					ShowUtil.showToast(getActivity(), R.string.gold_not_enough);
					break;
				case 2://对方不在线
					ShowUtil.showToast(getActivity(), R.string.hongbao_offline);
					break;
				case 3://不在包厢
					ShowUtil.showToast(getActivity(), R.string.hongbao_noin_room);
					break;
				case JniMessage.OPTION_TIMEOUT://超时
					ShowUtil.showToast(getActivity(), R.string.hongbao_timeout);
					break;
				}
				break;
			}
		}
	}
}
