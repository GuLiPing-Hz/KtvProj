package assistant.fragment.home;

import java.io.UnsupportedEncodingException;
import java.net.URLEncoder;

import tiange.ktv.assistant.R;
import android.annotation.SuppressLint;
import android.graphics.drawable.Drawable;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.text.TextUtils;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageButton;
import android.widget.TextView;
import assistant.activity.Home;
import assistant.fragment.BaseFragment;
import assistant.global.AppStatus;
import assistant.global.KtvAssistantAPIConfig;
import assistant.manager.NetUtilManager.INetUtilListener;
import assistant.util.ShowUtil;
import assistant.util.ToolUtil;
import assistant.view.RoundedImageView;

import com.alipay.AlipayKeys;
import com.alipay.AlipayResult;
import com.alipay.SignUtils;
import com.alipay.sdk.app.PayTask;
import com.jni.netutil.XGMessage;
//支付界面 - 购买金币
public class ChargeFragment extends BaseFragment implements View.OnClickListener
	,INetUtilListener{

	//RMB
	//测试 第一个以后需要改成10元
	final static float MONEY_TO_PAY[] = {10,20,50,100};
	//商品名称，商品介绍
	final static String GOLD_NUMBER[] = {"金币 7000","金币 14000","金币 35000","金币 70000"};
	//支付宝监听事件
	final static int SDK_PAY_FLAG = 1;
	final static int SDK_CHECK_FLAG = 2;
	
	//Home m_Home;
	
	ImageButton m_imgBtnBack;
	TextView m_textViewTitle;
	RoundedImageView m_rimgViewHead;
	TextView m_textViewName;
	TextView m_textViewGold;
	View m_viewGold1;
	View m_viewGoldSelected1;
	View m_viewGold2;
	View m_viewGoldSelected2;
	View m_viewGold3;
	View m_viewGoldSelected3;
	View m_viewGold4;
	View m_viewGoldSelected4;
	View m_viewPay;
	
	int m_curGoldIndex = -1;

	@Override
	public void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
			
		//m_Home = (Home) this.getActivity();
		//m_Home.hidePlayControl();
		//m_Home.showMiddle(false);
	}
	
	@Override
	public void onDestroy() {
		// TODO Auto-generated method stub
		super.onDestroy();
		//m_Home.showMiddle(true);
	}

	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup container,
			Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		View root = inflater.inflate(R.layout.fragment_charge, null);
		
		m_imgBtnBack = (ImageButton) root.findViewById(R.id.iv_back);
		m_textViewTitle = (TextView) root.findViewById(R.id.tv_title);
		m_rimgViewHead = (RoundedImageView) root.findViewById(R.id.rimgViewHead);
		m_textViewName = (TextView) root.findViewById(R.id.textViewName);
		m_textViewGold = (TextView) root.findViewById(R.id.textViewGold);
		m_viewGold1 = root.findViewById(R.id.llGold1);
		m_viewGoldSelected1 = root.findViewById(R.id.imgGold1);
		m_viewGold2 = root.findViewById(R.id.llGold2);
		m_viewGoldSelected2 = root.findViewById(R.id.imgGold2);
		m_viewGold3 = root.findViewById(R.id.llGold3);
		m_viewGoldSelected3 = root.findViewById(R.id.imgGold3);
		m_viewGold4 = root.findViewById(R.id.llGold4);
		m_viewGoldSelected4 = root.findViewById(R.id.imgGold4);
		m_viewPay = root.findViewById(R.id.btnPaySure);
		
		m_imgBtnBack.setOnClickListener(this);
		m_viewGold1.setOnClickListener(m_goldSelectListener);
		m_viewGold2.setOnClickListener(m_goldSelectListener);
		m_viewGold3.setOnClickListener(m_goldSelectListener);
		m_viewGold4.setOnClickListener(m_goldSelectListener);
		m_viewPay.setOnClickListener(this);
		
		//m_imgBtnBack.setImageResource(R.drawable.btn_home_menu);
		m_textViewTitle.setText(R.string.menuitem_buymoney);
		if(AppStatus.user != null)
		{
			m_textViewName.setText(AppStatus.user.nickname);
			if (Home.bitmap != null) {
				m_rimgViewHead.setImageBitmap(Home.bitmap);
				m_rimgViewHead.setBorder(2);
			}
			Drawable left = getResources().getDrawable(
					AppStatus.user.gender==1?R.drawable.icon_sex2_man:R.drawable.icon_sex2_woman);
			left.setBounds(0, 0, left.getMinimumWidth(), left.getMinimumHeight());
			m_textViewName.setCompoundDrawables(left, null, null, null);
			
			m_textViewGold.setText(""+AppStatus.user.gold);
		}
		
		AppStatus.s_NetUtilMgr.registerListener(this);
		return root;
	}
	
	@Override
	public void onDestroyView() {
		// TODO Auto-generated method stub
		AppStatus.s_NetUtilMgr.unregisterListener(this);
		super.onDestroyView();
	}

	void switchGoldSelect(int index){
		
		if(m_curGoldIndex == index)
			return ;
		
		switch(index)
		{
		case 0:
		{
			m_viewGoldSelected1.setVisibility(View.VISIBLE);
			m_viewGoldSelected2.setVisibility(View.GONE);
			m_viewGoldSelected3.setVisibility(View.GONE);
			m_viewGoldSelected4.setVisibility(View.GONE);
			break;
		}
		case 1:
		{
			m_viewGoldSelected1.setVisibility(View.GONE);
			m_viewGoldSelected2.setVisibility(View.VISIBLE);
			m_viewGoldSelected3.setVisibility(View.GONE);
			m_viewGoldSelected4.setVisibility(View.GONE);
			break;
		}
		case 2:
		{
			m_viewGoldSelected1.setVisibility(View.GONE);
			m_viewGoldSelected2.setVisibility(View.GONE);
			m_viewGoldSelected3.setVisibility(View.VISIBLE);
			m_viewGoldSelected4.setVisibility(View.GONE);
			break;
		}
		case 3:
		{
			m_viewGoldSelected1.setVisibility(View.GONE);
			m_viewGoldSelected2.setVisibility(View.GONE);
			m_viewGoldSelected3.setVisibility(View.GONE);
			m_viewGoldSelected4.setVisibility(View.VISIBLE);
			break;
		}
		}
		
		m_curGoldIndex = index;
	}
	
	View.OnClickListener m_goldSelectListener = new View.OnClickListener() {
		
		@Override
		public void onClick(View v) {
			// TODO Auto-generated method stub
			switch(v.getId())
			{
			case R.id.llGold1:
				switchGoldSelect(0);
				break;
			case R.id.llGold2:
				switchGoldSelect(1);
				break;
			case R.id.llGold3:
				switchGoldSelect(2);
				break;
			case R.id.llGold4:
				switchGoldSelect(3);
				break;
			}
		}
	};
	
	@Override
	public void onClick(View v) {
		// TODO Auto-generated method stub
		switch(v.getId())
		{
		case R.id.iv_back:
			//m_Home.showMenu();
			this.getActivity().finish();
			break;
		case R.id.btnPaySure:
			//检测是否选择金币数量
			if(m_curGoldIndex == -1)
			{
				ShowUtil.showToast(getActivity(), R.string.pay_noselect_tip);
				return ;
			}
			//调出支付宝支付界面
			payForProduct(m_curGoldIndex);
			break;
		}
	}

	@Override
	public void dealWithMsg(Message msg) {
		// TODO Auto-generated method stub
		switch(msg.what)
		{
		//更新用户金币信息
		case XGMessage.XGMSG_USERGOLD_UPDATE:
			m_textViewGold.setText(""+AppStatus.user.gold);
			break;
		}
	}
	
	void payForProduct(int index){
		String orderInfo = getOrderInfo(GOLD_NUMBER[index], GOLD_NUMBER[index], ""+MONEY_TO_PAY[index]);
		String sign = sign(orderInfo);
		try {
			// 仅需对sign 做URL编码
			sign = URLEncoder.encode(sign, "UTF-8");
		} catch (UnsupportedEncodingException e) {
			e.printStackTrace();
		}
		final String payInfo = orderInfo + "&sign=\"" + sign + "\"&"+ getSignType();

		Runnable payRunnable = new Runnable() {

			@Override
			public void run() {
				// 构造PayTask 对象
				PayTask alipay = new PayTask(getActivity());
				// 调用支付接口
				String result = alipay.pay(payInfo);

				Message msg = mHandler.obtainMessage();
				msg.what = SDK_PAY_FLAG;
				msg.obj = result;
				mHandler.sendMessage(msg);
			}
		};

		Thread payThread = new Thread(payRunnable);
		payThread.start();
	}
	
	@SuppressLint("HandlerLeak")
	Handler mHandler = new Handler() {
		public void handleMessage(android.os.Message msg) {
			switch (msg.what) {
			case SDK_PAY_FLAG:
			{
				AlipayResult resultObj = new AlipayResult((String) msg.obj);
				String resultStatus = resultObj.resultStatus;

				// 判断resultStatus 为“9000”则代表支付成功，具体状态码代表含义可参考接口文档
				if (TextUtils.equals(resultStatus, "9000")) {
					ShowUtil.showToast(getActivity(), "支付成功");
				} else {
					// 判断resultStatus 为非“9000”则代表可能支付失败
					// “8000” 代表支付结果因为支付渠道原因或者系统原因还在等待支付结果确认，最终交易是否成功以服务端异步通知为准（小概率状态）
					if (TextUtils.equals(resultStatus, "8000")) {
						ShowUtil.showToast(getActivity(), "支付结果确认中");
					} else {
						ShowUtil.showToast(getActivity(), "支付失败");
					}
				}
				break;
			}
			case SDK_CHECK_FLAG:
			{
				break;
			}
			default:
				break;
			}
		};
	};
	
	//Alipay
	public String getOrderInfo(String subject, String body, String price) {
		// 合作者身份ID
		String orderInfo = "partner=" + "\"" + AlipayKeys.PARTNER + "\"";

		// 卖家支付宝账号
		orderInfo += "&seller_id=" + "\"" + AlipayKeys.SELLER + "\"";

		// 商户网站唯一订单号
		orderInfo += "&out_trade_no=" + "\"" + getOutTradeNo() + "\"";

		// 商品名称
		orderInfo += "&subject=" + "\"" + subject + "\"";

		// 商品详情
		orderInfo += "&body=" + "\"" + body + "\"";

		// 商品金额
		orderInfo += "&total_fee=" + "\"" + price + "\"";

		// 服务器异步通知页面路径
		orderInfo += "&notify_url=" + "\"" + KtvAssistantAPIConfig.ALIPAY_NOTIFY_URL+ "\"";

		// 接口名称， 固定值
		orderInfo += "&service=\"mobile.securitypay.pay\"";

		// 支付类型， 固定值
		orderInfo += "&payment_type=\"1\"";

		// 参数编码， 固定值
		orderInfo += "&_input_charset=\"utf-8\"";

		// 设置未付款交易的超时时间
		// 默认30分钟，一旦超时，该笔交易就会自动被关闭。
		// 取值范围：1m～15d。
		// m-分钟，h-小时，d-天，1c-当天（无论交易何时创建，都在0点关闭）。
		// 该参数数值不接受小数点，如1.5h，可转换为90m。
		orderInfo += "&it_b_pay=\"30m\"";

		// 支付宝处理完请求后，当前页面跳转到商户指定页面的路径，可空
		//orderInfo += "&return_url=\"m.alipay.com\"";

		// 调用银行卡支付，需配置此参数，参与签名， 固定值
		// orderInfo += "&paymethod=\"expressGateway\"";

		return orderInfo;
	}

	/**
	 * get the out_trade_no for an order. 获取外部订单号
	 * 
	 * 年月日时分秒-KTVID-ROOMID-用户IDX-1
	 */
	public String getOutTradeNo() {
		String key = ToolUtil.getStringDate("yyyyMMddHHmmss");

		key += "-"+AppStatus.ktvId+"-"+AppStatus.roomId+"-"+AppStatus.userIdx+"-1";
		return key;
	}

	/**
	 * sign the order info. 对订单信息进行签名
	 * 
	 * @param content
	 *            待签名订单信息
	 */
	public String sign(String content) {
		return SignUtils.sign(content, AlipayKeys.PRIVATE);
	}

	/**
	 * get the sign type we use. 获取签名方式
	 * 
	 */
	public String getSignType() {
		return "sign_type=\"RSA\"";
	}
	
	/**
	 * check whether the device has authentication alipay account.
	 * 查询终端设备是否存在支付宝认证账户
	 * 
	 */
	public void check(View v) {
		Runnable checkRunnable = new Runnable() {

			@Override
			public void run() {
				PayTask payTask = new PayTask(getActivity());
				boolean isExist = payTask.checkAccountIfExist();

				Message msg = mHandler.obtainMessage();
				msg.what = SDK_CHECK_FLAG;
				msg.arg1 = isExist?1:0;
				mHandler.sendMessage(msg);
			}
		};

		Thread checkThread = new Thread(checkRunnable);
		checkThread.start();

	}

	/**
	 * get the sdk version. 获取SDK版本号
	 * 
	 */
	public void getSDKVersion() {
		PayTask payTask = new PayTask(getActivity());
		String version = payTask.getVersion();
		ShowUtil.showToast(getActivity(), "Alipay version = "+version);
	}
}
