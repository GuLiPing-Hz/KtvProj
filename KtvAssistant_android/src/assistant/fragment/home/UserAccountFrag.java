package assistant.fragment.home;

import java.io.File;

import tiange.ktv.assistant.R;
import android.annotation.SuppressLint;
import android.app.Activity;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.drawable.BitmapDrawable;
import android.graphics.drawable.Drawable;
import android.net.Uri;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.provider.MediaStore;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentManager;
import android.support.v4.app.FragmentPagerAdapter;
import android.support.v4.view.ViewPager;
import android.util.DisplayMetrics;
import android.view.Display;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.view.animation.Animation;
import android.view.animation.Animation.AnimationListener;
import android.view.animation.TranslateAnimation;
import android.widget.Button;
import android.widget.ImageButton;
import android.widget.TextView;
import assistant.activity.ChangeUserNameActivity;
import assistant.activity.Home;
import assistant.activity.SongCollectActivity;
import assistant.dialog.HeadSelectDialog;
import assistant.dialog.HeadSelectDialog.OnHeadClick;
import assistant.dialog.LoadingDialog;
import assistant.fragment.BaseFragment;
import assistant.fragment.pager.UserAcctGiftFragment;
import assistant.global.AppStatus;
import assistant.global.KtvAssistantAPIConfig;
import assistant.global.MessageDef;
import assistant.global.RequestCodeDef;
import assistant.manager.NetUtilManager.INetUtilListener;
import assistant.task.userapi.GetRoomGoldTask;
import assistant.task.userapi.UploadPhotoTask;
import assistant.util.ImageUtil;
import assistant.util.MobileNetStatUtil;
import assistant.util.ShowLog;
import assistant.util.ShowUtil;
import assistant.view.RoundedImageView;

import com.jni.netutil.XGMessage;
import com.zxing.activity.CaptureActivity;

public class UserAccountFrag extends BaseFragment implements INetUtilListener{
	//public Home mHome;
	private File tempFile;
	private static final String IMAGE_FILE_NAME = "head_img.jpg";
	private String[] items = new String[] { "选择本地图片", "拍照" };
	public static String userName;
	public static String hasScoreToGet = null;
	public static boolean hasScoreChanged;
	
	public boolean isLoading = false;
	private Bitmap photo;// 保存头像图片
	// 歌曲收藏
	private Button btn_songCollect;
	// 扫描二维码
	private Button btn_scanCode;
	/** =====================界面显示=================== **/
	private View vRoot;
	private ImageButton btn_backToHome;// 返回菜单图标
	private TextView title_lable;// 标题
	private RoundedImageView use_headImg;// 头像旁的摄像头
	private RoundedImageView use_headImgHead;// 头像
	private TextView nickname_lable;// 昵称 性别图标
	private TextView uc_gold;// 昵称 性别图标

	public final static Long AnimaDuration = 100L;

	private Button btn_Sysaward;// 系统奖励
	private Button btn_Getgift;// 收到的礼物
	private Button btn_Sendgift;// 送出的礼物
	private View view_Selected;// 选中显示状态
	private ViewPager pc_viewpager;// 选项卡

	private int m_curIndex = 0;
	private int m_nextIndex = 0;
	private int m_viewSelectWidth = 0;
	private Animation animation = null;
	LoadingDialog loadingDialog;
	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup container,
			Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreateView(inflater, container, savedInstanceState);
		
		vRoot = inflater.inflate(R.layout.percenter_mainpage, container, false);
		initView();
		initLoginAccount();
		
