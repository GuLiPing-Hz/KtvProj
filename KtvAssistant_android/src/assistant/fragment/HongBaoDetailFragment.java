package assistant.fragment;

import java.util.Arrays;
import java.util.Comparator;

import tiange.ktv.assistant.R;
import android.graphics.Bitmap;
import android.os.Bundle;
import android.text.TextUtils;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ListView;
import android.widget.TextView;
import assistant.fragment.adapter.HongBaoDetailAdapter;
import assistant.fragment.home.ChatFragment;
import assistant.global.AppStatus;
import assistant.util.ImageUtil;
import assistant.view.RoundedImageView;

import com.jni.netutil.ResultData_HBDetail;
import com.jni.netutil.out.HongBaoDetailInfoToUi;

public class HongBaoDetailFragment extends BaseFragment {

	public static final String ARGUMENT_ISCATCHED = "assistant.fragment.HongBaoDetailFragment";
	
	TextView mUiTitle;
	RoundedImageView mUiHead;
	TextView mUiFrom;
	TextView mUiMark;
	View mUiGoldInfo;
	TextView mUiGold;
	
	TextView mUiTips;
	ListView mUiPeople;
	HongBaoDetailAdapter mAdapter;
	
	HongBaoDetailInfoToUi mHBDetailInfo;
	
	boolean mIsCatched = false;
	int mSelfGold = 0;
	
	@Override
	public void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		
		mHBDetailInfo = ChatFragment.sHongBaoDetail;
		
		//做一个排序
		Arrays.sort(mHBDetailInfo.list, new Comparator<ResultData_HBDetail>(){

			@Override
			public int compare(ResultData_HBDetail lhs, ResultData_HBDetail rhs) {
				// TODO Auto-generated method stub
				
				return rhs.time - lhs.time;
			}
			
		});
		
		for(ResultData_HBDetail detail : mHBDetailInfo.list)
		{
			if(detail != null && detail.idx == AppStatus.userIdx)
			{
				mIsCatched = true;
				mSelfGold = detail.gold;
			}
		}
	}

	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup container,
			Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		View root = inflater.inflate(R.layout.fm_hb_detail, null);
		
		root.findViewById(R.id.iv_back).setOnClickListener(new View.OnClickListener() {
			
			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				getActivity().finish();
			}
		});
		
		mUiTitle = (TextView) root.findViewById(R.id.tv_title);
		mUiHead = (RoundedImageView) root.findViewById(R.id.iv_userhead);
		mUiFrom = (TextView) root.findViewById(R.id.tv_hb_from);
		mUiMark = (TextView) root.findViewById(R.id.tv_hb_mark);
		mUiGoldInfo = root.findViewById(R.id.ll_self_gold);
		mUiGold = (TextView) root.findViewById(R.id.tv_gold);
		mUiTips = (TextView) root.findViewById(R.id.tv_hb_message);
		mUiPeople = (ListView) root.findViewById(R.id.lv_hb_detail);
		
		mUiTitle.setText("红包领取详情");
		
		if(mHBDetailInfo.idx == 1)
		{
			//系统红包，显示系统红包专用头像
			mUiHead.setImageResource(R.drawable.ic_system_profile);
		}
		else
		{
			Bitmap bm = ImageUtil.getBitmapPhoto(mHBDetailInfo.photo);
			if(bm != null)
				mUiHead.setImageBitmap(bm);
		}
		
		mUiFrom.setText(mHBDetailInfo.name+getResources().getString(R.string.hongbao_from));
		mUiMark.setText(mHBDetailInfo.remark);
		
		if(mIsCatched)
			mUiGold.setText(""+mSelfGold);
		else
			mUiGoldInfo.setVisibility(View.GONE);
		
		if(mHBDetailInfo.hbtype == 0)//个人红包
		{
			if(mHBDetailInfo.list.length == mHBDetailInfo.totalnum)
				mUiTips.setText(getResources().getString(R.string.hongbao_is_catched));
			else
				mUiTips.setText(getResources().getString(R.string.hongbao_wait_for));
		}
		else
		{
			if(mHBDetailInfo.list.length == mHBDetailInfo.totalnum)
			{
				int seconds = 0;
				if(mHBDetailInfo.list.length != 0)
					seconds = mHBDetailInfo.list[0].time - mHBDetailInfo.sendtime;
				
				String fmt = String.format(getResources().getString(R.string.hongbao_fmt_finish)
						, mHBDetailInfo.list.length,getSuitableTime(seconds));
				mUiTips.setText(fmt);
			}
			else
			{
				int golds = 0;
				for(ResultData_HBDetail detail : mHBDetailInfo.list)
					golds += detail.gold;
				String fmt = String.format(getResources().getString(R.string.hongbao_fmt_unfinish)
						, mHBDetailInfo.list.length,mHBDetailInfo.totalnum,golds,mHBDetailInfo.totalgold);
				mUiTips.setText(fmt);
			}
		}
		
		mAdapter = new HongBaoDetailAdapter(getActivity(),mHBDetailInfo.list
				,mHBDetailInfo.list.length == mHBDetailInfo.totalnum && mHBDetailInfo.hbtype != 0);
		mUiPeople.setAdapter(mAdapter);
		
		return root;
	}

	String getSuitableTime(int seconds){
		String time = "";
		
		if(seconds > 86400)//24*3600
		{
			time += (""+(seconds/86400)+"天");
			seconds = seconds%86400;
		}
		
		if(seconds > 3600)
		{
			time += (""+(seconds/3600)+"小时");
			seconds = seconds%3600;
		}
		
		if(seconds > 60)
		{
			time += (""+(seconds/60)+"分");
			seconds = seconds%60;
		}
		
		if(seconds > 0 || TextUtils.isEmpty(time))
			time += (""+seconds+"秒");
		
		return time;
	}
}