		return vRoot;
	}

	@Override
	public void onDestroyView() {
		// TODO Auto-generated method stub
//		mHome.removeIgnoredAboveView(pc_viewpager);
//		mHome.clearFragmentInMem();
		
		super.onDestroyView();
	}

	protected void initView() {
//		mHome = (Home) this.getActivity();
		findView();
		setListener();
		InitData();

	}

	protected void findView() {
		btn_backToHome = (ImageButton) vRoot.findViewById(R.id.iv_back);
		//btn_backToHome.setImageResource(R.drawable.btn_home_menu);
		title_lable = (TextView) vRoot.findViewById(R.id.tv_title);
		use_headImg = (RoundedImageView) vRoot.findViewById(R.id.ac_userheadupload);
		use_headImgHead = (RoundedImageView) vRoot.findViewById(R.id.ac_userheadImg);
		nickname_lable = (TextView) vRoot.findViewById(R.id.txt_nickname);
		uc_gold = (TextView) vRoot.findViewById(R.id.txt_uchead_gold);
		btn_songCollect = (Button) vRoot.findViewById(R.id.ac_mysongs);
		btn_scanCode = (Button) vRoot.findViewById(R.id.ac_getgold);
		btn_Sysaward = (Button) vRoot.findViewById(R.id.btn_pcmain_sysaward);
		btn_Getgift = (Button) vRoot.findViewById(R.id.btn_pcmain_giftget);
		btn_Sendgift = (Button) vRoot.findViewById(R.id.btn_pcmain_giftsend);
		view_Selected = (View) vRoot.findViewById(R.id.viewSelected);
		pc_viewpager = (ViewPager) vRoot.findViewById(R.id.pc_viewpager);
	}

	protected void InitData() {
		nickname_lable.setClickable(true);
		nickname_lable.setFocusable(true);
		userName = nickname_lable.getText().toString();
		// AppStatus.user
		if (null != Home.bitmap) {
			use_headImgHead.setImageBitmap(Home.bitmap);
		}
		title_lable.setText("个人中心");
		uc_gold.setText("" + AppStatus.user.gold);

		Display display = getActivity().getWindowManager().getDefaultDisplay();
		DisplayMetrics dm = new DisplayMetrics();
		display.getMetrics(dm);
		m_viewSelectWidth = dm.widthPixels / 3;

		ViewGroup.LayoutParams params = view_Selected.getLayoutParams();
		params.width = m_viewSelectWidth;
		view_Selected.setLayoutParams(params);

		FragmentManager fm = this.getActivity().getSupportFragmentManager();
		pc_viewpager.setAdapter(new FragmentPagerAdapter(fm) {

			@Override
			public Fragment getItem(int arg0) {
				// TODO Auto-generated method stub

				return UserAcctGiftFragment.newInstance(arg0);

			}

			@Override
			public int getCount() {
				// TODO Auto-generated method stub
				// 系统奖励，收到的礼物，送出的礼物
				return 3;
			}

		});
		switchMenuState(0);
//		mHome.addIgnoredAboveView(pc_viewpager);
	}

	protected void setListener() {

		// 回到首页
		btn_backToHome.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				//mHome.showMenu();
				getActivity().finish();
			}
		});

		// 更换头像
		use_headImg.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View v) {
				showDialog();
			}

		});
		// 更换头像
		use_headImgHead.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View v) {
				showDialog();
			}

		});
		// 修改昵称
		nickname_lable.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				Intent intent = new Intent(UserAccountFrag.this.getActivity(), ChangeUserNameActivity.class);
				startActivityForResult(intent, RequestCodeDef.NICKNAME_MODIFY);
			}
		});

		btn_Sysaward.setOnClickListener(m_OnClickListenerSwitchMenu);
		btn_Getgift.setOnClickListener(m_OnClickListenerSwitchMenu);
		btn_Sendgift.setOnClickListener(m_OnClickListenerSwitchMenu);

		pc_viewpager
				.setOnPageChangeListener(new ViewPager.OnPageChangeListener() {

					@Override
					public void onPageSelected(int position) {
						// TODO Auto-generated method stub
						switchMenuAnimation(position);
					}

					@Override
					public void onPageScrolled(int arg0, float arg1, int arg2) {
						// TODO Auto-generated method stub
					}

					@Override
					public void onPageScrollStateChanged(int arg0) {
						// TODO Auto-generated method stub

					}
				});
		
		//歌曲收藏
		btn_songCollect.setOnClickListener(new OnClickListener(){

			@Override
			public void onClick(View arg0) {
				// TODO Auto-generated method stub
				Intent intent = new Intent(getActivity(),SongCollectActivity.class);
				startActivity(intent);
			}
			
		});
		
		//扫描二维码获取金币
		btn_scanCode.setOnClickListener(new OnClickListener(){

			@Override
			public void onClick(View arg0) {
				// TODO Auto-generated method stub
				Intent intent = new Intent(UserAccountFrag.this.getActivity(),CaptureActivity.class);
				intent.putExtra(CaptureActivity.EXTRA_SACN_TYPE, CaptureActivity.SCANTYPE_GOLD);
				startActivityForResult(intent,RequestCodeDef.REQUEST_SCAN_GOLD);
			}
			
		});
		
	
	}

	private View.OnClickListener m_OnClickListenerSwitchMenu = new View.OnClickListener() {

		@Override
		public void onClick(View arg0) {
			// TODO Auto-generated method stub
			switch (arg0.getId()) {
			case R.id.btn_pcmain_sysaward:
				pc_viewpager.setCurrentItem(0, true);
				break;
			case R.id.btn_pcmain_giftget:
				pc_viewpager.setCurrentItem(1, true);
				break;
			case R.id.btn_pcmain_giftsend:
				pc_viewpager.setCurrentItem(2, true);
				break;
			}
		}
	};

	// 更新选择栏的状态显示
	void switchMenuAnimation(int index) {
		if (m_curIndex == index || m_nextIndex == index)
			return;

		if (animation != null)
			animation.cancel();

		m_nextIndex = index;

		animation = new TranslateAnimation(m_viewSelectWidth * m_curIndex,
				m_viewSelectWidth * index, 0, 0);
		animation.setDuration(AnimaDuration);
		animation.setFillAfter(true);
		animation.setFillEnabled(true);
		animation.setAnimationListener(new AnimationListener() {

			@Override
			public void onAnimationEnd(Animation arg0) {
				// TODO Auto-generated method stub
				// 再调整序号值
				m_curIndex = m_nextIndex;
				switchMenuState(m_curIndex);
			}

			@Override
			public void onAnimationRepeat(Animation arg0) {
				// TODO Auto-generated method stub

			}

			@Override
			public void onAnimationStart(Animation arg0) {
				// TODO Auto-generated method stub

			}

		});
		view_Selected.startAnimation(animation);
	}

	void switchMenuState(int index) {
		switch (m_curIndex) {
		case 0:
			btn_Sysaward.setSelected(true);
			btn_Getgift.setSelected(false);
			btn_Sendgift.setSelected(false);
			break;
		case 1:
			btn_Sysaward.setSelected(false);
			btn_Getgift.setSelected(true);
			btn_Sendgift.setSelected(false);
			break;
		case 2:
			btn_Sysaward.setSelected(false);
			btn_Getgift.setSelected(false);
			btn_Sendgift.setSelected(true);
			break;
		}
	}

	private void showDialog() {
		new HeadSelectDialog(getActivity(), new OnHeadClick(){

			@Override
			public void onSelect(int type) {
				// TODO Auto-generated method stub
				if(type == HeadSelectDialog.PICTURE)
				{
					Intent intentFromGallery = new Intent(Intent.ACTION_PICK);//Intent.ACTION_GET_CONTENT
					intentFromGallery.setType("image/*");
					startActivityForResult(intentFromGallery,RequestCodeDef.REQUEST_HEADFROM_IMAGES);
				}
				else if(type == HeadSelectDialog.TAKEPHOTO)
				{
					Intent intentFromCapture = new Intent(MediaStore.ACTION_IMAGE_CAPTURE);
					
					intentFromCapture.putExtra(MediaStore.EXTRA_OUTPUT,Uri.fromFile(
							new File(AppStatus.s_appDirUtil.getHeadDir()+IMAGE_FILE_NAME)));
					
					startActivityForResult(intentFromCapture,RequestCodeDef.REQUEST_HEADFROM_CAREMA);
				}
			}
			
		}).show();
	}

	/**
	 * 裁剪图片
	 * 
	 * @param uri
	 */
	private void startPhotoZoom(Uri uri) {
		Intent intent = new Intent("com.android.camera.action.CROP");
		intent.setDataAndType(uri, "image/*");
		// 设置裁剪
		intent.putExtra("crop", "true");
		// 宽高比例
		intent.putExtra("aspectX", 1);
		intent.putExtra("aspectY", 1);
//		intent.putExtra("outputFormat", "JPEG");// 图片格式
//		intent.putExtra("noFaceDetection", true);// 取消人脸识别
		// 裁剪图片宽高
		intent.putExtra("outputX", 320);
		intent.putExtra("outputY", 320);
		intent.putExtra("return-data", true);
		startActivityForResult(intent, RequestCodeDef.REQUEST_HEAD_CUT);
	}

	/**
	 * 将图片显示在ImageView控件上面，并将图片上传到服务器
	 * 
	 * @param data
	 */
	private void getImageToView(Intent data) {
		Bundle extras = data.getExtras();
		if (extras != null) {
			photo = extras.getParcelable("data");
			Drawable drawable = new BitmapDrawable(photo);
			use_headImgHead.setImageDrawable(drawable);

			if(!MobileNetStatUtil.checkCurrentNetState(getActivity(),true))
			{
				ShowUtil.showToast(this.getActivity(),R.string.fealure_upload_headimg);
				return ;
			}
			if (!AppStatus.isAleadyLogin()) {
				ShowUtil.showToast(getActivity(), R.string.msg_login_system);
				return;
			}
			
			if(loadingDialog == null)
				loadingDialog = new LoadingDialog(getActivity(),"");
			loadingDialog.show();
			
			UploadPhotoTask upLoadPhotoTask = new UploadPhotoTask(handler,
					AppStatus.userIdx, AppStatus.roomId, IMAGE_FILE_NAME, photo);
			upLoadPhotoTask.execute();
		}
	}

	@Override
	public void onActivityResult(int requestCode, int resultCode, Intent data) {
		// TODO Auto-generated method stub
		super.onActivityResult(requestCode, resultCode, data);
		
		switch (requestCode) {
		case RequestCodeDef.REQUEST_SCAN_GOLD:
		{
			//扫码成功
			if(resultCode == Home.RESULT_OK)
			{
				if(loadingDialog == null)
					loadingDialog = new LoadingDialog(getActivity(),"");
				loadingDialog.show();
				
				String resultString = data.getStringExtra(CaptureActivity.EXTRA_RESULT_DATA);
				//登陆包厢
				if(Home.enterRoomManager != null)
					Home.enterRoomManager.startLoginRoom(resultString, true);
				//启动task向服务器请求房间金币
				GetRoomGoldTask task = new GetRoomGoldTask(goldHandler,resultString);
				task.execute();
			}
			else if(resultCode == Home.RESULT_CANCELED)
				ShowUtil.showToast(getActivity(), R.string.scangold_cancel);
			break;
		}
		case RequestCodeDef.NICKNAME_MODIFY:
			if (resultCode == Home.RESULT_OK) {
				nickname_lable.setText(AppStatus.user.nickname);
				//mHome.tvUserName.setText(AppStatus.user.nickname);
			}
			break;
			// 直接从相册获取
		case RequestCodeDef.REQUEST_HEADFROM_IMAGES:
			if (resultCode == Activity.RESULT_OK && data != null)
				startPhotoZoom(data.getData());
			break;
		// 使用相机拍照
		case RequestCodeDef.REQUEST_HEADFROM_CAREMA:
			if(resultCode == Activity.RESULT_OK)
			{
				tempFile = new File(AppStatus.s_appDirUtil.getHeadDir()+ IMAGE_FILE_NAME);
				startPhotoZoom(Uri.fromFile(tempFile));
			}
			break;
		// 取得裁剪后的图片
		case RequestCodeDef.REQUEST_HEAD_CUT:
			if (data != null)
				getImageToView(data);
			break;
		}
	}

	@SuppressLint("HandlerLeak")
	Handler goldHandler = new Handler(){

		@Override
		public void handleMessage(Message msg) {
			// TODO Auto-generated method stub
			super.handleMessage(msg);
			
			if(loadingDialog!=null&&loadingDialog.isShowing())
				loadingDialog.dismiss();
			
			switch(msg.what)
			{
			case MessageDef.WM_GET_ROOM_GOLD:
			{
				if(msg.arg1 == KtvAssistantAPIConfig.APIErrorCode.Nothing)
				{
					ShowUtil.showToast(getActivity(), R.string.scangold_success);
					//更新金币信息
					//mHome.updateGold(msg.arg2);
					uc_gold.setText(""+AppStatus.user.gold);
				}
				else if(msg.arg1 == KtvAssistantAPIConfig.APIErrorCode.NoRoomGold)
					ShowUtil.showToast(getActivity(), R.string.scangold_nogold);
				else
					ShowUtil.showToast(getActivity(), R.string.scangold_failed);
				break;
			}
			}
		}
		
	};
	
	@SuppressLint("HandlerLeak")
	Handler handler = new Handler() {

		@Override
		public void handleMessage(Message msg) {
			super.handleMessage(msg);
			try {
				if(loadingDialog != null && loadingDialog.isShowing())
					loadingDialog.dismiss();
				switch (msg.what) {
				case MessageDef.WM_UPLOAD_PHOTO:
					if(msg.arg1 == KtvAssistantAPIConfig.APIErrorCode.Nothing)
					{
						ShowUtil.showToast(getActivity(), R.string.upload_head_img_success);
						Home.bitmap = photo;
						use_headImgHead.setImageBitmap(Home.bitmap);
					}
					else
						ShowUtil.showToast(getActivity(), "上传头像失败:"+(String)msg.obj);
					break;
				}
			} catch (Exception e) {
				// TODO: handle exception
				ShowLog.showException(e);
			}
			
		}
	};

	protected void initLoginAccount() {

		if (AppStatus.user == null)
			return;

		// img_head
		Bitmap photo = ImageUtil.getBitmap(AppStatus.user.headphoto);
		if (photo != null) {
			Drawable drawable1 = new BitmapDrawable(photo);
			use_headImgHead.setBackgroundDrawable(drawable1);
		}

		nickname_lable.setText(AppStatus.user.nickname);

		// 根据用户的性别来显示性别图片
		Drawable drawable;
		if (0 == AppStatus.user.gender) {
			drawable = getResources().getDrawable(R.drawable.icon_sex2_woman);
			drawable.setBounds(0, 0, drawable.getMinimumWidth(),
					drawable.getMinimumHeight());
			nickname_lable.setCompoundDrawables(drawable, null, null, null);
		} else if (1 == AppStatus.user.gender) {
			drawable = getResources().getDrawable(R.drawable.icon_sex2_man);
			drawable.setBounds(0, 0, drawable.getMinimumWidth(),
					drawable.getMinimumHeight());
			nickname_lable.setCompoundDrawables(drawable, null, null, null);
		}
	}

	@Override
	public void dealWithMsg(Message msg) {
		// TODO Auto-generated method stub
		switch(msg.what)
		{
		case XGMessage.XGMSG_USERGOLD_UPDATE:
		{
			uc_gold.setText(""+AppStatus.user.gold);
			break;
		}
		}
	}
}
